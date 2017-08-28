#include "MemoryAccess.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <iostream>


#include "Aes.h"

#define sbi(byte, bit) (byte) |= (1 << (bit))
#define cbi(byte, bit) (byte) &= ~(1 << (bit))
#define ubi(byte, bit, value) if((value)) {sbi(byte, bit);} else {cbi(byte, bit);}

#define set_throw_if_empty(section, name, block) block = m_rep.position(section, name);\
if (!block) { throw std::runtime_error("Cannot find \"" + std::string(name) + "\" in section \"" + std::string(section) + "\"."); }


template <typename T>
static T from_data(std::vector<uint8_t>& data, unsigned offset, unsigned size);

template <> uint8_t from_data<uint8_t> (std::vector<uint8_t>& data, unsigned offset, unsigned size)
{
	if(size != sizeof(uint8_t)){
		throw std::length_error("from_data<uint8_t>() : sizeof(uint8_t) != size");
	}
	return data.at(offset);
}

template <> uint16_t from_data<uint16_t> (std::vector<uint8_t>& data, unsigned offset, unsigned size)
{
	if(size != sizeof(uint16_t)){
		throw std::length_error("from_data<uint16_t>() : sizeof(uint16_t) != size");
	}
	// Avr is little endian
	return static_cast<uint16_t>(data.at(offset) + (data.at(offset+1)<<8));
}

Option::Option():
	sorting_method(0),
	orientation(0),
	initialized(0),
	qwerty(0)
{}

MemoryAccess::MemoryAccess(MemoryRepresentation& rep):
	m_rep(rep),
	m_data(),
	m_key()
{

}

void MemoryAccess::loadFromFile(const std::string& fileName)
{
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
	if(!file){
		throw std::invalid_argument("Cannot open file \"" + fileName + "\".");
	}
	std::ifstream::pos_type size = file.tellg();

	m_data.resize(size);

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&m_data[0]), size);
}

void MemoryAccess::setMemoryRepresentation(MemoryRepresentation& rep)
{
	m_rep = rep;
}

void MemoryAccess::setKey(const std::vector<uint8_t>& key)
{
	if(key.size() != 16){
		throw std::invalid_argument("Wrong key size.");
	}
	m_key = key;
}

void MemoryAccess::setKey(const std::string& key_enc85)
{
	if(key_enc85.size() != 20){
		throw std::invalid_argument("Wrong key size.");
	}
	m_key.resize(16);

	const char *input= key_enc85.c_str();

	uint8_t i = 0;
	uint32_t powers[5] = {52200625, 614125, 7225, 85, 1};
	uint8_t pos = 0;
	for(; i < 4; ++i)
	{
		uint32_t sum = 0;
		uint8_t j = 0;
		for(; j < 5; ++j)
		{
			sum += (input[j]-33) * powers[j];
		}
		m_key[0+pos] = sum >> 24;
		m_key[1+pos] = (sum & 0xFF0000) >> 16;
		m_key[2+pos] = (sum & 0xFF00) >> 8;
		m_key[3+pos] = sum & 0xFF;

		input += 5;
		pos += 4;
	}
}

Password MemoryAccess::password(Password::id_t id)
{
	if(m_key.empty()){
		throw std::logic_error("Key has not been specified.");
	}

	MemoryBlock blockPassword = m_rep.position("password");
	if(!blockPassword){
		throw std::runtime_error("Cannot find section \"password\"");
	}
	std::size_t pos = blockPassword.offset + id * blockPassword.size;

	MemoryBlock pwdBlock;
	Password pwd;

	set_throw_if_empty("password", "counter", pwdBlock);
	pwd.counter = from_data<decltype(pwd.counter)>(m_data, pos+pwdBlock.offset, pwdBlock.size);

	set_throw_if_empty("password", "name", pwdBlock)
	for(std::size_t i(pos + pwdBlock.offset); i < pos + pwdBlock.offset + pwdBlock.size; ++i)
	{
		char ch = static_cast<char>(m_data[i]);
		if(ch == 0){
			break;
		}
		pwd.name += ch;
	}

	// Get password

	std::vector<uint8_t> iv;
	std::vector<uint8_t> data_enc;

	set_throw_if_empty("password", "iv_data", pwdBlock);
	iv.resize(pwdBlock.size);
	for(std::size_t i(0); i  < iv.size(); ++i)
	{
		iv[i] = m_data[pos+pwdBlock.offset+i];
	}

	set_throw_if_empty("password", "data", pwdBlock);
	data_enc.resize(pwdBlock.size);
	for(std::size_t i(0); i  < data_enc.size(); ++i)
	{
		data_enc[i] = m_data[pos+pwdBlock.offset+i];
	}

	std::vector<uint8_t> data_plain(data_enc.size());

	AES128_CBC_decrypt_buffer(data_plain.data(), data_enc.data(), data_enc.size(), m_key.data(), iv.data());

	for(uint8_t b : data_plain){
		pwd.password += b;
	}

	// Get user

	iv.clear();
	data_enc.clear();

	set_throw_if_empty("password", "iv_usr", pwdBlock);
	iv.resize(pwdBlock.size);
	for(std::size_t i(0); i  < iv.size(); ++i)
	{
		iv[i] = m_data[pos+pwdBlock.offset+i];
	}

	set_throw_if_empty("password", "usr_name", pwdBlock);
	data_enc.resize(pwdBlock.size);
	for(std::size_t i(0); i  < data_enc.size(); ++i)
	{
		data_enc[i] = m_data[pos+pwdBlock.offset+i];
	}

	data_plain.resize(data_enc.size());

	AES128_CBC_decrypt_buffer(data_plain.data(), data_enc.data(), data_enc.size(), m_key.data(), iv.data());

	for(uint8_t b : data_plain){
		pwd.userName += b;
	}

	return pwd;
}


std::vector<Password> MemoryAccess::allPasswords()
{
	std::vector<Password> all;
	MemoryBlock block;
	set_throw_if_empty("header", "memory_map", block);
	for(std::size_t i = block.offset; i < block.offset + block.size; ++i)
	{
		uint8_t byte = m_data[i];
		for(uint8_t bit(0); bit < 8; ++bit)
		{
			if(byte & (1<<bit))
			{
				all.push_back(password((i-block.offset)*8+bit));
			}
		}
	}
	return all;
}

Option MemoryAccess::option() const
{
	MemoryBlock block = m_rep.position("header", "option");
	if(!block){
		throw std::runtime_error("Cannot find block \"option\" in section \"header\"");
	}
	Option opt;
	const MemoryBlock::mempos_t off = block.offset;
	MemoryBlock bitBlock;
	
	set_throw_if_empty("header", "option_sorting_method_l", bitBlock)
	uint8_t sorting_method_l = m_data.at(off) & (1<<bitBlock.bitPos);

	set_throw_if_empty("header", "option_sorting_method_h", bitBlock)
	uint8_t sorting_method_h = m_data.at(off) & (1<<bitBlock.bitPos);

	set_throw_if_empty("header", "option_orientation", bitBlock)
	opt.orientation = m_data.at(off) & (1<<bitBlock.bitPos);

	set_throw_if_empty("header", "option_initialized", bitBlock)
	opt.initialized = m_data.at(off) & (1<<bitBlock.bitPos);

	set_throw_if_empty("header", "option_qwerty", bitBlock)
	opt.qwerty =m_data.at(off) & (1<<bitBlock.bitPos);

	opt.sorting_method = (sorting_method_h << 1) | sorting_method_l;

	return opt;
}

void MemoryAccess::setOption(Option opt)
{
	MemoryBlock bitBlock;

	uint8_t sorting_method_l = opt.sorting_method & 0x01;
	set_throw_if_empty("header", "option_sorting_method_l", bitBlock);
	ubi(m_data.at(bitBlock.offset), bitBlock.bitPos, sorting_method_l)

	uint8_t sorting_method_h = opt.sorting_method >> 1;
	set_throw_if_empty("header", "option_sorting_method_h", bitBlock);
	ubi(m_data.at(bitBlock.offset), bitBlock.bitPos, sorting_method_h)

	set_throw_if_empty("header", "option_orientation", bitBlock);
	ubi(m_data.at(bitBlock.offset), bitBlock.bitPos, opt.orientation)

	set_throw_if_empty("header", "option_initialized", bitBlock);
	ubi(m_data.at(bitBlock.offset), bitBlock.bitPos, opt.initialized)

	set_throw_if_empty("header", "option_qwerty", bitBlock);
	ubi(m_data.at(bitBlock.offset), bitBlock.bitPos, opt.qwerty)
}

Password::id_t MemoryAccess::numPwd()
{
	MemoryBlock numBlock = m_rep.position("header", "num_pwd");
	if(!numBlock)
	{
		return maxPwd();
	}

	return from_data<Password::id_t>(m_data, numBlock.offset, numBlock.size); 
}

Password::id_t MemoryAccess::maxPwd()
{
	return (m_data.size() - m_rep.position("password").offset) / m_rep.position("password").size;
}
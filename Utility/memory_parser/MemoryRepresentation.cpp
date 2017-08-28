#include "MemoryRepresentation.h"

#include "pugixml.hpp"
#include <iostream>

MemoryBlock::MemoryBlock():
	offset(0), size(0), bitPos(255)
{

}


bool MemoryBlock::valid() const
{
	return size != 0 && offset != 0;
}

MemoryRepresentation::MemoryRepresentation():
	m_nameBlock(),
	m_valid(false),
	m_error(NoError)
{

}

#define return_if_empty(attr) if(attr.empty()) { m_error = MissingAttribute; return false; }

bool MemoryRepresentation::loadFromFile(const std::string& fileName)
{
	m_valid = false;

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName.c_str());
	if(!result){
		std::cerr << "MemoryRepresentation::loadFromFile() : error while parsing xml file \"" << fileName << "\", " << result.description() << '\n';
		return false;
	}

	pugi::xml_node root_n = doc.child("root");
	if(root_n.empty())
	{
		m_error = MissingNode;
		return false;
	}

	MemoryBlock::mempos_t totalSize = 0;

	for (pugi::xml_node section_n: root_n.children("section"))
	{
		const pugi::xml_attribute name_a = section_n.attribute("name");
		return_if_empty(name_a);
		const std::string sectionName = name_a.value();

		MemoryBlock blockSection;
		blockSection.offset = totalSize;

		for (pugi::xml_node child_n: section_n.children())
		{
			if(std::string(child_n.name()) == "byte")
			{
				MemoryBlock byteBlock;
				byteBlock.offset = totalSize;
				byteBlock.size = 1;
				totalSize += byteBlock.size;

				const pugi::xml_attribute name_byte_a = child_n.attribute("name");
				return_if_empty(name_byte_a);
				const std::string nameByte = name_byte_a.value();
				if(!insertValue(sectionName, nameByte, byteBlock)){
					m_error = NameRedefinition;
					return false;
				}

				for(pugi::xml_node bit_n : child_n.children("bit"))
				{
					MemoryBlock bitBlock;
					bitBlock.offset = byteBlock.offset;
					bitBlock.size = byteBlock.size;

					const pugi::xml_attribute pos_a = bit_n.attribute("pos");
					return_if_empty(pos_a);
					bitBlock.bitPos = pos_a.as_int(255);
					if(bitBlock.bitPos == 255)
					{
						m_error = InvalidAttribute;
						return false;
					}

					const pugi::xml_attribute name_bit_a = bit_n.attribute("name");
					return_if_empty(name_bit_a);

					const std::string nameBit = name_bit_a.value();
					if(!insertValue(sectionName, nameByte + "_" + nameBit, bitBlock)){
						m_error = NameRedefinition;
						return false;
					}
				}

				// size = 1;
				// uint8_t bitPos = 255;
				// MemoryBlock block;
				// block.offset = totalSize;
				// block.size = 1;
				// totalSize += block.size;
				// for(pugi::xml_node bit_n : child_n.children("bit"))
				// {
				// 	const pugi::xml_attribute pos_a = bit_n.attribute("pos");
				// 	return_if_empty(pos_a);
				// 	bitPos = pos_a.as_int(255);
				// 	if(bitPos == 255)
				// 	{
				// 		m_error = InvalidAttribute;
				// 		return false;
				// 	}
				// 	const pugi::xml_attribute name_a = bit_n.attribute("name");
				// 	return_if_empty(name_a);
				// 	std::string name = name_a.value();
				// 	if(!insertValue(sectionName, name, block)){
				// 		m_error = NameRedefinition;
				// 		return false;
				// 	}
				// }
			}
			else if(std::string(child_n.name()) == "block")
			{
				const pugi::xml_attribute size_a = child_n.attribute("size");
				return_if_empty(size_a);
				MemoryBlock::mempos_t size = size_a.as_int(0);
				if(size == 0)
				{
					m_error = InvalidAttribute;
					return false;
				}
				
				const pugi::xml_attribute name_c_a = child_n.attribute("name");
				return_if_empty(name_c_a);

				MemoryBlock block;
				block.offset = totalSize;
				block.size = size;
				totalSize += block.size;

				if(!insertValue(sectionName, name_c_a.value(), block)){
					m_error = NameRedefinition;
					return false;
				}
			}
		} // For : child
		blockSection.size = totalSize - blockSection.offset;
		if(!insertValue(sectionName, blockSection)){
			std::cerr << sectionName << std::endl;
			
			m_error = NameRedefinition;
			return false;
		}
	} // For : section

	m_valid = true;
	return true;
}

bool MemoryRepresentation::isValid() const
{
	return m_valid;
}

MemoryBlock MemoryRepresentation::position(std::pair<std::string, std::string> sec_name) const
{
	auto it = m_nameBlock.find(sec_name);
	if(it == m_nameBlock.end()){
		return MemoryBlock();
	}
	else{
		return it->second;
	}
}

MemoryBlock MemoryRepresentation::position(const std::string& section, const std::string& name) const
{
	return position({section, name});
}

MemoryBlock MemoryRepresentation::position(const std::string& section) const
{
	return position({section, "__special__section__"});
}

bool MemoryRepresentation::error() const
{
	return m_error != NoError;
}

std::string MemoryRepresentation::errorString() const
{
	std::string errStr;
	switch(m_error)
	{
	case NoError:
		errStr = "Aucune erreur.";
		break;
	case InvalidFile:
		errStr = "Fichier invalide.";
		break;
	case MissingNode:
		errStr = "Noeud manquant.";
		break;
	case MissingAttribute:
		errStr = "Attribut manquant.";
		break;
	case InvalidAttribute:
		errStr = "Attribut invalide";
		break;
	case NameRedefinition:
		errStr = "Mutliple definition d'un nom";
		break;
	default:
		errStr = "Erreur inconnue";
	}
	return errStr;
}

bool MemoryRepresentation::insertValue(const std::string& section, const MemoryBlock& block)
{
	return insertValue(section, "__special__section__", block);
}

bool MemoryRepresentation::insertValue(const std::string& section, const std::string& name, const MemoryBlock& block)
{
	if(m_nameBlock.find({section, name}) != m_nameBlock.end()){
		return false;
	}
	else{
		m_nameBlock[{section, name}] = block;
		return true;
	}
}
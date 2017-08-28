#ifndef MEMORYACCESS_HEADER_THOMAS_CHEVALIER
#define MEMORYACCESS_HEADER_THOMAS_CHEVALIER

#include <cstdint>

#include "Password.h"
#include "MemoryRepresentation.h"

struct Option
{
	Option();

	uint8_t sorting_method : 2;
	uint8_t orientation : 1;
	uint8_t initialized : 1;
	uint8_t qwerty : 1;
};

class MemoryAccess
{
public:
	MemoryAccess(MemoryRepresentation& rep);

	void loadFromFile(const std::string& fileName);

	void setMemoryRepresentation(MemoryRepresentation& rep);
	void setKey(const std::vector<uint8_t>& key);
	void setKey(const std::string& key_enc85);

	Password password(Password::id_t id);
	std::vector<Password> allPasswords();

	Option option() const;
	void setOption(Option opt);

	// Meta
	Password::id_t numPwd();
	Password::id_t maxPwd();

private:

	MemoryRepresentation& m_rep;
	std::vector<uint8_t> m_data;
	std::vector<uint8_t> m_key;
};

#endif // MEMORYACCESS_HEADER_THOMAS_CHEVALIER
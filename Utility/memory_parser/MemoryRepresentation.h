#ifndef MEMORYREPRESENTATION_HEADER_THOMAS_CHEVALIER
#define MEMORYREPRESENTATION_HEADER_THOMAS_CHEVALIER

#include <vector>
#include <map>
#include <utility>
#include <string>

#include <cstdint>

struct MemoryBlock
{
	typedef unsigned mempos_t;
	mempos_t offset;
	mempos_t size;

	uint8_t bitPos;

	MemoryBlock();

	bool valid() const;
	operator bool() const { return valid(); }
};

class MemoryRepresentation
{
public:

	enum ErrorType
	{
		NoError,
		InvalidFile,
		MissingNode,
		MissingAttribute,
		InvalidAttribute,
		NameRedefinition
	};

	typedef MemoryBlock::mempos_t mempos_t;

	MemoryRepresentation();

	bool loadFromFile(const std::string& fileName);

	bool isValid() const;

	bool error() const;
	std::string errorString() const;

	MemoryBlock position(std::pair<std::string, std::string> sec_name) const;
	MemoryBlock position(const std::string& section, const std::string& name) const;
	MemoryBlock position(const std::string& section) const;

private:
	typedef std::pair<std::string, std::string> pairString;
	std::map<pairString, MemoryBlock> m_nameBlock;

	bool insertValue(const std::string& section, const MemoryBlock& block);
	bool insertValue(const std::string& section, const std::string& name, const MemoryBlock& block);

	bool m_valid;
	ErrorType m_error;
};

#endif // MEMORYREPRESENTATION_HEADER_THOMAS_CHEVALIER
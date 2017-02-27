#ifndef APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER
#define APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER

#include <fstream>
#include <string>

class Command;


struct Key
{
	Key();
	bool isValid() const {return valid;}
	void set(uint8_t data_, uint8_t lenght);
	uint8_t data[16];
	bool valid;
};

class Application
{
public:
	Application(const std::string& fileName);

	void run();

private:

	void openFile(const std::string& fileName);
	void quit();
	std::string readCommand();
	void execute(const Command& cmd);

	std::ifstream m_file;
	int m_currentId;
	Key m_key;
};

#endif // APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER

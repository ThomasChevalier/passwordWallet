#ifndef APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER
#define APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER

#include <fstream>
#include <string>

class Command;

class Application
{
public:
	Application(const std::string& fileName);

	void run();

private:

	void openFile(const std::string& fileName);
	void quit();
	void execute(const Command& cmd);

	std::ifstream m_file;
	int m_currentId;
};

#endif // APPLICATION_HELPER_HEADER_THOMAS_CHEVALIER

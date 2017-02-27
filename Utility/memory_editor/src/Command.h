#ifndef COMMAND_HELPER_HEADER_THOMAS_CHEVALIER
#define COMMAND_HELPER_HEADER_THOMAS_CHEVALIER

#include <string>

struct Command
{
	enum Command_Type
	{
		None,

		ReadInfo,
		ReadNextUtil,
		ReadPrevUtil,
		ReadNextAlpha,
		ReadPrevAlpha,
		ReadNext,
		ReadPrev,
		ReadCurrent,
		ReadIndex,
		ReadAll,

		SetKey,

		Quit,

		Unknow
	};

	Command();
	Command(const std::string& command);

	int getArgument() const;
	Command_Type getType() const;

	void setString(const std::string& command);
	std::string getString() const { return m_command; }

private:
	void parseCommand();

	Command_Type m_type;
	std::string m_command;
	int m_argument;
};

#endif // COMMAND_HELPER_HEADER_THOMAS_CHEVALIER

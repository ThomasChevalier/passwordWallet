#ifndef COMMAND_HEADER_THOMAS_CHEVALIER
#define COMMAND_HEADER_THOMAS_CHEVALIER

#include <string>

struct PasswordInfo
{
	int id;

	std::string name;
	std::string pwd;
	std::string usr;

	enum Operation
	{
		Create = 0,
		ChangeName = 1,
		ChangePwd = 2,
		ChangeUsr = 3,
		Del
	};

	Operation op;
};

class Command
{
public:
	Command();

	void read();
	void exec();

	enum Type
	{
		New = 0,
		Delete,
		Change,
		Show,
		Exit,

		None
	};

	Type type() const { return m_type; }

private:

	void ask_new();
	void ask_which();
	void ask_what();

	Type m_type;
	PasswordInfo m_currentPwd;
};

#endif // COMMAND_HEADER_THOMAS_CHEVALIER
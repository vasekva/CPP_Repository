#include <string>
#include <iostream>

void clear_eof(std::string &msg)
{
	/** TRIM */
	msg.erase(msg.find_last_not_of(' ') + 1);  //suffixing spaces
	msg.erase(0, msg.find_first_not_of(' '));   //prefixing spaces
	/** DELETING LAST '\n' */
	if (msg[msg.length() - 1] == '\n')
		msg.erase(msg.length() - 1, 1);
}

bool is_statistic(std::string &msg)
{
	bool ret_val = true;

	if (msg.length() != 11)
	{
		ret_val = false;
		std::cout << msg << " isn't equal a flag --statistic!" << std::endl;
	}
	return (ret_val);
}

bool check_spec_symbols(std::string &msg)
{
	bool ret_val = true;
	int	symbls[4] = {0}; // 0 = ' ', 1 = ':', 2 = '.', 3 = '-'

	for (int i = 0; i < msg.length(); i++)
	{
		if (msg[i] == ' ')
			symbls[0]++;
		else if (msg[i] == ':')
			symbls[1]++;
		else if (msg[i] == '.')
			symbls[2]++;
		else if (msg[i] == '-')
			symbls[3]++;
	}
	if (symbls[0] > 2 || symbls[1] > 2 || symbls[2] > 2 || symbls[3] > 2)
	{
		std::cout << "check_spec_symbols() error" << std::endl;
		ret_val = false;
	}
	return (ret_val);
}

bool is_valid_symbols(std::string &msg)
{
	bool ret_val = true;

	for (int i = 0; i < msg.length(); i++)
	{
		if (msg[i] != ':' && msg[i] != ' ' &&
			msg[i] != '.' && msg[i] != '-' && !std::isdigit(msg[i]))
		{
			std::cout << "is_valid_symbols() error with value " << "'" << msg[i] << "'" << std::endl;
			ret_val = false;
			break;
		}
	}
	return (ret_val);
}

bool is_valid_msg(std::string &msg)
{
	if (msg == "--statistic")
	{
		if (msg.length() == 11)
			return (true);
		else
		{
			std::cout << "this isn't a '--statistic' message" << std::endl;
			return (false);
		}
	}
	else
		return (check_spec_symbols(msg) && is_valid_symbols(msg));
}
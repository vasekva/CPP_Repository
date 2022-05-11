#include "Client.hpp"

static bool is_host_valid(std::string host)
{
	char dot_count = 0;
	bool ret = true;

	for (int i = 0; i < host.length(); i++)
	{
		if ((!std::isdigit(host[i]) && host[i] != '.') // is digit or dot??
			|| ((i == 0 || i == host.length() - 1) && host[i] == '.') // is the first or last value a digit?
			|| (i != 0 && host[i] == '.' && host[i - 1] == '.') // is a value before dot a digit?
		)
		{
			ret = false;
			break;
		}
		if (host[i] == '.')
			++dot_count;
	}

	if (dot_count != 3)
		ret = false;

	return (ret);
}

static bool is_port_valid(std::string port)
{
	bool ret = true;

	if (port.length() < 2 || port.length() > 4)
		return (false);

	for (int i = 0; i < port.length(); i++)
	{
		if (!std::isdigit(port[i]))
		{
			ret = false;
			break;
		}
	}

	return (ret);
}

static bool is_correct_input(int argc, char **argv)
{
	bool ret_flag = true;

	if (argc == 4 && std::string(argv[3]) != "--statistic")
	{
		std::cerr << "Maybe you wanted to specify --statistic ?" << std::endl;
		ret_flag = false;
	}
	if (!is_host_valid(std::string(argv[1])))
	{
		std::cerr << "Incorrect host! Example: 127.0.0.1" << std::endl;
		ret_flag = false;
	}
	if (!is_port_valid(std::string(argv[2])))
	{
		std::cerr << "Incorrect port! Example: 8080 or 80" << std::endl;
		ret_flag = false;
	}
	return (ret_flag);
}

//TODO: проверка корректности сообщения для отправки:
// отправлять сообщение только если оно не пустое
int main(int argc, char **argv)
{
	std::string host;
	std::string port;
	std::string flag;

	std::cout << "Hello, World!" << std::endl;
	try
	{
		if (argc != 3 && argc != 4)
		{
			std::cerr << "Usage: client <host> <port> or client <host> <port> --statistic" << std::endl;
			return (-1);
		}
		if (!is_correct_input(argc, argv))
		{
			std::cerr << "Error(" << std::endl;
			return (-1);
		}
		host = argv[1];
		port = argv[2];
		if (argc == 4)
			flag = argv[3];

		Client client = Client();
		client.start_messaging(host, port, flag);
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

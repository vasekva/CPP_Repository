#include "includes/Server.hpp"

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

int main(int argc, char **argv)
{
	io_context context;
	std::uint16_t port = 0;
	std::cout << "Hello, World!" << std::endl;

	try
	{
		if (argc != 2)
		{
			std::cerr << "server <port>\n";
			return (-1);
		}
		if (!is_port_valid(std::string(argv[1])))
		{
			std::cerr << "Incorrect port! Example: 8080 or 80" << std::endl;
			return (-1);
		}
		port = std::atoi(argv[1]);

		Server serv(context, port);
		serv.async_accept();
		context.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << "Exception(main): " << e.what() << "\n";
	}
	return 0;
}
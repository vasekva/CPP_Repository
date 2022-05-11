#include "Server.hpp"

int main(int argc, char **argv)
{
	std::uint16_t port = 0;
	std::string cmd;

	std::cout << "Hello, World!" << std::endl;

	try
	{
		if (argc != 2 && argc != 3)
		{
			std::cerr << "server <port> or server <port> --statistic\n";
			return (-1);
		}
		io_context context;
		port = std::atoi(argv[1]);
		if (argc == 3)
		{
			if (std::string(argv[2]) != "--statistic")
			{
				std::cerr << "server <port> or server <port> --statistic\n";
				return (-1);
			}
			cmd = argv[2];
		}

		Server serv(context, port);
		serv.async_accept();
		context.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

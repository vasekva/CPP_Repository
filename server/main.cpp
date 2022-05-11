#include "Server.hpp"

int main(int argc, char **argv)
{
	std::uint16_t port = 0;

	std::cout << "Hello, World!" << std::endl;

	try
	{
		if (argc != 2)
		{
			std::cerr << "server <port>\n";
			return (-1);
		}
		io_context context;
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

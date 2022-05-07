#include <iostream>

#include "Server.hpp"

#include <boost/asio.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

int main(int argc, char **argv)
{
	std::cout << "Hello, World!" << std::endl;

	try
	{
		if (argc != 2)
		{
			std::cerr << "server <port>\n";
			return (-1);
		}
		io_context context;

		Server serv(context, std::atoi(argv[1]));
		serv.async_accept();
		context.run();
	}
	catch (std::exception const &e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}

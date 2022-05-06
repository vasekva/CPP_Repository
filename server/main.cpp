#include <iostream>

#include "Server.hpp"

#include <boost/asio.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

int main(int argc, char **argv)
{
	std::cout << "Hello, World!" << std::endl;

	Server server = Server();
	server.make_listen();

	return 0;
}

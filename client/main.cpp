
#include "Client.hpp"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

int main(int argc, char **argv)
{
	std::cout << "Hello, World!" << std::endl;

	Client client = Client();
	client.make_connect();

	return 0;
}

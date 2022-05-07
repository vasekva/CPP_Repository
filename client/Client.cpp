//
// Created by Josephine Beregond on 5/6/22.
//

#include "Client.hpp"

void Client::start_messaging(std::string &host, std::string &port)
{
	io_context context;

	tcp::socket sock(context);

	// provides the ability to resolve a query to a list of endpoints
	tcp::resolver resolver_t(context);
	connect(sock, resolver_t.resolve(host, port));

	std::cout << "[Client] Enter a message: ";
	char request[max_length];
	std::cin.getline(request, max_length);
	size_t request_length = std::strlen(request);
	boost::asio::write(sock, boost::asio::buffer(request, request_length));

	char reply[max_length];
	size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, request_length));
	std::cout << "[Client] Reply message is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";
}
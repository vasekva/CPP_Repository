//
// Created by Josephine Beregond on 5/6/22.
//

#include "Client.hpp"

std::vector<char> buff(256);

void ReadHandler(boost::system::error_code ex)
{
	std::cout << " print the buffer data..." << std::endl;
	std::cout << buff.data() << std::endl;
}

void Client::make_connect()
{

	io_service service;
	tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 4000);
	tcp::socket socket(service);

	std::cout << "[Client] Connecting to server..." << std::endl;

	socket.connect(endpoint);
	std::cout << "[Client] Connection successful!" << std::endl;
}
//
// Created by Josephine Beregond on 5/6/22.
//

#include "Server.hpp"

std::vector<char> buff(256);

void SendHandler(boost::system::error_code ex)
{
	std::cout << " do something here" << std::endl;
}

void Server::make_listen()
{
	io_service service;
	tcp::endpoint endpoint(tcp::v4(), 4000);
	tcp::acceptor acceptor(service, endpoint);
	tcp::socket socket(service);

	std::cout << "[Server] Waiting for connection" << std::endl;
	acceptor.accept(socket);

	std::cout << "[Server] Server has accepted a connection" << std::endl;
}
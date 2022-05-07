//
// Created by Josephine Beregond on 5/6/22.
//

#include "Server.hpp"

const int max_length = 1024;

void session(tcp::socket sock)
{
	std::cout << "[Server] Connection has been accepted" << std::endl;
	try
	{
		for (;;)
		{
			char data[max_length];

			boost::system::error_code error;
			size_t length = sock.read_some(boost::asio::buffer(data), error);
			if (length != 0)
			{
				std::cout << "[Server] has received a message: " <<
					data << std::endl;
			}

			if (error == boost::asio::error::eof)
				break;
			else if (error)
				throw boost::system::system_error(error);

			boost::asio::write(sock, boost::asio::buffer(data, length));
		}
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void Server::make_accept(io_context &context, int port)
{
	tcp::endpoint endpoint(tcp::v4(), port);
	tcp::acceptor acceptor(context, endpoint);

	std::cout << "[Server] is listening port " << port << "..." << std::endl;
	for (;;)
	{
		std::thread(session, acceptor.accept()).detach();
	}
}
//
// Created by Josephine Beregond on 5/6/22.
//

#include "Server.hpp"

const int max_length = 1024;


class Session
	: public std::enable_shared_from_this<Session>
{
	public:
		Session(tcp::socket sock)
			: _sock(std::move(sock))
		{}

		void start()
		{
			do_read();
		}

	private:
		void do_read()
		{
			auto self(shared_from_this());
			_sock.async_read_some(boost::asio::buffer(_data, max_length),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::cout << "[Server] has received a message: " <<
						_data << std::endl;
					do_write(length);
				}
			});
		}

		void do_write(std::size_t length)
		{
			auto self(shared_from_this());
			boost::asio::async_write(_sock, boost::asio::buffer(_data, length),
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
					do_read();
			});
		}

		tcp::socket _sock;
		enum { max_length = 1024 };
		char _data[max_length];
};

Server::Server(boost::asio::io_context &context, short port)
	: _acceptor(context, tcp::endpoint(tcp::v4(), port))
{
	do_accept();
}

Server::~Server() {}

void Server::do_accept()
{
	_acceptor.async_accept(
		[this](boost::system::error_code ec, tcp::socket sock)
		{
			if (!ec)
				std::make_shared<Session>(std::move(sock))->start();

			do_accept();
		});
}

//static void session(tcp::socket sock)
//{
//	std::cout << "[Server] Connection has been accepted" << std::endl;
//	try
//	{
//		for (;;)
//		{
//			char data[max_length];
//
//			boost::system::error_code error;
//			size_t length = sock.read_some(boost::asio::buffer(data), error);
//			if (length != 0)
//			{
//				std::cout << "[Server] has received a message: " <<
//					data << std::endl;
//			}
//
//			if (error == boost::asio::error::eof)
//				break;
//			else if (error)
//				throw boost::system::system_error(error);
//
//			boost::asio::write(sock, boost::asio::buffer(data, length));
//		}
//	}
//	catch (std::exception &e)
//	{
//		std::cerr << "Exception in thread: " << e.what() << "\n";
//	}
//}

//void Server::make_accept(io_context &context, int port)
//{
//	tcp::endpoint endpoint(tcp::v4(), port);
//	tcp::acceptor acceptor(context, endpoint);
//
//	std::cout << "[Server] is listening port " << port << "..." << std::endl;
//	for (;;)
//	{
//		std::thread(session, acceptor.accept()).detach();
//	}
//}
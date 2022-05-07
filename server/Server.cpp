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
			auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю

			_sock.async_read_some(boost::asio::buffer(_data, max_length),
			[this, self](boost::system::error_code error, std::size_t length)
			{
				// получение сообщения
				if (!error)
				{
					std::cout << "[Server] client " << _sock.remote_endpoint(error) <<
						" has send a message: " << _data << std::endl;
					do_write(length);
				}
				// проверка но отключение, либо ошибку
				else
				{
					if (error == boost::asio::error::eof)
					{
						std::cout << "[Server] client " << _sock.remote_endpoint(error) <<
							" disconnected" << std::endl;
					}
					else
						std::cerr << error.message() << "\n";
				}
			});
			memset(_data, 0, max_length);
		}

		void do_write(std::size_t length)
		{
			auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю
			boost::asio::async_write(_sock, boost::asio::buffer(_data, length),
			[this, self](boost::system::error_code error, std::size_t length)
			{
				if (!error)
					do_read();
				else
					std::cerr << error.message() << "\n";
			});
		}

		tcp::socket _sock;
		enum { max_length = 1024 };
		char _data[max_length];
};

Server::Server(boost::asio::io_context &context, std::uint16_t  port)
	: _acceptor(context, tcp::endpoint(tcp::v4(), port))
{
}

Server::~Server() {}

void Server::async_accept()
{
	std::cout << "[Server] is waiting a new connection..." << std::endl;
	// ожидает входящие подключения,
	// после этого создает новый объект сеанса и
	// перемещает связанный сокет в новый сеанс,
	// переключаясь на ожидание нового подключения
	_acceptor.async_accept(
		[this](boost::system::error_code error, tcp::socket sock)
		{
			std::cout << "New connection has been accepted!" << std::endl;
			std::cout << "New client is: " << sock.remote_endpoint(error) << std::endl;
			if (!error)
				std::make_shared<Session>(std::move(sock))->start();
			else
				std::cerr << error.message() << "\n";

			async_accept();
		});
}
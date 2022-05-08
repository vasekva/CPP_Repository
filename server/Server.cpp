//
// Created by Josephine Beregond on 5/6/22.
//

#include "Server.hpp"

const int max_length = 1024;

#include <time.h>
class Session
//	: public std::enable_shared_from_this<Session>
{
	public:
		Session(tcp::socket sock, uint16_t uuid)
			:	_sock(std::move(sock)),
			  	_uuid(uuid)
		{}

		void start()
		{
			do_read();
		}

		uint16_t get_uuid() const
		{
			return (this->_uuid);
		}

	private:
		void do_read()
		{
//			auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю

			_sock.async_read_some(boost::asio::buffer(_data, max_length),
//			[this, self](boost::system::error_code error, std::size_t length)
			[this](boost::system::error_code error, std::size_t length)
			{
				// получение сообщения
				if (!error)
				{
					std::cout << "=================[Server]=================\n" << std::endl;
					std::cout << get_curr_time() << std::endl;
					std::cout << "client #" << this->get_uuid() << " "  << _sock.remote_endpoint(error) <<
						" has send a message: " << _data << std::endl;
					std::cout << "==========================================" << std::endl;


					//TODO: удалить, ибо отправлять это же сообщение обратно не нужно будет
					do_write(length);
				}
				// проверка но отключение, либо ошибку
				else
				{
					if (error == boost::asio::error::eof)
					{
						std::cout << "[Server] client #" << this->get_uuid() << " " << _sock.remote_endpoint(error) <<
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
//			auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю
			boost::asio::async_write(_sock, boost::asio::buffer(_data, length),
//			[this, self](boost::system::error_code error, std::size_t length)
			[this](boost::system::error_code error, std::size_t length)
			{
				if (!error)
					do_read();
				else
					std::cerr << error.message() << "\n";
			});
		}

		std::string get_curr_time()
		{
			std::string time_str;
			char outstr[200];
			time_t t;
			struct tm *tmp;
			const char* fmt = "%a, %d %b %y %T %z";

			t = time(NULL);
			tmp = gmtime(&t);
			if (tmp == NULL)
			{
				perror("gmtime error");
				exit(EXIT_FAILURE);
			}

			if (strftime(outstr, sizeof(outstr), fmt, tmp) == 0)
			{
				fprintf(stderr, "strftime returned 0");
				exit(EXIT_FAILURE);
			}
			time_str = outstr;
			return (time_str);
		}

		tcp::socket _sock;
		uint16_t _uuid;
		enum { max_length = 1024 };
		char _data[max_length];
};

Server::Server(boost::asio::io_context &context, std::uint16_t  port)
	: _acceptor(context, tcp::endpoint(tcp::v4(), port)),
	_num_of_clients(0)
{
}

Server::~Server() {}

void Server::async_accept()
{

	//TODO: сделать обработку ошибок в этом методе
	std::cout << "[Server] is waiting a new connection..." << std::endl;
	// ожидает входящие подключения,
	// после этого создает новый объект сеанса и
	// перемещает связанный сокет в новый сеанс,
	// переключаясь на ожидание нового подключения
	_acceptor.async_accept(
		[this](boost::system::error_code error, tcp::socket sock)
		{
			std::cout << "New connection has been accepted!" << std::endl;

			std::string endpoint = boost::lexical_cast<std::string>(sock.remote_endpoint(error));
			std::cout << "New client is: " << endpoint << std::endl;

//			Session *session = new Session(std::move(sock));
			if (!error)
			{
				_sessions.insert(std::make_pair(endpoint,
					new Session(std::move(sock), _num_of_clients + 1)));
				_sessions[endpoint]->start();
				++_num_of_clients;
//				std::shared_ptr<Session> ptr(session)->start();
//				std::make_shared<Session>(std::move(sock))->start();
			}
			else
				std::cerr << error.message() << "\n";

			//TODO: обдумать ключевое значение в map, ибо endpoint очень редко может совпасть,
			// следовательно каждое новое подключение добавляет новый элемент в map, даже
			// если действующих соединений намного меньше
			std::cout << "Number of connections is: " << _sessions.size() << std::endl;

			async_accept();
		});
}
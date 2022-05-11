#include "Server.hpp"

Server::Server(boost::asio::io_context &context, std::uint16_t  port)
	: _acceptor(context, tcp::endpoint(tcp::v4(), port)),
	_db(nullptr),
	_db_error(nullptr)
{
	if (sqlite3_open(DB_NAME, &_db))
		fprintf(stderr, "Open/Create Database error: %s\n", sqlite3_errmsg(_db));
	else if (sqlite3_exec(_db, CREATE_TABLE.c_str(), 0, 0, &_db_error))
	{
		fprintf(stderr, "SQL Error: %s\n", _db_error);
		sqlite3_free(_db_error);
	}
	else
		printf("Database has running successfully!\n");
}

Server::~Server()
{
	sqlite3_close(_db);
}

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
//				boost::uuids::random_generator generator;
//				boost::uuids::uuid uuid = boost::uuids::random_generator()();
				std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
//    			std::cout << uuid << std::endl;
				_sessions.insert(std::make_pair(endpoint,
					new Session(std::move(sock), uuid, _db, _db_error)));
				_sessions[endpoint]->start();
//				std::shared_ptr<Session> ptr(session)->start();
//				std::make_shared<Session>(std::move(sock))->start();
			}
			else
				std::cerr << error.message() << "\n";

			std::cout << "Number of connections for all time: " << _sessions.size() << std::endl;

			async_accept();
		});
}
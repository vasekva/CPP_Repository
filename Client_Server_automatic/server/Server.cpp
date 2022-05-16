#include "includes/Server.hpp"

Server::Server(boost::asio::io_context &context, std::uint16_t port)
	: _acceptor(context, tcp::endpoint(tcp::v4(), port)),
	_db(nullptr),
	_db_error(nullptr)
{
	/** Создание или запуск БД */
	if (sqlite3_open(DB_NAME, &_db))
		fprintf(stderr, "Ошибка Открытия/Создания базы данных: %s\n", sqlite3_errmsg(_db));
	else if (sqlite3_exec(_db, CREATE_TABLE.c_str(), 0, 0, &_db_error))
	{
		fprintf(stderr, "Ошибка SQL_exec: %s\n", _db_error);
		sqlite3_free(_db_error);
	}
	else
		printf("База данных запущена успешно!\n");
}

Server::~Server()
{
	sqlite3_close(_db);
}

/**
 *  Ожидает входящее подключения, после этого создает новый объект сеанса
 *  и перемещает полученный сокет на выполнение в новосозданный объект,
 *  продолжая после этого ожидание нового подключения
 */
void Server::async_accept()
{

	std::cout << "[Server] ожидает новое подключение..." << std::endl;
	_acceptor.async_accept(
		[this](boost::system::error_code error, tcp::socket sock)
		{
			std::cout << "Новое соединение было принято!" << std::endl;
			std::string endpoint = boost::lexical_cast<std::string>(sock.remote_endpoint(error));
			std::cout << "Новый клиент: " << endpoint << std::endl;

			if (!error)
			{
				std::string uuid = boost::lexical_cast<std::string>(boost::uuids::random_generator()());
				_sessions.insert(std::make_pair(endpoint,
					new Session(std::move(sock), uuid, _db, _proto_requests, _proto_responses)));
				_sessions[endpoint]->start();
			}
			else
				std::cerr << error.message() << "\n";

			std::cout << "Кол-во соединений с сервером за все время: " << _sessions.size() << std::endl;

			async_accept();
		});
}
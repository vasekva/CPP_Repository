#include "Server.hpp"

const char *DB_NAME = "webserv_db";
const std::string TABLE_NAME = "Clients";

const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS Clients("
							 "UUID	TEXT NOT NULL, "
							 "Message_time	TEXT NOT NULL, "
							 "X	TEXT NOT NULL, "
							 "Y	TEXT NOT NULL"
							 ");";
//							 "PRIMARY KEY(\"UUID\"));";


class Session
//	: public std::enable_shared_from_this<Session>
{
	public:
		Session(tcp::socket sock, std::string uuid, sqlite3 *db_ptr, char *error)
			:	_sock(std::move(sock)),
			  	_uuid(uuid),
				_db(db_ptr),
				_error(error)
		{}

		void start()
		{
			do_read();
		}

		std::string get_uuid() const
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

					std::cout << "client " << this->get_uuid() << " has send a message: " << _data << std::endl;
					std::string msg = make_db_message(this->get_uuid(), _data);
					std::cout << "Msg for DB: " << msg << std::endl;

					if (sqlite3_exec(_db, msg.c_str(), 0, 0, &_error))
					{
						fprintf(stderr, "SQL Error: %s\n", _error);
						sqlite3_free(_error);
					}
					else
						printf("Data Inserting has done correct!\n");
					std::cout << "==========================================" << std::endl;


//					TODO: удалить, ибо отправлять это же сообщение обратно не нужно будет
					do_write("Try again!\n");
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

		void do_write(std::string msg)
		{
//			auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю
			boost::asio::async_write(_sock, boost::asio::buffer(msg, msg.length()),
//			[this, self](boost::system::error_code error, std::size_t length)
			[this](boost::system::error_code error, std::size_t length)
			{
				if (!error)
					do_read();
				else
					std::cerr << error.message() << "\n";
			});
		}

		std::string make_db_message(std::string uuid, std::string msg)
		{
			std::string time;
			int space_ind = 0;

			space_ind = msg.find_last_of(" ");
			time = &msg[space_ind + 1];
			msg.erase(space_ind, time.length());

			space_ind = msg.find_last_of(" ");
			std::string Y = &msg[space_ind + 1];
			msg.erase(space_ind, Y.length());

			std::string X = msg;
			msg.clear();

			//EXAMPLE: "INSERT INTO Clients VALUES('fsdfsd-wqerwe-1234-twerw', '10:59:46', '-23.5671', '42.1236');";
			std::string ret_msg = "INSERT INTO ";
			ret_msg.append(TABLE_NAME);
			ret_msg.append(" VALUES('");

			ret_msg.append(uuid);
			ret_msg.append("', '");
			ret_msg.append(time);
			ret_msg.append("', '");
			ret_msg.append(X);
			ret_msg.append("', '");
			ret_msg.append(Y + "');");

			return (ret_msg);
		}

		tcp::socket _sock;
		std::string _uuid;
		sqlite3		*_db;
		char		*_error;
		enum { max_length = 1024 };
		char _data[max_length];
};

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
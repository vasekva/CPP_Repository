#include "Session.hpp"

Session::Session(tcp::socket sock, std::string uuid, sqlite3 *db_ptr, char *error)
		:	_sock(std::move(sock)),
			 _uuid(uuid),
			 _db(db_ptr),
			 _db_error(error)
{}

Session::~Session(void) {}

void Session::start()
{
	do_read();
}

std::string Session::get_uuid(void) const
{
	return (this->_uuid);
}

void Session::do_read(void)
{
	// auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю

	_sock.async_read_some(boost::asio::buffer(_data, max_length),
	// [this, self](boost::system::error_code error, std::size_t length)
	[this](boost::system::error_code error, std::size_t length)
	{
		// получение сообщения
		if (!error)
		{
			std::cout << "=================[Server]=================\n" << std::endl;

			std::cout << "client " << this->get_uuid() << " has send a message: " << _data << std::endl;
			std::string msg = make_db_message(this->get_uuid(), _data);
//			if (!msg.empty())
//			{
				std::cout << "Msg for DB: " << msg << std::endl;
				if (sqlite3_exec(_db, msg.c_str(), 0, 0, &_db_error))
				{
					fprintf(stderr, "SQL Error: %s\n", _db_error);
					sqlite3_free(_db_error);
				}
				else
					printf("Data Inserting has done correct!\n");
//			}
//			else
//				std::cout << "Incorrect message format from a client!" << std::endl;
			std::cout << "==========================================" << std::endl;
			//	TODO: удалить, ибо отправлять это же сообщение обратно не нужно будет
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

void Session::do_write(std::string msg)
{
	//	auto self(shared_from_this()); // поддерживает жизнь сеанса благодаря умному указателю
	boost::asio::async_write(_sock, boost::asio::buffer(msg, msg.length()),
	//	[this, self](boost::system::error_code error, std::size_t length)
	[this](boost::system::error_code error, std::size_t length)
	{
		if (!error)
			do_read();
		else
			std::cerr << error.message() << "\n";
	});
}

std::string Session::get_stats()
{
	std::cout << "OH YEAH STATS" << std::endl;
}

//
//static bool check_spaces(std::string &msg)
//{
//	int spaces = 0;
//
//	for (int i = 0; i < msg.length(); i++)
//	{
//		if (msg[i] == ' ')
//			spaces++;
//	}
//	return (spaces == 2);
//}
//
//static bool has_valid_symbols(std::string &msg)
//{
//	bool ret_val = true;
//	int	dot_cnt = 0;
//	int colon_cnt = 0;
//
//	for (int i = 0; i < msg.length(); i++)
//	{
//		if (msg[i] != '-' && msg[i] != '.'
//			&& !std::isdigit(msg[i]) && msg[i] != ':')
//		{
//			ret_val = false;
//			break;
//		}
//		if (msg[i] == '.')
//		{
//			if (dot_cnt == 2)
//			{
//				ret_val = false;
//				break;
//			}
//			dot_cnt++;
//		}
//		if (msg[i] == ':')
//		{
//			if (colon_cnt == 2)
//			{
//				ret_val = false;
//				break;
//			}
//			colon_cnt++;
//		}
//	}
//	return (ret_val);
//}
//
//bool is_valid_msg(std::string msg)
//{
//	return (check_spaces(msg));
//}

std::string Session::make_db_message(std::string uuid, std::string msg)
{
	std::string time;
	int space_ind = 0;

//	if (!is_valid_msg(msg))
//		return ("");
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
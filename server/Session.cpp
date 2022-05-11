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

/**
 * Главная функция выполнения взаимодействия с БД
 * 1) make_insert_msg конструирует сообщение для вставки данных в бд
 * 2) полученная команда подается в sqlite3_exec()
 *
 * 3) НЕ СДЕЛАНО - вывод статистики
 * */
bool Session::make_sql_exec(void)
{
	std::string data = _data;
	clear_eof(data);

	if (data.find("--statistic") == std::string::npos)
	{
		std::string msg = make_insert_msg(this->get_uuid(), data);
		if (msg.empty())
		{
			std::cout << "INCORRECT MESSAGE FROM THE CLIENT!" << std::endl;
			return (false);
		}

		std::cout << "Msg for DB: " << msg << std::endl;
		if (sqlite3_exec(_db, msg.c_str(), 0, 0, &_db_error))
		{
			fprintf(stderr, "SQL Error: %s\n", _db_error);
			sqlite3_free(_db_error);
		}
		else
			printf("Data Inserting has done correct!\n");
		std::cout << "==========================================" << std::endl;
		do_write("Try again!\n");
	}
	else
	{
		if (!is_statistic(data))
			return (false);
//		TODO:: get_stats();

		std::cout << "    Getting statistic from the server..." << std::endl;
		get_stats();
//		std::cout << " OOPS, this functionality is not ready yet" << std::endl;
//		std::cout << "                 Try again                " << std::endl;
//		std::cout << "==========================================" << std::endl;
//		do_read();
	}
	return (true);
}

void Session::do_read(void)
{
	_sock.async_read_some(boost::asio::buffer(_data, max_length),
		[this](boost::system::error_code error, std::size_t length)
		{
			/** Выполнение обработки полученного сообщения */
			if (!error)
			{
				std::cout << "=================[Server]=================\n" << std::endl;
				std::cout << "client " << this->get_uuid() << " has send a message: " << _data << std::endl;

				/** Выполнение взаимодействия с БД */
				if (std::string(_data).empty() || !make_sql_exec())
				{
					if (std::string(_data).empty())
						std::cout << "Message from the client is empty!!" << std::endl;
					_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
					_sock.close();
					return ;
				}
			}
			/** проверка на отключение, либо ошибку */
			else
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "[Server] client #" << this->get_uuid() << " " << _sock.remote_endpoint(error) <<
							" disconnected" << std::endl;
				}
				else
				{
					std::cerr << error.message() << "\n";
					_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
					_sock.close();
				}
			}
		});
	/** обнуление буфера принятого сообщения */
	memset(_data, 0, max_length);
}

void Session::do_write(std::string msg)
{
	boost::asio::async_write(_sock, boost::asio::buffer(msg, msg.length()),
		[this](boost::system::error_code error, std::size_t length)
		{
			if (!error)
				do_read();
			else
				std::cerr << error.message() << "\n";
		});
}

int select_callback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
	auto *records = static_cast<std::vector<std::vector<std::string> > *>(p_data);
	try
	{
		records->emplace_back(p_fields, p_fields + num_fields);
	}
	catch (...)
	{
		// abort select on failure, don't let exception propogate thru sqlite3 call-stack
		return (1);
	}
	return (0);
}

std::vector<std::vector<std::string> > select_stmt(const std::string stmt, sqlite3 *db)
{
	std::vector<std::vector<std::string> > records;
	char *errmsg;

	int ret = sqlite3_exec(db, stmt.c_str(), select_callback, &records, &errmsg);
	if (ret != SQLITE_OK)
	{
		std::cerr << "Error in select statement " << stmt << "[" << errmsg << "]\n";
	}
	else
	{
		std::cerr << records.size() << " records returned.\n";
	}

	return (records);
}

// TODO: метод для реализации функционала статистики
std::string Session::get_stats()
{
	std::cout << "OH YEAH STATS" << std::endl;
	std::vector<std::vector<std::string> > records = select_stmt("SELECT * FROM " + TABLE_NAME, _db);
	// 0 - UUID
	// 1 - TIME
	// 2 - X
	// 3 - Y

	for (int i = 0; i < records.size(); i++)
	{
		std::cout << records[i][3] << std::endl;
	}

	// Получить список UUID
	// Сделать цикл по UUID
	// Получение последнего сообщения связанного с UUID
	// 1) Выборка и суммированние всех X значений данного пользователя,
	// где время в промежутке (lst.msg time - 1min)
	// 2) Выборка и суммированние всех Y значений данного пользователя,
	// где время в промежутке (lst.msg time - 1min)
	// 3) Выборка и суммированние всех X значений данного пользователя,
	// где время в промежутке (lst.msg time - 5min)
	// 4) Выборка и суммированние всех Y значений данного пользователя,
	// где время в промежутке (lst.msg time - 5min)

	return ("");
}

/**
 *  Конструирует INSERT команду для вставки данных в бд
 *  путем вытаскивания из 'msg' данных для отдельных столбцов,
 *  связывая полученные отдельные элементы в единое сообщение
 */
std::string Session::make_insert_msg(std::string uuid, std::string msg)
{
	int space_ind = 0;
	std::string time;
	std::string Y;
	std::string X;

	/** CHECK SYNTAX */
	if (!is_valid_msg(msg))
		return ("");

	/** Message_time */
	space_ind = msg.find_last_of(" ");
	time = &msg[space_ind + 1];
	msg.erase(space_ind, time.length() + 1); // "-85.2232 27.1889 05:04:14" --> "-85.2232 27.1889"
	/** Y */
	space_ind = msg.find_last_of(" ");
	Y = &msg[space_ind + 1];
	msg.erase(space_ind, Y.length() + 1); // "-85.2232 27.1889" --> "-85.2232"
	/** X */
	X = msg;
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
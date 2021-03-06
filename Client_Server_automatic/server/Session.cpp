#include "includes/Session.hpp"

Session::Session(tcp::socket sock, std::string uuid, sqlite3 *db_ptr,
	protobuf_pack::Request &requests, protobuf_pack::Response &responses)
		:	_sock(std::move(sock)),
			 _uuid(uuid),
			 _db(db_ptr),
			 _proto_requests(requests),
			 _proto_responses(responses)
{
	_db_error = nullptr;
}

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
 * 1) сериализация входной строки от клиента.
 * 2) make_insert_msg конструирует сообщение для вставки данных в бд
 * 3) полученная команда подается в sqlite3_exec()
 *
 * 4) Конструирует сообщение статистики для клиента путем вызова
 * get_stats() и отправляет сообщение клиенту, закрывая соединение.
 * */
bool Session::make_sql_exec(void)  /** Cериализация */
{
	boost::system::error_code error;
	std::string data = _data;
	clear_eof(data);

	_proto_requests.add_request_msg(_data);
	std::fstream output = std::fstream("data.bin", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if (!_proto_requests.SerializeToOstream(&output))
	{
		std::cerr << "Ошибка записи данных запроса." << std::endl;
	}

	if (data.find("--statistic") == std::string::npos)
	{
		std::string msg = make_insert_msg(this->get_uuid(), data);
		if (msg.empty())
		{
			std::cout << "Неверный формат сообщения от пользователя!" << std::endl;
			return (false);
		}

		std::cout << "Сообщение для посылки в БД: " << msg << std::endl;
		if (sqlite3_exec(_db, msg.c_str(), 0, 0, &_db_error))
		{
			fprintf(stderr, "Ошибка SQL_exec: %s\n", _db_error);
			sqlite3_free(_db_error);
		}
		else
			printf("Вставка данных прошла успешно!\n");
		std::cout << "==========================================" << std::endl;
		do_write("Try again!\n");
	}
	else
	{
		if (!is_statistic(data))
			return (false);

		std::cout << "Получение информации с базы данных..." << std::endl;
		std::string statistic = get_stats();
		if (statistic.empty())
			throw std::runtime_error("make_sql_exec() error: Строка статистики пустая!");
		std::cout << "Отправка информации клиенту..." << std::endl;
		do_write(statistic);
		_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
		_sock.close();
	}
	return (true);
}

void Session::do_read(void)
{
	_sock.async_read_some(boost::asio::buffer(_data, max_length),
		[this](boost::system::error_code error, std::size_t length)
		{
			/* Выполнение обработки полученного сообщения */
			if (!error)
			{
				std::cout << "=================[Server]=================\n" << std::endl;
				std::cout << "Клиент " << this->get_uuid() << " отправил сообщение: " << _data << std::endl;

				/** Выполнение взаимодействия с БД */
				if (std::string(_data).empty() || !make_sql_exec())
				{
					if (std::string(_data).empty())
						std::cout << "Сообщение от клиента пустое!!" << std::endl;
					_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
					_sock.close();
					return ;
				}
			}
			/* проверка на отключение, либо ошибку */
			else
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "[Server] клиент #" << this->get_uuid() << " " << _sock.remote_endpoint(error) <<
							" отключился" << std::endl;
				}
				else
				{
					std::cerr << error.message() << "\n";
					_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
					_sock.close();
				}
			}
		});
	/* обнуление буфера принятого сообщения */
	memset(_data, 0, max_length);
}

/**
 * Отправление данных клиенту и сериализация
 * */
void Session::do_write(std::string msg)
{
	_proto_responses.add_response_msg(msg);

	std::fstream output = std::fstream("data.bin", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if (!_proto_responses.SerializeToOstream(&output))
	{
		std::cerr << "Ошибка записи данных ответа." << std::endl;
	}
	output.close();

	boost::asio::async_write(_sock, boost::asio::buffer(msg, msg.length()),
		[this](boost::system::error_code error, std::size_t length)
		{
			if (!error)
				do_read();
			else
				std::cerr << error.message() << "\n";
		});
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
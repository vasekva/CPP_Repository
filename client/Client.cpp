#include "Client.hpp"

static int current_time_nanoseconds()
{
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	return (tm.tv_nsec);
}

std::string Client::get_curr_time() const
{
	std::time_t now = std::time(0);
	std::tm* now_tm = std::gmtime(&now);
	char buf[42];
	std::strftime(buf, 42, "%X", now_tm);
	return (buf);
}

double Client::get_random_value(std::string type) const
{
	double	d_val = 0.0;
	int 	delay = 0;

	boost::random::mt19937 defEngine(current_time_nanoseconds());
	if (type == "DOUBLE")
	{
		std::uniform_real_distribution<double> dblDistro(-90.0, 90.0); // uniform_real_distribution for double
		d_val = dblDistro(defEngine);
		return (d_val);
	}
	else
	{
		boost::random::uniform_int_distribution<int> dblDistro(5, 30);
		delay = dblDistro(defEngine);
		std::cout << "Delay: " << delay << " sec" << std::endl;
		return (delay);
	}
}

/**
 *  Метод в 'бесконечном' цикле сначала получает 3 значения:
 *  1) double в диапазоне от -90 до 90 от get_random_value(DOUBLE)
 *  2) значение задержки в диапазоне от 5 до 30 от get_random_value(INTEGER)
 *  3) double в диапазоне от -90 до 90 от get_random_value(DOUBLE)
 *  После конструируется сообщение для отправки серверу из значений
 *  пунктов 1, 3 и строки из get_curr_time(), после чего сообщение
 *  отсылается серверу и клиент ожидает ответного сообщения
 */
void Client::loop_messaging(tcp::socket sock)
{
	std::string rqst_msg;
	std::string X;
	std::string Y;

	while (1)
	{
		X = std::to_string(get_random_value("DOUBLE"));
		X.resize(X.length() - 2); // изменение точности double с 6 до 4 знаков
		sleep((int)get_random_value("INTEGER"));
		Y = std::to_string(get_random_value("DOUBLE"));
		Y.resize(Y.length() - 2); // изменение точности double с 6 до 4 знаков

		rqst_msg = X; // 23.1352
		rqst_msg.append(" ");
		rqst_msg.append(Y); // -52.1234
		rqst_msg.append(" ");
		rqst_msg.append(get_curr_time()); // 15:23:43
		boost::asio::write(sock, boost::asio::buffer(rqst_msg, rqst_msg.length()));

		char reply[max_length];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 11));
		if (reply_length == 0)
			break;
		if (reply[reply_length - 1] != '\n')
			std::cout << "\n";
		std::cout << "[Client] Reply message is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}

void Client::show_statistic(tcp::socket sock)
{
}

void Client::start_messaging(std::string &host, std::string &port, std::string &flag)
{
	io_context context;
	tcp::socket sock(context);

	// provides the ability to resolve a query to a list of endpoints
	tcp::resolver resolver_t(context);
	connect(sock, resolver_t.resolve(host, port));

	loop_messaging(std::move(sock));
	//TODO: if (!flag.empty()) -> show_statistic(sock);
}
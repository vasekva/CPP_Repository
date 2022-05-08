//
// Created by Josephine Beregond on 5/6/22.
//

#include "Client.hpp"

#include <ctime>
#include <random>

double Client::get_random_double()
{
	double value;

	std::default_random_engine defEngine(time(0));
	std::uniform_real_distribution<double> dblDistro(-90.0, 90.0); // uniform_real_distribution for double
	value = dblDistro(defEngine);
	return (value);
}

int Client::get_random_delay()
{
	int delay;

	std::default_random_engine defEngine(time(0));
	std::uniform_int_distribution<int> dblDistro(5, 30); // uniform_int_distribution for int
	delay = dblDistro(defEngine);
	std::cout << "Delay: " << delay << " sec";
	return (delay);
}

void Client::start_messaging(std::string &host, std::string &port)
{
	io_context context;

	tcp::socket sock(context);

	// provides the ability to resolve a query to a list of endpoints
	tcp::resolver resolver_t(context);
	connect(sock, resolver_t.resolve(host, port));

	//TODO: отключение, если сервер закрылся
	std::string rqst_msg;
	while (1)
	{
		int delay = get_random_delay();
		sleep(delay);

		rqst_msg = std::string("value: ");
		rqst_msg.append(std::to_string(get_random_double()));
		rqst_msg.append(" delay: ");
		rqst_msg.append(std::to_string(delay));
		boost::asio::write(sock, boost::asio::buffer(rqst_msg, rqst_msg.length()));

		char reply[max_length];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, rqst_msg.length()));
		if (reply[reply_length - 1] != '\n')
			std::cout << "\n";
		std::cout << "[Client] Reply message is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}
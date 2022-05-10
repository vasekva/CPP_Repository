//
// Created by Josephine Beregond on 5/6/22.
//

#include "Client.hpp"

#include <ctime>
#include <random>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

double Client::get_random_double()
{
	double value = 0.0;

	std::default_random_engine defEngine(time(0));
	std::uniform_real_distribution<double> dblDistro(-90.0, 90.0); // uniform_real_distribution for double
	value = dblDistro(defEngine);
	return (value);
}

int current_time_nanoseconds()
{
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec;
}

int Client::get_random_delay()
{
	int delay = 0;

	boost::random::mt19937 defEngine(current_time_nanoseconds());
	boost::random::uniform_int_distribution<> dblDistro(5,30);

	delay =  dblDistro(defEngine);

//	for(unsigned int i=0; i<dice_rolls; i++)
//	{
//		std::cout << six(rng) << std::endl;
//	}
//
//	std::default_random_engine defEngine(time(0));
//	std::uniform_int_distribution<int> dblDistro(5, 30); // uniform_int_distribution for int
//	delay = dblDistro(defEngine);
	std::cout << "Delay: " << delay << " sec\n";
	return (delay);
}

void Client::start_messaging(std::string &host, std::string &port)
{
	io_context context;

	tcp::socket sock(context);

	// provides the ability to resolve a query to a list of endpoints
	tcp::resolver resolver_t(context);
	connect(sock, resolver_t.resolve(host, port));

	std::string rqst_msg;
	double x = 0.0;
	double y = 0.0;
	int delay = 0;
	while (1)
	{
		x = get_random_double();
		delay = get_random_delay();
		sleep(delay);
		y = get_random_double();

//		rqst_msg = std::string("X:");
		rqst_msg = std::to_string(x);
		rqst_msg.append(" ");
		rqst_msg.append(std::to_string(y));
//		rqst_msg.append(" delay: ");
//		rqst_msg.append(std::to_string(delay));
		boost::asio::write(sock, boost::asio::buffer(rqst_msg, rqst_msg.length()));
//		rqst_msg.clear();

//		std::cout << "[Client] Enter a message: ";
//		char request[max_length];
//		std::cin.getline(request, max_length);
//		size_t request_length = std::strlen(request);
//		boost::asio::write(sock, boost::asio::buffer(request, request_length));

		char reply[max_length];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(reply, 11));
		if (reply[reply_length - 1] != '\n')
			std::cout << "\n";
		std::cout << "[Client] Reply message is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
}
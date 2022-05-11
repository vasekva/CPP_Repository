#ifndef CLIENT_H
#define CLIENT_H

#include <ctime>
#include <random>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

using namespace boost::asio;
using boost::asio::ip::tcp;

class Client
{
	public:
		void start_messaging(std::string &host, std::string &port, std::string &flag);
	private:
		void	show_statistic(tcp::socket sock);
		void	loop_messaging(tcp::socket sock);
		std::string	get_curr_time() const;
		double		get_random_value(std::string type) const;
		enum { max_length = 1024 };
};


#endif

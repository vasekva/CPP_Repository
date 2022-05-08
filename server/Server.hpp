//
// Created by Josephine Beregond on 5/6/22.
//

#ifndef WEBSERV_T_EXERCISE_SERVER_HPP
#define WEBSERV_T_EXERCISE_SERVER_HPP

#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <map>

using namespace boost::asio;
using boost::asio::ip::tcp;

class Session;

class Server
{
	private:
		uint16_t _num_of_clients;
		tcp::acceptor _acceptor;
		std::map<std::string, Session *> _sessions;
	public:
		Server(boost::asio::io_context &context, std::uint16_t  port);
		~Server();

		void async_accept();
//		void make_accept(io_context &context, int port);
};


#endif //WEBSERV_T_EXERCISE_SERVER_HPP

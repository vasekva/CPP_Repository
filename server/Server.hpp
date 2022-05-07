//
// Created by Josephine Beregond on 5/6/22.
//

#ifndef WEBSERV_T_EXERCISE_SERVER_HPP
#define WEBSERV_T_EXERCISE_SERVER_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

using namespace boost::asio;
using boost::asio::ip::tcp;

class Server
{
	private:
		tcp::acceptor _acceptor;
	public:
		Server(boost::asio::io_context &context, std::uint16_t  port);
		~Server();

		void async_accept();
//		void make_accept(io_context &context, int port);
};


#endif //WEBSERV_T_EXERCISE_SERVER_HPP

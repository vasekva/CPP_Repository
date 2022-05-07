//
// Created by Josephine Beregond on 5/6/22.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
using boost::asio::ip::tcp;

class Client
{
	public:
		void start_messaging(std::string &host, std::string &port);

	private:
		enum { max_length = 1024 };

};


#endif

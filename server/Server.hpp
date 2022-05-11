#ifndef WEBSERV_T_EXERCISE_SERVER_HPP
#define WEBSERV_T_EXERCISE_SERVER_HPP

#include <boost/asio.hpp>

/** FOR_WINDOWS
 *
#include <windows.h>
#include <bcrypt.h>
#pragma(lib, "bcrypt.lib")

*/

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>     	  // streaming operators etc.
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <time.h>
#include <memory>
#include <map>

#include "../sqlite/sqlite3.h"

using namespace boost::asio;
using boost::asio::ip::tcp;

class Session;

class Server
{
	private:
		tcp::acceptor _acceptor;
		std::map<std::string, Session *> _sessions;
		sqlite3	*_db;
		char *_db_error;
	public:
		Server(boost::asio::io_context &context, std::uint16_t  port);
		~Server();

		void async_accept();
};


#endif //WEBSERV_T_EXERCISE_SERVER_HPP

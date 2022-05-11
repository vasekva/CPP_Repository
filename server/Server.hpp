#ifndef SERVER_HPP
#define SERVER_HPP

#include "main_header.hpp"
#include "Session.hpp"
#include "db.hpp"

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

#endif

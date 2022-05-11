#ifndef SERVER_HPP
#define SERVER_HPP

#include "main_header.hpp"
#include "Session.hpp"
#include "db.hpp"

class Session;

class Server
{
	private:
		std::map<std::string, Session *> _sessions;
		tcp::acceptor	_acceptor;
		std::string		_cmd;
		char	*_db_error;
		sqlite3	*_db;
	public:
		Server(boost::asio::io_context &context, std::uint16_t port);
		~Server();

		void async_accept();
};

#endif

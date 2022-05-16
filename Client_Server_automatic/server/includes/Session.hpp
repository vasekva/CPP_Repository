#ifndef SESSION_HPP
#define SESSION_HPP

#include <fstream>

#include "main_header.hpp"
#include "db.hpp"

class Session
{
	public:
		Session(tcp::socket sock, std::string uuid, sqlite3 *db_ptr,
			protobuf_pack::Request &requests, protobuf_pack::Response &responses);
		~Session(void);
		void start(void);
	private:
		std::string	get_stats();
		std::string	get_uuid(void) const;
		void 		do_read(void);
		void 		do_write(std::string msg);
		std::string make_insert_msg(std::string uuid, std::string msg);
		bool		make_sql_exec(void);

		protobuf_pack::Request &_proto_requests;
		protobuf_pack::Response &_proto_responses;
		tcp::socket _sock;
		std::string _uuid;
		sqlite3		*_db;
		char		*_db_error;
		enum { max_length = 1024 };
		char _data[max_length];
};

#endif

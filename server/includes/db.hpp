#ifndef DB_HPP
#define DB_HPP

#include <string>

#ifndef DB_NAME
	#define DB_NAME "webserv_db"
#endif

const std::string TABLE_NAME = "Clients";

const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS " + TABLE_NAME + "("
								 "UUID	TEXT NOT NULL, "
								 "Message_time	TEXT NOT NULL, "
								 "X	REAL NOT NULL, "
								 "Y	REAL NOT NULL"
								 ");";

#endif

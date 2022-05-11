#ifndef MAIN_HEADER_HPP
#define MAIN_HEADER_HPP

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

#endif

cmake_minimum_required(VERSION 3.20)
project(Webserv_t_exercise)

message(STATUS "start running cmake...")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(Boost_INCLUDE_DIR H:/Programs/MinGW/include/)
    set(Boost_LIBRARY_DIR H:/Programs/MinGW/lib/)
    #add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
elseif(APPLE)
    set(Boost_INCLUDE_DIR /goinfre/jberegon/.brew/include/)
    set(Boost_LIBRARY_DIR /goinfre/jberegon/.brew/lib/)
    include_directories(${Boost_INCLUDE_DIR})
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
else()
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    set(SOURCE_FILES
            main.cpp
            ../sqlite/sqlite3.c
            Server.cpp
            Session.cpp
            )

    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(Webserv_t_exercise ${SOURCE_FILES})
    target_link_libraries(Webserv_t_exercise ${Boost_LIBRARIES})
    #FOR WINDOWS
    if (WIN32)
        target_link_libraries(Webserv_t_exercise ws2_32 wsock32 -lbcrypt)
    endif()
endif()
cmake_minimum_required(VERSION 3.20)
project(Webserv_t_exercise)

message(STATUS "start running cmake...")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)

#FOR Windows
#set(CMAKE_EXE_LINKER_FLAGS -static)
if (WIN32)
set(Boost_INCLUDE_DIR H:/Programs/MinGW/include/)
set(Boost_LIBRARY_DIR H:/Programs/MinGW/lib/)
#add_definitions(-DBOOST_ALL_DYN_LINK -DBOOST_ALL_NO_LIB)
endif()
#find_package(Boost 1.77.0 COMPONENTS system filesystem REQUIRED) <-- DELETE


#FOR MAC_OS
#set(Boost_INCLUDE_DIR /goinfre/jberegon/.brew/include/)
#set(Boost_LIBRARY_DIR /goinfre/jberegon/.brew/lib/)
#include_directories(${Boost_INCLUDE_DIR})



find_package(Boost COMPONENTS system filesystem REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(SOURCE_FILES
        main.cpp
        ../sqlite/sqlite3.c
        Server.cpp
    )

add_executable(Webserv_t_exercise ${SOURCE_FILES})

if (Boost_FOUND)
    target_link_libraries(Webserv_t_exercise ${Boost_LIBRARIES})
endif()

#FOR WINDOWS
if (WIN32)
    target_link_libraries(Webserv_t_exercise ws2_32 wsock32 -lbcrypt)
endif()
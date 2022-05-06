cmake_minimum_required(VERSION 3.21)
project(Webserv_t_exercise)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_EXE_LINKER_FLAGS -static)
#set(Boost_INCLUDE_DIR "/goinfre/jberegon/.brew/Cellar/boost/1.78.0_1")
#set(Boost_LIBRARY_DIR "/goinfre/jberegon/.brew/Cellar/boost/1.78.0_1")

#set(Boost_INCLUDE_DIR c:/mingw/include/)
#set(Boost_LIBRARY_DIR c:/mingw/lib/)
set(Boost_INCLUDE_DIR /goinfre/jberegon/.brew/include/)
set(Boost_LIBRARY_DIR /goinfre/jberegon/.brew/lib/)
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

set(SOURCE_FILES main.cpp
        Client.cpp
    )

add_executable(Webserv_t_exercise ${SOURCE_FILES})
#add_executable(Webserv_t_exercise main.cpp)
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
#    set(Protobuf_INCLUDE_DIR /goinfre/jberegon/.brew/opt/protobuf/include)
#    set(Protobuf_LIBRARY_DIR /goinfre/jberegon/.brew/opt/protobuf/lib)
    include_directories(${Boost_INCLUDE_DIR})
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)
#find_package(Protobuf REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
else()
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    set(SOURCE_FILES
            main.cpp
            parse_functions.cpp
            get_stats.cpp
            tools.cpp
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

#if (NOT Protobuf_FOUND)
#    message(FATAL_ERROR "Could not find boost!")
#else()
#    message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
#    message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
#    message(STATUS "Protobuf_VERSION: ${Protobuf_VERSION}")
#
#    #include_directories(${CMAKE_CURRENT_BINARY_DIR})
#    include_directories(${Protobuf_INCLUDE_DIR})
#
#    protobuf_generate_cpp(
#            PROTO_SRCS PROTO_HDRS
#            person.proto
#            address.proto
#    )
#
#    set(SOURCE_FILES
#            main.cpp ${PROTO_SRCS} ${PROTO_HDRS}
#            )
#
#    add_executable(untitled1 ${SOURCE_FILES})
#    target_link_libraries(untitled1 ${Protobuf_LIBRARIES})
#endif()
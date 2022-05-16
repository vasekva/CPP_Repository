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
    set(Protobuf_PREFIX_PATH
            "/goinfre/jberegon/.brew/opt/protobuf/include"
            "/goinfre/jberegon/.brew/opt/protobuf/lib"
            "/goinfre/jberegon/.brew/opt/protobuf/bin" )
    list(APPEND CMAKE_PREFIX_PATH "${Protobuf_PREFIX_PATH}")
    include_directories(${Boost_INCLUDE_DIR})
endif()

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Protobuf REQUIRED)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
elseif(NOT_Protobuf_FOUND)
    message(FATAL_ERROR "Could not find protobuf!")
else()
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    message(STATUS "Protobuf_INCLUDE_DIRS: ${Protobuf_INCLUDE_DIRS}")
    message(STATUS "Protobuf_LIBRARIES: ${Protobuf_LIBRARIES}")
    message(STATUS "Protobuf_VERSION: ${Protobuf_VERSION}")

    protobuf_generate_cpp(
            PROTO_SRCS PROTO_HDRS
            requests.proto
            responses.proto
    )

    set(SOURCE_FILES
            main.cpp ${PROTO_SRCS} ${PROTO_HDRS}
            utils/parse_functions.cpp utils/get_stats.cpp utils/statistic_tools.cpp
            ../sqlite/sqlite3.c
            Server.cpp Session.cpp
            )

    include_directories(${Boost_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIR})
    add_executable(Webserv_t_exercise ${SOURCE_FILES})
    target_link_libraries(Webserv_t_exercise ${Boost_LIBRARIES} ${Protobuf_LIBRARIES})
    #FOR WINDOWS
    if (WIN32)
        target_link_libraries(Webserv_t_exercise ws2_32 wsock32 -lbcrypt)
    endif()
endif()
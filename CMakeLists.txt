cmake_minimum_required(VERSION 3.23)
project(chat)

set(CMAKE_CXX_STANDARD 14)

add_executable(chat_server
        server.cpp LengthHeaderCodec.h ChatServer.cpp ChatServer.h)
target_link_libraries(chat_server muduo_net muduo_base pthread)

add_executable(chat_client
        client.cpp ChatClient.cpp ChatClient.h)
target_link_libraries(chat_client muduo_net muduo_base pthread)

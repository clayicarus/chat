//
// Created by clay on 22-10-12.
//

#ifndef CHAT_CHATSERVER_H
#define CHAT_CHATSERVER_H

#include "LengthHeaderCodec.h"
#include <set>
#include <functional>
#include <muduo/net/TcpServer.h>

class ChatServer : muduo::noncopyable {
public:
    ChatServer(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr)
        : server_(loop, listenAddr, "ChatServer"),
        codec_(std::bind(&ChatServer::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;

        server_.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        server_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    }
    void start();
private:
    void onStringMessage(const muduo::net::TcpConnectionPtr &conn,
                         const muduo::string &message,
                         muduo::Timestamp time);
    void onConnection(const muduo::net::TcpConnectionPtr &conn);

    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionSet;
    muduo::net::TcpServer server_;
    LengthHeaderCodec codec_;
    ConnectionSet connections_;
};


#endif //CHAT_CHATSERVER_H

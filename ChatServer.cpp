//
// Created by clay on 22-10-12.
//

#include "ChatServer.h"

void ChatServer::onConnection(const muduo::net::TcpConnectionPtr &conn) {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if(conn->connected()) {
        connections_.insert(conn);
    } else {
        connections_.erase(conn);
    }
}

void ChatServer::onStringMessage(const muduo::net::TcpConnectionPtr &conn, const muduo::string &message,
                                 muduo::Timestamp time)
{
    for(const auto &i : connections_) {
        codec_.send(i.get(), message);
    }
}

void ChatServer::start()
{
    server_.start();
}

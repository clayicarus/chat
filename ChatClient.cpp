//
// Created by clay on 22-10-12.
//

#include "ChatClient.h"
using namespace muduo;

void ChatClient::write(const muduo::StringPiece &message)
{
    MutexLockGuard lock(mutex_);
    if(connection_)
    {
        codec_.send(connection_.get(), message);
    }
}

void ChatClient::connect()
{
    client_.connect();
}

void ChatClient::onConnection(const muduo::net::TcpConnectionPtr &conn)
{
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    MutexLockGuard lock(mutex_);
    if(conn->connected())
    {
        connection_ = conn;
    } else {
        connection_.reset();
    }
}

void ChatClient::onStringMessage(const net::TcpConnectionPtr &conn, const string &message, muduo::Timestamp time)
{
    printf("<<< %s\n", message.c_str());
}

//
// Created by clay on 22-10-12.
//

#ifndef CHAT_CHATCLIENT_H
#define CHAT_CHATCLIENT_H

#include "LengthHeaderCodec.h"
#include <muduo/net/TcpClient.h>
#include <muduo/base/Mutex.h>

class ChatClient : muduo::noncopyable {
public:
    ChatClient(muduo::net::EventLoop *loop, const muduo::net::InetAddress &serverAddr)
        : loop_(loop), client_(loop, serverAddr, "ChatClient"),
          codec_(std::bind(&ChatClient::onStringMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    {
        using std::placeholders::_1;
        using std::placeholders::_2;
        using std::placeholders::_3;
        client_.setConnectionCallback(std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
        client_.enableRetry();
    }

    void connect();

    void disconnect()
    {
        // client_.disconnect();
    }

    void write(const muduo::StringPiece &message);

private:
    void onStringMessage(const muduo::net::TcpConnectionPtr &conn, const muduo::string &message,
                         muduo::Timestamp time);
    void onConnection(const muduo::net::TcpConnectionPtr &conn);

    muduo::net::EventLoop *loop_;
    muduo::net::TcpClient client_;
    LengthHeaderCodec codec_;
    muduo::net::TcpConnectionPtr connection_;
    muduo::MutexLock mutex_;
};


#endif //CHAT_CHATCLIENT_H

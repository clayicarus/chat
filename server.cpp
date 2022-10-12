//
// Created by clay on 22-10-12.
//

#include "ChatServer.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
using namespace std;
using namespace muduo::net;

int main(int argc, char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    if(argc > 1) {
        EventLoop loop;
        uint16_t port = atoi(argv[1]);
        InetAddress serverAddr(port);
        ChatServer server(&loop, serverAddr);
        server.start();
        loop.loop();
    } else {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
    }
    return -1;
}
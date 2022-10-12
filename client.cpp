//
// Created by clay on 22-10-12.
//

#include "ChatClient.h"
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/InetAddress.h>
#include <iostream>
using namespace muduo::net;
using namespace std;

int main(int argc, char *argv[])
{
    LOG_INFO << "pid = " << getpid();
    if(argc > 2)
    {
        EventLoopThread loopThread;
        uint16_t port = atoi(argv[2]);
        InetAddress serverAddr(argv[1], port);

        ChatClient client(loopThread.startLoop(), serverAddr);
        client.connect();
        string line;
        while(getline(cin, line))
        {
            client.write(line);
        }
        client.disconnect();
    } else {
        fprintf(stderr, "Usage: %s host_ip port\n", argv[0]);
    }
    return 0;
}
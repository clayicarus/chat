//
// Created by clay on 22-10-12.
//

#ifndef CHAT_LENGTHHEADERCODEC_H
#define CHAT_LENGTHHEADERCODEC_H

#include <muduo/base/Logging.h>
#include <muduo/net/TcpConnection.h>

class LengthHeaderCodec : muduo::noncopyable {
public:
    typedef std::function<void (const muduo::net::TcpConnectionPtr&,
                                const muduo::string& message,
                                muduo::Timestamp)> StringMessageCallback;
    explicit LengthHeaderCodec(const StringMessageCallback &cb)
        : messageCallback_(cb) {}
    void send(muduo::net::TcpConnection *conn,
              const muduo::StringPiece &message)
    {
         muduo::net::Buffer buf;
         buf.append(message.data(), message.size());
         int32_t len = static_cast<int32_t>(message.size());
         int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
         buf.prepend(&be32, sizeof be32);
         conn->send(&buf);
    }
    void onMessage(const muduo::net::TcpConnectionPtr &conn,
                   muduo::net::Buffer *buf,
                   muduo::Timestamp receiveTime)
    {
        while(buf->readableBytes() >= kHeaderLen) {
            // FIXME: use Buffer::peekInt32()
            const void *data = buf->peek();
            int32_t be32 = *static_cast<const int32_t *>(data);
            const int32_t len = muduo::net::sockets::networkToHost32(be32);
            if(len > 65536 || len < 0) {
                LOG_ERROR << "Invalid length " << len;
                // conn->shutdown();   // FIXME: disable reading
                conn->forceClose();
                buf->retrieveAll();
                break;
            } else if (buf->readableBytes() >= len + kHeaderLen) {
                buf->retrieve(kHeaderLen);
                muduo::string message(buf->peek(), len);
                messageCallback_(conn, message, receiveTime);
                buf->retrieve(len);
            } else {
                break;
            }
        }
    }
private:
    static const size_t kHeaderLen = sizeof(int32_t);
    StringMessageCallback messageCallback_;
};


#endif //CHAT_LENGTHHEADERCODEC_H

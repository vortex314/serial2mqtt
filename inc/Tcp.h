#ifndef TCP_H
#define TCP_H

#include "Bytes.h"
#include "Actor.h"
#include "BufferedByteStream.h"

class Tcp : public Actor,public BufferedByteStream
{
private:
    int _sockfd;
    bool _connected;
    const char* _host;
    uint16_t _port;
public:

    static int CONNECTED,DISCONNECTED,RXD,MESSAGE,FREE,ERROR;
    Tcp( const char *host,uint16_t port);
    void setHost(const char* host);
    void setPort(uint16_t port);
    ~Tcp();
    void onEvent(Header);

    Erc connect();
    Erc disconnect();
    bool isConnected();

    Bytes* recv();
    uint8_t read();
    bool hasData();
    Erc send(Bytes& pb);
    Erc open();
    Erc close();
    void flush();

    void mqttRead(int32_t b);
    int fd();
};





#endif // TCP_H

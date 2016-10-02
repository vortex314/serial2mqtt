#ifndef USB_H
#define USB_H

// #include <Sequence.h>
#include "Actor.h"
#include "BufferedByteStream.h"


class Usb :public Actor,public BufferedByteStream
{

public:
    const static int CONNECTED,DISCONNECTED,RXD,ERROR,MESSAGE,FREE;

    Usb(const char* device) ;
    Erc connect() ;
    Erc disconnect() ;
    Erc send(Bytes& bytes) ;
    Bytes* recv() ;


    void setDevice(const char* device) ;
    void setBaudrate(uint32_t baud);
    void logStats();
    void onEvent(Header);
    int fd();
    Erc open();
    Erc close();
        bool hasData();
            uint8_t read() ;
private :


    int _fd;
    const char* _device;
    uint32_t _baudrate;
    bool _isComplete;

    void flush();
};





#endif // USB_H

#include "Tcp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include "Logger.h"

extern Tcp tcp;

void SigPipeHandler(int signal_number){
//    MsgQueue::publish(&tcp,SIG_DISCONNECTED);
        LOGF(  "SIGPIPE detected " );
}

Tcp::Tcp( const char *host,uint16_t port) : Actor("TCP"),BufferedByteStream(256)
{
    _host=host;
    _port=port;
    _connected=false;
};

void Tcp::setHost(const char* host)
{
    _host=host;
}

void Tcp::setPort(uint16_t port)
{
    _port=port;
}

Tcp::~Tcp()
{
}

bool Tcp::isConnected()
{
    return _connected;
}

Erc Tcp::open()
{
    LOGF( " connect() to %s:%d " , _host , _port );
    struct sockaddr_in serv_addr;
    struct hostent *server;
    /* Create a socket point */
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
    {
        LOGF("socket() errno : %d",errno);
        return E_INVAL;
    }
    server = gethostbyname(_host);
    if (server == NULL)
    {
        LOGF("gethostbyname() errno : %d",errno);
        return E_NOT_FOUND;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(_port);

    /* Now connect to the server */
    if (::connect(_sockfd,(const sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        LOGF(" socket connect() errno : %d",errno);
        return E_CONN_LOSS;
    }
    _connected=true;
    LOGF("connect() connected to %s:%d ",_host , _port);
    signal(SIGPIPE, SigPipeHandler);
//    MsgQueue::publish(this,SIG_CONNECTED);

//    signal(SIGPIPE, sigTcpHandler);;
    return E_OK;
}

Erc Tcp::close()
{
    ::close(_sockfd);
    _connected=false;
    _sockfd = 0;
//    MsgQueue::publish(this,SIG_DISCONNECTED);
    return E_OK;
}

void Tcp::flush(){
}



#include <cstdio>
uint8_t Tcp::read()
{
    int32_t n;
    uint8_t b;
    n=::read(_sockfd,&b,1) ;
    if (n <= 0)
    {
        close();
        LOGF("read() errno : %d : %s ",errno,strerror(errno));
        return -1;
    }
    return b;
}



Erc Tcp::send(Bytes& bytes)
{
    int n;
   signal(SIGPIPE, SigPipeHandler);     // will be detected in n < 0
//   Log::log().message("TCP send : " ,bytes);
    n=::write(_sockfd,bytes.data(),bytes.length()) ;
    if (n < 0)
    {
         LOGF("write() errno : %d : %s ",errno,strerror(errno));
        close();
        return E_CONN_LOSS;
    }
    return E_OK;
}

#include <sys/ioctl.h>
bool Tcp::hasData()
{
    int count;
    int rc = ioctl(_sockfd, FIONREAD, (char *) &count);
    if (rc < 0  )
    {
         LOGF("ioctl() errno : %d : %s ",errno,strerror(errno));
        close();
        return false;
    }
    return true;
}





int Tcp::fd()
{
    if ( _connected )
        return _sockfd;
    else
        return 0;
}




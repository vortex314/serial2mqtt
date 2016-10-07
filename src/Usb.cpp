#include "Usb.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct
{
    uint32_t baudrate;
    uint32_t symbol;
} BAUDRATE;
BAUDRATE BAUDRATES[]=
{
    {50,  B50	},
    {75, B75	},
    {110,  B110	},
    {134,  B134	},
    {150,  B150	},
    {200,  B200	},
    {300,  B300	},
    {600,  B600	},
    {1200,  B1200	},
    {1800,  B1800	},
    {2400,  B2400	},
    {4800,  B4800	},
    {9600,  B9600	},
    {19200,  B19200	},
    {38400,  B38400	},
    {57600,  B57600   },
    {115200,  B115200  },
    {230400,  B230400  },
    {460800,  B460800  },
    {500000,  B500000  },
    {576000,  B576000  },
    {921600,  B921600  },
    {1000000,  B1000000 },
    {1152000,  B1152000 },
    {1500000,  B1500000 },
    {2000000,  B2000000 },
    {2500000,  B2500000 },
    {3000000,  B3000000 },
    {3500000,  B3500000 },
    {4000000,  B4000000 }
};

Usb::Usb(const char* device) :  Actor("USB"),BufferedByteStream(256)
{
    _device =  device;
//    isConnected(false);
    _fd=0;
//   restart();
    _isComplete = false;
}

void Usb::setDevice(const char* device)
{
    _device =  device;
}

void Usb::setBaudrate(uint32_t baudrate)
{
    _baudrate =  baudrate;
}

uint32_t baudSymbol(uint32_t value)
{
    for(uint32_t i=0; i<sizeof(BAUDRATES)/sizeof(BAUDRATE); i++)
        if ( BAUDRATES[i].baudrate == value) return BAUDRATES[i].symbol;
    LOGF("connect: baudrate %s  not found, default to 115200.",value);
    return B115200;
}

void Usb::flush()
{
}

Erc Usb::open()
{
    struct termios options;
    LOGF( "Connecting to %s ...." , _device  );

    _fd = ::open(_device, O_EXCL | O_RDWR | O_NOCTTY | O_NDELAY);

    if (_fd == -1)
    {
        LOGF("connect: Unable to open %s errno : %d : %s ",_device,errno,strerror(errno));
        _fd=0;
        return E_AGAIN;
    }
    fcntl(_fd, F_SETFL, FNDELAY);

    tcgetattr(_fd, &options);
    cfsetispeed(&options, baudSymbol(_baudrate));
    cfsetospeed(&options, baudSymbol(_baudrate));


    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |=  CS8;
    options.c_cflag &= ~CRTSCTS;               /* Disable hardware flow control */


    options.c_lflag &= ~(ICANON | ECHO | ISIG);
    cfmakeraw(&options);

    tcsetattr(_fd, TCSANOW, &options);
    LOGF("set baudrate to ",_baudrate);
    LOGF( "open %s succeeded." , _device );
//    isConnected(true);
//   MsgQueue::publish(this,SIG_CONNECTED);
    return E_OK;
}
#include <linux/serial.h>
#include <sys/ioctl.h>
void Usb::logStats()
{
    struct serial_icounter_struct counters;
    int rc = ioctl(_fd, TIOCGICOUNT, &counters);
    if ( rc < 0 )
    {
        LOGF("ioctl(TIOCGICOUNT) failed errno : %d : %s ",errno,strerror(errno));

        return;
    }
    LOGF(" overrun : %d",counters.buf_overrun);
}

Erc Usb::close()
{
//    isConnected(false);
    _isComplete=false;
    ::close(_fd) ;
    _fd=0;
//    MsgQueue::publish(this,SIG_closeED);
    return E_OK;
}

Erc Usb::send(Bytes& bytes)
{
//   _outBuffer.clear();
//   _outBuffer.append(bytes);
    LOGF("send() ");// << bytes;
    uint8_t buffer[100];
    uint32_t i=0;
    while ( i< sizeof(buffer) && hasData()) {
        buffer[i++]=read();
    }

    uint32_t count = ::write(_fd,buffer,i);
    if ( count != i)
    {
        close();
        LOGF("send() failed errno : %d : %s ",errno,strerror(errno));
        return E_AGAIN;
    }
    return E_OK;
}

uint8_t Usb::read()
{
    uint8_t b;
    int erc = ::read(_fd,&b,1);
    if (erc<0)
    {
        LOGF(" read() : %d %s",errno,strerror(errno));
        close();
        return 0;
    }
    return b;
    return 0;
}

#include <sys/ioctl.h>
bool Usb::hasData()
{
    int count;
    int rc = ioctl(_fd, FIONREAD, (char *) &count);
    if (rc < 0)
    {
        perror("ioctl() ");

        close();
        return 0;
    }
    return count;
}

void Usb::onEvent(Cbor& cbor)
{
    /*
        uint8_t b;
        uint32_t i;
        uint32_t count;

        if ( msg.is(os,SIG_ERC,fd(),0))
        {
            logger.level(Logger::WARN) << " error occured. Reconnecting.";
            logger.flush();
            close();
    //        connect();
            return 0;
        }
        PT_BEGIN ( );
        while(true)
        {
            PT_YIELD_UNTIL ( msg.is(this,SIG_CONNECTED));
            while( true )
            {
                PT_YIELD_UNTIL(msg.is(os,SIG_RXD,fd(),0)|| msg.is(os,SIG_ERC,fd(),0));//event->is(RXD) || event->is(FREE) || ( inBuffer.hasData() && (_isComplete==false)) );
                if ( msg.is(os,SIG_RXD,fd(),0)  &&  hasData())
                {
                    count =hasData();
                    for(i=0; i<count; i++)
                    {
                        b=read();
                        inBuffer.write(b);
                    }
                    logger.level(Logger::DEBUG)<< "recvd: " << inBuffer.size() << " bytes.";
                    logger.flush();
                    while( inBuffer.hasData() )
                    {
                        if ( _inBytes.Feed(inBuffer.read()))
                        {
                            Str l(256);
                            _inBytes.toString(l);
                            logger.level(Logger::DEBUG)<< "recv : " << l;
                            logger.flush();
                            _inBytes.Decode();
                            if ( _inBytes.isGoodCrc() )
                            {
                                _inBytes.RemoveCrc();
                                Str l(256);
                                _inBytes.toString(l);
                                logger.level(Logger::INFO)<<" recv clean : " <<l;
                                logger.flush();

                                MqttIn* _mqttIn=new MqttIn(256);
                                _inBytes.offset(0);
                                while(_inBytes.hasData())
                                    _mqttIn->Feed(_inBytes.read());

                                if ( _mqttIn->parse())
                                {
                                    MsgQueue::publish(this,SIG_RXD,_mqttIn->type(),_mqttIn); // _mqttIn will be deleted by msg process
                                }
                                else
                                {
                                    Sys::warn(EINVAL, "MQTT");
                                    delete _mqttIn;
                                }
                            }
                            else
                            {
                                logger.level(Logger::WARN)<<"Bad CRC. Dropped packet. ";
                                logger.flush();
                                _inBytes.clear(); // throw away bad data
                            }
                            _inBytes.clear();
                        }
                    }
                }
                else if ( msg.is(os,SIG_ERC,fd(),0) )
                {
                    _inBytes.clear();
                    break;
                }

                PT_YIELD ( );
            }

        }

        PT_END (  );*/
}


int Usb::fd()
{
    return _fd;
}









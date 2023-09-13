/*
 * ByteBuffer.h
 *
 *  Created on: 24-aug.-2012
 *      Author: lieven
 */

#ifndef BYTES_H_
#define BYTES_H_
//#include "base.h"
#include <stdint.h>

#include "Erc.h"
#include "Sys.h"
// #include "Str.h"
#include <assert.h>

#define MAX_BUFFER_SIZE 512

class Str;

class Bytes {
  public:
    Bytes();
    Bytes(uint32_t size);
    Bytes(uint8_t* start, uint32_t size);
    Bytes& map(uint8_t* start, uint32_t size);
    Bytes(const Bytes& in);
    ~Bytes();
    Bytes& clone(Bytes& src);
    //    void map(uint8_t *start, uint32_t size);
    Bytes& sub(Bytes* parent, uint32_t length);
    Bytes& copy(Bytes* from);
    Bytes& operator=( Bytes& src);
        Bytes& operator=( const Bytes& src);
    Bytes& operator=(const char* s);
    Bytes& append( Bytes& src);
    Bytes& append(const char s[]);
    Bytes& append(uint8_t* data, uint32_t length);

    int capacity();
    uint32_t length() const;
    int length(int l);
    int available();
    int offset(int32_t offset);
    int offset();
    Bytes& move(int32_t distance);
    Erc insert(uint32_t offset, Bytes& data);
    Erc insert(uint32_t offset, uint8_t* data, uint32_t length);
    uint8_t* data() const;

    int poke(uint32_t offset, uint8_t b);
    int peek(int32_t offset);
    int peek();
    int used();
    bool seek(uint8_t b);

    bool hasData();
    bool hasData(uint32_t size);
    bool hasSpace(uint32_t size);
    bool equals(const uint8_t* pb, uint32_t length);

    uint8_t read();

    Erc read(uint8_t* data);
    inline Erc write(uint8_t value) {
        if (_offset < _capacity) {
            _start[_offset++] = value;
            _limit = _offset;
        } else
            return E_LACK_RESOURCE;
        return 0;
    };
    Erc write(uint8_t* data, int offset, int length);
    Erc write(Bytes* data);
    Bytes& clear();

    void AddCrc();
    void RemoveCrc();
    static uint16_t Fletcher16(uint8_t* begin, int length);
    void Encode();
    void Decode();
    bool isGoodCrc();
    void Frame();
    //	bool Feed(uint8_t b);

    const char* toString(Str& str);
    const char* toHex(Str& str);
    bool fromHex(Str& str);

  public:
    uint8_t* _start;
    uint32_t _limit;
    uint32_t _offset;
    uint32_t _capacity;
    bool isMemoryOwner;
};

#endif /* BYTEBUFFER_H_ */

/*
 * CircBuf.h
 *
 *  Created on: 19-aug.-2012
 *      Author: lieven
 */

#ifndef CIRCBUF_H_
#define CIRCBUF_H_
//#include "base.h"
#include <stdint.h>
#include <errno.h>
#include <Sys.h>

#define POW 4
#define CIRCBUF_SIZE (1<<POW)
#define CIRCBUF_MASK (CIRCBUF_SIZE-1)

class CircBuf {
private:
	uint8_t* start;
	uint64_t readPos;
	uint64_t writePos;
	uint32_t limit;
public: 
	CircBuf(uint32_t size);
	~CircBuf();
	int write(uint8_t b);
	int writeFromIsr(uint8_t b);
	int read();
	int readFromIsr();
	bool hasSpace();
	bool hasSpace(uint32_t size);
	bool hasData();
	void clear();
	uint32_t size();
	uint32_t space();
};

#endif /* CIRCBUF_H_ */

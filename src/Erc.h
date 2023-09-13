/*
 * Erc.h
 *
 *  Created on: 28-aug.-2013
 *      Author: lieven2
 */

#ifndef ERC_H_
#define ERC_H_
#include <stdint.h>
#include <errno.h>
//#include "Sys.h"
#define E_OK				0
#define E_INVAL 			(1<<0)		// Inavlid arguments
#define E_LACK_RESOURCE  	(1<<1)		// missing resource or capacity
#define E_NO_ACCESS 		(1<<2) 		// access denied
#define E_TIMEOUT 			(1<<3)		// timeout while operating
#define E_AGAIN 			(1<<4)		// try again later
#define E_LOCKED			(1<<5)		// LOCKED
#define E_NOT_FOUND			(1<<6)		// request item not found
#define E_CONN_LOSS                     (1<<7)      // not connected
#define E_NO_DATA                   (1<<8)  // EOF or no data found

#define E_RESERVED_1		(1<<31)

typedef uint32_t Erc;

#define CER(_x)	{ Erc erc; if ( erc=_x ) return erc;}
#ifndef ASSERT
//#define ASSERT(__expr) if ((__expr)==false) {ERROR(" ASSERT failed "); while(1);};
#endif
#endif /* ERC_H_ */

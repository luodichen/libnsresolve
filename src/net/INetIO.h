/*
 * INetIO.h
 *
 *  Created on: 2015年4月28日
 *      Author: luodichen
 */

#ifndef SRC_NET_INETIO_H_
#define SRC_NET_INETIO_H_

#include <stdint.h>
#include <stdlib.h>

class INetIO
{
public:
    INetIO() { };
    virtual ~INetIO() { };
    virtual int Write(const uint8_t *pBuffer, size_t size) = 0;
    virtual int Read(uint8_t *pBuffer, size_t max) = 0;
};

#endif /* SRC_NET_INETIO_H_ */

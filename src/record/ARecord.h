/*
 * ARecord.h
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_ARECORD_H_
#define SRC_RECORD_ARECORD_H_

#include "BaseRecord.h"
#include <arpa/inet.h>

class ARecord : public BaseRecord
{
public:
    ARecord(size_t size);
    virtual ~ARecord();
    
public:
    in_addr_t GetIPAddress();
};

#endif /* SRC_RECORD_ARECORD_H_ */

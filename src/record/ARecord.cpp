/*
 * ARecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "ARecord.h"
#include <string.h>

ARecord::ARecord(size_t size)
    : BaseRecord(size)
{
    
}

ARecord::~ARecord()
{
    
}

in_addr_t ARecord::GetIPAddress()
{
    in_addr_t ret = 0;
    memcpy((void *)&ret, GetRawData(), sizeof(ret));
    
    return ret;
}

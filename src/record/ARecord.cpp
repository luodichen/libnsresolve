/*
 * ARecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "ARecord.h"
#include "../common/constants.h"
#include <string.h>

const uint16_t ARecord::TYPE = QTYPE::A;

ARecord::ARecord(size_t size)
    : BaseRecord(size)
{
    m_sType = TYPE;
}

ARecord::~ARecord()
{
    
}

in_addr_t ARecord::GetIPAddress() const
{
    in_addr_t ret = 0;
    memcpy((void *)&ret, GetRawData(), sizeof(ret));
    
    return ret;
}

size_t ARecord::GetDataLength() const
{
    return sizeof(in_addr_t);
}

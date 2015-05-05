/*
 * NSRecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "NSRecord.h"
#include "../common/constants.h"

const uint16_t NSRecord::TYPE = QTYPE::NS;

NSRecord::NSRecord(size_t size, const std::vector<uint8_t> &vBuffer)
    : BaseRecord(size)
    , m_parser(vBuffer)
{
    
}

NSRecord::~NSRecord()
{
    
}

int NSRecord::StreamInput(uint8_t word)
{
    int ret = 0;
    if ((ret = BaseRecord::StreamInput(word)) < 0)
        return ret;
    
   ret = m_parser.StreamInput(word);
   
   return ret;
}

std::string NSRecord::GetDomainName() const
{
    return m_parser.GetDomainName();
}

size_t NSRecord::GetDataLength() const
{
    return GetDomainName().length() + 1;
}

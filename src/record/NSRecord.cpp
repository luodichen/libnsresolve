/*
 * NSRecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "NSRecord.h"

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

std::string NSRecord::GetDomainName()
{
    return m_parser.GetDomainName();
}

/*
 * CNAMERecord.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: luodichen
 */

#include "CNAMERecord.h"
#include "../common/constants.h"

const uint16_t CNAMERecord::TYPE = QTYPE::CNAME;

CNAMERecord::CNAMERecord(size_t size, const std::vector<uint8_t> &vBuffer)
    : BaseRecord(size)
    , m_parser(vBuffer)
{
    m_sType = TYPE;
}

CNAMERecord::~CNAMERecord()
{
    
}

int CNAMERecord::StreamInput(uint8_t word)
{
    int ret = 0;
    if ((ret = BaseRecord::StreamInput(word)) < 0)
        return ret;
    
   ret = m_parser.StreamInput(word);
   
   return ret;
}

std::string CNAMERecord::GetDomainName() const
{
    return m_parser.GetDomainName();
}

size_t CNAMERecord::GetDataLength() const
{
    return GetDomainName().length() + 1;
}

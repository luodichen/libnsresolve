/*
 * LResourceRecord.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#include "LResourceRecord.h"
#include <arpa/inet.h>

LResourceRecord::LResourceRecord(const std::vector<uint8_t> &vBuffer)
    : m_strDomainName("")
    , m_sType(0)
    , m_sClass(0)
    , m_nTTL(0)
    , m_pRecordBuffer(NULL)
    , m_sRecordLength(0)
    , m_header()
    , m_status(PARSING_DOMAIN_NAME)
    , m_dnp(vBuffer)
    , m_pHeaderCur((uint8_t*)&m_header)
    , m_sBytesReaded(0)
{

}

LResourceRecord::~LResourceRecord()
{
    if (NULL != m_pRecordBuffer)
    {
        delete [] m_pRecordBuffer;
        m_pRecordBuffer = NULL;
    }
}

int LResourceRecord::StreamInput(uint8_t word)
{
    int result = 0;
    switch (m_status)
    {
    case PARSING_DOMAIN_NAME:
        result = m_dnp.StreamInput(word);
        CHKERR(result >= 0, result);

        if (0 == result)
        {
            m_status = PARSING_HEADER;
            m_strDomainName = m_dnp.GetDomainName();
        }
        break;

    case PARSING_HEADER:
        *m_pHeaderCur = word;
        if (sizeof(m_header) == ++m_pHeaderCur - (uint8_t*)&m_header)
        {
            m_sType = ntohs(m_header.sType);
            m_sClass = ntohs(m_header.sClass);
            m_nTTL = ntohl(m_header.nTTL);
            m_sRecordLength = ntohs(m_header.sDataLen);

            if (m_sRecordLength > 0)
            {
                m_pRecordBuffer = new uint8_t[m_sRecordLength];
                m_status =  READING_DATA;
            }
            else
            {
                m_status = STOPED;
            }
        }
        break;

    case READING_DATA:
        m_pRecordBuffer[m_sBytesReaded++] = word;
        if (m_sBytesReaded == m_sRecordLength)
        {
            m_status = STOPED;
        }
        break;

    case STOPED:
        return ERR::HIT_THE_END_OF_RESOURCE_RECORD;
        break;
    }

    return STOPED == m_status ? 0 : 1;
}

uint16_t LResourceRecord::GetType() const
{
    return m_sType;
}

uint16_t LResourceRecord::GetClass() const
{
    return m_sClass;
}

uint32_t LResourceRecord::GetTTL() const
{
    return m_nTTL;
}

const uint8_t *LResourceRecord::GetBuffer(size_t *pSize) const 
{
    if (NULL != pSize)
    {
        *pSize = m_sRecordLength;
    }

    return m_pRecordBuffer;
}

std::string LResourceRecord::GetDomainName() const
{
    return m_strDomainName;
}

size_t LResourceRecord::GetBufferSize() const
{
    return m_sRecordLength;
}

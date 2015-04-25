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

int LResourceRecord::StringInput(uint8_t word)
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

        break;
    }

    return STOPED == m_status ? 0 : 1;
}


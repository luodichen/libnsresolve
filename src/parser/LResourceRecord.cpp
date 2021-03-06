/*
 * Copyright (C) 2015 luodichen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * LResourceRecord.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#include "LResourceRecord.h"
#include "../common/constants.h"
#include "../common/socket.h"
#include <string.h>

LResourceRecord::LResourceRecord(const std::vector<uint8_t> &vBuffer)
    : m_strDomainName("")
    , m_sType(0)
    , m_sClass(0)
    , m_nTTL(0)
    , m_sRecordLength(0)
    , m_pRecord(NULL)
    , m_header()
    , m_status(PARSING_DOMAIN_NAME)
    , m_dnp(vBuffer)
    , m_pHeaderCur((uint8_t*)&m_header)
    , m_vBuffer(vBuffer)
{

}

LResourceRecord::~LResourceRecord()
{
    if (NULL != m_pRecord)
    {
        delete m_pRecord;
        m_pRecord = NULL;
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
                m_status =  READING_DATA;
            }
            else
            {
                m_status = STOPED;
            }
        }
        break;

    case READING_DATA:
        if (NULL == m_pRecord)
        {
            m_pRecord = MakeRecord(m_sType);
        }

        result = m_pRecord->StreamInput(word);
        if (result < 0)
        {
            return result;
        }
        else if (0 == result)
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

const BaseRecord *LResourceRecord::GetRecord() const
{
    return m_pRecord;
}

std::string LResourceRecord::GetDomainName() const
{
    return m_strDomainName;
}

size_t LResourceRecord::GetBufferSize() const
{
    return m_pRecord->GetDataLength();
}

BaseRecord *LResourceRecord::MakeRecord(uint16_t type)
{
    BaseRecord *ret = NULL;
    switch (type)
    {
    case QTYPE::A:
        ret = new ARecord(m_sRecordLength);
        break;

    case QTYPE::NS:
        ret = new NSRecord(m_sRecordLength, m_vBuffer);
        break;

    case QTYPE::MX:
        ret = new MXRecord(m_sRecordLength, m_vBuffer);
        break;
        
    case QTYPE::TXT:
        ret = new TXTRecord(m_sRecordLength);
        break;
        
    case QTYPE::CNAME:
        ret = new CNAMERecord(m_sRecordLength, m_vBuffer);
        break;

    default:
        ret = new BaseRecord(m_sRecordLength);
        break;
    }

    return ret;
}

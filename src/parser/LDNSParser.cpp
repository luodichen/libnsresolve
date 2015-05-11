/*
 * LDNSParser.cpp
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#include "LDNSParser.h"

LDNSParser::LDNSParser()
	: m_header()
    , m_buffer()
    , m_pQuestion(NULL)
    , m_vAnswerRRs()
    , m_vAuthorityRRs()
    , m_vAdditionalRRs()
    , m_sAnswer(0)
    , m_sAuthority(0)
    , m_sAdditional(0)
    , m_status(WAIT_HEADER)
    , m_cur()
    , m_nLastError(ERR::NOERROR)
{

}

LDNSParser::~LDNSParser()
{
    if (NULL != m_pQuestion)
    {
        delete m_pQuestion;
        m_pQuestion = 0;
    }
    
    if (NULL != m_cur.pResourceRecord)
    {
        delete m_cur.pResourceRecord;
        m_cur.pResourceRecord = NULL;
    }
    
    ClearRecords(m_vAnswerRRs);
    ClearRecords(m_vAuthorityRRs);
    ClearRecords(m_vAdditionalRRs);
}

int LDNSParser::StreamInput(const uint8_t *pBuffer, int nSize)
{
    for (int i = 0; i < nSize; i++)
    {
        int e = StreamInput(pBuffer[i]);
        if (e < 0) return e;
    }
    
    return ERR::NOERROR;
}

int LDNSParser::StreamInput(uint8_t word)
{
    m_buffer.push_back(word);
    int result = 0;
    
    switch (m_status)
    {
    case WAIT_HEADER:
        ((uint8_t*)&m_header)[m_cur.nHeaderWords++] = word;
        if (sizeof(m_header) == m_cur.nHeaderWords)
        {
            m_status = WAIT_QUESTION;
            m_pQuestion = new LQuestion(m_buffer);
            
            ReadHeader();
        }
        break;
        
    case WAIT_QUESTION:
        result = m_pQuestion->StreamInput(word);
        if (result < 0)
        {
            SetError(result);
        }
        else if (0 == result)
        {
            m_status = WAIT_RESOURCES;
            m_cur.pResourceRecord = new LResourceRecord(m_buffer);
        }
        break;
        
    case WAIT_RESOURCES:
        result = m_cur.pResourceRecord->StreamInput(word);
        if (result < 0)
        {
            SetError(result);
            return result;
        }
        else if (0 == result)
        {
            if (m_cur.nAnswerCount < m_sAnswer)
            {
                m_vAnswerRRs.push_back(m_cur.pResourceRecord);
                m_cur.nAnswerCount++;
            } 
            else if (m_cur.nAuthorityCount < m_sAuthority)
            {
                m_vAuthorityRRs.push_back(m_cur.pResourceRecord);
                m_cur.nAuthorityCount++;
            }
            else if (m_cur.nAdditinalCount < m_sAdditional)
            {
                m_vAdditionalRRs.push_back(m_cur.pResourceRecord);
                m_cur.nAdditinalCount++;
            }
            else
            {
                SetError(ERR::TOO_MUCH_RESOURCE_RECORDS);
                return m_nLastError;
            }
            
            if (m_cur.nAnswerCount + m_cur.nAuthorityCount + m_cur.nAdditinalCount
                    == m_sAnswer + m_sAuthority + m_sAdditional)
            {
                m_status = STOPED;
                m_cur.pResourceRecord = NULL;
            }
            else
            {
                m_cur.pResourceRecord = new LResourceRecord(m_buffer);
            }
        }
        break;
        
    case STOPED:
        SetError(ERR::NO_MORE_WORDS);
        break;
        
    case ERROR:
        return m_nLastError;
        break;
    }
    
    return STOPED == m_status ? 0 : 1;
}

void LDNSParser::ClearRecords(std::vector<LResourceRecord *> &record)
{
    std::vector<LResourceRecord *>::iterator iter = record.begin();
    for (; iter != record.end(); iter++)
    {
        if (NULL != *iter) delete *iter;
        *iter = NULL;
    }
    
    record.clear();
}

void LDNSParser::SetError(int errcode)
{
    m_status = ERROR;
    m_nLastError = errcode;
}

void LDNSParser::ReadHeader()
{
    m_sAnswer = ntohs(m_header.wAnswerRRs);
    m_sAuthority = ntohs(m_header.wAuthorityRRs);
    m_sAdditional = ntohs(m_header.wAdditionalRRs);
}

std::vector<const LResourceRecord *> LDNSParser::GetAnswers() const
{
    return GetResources(m_vAnswerRRs);
}

std::vector<const LResourceRecord *> LDNSParser::GetAuthoritys() const
{
    return GetResources(m_vAuthorityRRs);
}

std::vector<const LResourceRecord *> LDNSParser::GetAdditionals() const
{
    return GetResources(m_vAdditionalRRs);
}

std::vector<const LResourceRecord *> LDNSParser::GetResources(const std::vector<LResourceRecord *> &v) const
{
    std::vector<const LResourceRecord *> ret;
    std::vector<LResourceRecord *>::const_iterator iter = v.begin();
    for (; iter != v.end(); iter++)
    {
        ret.push_back(*iter);
    }
    
    return ret;
}

DNSHEADER LDNSParser::GetHeader() const
{
    return m_header;
}

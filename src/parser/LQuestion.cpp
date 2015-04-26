/*
 * LQuestion.cpp
 *
 *  Created on: Apr 26, 2015
 *      Author: luodichen
 */

#include "LQuestion.h"
#include <string.h>

LQuestion::LQuestion(const std::vector<uint8_t> &vBuffer)
    : m_strDomainName("")
    , m_pDomainNameParser(new LDomainNameParser(vBuffer))
    , m_header()
    , m_status(PARSING_DOMAIN_NAME)
    , m_nBytesReaded(0)
{

}

LQuestion::LQuestion(const char *szDomainName, uint16_t sType, uint16_t sClass)
    : m_strDomainName(szDomainName)
    , m_pDomainNameParser(NULL)
    , m_header()
    , m_status(STOPED)
    , m_nBytesReaded(sizeof(m_header))
{
    m_header.sType = htons(sType);
    m_header.sClass = htons(sClass);
}

LQuestion::~LQuestion()
{
    if (NULL != m_pDomainNameParser)
    {
        delete m_pDomainNameParser;
        m_pDomainNameParser = NULL;
    }
}

int LQuestion::StreamInput(uint8_t word)
{
    int result = 0;
    switch (m_status)
    {
    case PARSING_DOMAIN_NAME:
        result = m_pDomainNameParser->StreamInput(word);
        CHKERR(result >= 0, result);

        if (0 == result)
        {
            m_status = PARSING_HEADER;
            m_strDomainName = m_pDomainNameParser->GetDomainName();
        }
        break;

    case PARSING_HEADER:
        ((uint8_t*)&m_header)[m_nBytesReaded++] = word;
        if (sizeof(m_header) == m_nBytesReaded)
        {
            m_status = STOPED;
        }
        break;

    case STOPED:
        return ERR::HIT_THE_END_OF_QUESTION;
        break;
    }

    return STOPED == m_status ? 0 : 1;
}

int LQuestion::MakeBuffer(uint8_t *pBuffer, size_t *pSize)
{
    int ret = ERR::NOERROR;
    if (NULL == pSize)
    {
        ret = ERR::INVALID_PARAM;
    }
    else if (*pSize < m_strDomainName.length() + sizeof(m_header) + 2)
    {
        *pSize = m_strDomainName.length() + sizeof(m_header) + 2;
        ret = ERR::NEED_LARGER_BUF;
    }
    else if (NULL == pBuffer)
    {
        ret = ERR::INVALID_PARAM;
    }
    else
    {
        int nPartLen = 0;
        int nCur = 0;
        int nDomainNameLen = m_strDomainName.length();
        pBuffer[nCur++] = 0;

        for (int i = 0; i < nDomainNameLen; i++)
        {
            uint8_t word = m_strDomainName[i];
            if ('.' == word)
            {
                pBuffer[nCur] = 0;
                pBuffer[nCur - nPartLen - 1] = nPartLen;
                nCur++;
                nPartLen = 0;
            }
            else
            {
                pBuffer[nCur++] = word;
                nPartLen++;
            }
        }

        pBuffer[nCur - nPartLen - 1] = nPartLen;
        pBuffer[nCur++] = 0;

        memcpy((void*)(pBuffer + nCur), (void*)&m_header, sizeof(m_header));
    }
    return ret;
}


/*
 * BaseRecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "BaseRecord.h"

const uint16_t BaseRecord::TYPE = 0;

BaseRecord::BaseRecord(size_t size)
    : m_nRawSize(size)
    , m_pRawData(new uint8_t[size])
    , m_nWordsReaded(0)
    , m_sType(TYPE)
{
    
}

BaseRecord::BaseRecord(const BaseRecord &that)
    : m_nRawSize(that.m_nRawSize)
    , m_pRawData(new uint8_t[that.m_nRawSize])
    , m_nWordsReaded(that.m_nWordsReaded)
    , m_sType(0)
{
    memcpy((void *)m_pRawData, (void *)that.m_pRawData, m_nRawSize);
}

BaseRecord::~BaseRecord()
{
    if (NULL != m_pRawData)
    {
        delete [] m_pRawData;
        m_pRawData = NULL;
    }
}

BaseRecord &BaseRecord::operator=(const BaseRecord &that)
{
    if (&that == this)
        return *this;
    
    if (NULL != m_pRawData)
    {
        delete [] m_pRawData;
        m_pRawData = NULL;
    }
    
    m_nRawSize = that.m_nRawSize;
    m_nWordsReaded = that.m_nWordsReaded;
    m_pRawData = new uint8_t[m_nRawSize];
    
    memcpy(m_pRawData, that.m_pRawData, m_nRawSize);
    
    return *this;
}

int BaseRecord::StreamInput(uint8_t word)
{
    if (m_nWordsReaded >= m_nRawSize)
        return ERR::NO_MORE_WORDS;
    
    m_pRawData[m_nWordsReaded++] = word;
    
    return m_nWordsReaded < m_nRawSize ? 1 : 0;
}

const uint8_t *BaseRecord::GetRawData() const
{
    return m_pRawData;
}

size_t BaseRecord::GetRawDataLength() const
{
    return m_nRawSize;
}

size_t BaseRecord::GetDataLength() const
{
    return GetRawDataLength();
}

uint16_t BaseRecord::GetType() const
{
    return m_sType;
}

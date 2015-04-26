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
{

}

LDNSParser::~LDNSParser()
{

}

void LDNSParser::StreamInput(const uint8_t *pBuffer, int nSize)
{
    for (int i = 0; i < nSize; i++)
    {
        StreamInput(pBuffer[i]);
    }
}

void LDNSParser::StreamInput(uint8_t word)
{
    m_buffer.push_back(word);
}

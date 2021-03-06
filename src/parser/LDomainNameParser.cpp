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
 * LDomainNameParser.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: luodichen
 */

#include "../common/errcode.h"
#include "LDomainNameParser.h"
#include <assert.h>

#define PTRMASK (0xc0)
#define ISPTR(w) (((w) & PTRMASK) == PTRMASK)
#define GETPTR(w) ((w) & (~PTRMASK))

static const int MAX_SUB_LEN = 63;

LDomainNameParser::LDomainNameParser(const std::vector<uint8_t> &vBuffer)
    : m_status(WAIT_LENGTH)
    , m_nSubLen(0)
    , m_ptr(0)
    , m_vBuffer(vBuffer)
    , m_strDomainName("")
{

}

LDomainNameParser::~LDomainNameParser()
{

}

int LDomainNameParser::StreamInput(uint8_t word)
{
    int nPtr = 0;
    int nResult = 0;
    switch (m_status)
    {
    case WAIT_LENGTH:
        CHKERR(word <= MAX_SUB_LEN || ISPTR(word),
                ERR::DOMAIN_NAME_FORMAT_INCORRECT);
        if (ISPTR(word))
        {
            m_ptr = GETPTR(word) << 8;
            m_status = WAIT_PTR;
        }
        else if (word > 0)
        {
            m_nSubLen = word;
            m_status = WAIT_WORDS;
        }
        else
        {
            m_status = STOPED;
        }
        break;
        
    case WAIT_PTR:
        m_ptr += word;
        nPtr = m_ptr;
        m_ptr = 0;
        nResult = 0;
        m_status = WAIT_LENGTH;
        
        while ((nResult = StreamInput(m_vBuffer[nPtr++])) > 0)
            ;
        CHKERR(nResult >= 0, nResult);
        break;

    case WAIT_WORDS:
        m_strDomainName.append(1, (char)word);
        if (0 == --m_nSubLen)
        {
            m_strDomainName.append(1, '.');
            m_status = WAIT_LENGTH;
        }
        break;

    case STOPED:
        return ERR::HIT_THE_END_OF_DOMAIN_NAME;
        break;

    default:
        break;
    }
    return STOPED == m_status ? 0 : 1;
}

std::string LDomainNameParser::GetDomainName() const
{
    return m_strDomainName;
}

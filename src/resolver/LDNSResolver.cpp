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
 * LDNSResolver.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "LDNSResolver.h"
#include "../common/constants.h"
#include "../common/socket.h"
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

static const int MAX_TIMEOUT = 8;
static const int MAX_RETRY = 10;

LDNSResolver::LDNSResolver(in_addr_t address, uint32_t timeout)
    : m_address(address)
    , m_timeout(timeout)
    , m_pParser(NULL)
{

}

LDNSResolver::LDNSResolver(const char *szAddress, uint32_t timeout)
    : m_address(inet_addr(szAddress))
    , m_timeout(timeout)
    , m_pParser(NULL)
{

}

LDNSResolver::~LDNSResolver()
{
    Cleanup();
}

const static size_t MAX_BUF_SIZE = 2048;
int LDNSResolver::Query(const char *szName, uint16_t sType)
{
    Cleanup();
    
    int ret = 0;
    m_pParser = new LDNSParser();
    
    DNSHEADER header;
    HEADER_FLAG flag;

    flag.wFlag = 0;
    flag.sFlag.rd = 1;

    header.wTransId = htons(1);
    header.wFlag = htons(flag.wFlag);
    header.wQuestions = htons(1);

    LQuestion question(szName, sType, QCLASS::INET);
    size_t bufsize = 0;
    int result = question.MakeBuffer(NULL, &bufsize);
    if (ERR::NEED_LARGER_BUF != result)
    {
        ret = result;
        return ret;
    }
    else if (bufsize > MAX_BUF_SIZE)
    {
        return ERR::DOMAIN_NAME_TOO_LONG;
    }
    bufsize += sizeof(header);
    uint8_t *pBuffer = new uint8_t[bufsize];

    memcpy((void *)pBuffer, (void *)&header, sizeof(header));
    result = question.MakeBuffer(pBuffer + sizeof(header), &bufsize);
    if (ERR::ERR_NOERROR != result)
    {
        delete [] pBuffer;
        pBuffer = NULL;
        ret = result;
        return ret;
    }

    result = NetQuery(pBuffer, bufsize, LClient::UDP);
    DNSHEADER stResponseHeader = m_pParser->GetHeader();
    HEADER_FLAG *pFlag = (HEADER_FLAG *)&stResponseHeader.wFlag;
    if (pFlag->sFlag.tc)
    {
        Cleanup();
        m_pParser = new LDNSParser();

        result = NetQuery(pBuffer, bufsize, LClient::TCP);
    }

    delete [] pBuffer;
        pBuffer = NULL;

    ret = result;

    return ret;
}

const LDNSParser *LDNSResolver::GetParser()
{
    return m_pParser;
}

void LDNSResolver::Cleanup()
{
    if (NULL != m_pParser)
    {
        delete m_pParser;
        m_pParser = NULL;
    }
}

int LDNSResolver::NetQuery(const uint8_t *pBuffer, size_t size, LClient::TYPE type)
{
    int result = 0;
    LClient client(m_address, 53, type);

    if (LClient::TCP == type)
    {
        uint16_t sSendLength = htons(size);
        result = client.Write((const uint8_t *)&sSendLength, sizeof(sSendLength), m_timeout);
        if (result < 0)
            return result;
    }

    uint8_t recvbuf[1024] = {0};
    uint16_t sTCPStreamLength = 0;

    if (LClient::TCP == type)
    {
        result = client.Write(pBuffer, size, m_timeout);
        if (result < 0)
            return result;

        result = client.Read((uint8_t *)&sTCPStreamLength, sizeof(sTCPStreamLength), m_timeout);
        if (result < 0)
            return result;
        uint16_t sReceived = 0;
        sTCPStreamLength = ntohs(sTCPStreamLength);
        while (sReceived < sTCPStreamLength)
        {
            result = client.Read(recvbuf, sizeof(recvbuf), m_timeout);
            if (result < 0)
                return result;
            else if (0 == result)
                break;

            sReceived += result;
            result = m_pParser->StreamInput(recvbuf, result);
            if (result < 0)
                return result;
        }
    }
    else if (LClient::UDP == type)
    {
        uint32_t nTimeout = 2;
        int nRetry = 0;
        time_t tStart = time(NULL);

        do
        {
            result = client.Write(pBuffer, size, nTimeout);
            if (result < 0)
                return result;

            result = client.Read(recvbuf, sizeof(recvbuf), nTimeout);
            nTimeout = ((nTimeout * 2) <= MAX_TIMEOUT) ? (nTimeout * 2) : MAX_TIMEOUT;
        } while (result < 0 && nRetry < MAX_RETRY && (time(NULL) - tStart < m_timeout));
        
        if (result < 0)
            return time(NULL) - tStart < m_timeout ? result : ERR::RECV_TIMEOUT;

        m_pParser->StreamInput(recvbuf, result);
    }
    else
    {
        // IT'S IMPOSSABLE
        assert(false);
    }

    return 0;
}

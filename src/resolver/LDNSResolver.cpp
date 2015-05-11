/*
 * LDNSResolver.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "LDNSResolver.h"
#include "../common/constants.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>

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
    LClient client(m_address, 53, LClient::TCP, m_timeout);
    m_pParser = new LDNSParser();
    
    DNSHEADER header;
    HEADER_FLAG flag;

    flag.wFlag = 0;
    flag.sFlag.rd = 1;

    header.wTransId = htons(1);
    header.wFlag = htons(flag.wFlag);
    header.wQuestions = htons(1);

    LQuestion question(szName, sType, QCLASS::IN);
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
    if (ERR::NOERROR != result)
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
    LClient client(m_address, 53, type, m_timeout);

    if (LClient::TCP == type)
    {
        uint16_t sSendLength = htons(size);
        result = client.Write((const uint8_t *)&sSendLength, sizeof(sSendLength));
        if (result < 0)
            return result;
    }

    result = client.Write(pBuffer, size);
    if (result < 0)
        return result;

    uint8_t recvbuf[1024] = {0};
    uint16_t sTCPStreamLength = 0;

    if (LClient::TCP == type)
    {
        result = client.Read((uint8_t *)&sTCPStreamLength, sizeof(sTCPStreamLength));
        if (result < 0)
            return result;
        uint16_t sReceived = 0;
        sTCPStreamLength = ntohs(sTCPStreamLength);
        while (sReceived < sTCPStreamLength)
        {
            result = client.Read(recvbuf, sizeof(recvbuf));
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
        result = client.Read(recvbuf, sizeof(recvbuf));
        if (result < 0)
            return result;

        m_pParser->StreamInput(recvbuf, result);
    }
    else
    {
        // IT'S IMPOSSABLE
        assert(false);
    }

    return 0;
}

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
{

}

LDNSResolver::LDNSResolver(const char *szAddress, uint32_t timeout)
    : m_address(inet_addr(szAddress))
    , m_timeout(timeout)
{

}

LDNSResolver::~LDNSResolver()
{

}

const static size_t MAX_BUF_SIZE = 2048;
int LDNSResolver::Query(const char *szName, uint16_t sType)
{
    int ret = 0;
    LClient client(m_address, 53, LClient::UDP, m_timeout);
    LDNSParser parser;
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

    result = client.Write(pBuffer, bufsize);
    delete [] pBuffer;
    pBuffer = NULL;

    if (ERR::NOERROR != result)
    {
        ret = result;
        return ret;
    }

    uint8_t recvbuf[1024] = {0};
    result = client.Read(recvbuf, sizeof(recvbuf));
    if (result < 0)
    {
        ret = result;
        return ret;
    }

    result = parser.StreamInput(recvbuf, result);
    if (ERR::NOERROR != result)
    {
        ret = result;
        return ret;
    }

    printf("%s:", parser.GetAnswers()[0]->GetDomainName().c_str());

    uint8_t ip[4];
    size_t size = 0;
    memcpy(ip, parser.GetAnswers()[0]->GetBuffer(&size), sizeof(ip));
    printf("%u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);

    return ret;
}

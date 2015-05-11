/*
 * LDNSResolver.h
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#ifndef SRC_RESOLVER_LDNSRESOLVER_H_
#define SRC_RESOLVER_LDNSRESOLVER_H_

#include "../net/LClient.h"
#include "../parser/LDNSParser.h"
#include "../parser/LQuestion.h"
#include "../common/errcode.h"

class LDNSResolver
{
public:
    LDNSResolver(in_addr_t address, uint32_t timeout = 15);
    LDNSResolver(const char *szAddress, uint32_t timeout = 15);
    virtual ~LDNSResolver();

public:
    int Query(const char *szName, uint16_t sType);
    const LDNSParser *GetParser();
    
private:
    void Cleanup();
    int NetQuery(const uint8_t *pBuffer, size_t size, LClient::TYPE type);

private:
    in_addr_t m_address;
    uint32_t m_timeout;
    LDNSParser *m_pParser;
};

#endif /* SRC_RESOLVER_LDNSRESOLVER_H_ */

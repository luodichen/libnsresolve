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

private:
    in_addr_t m_address;
    uint32_t m_timeout;
};

#endif /* SRC_RESOLVER_LDNSRESOLVER_H_ */

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

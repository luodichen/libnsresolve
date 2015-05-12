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
 * nsresolve.h
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#ifndef SRC_INCLUDE_NSRESOLVE_H_
#define SRC_INCLUDE_NSRESOLVE_H_

#include "../resolver/LDNSResolver.h"
#include "../record/BaseRecord.h"
#include "../record/ARecord.h"
#include "../record/MXRecord.h"
#include "../record/NSRecord.h"
#include "../record/CNAMERecord.h"
#include "../common/DNSHeader.h"
#include <stdint.h>
#include <time.h>
//#include <arpa/inet.h>
#include "../common/socket.h"
//#include <netinet/in.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _NSRRESRECORD
{
    uint16_t sType;
    uint16_t sClass;
    uint32_t nTTL;
    uint16_t sResLen;
    const char *szDomainName;
    const void *pResData;
} NSRESRECORD, *PNSRESRECORD;

typedef struct _NSRRESULT
{
    DNSHEADER header;
    const NSRESRECORD **pAnswers;
    const NSRESRECORD **pAuthoritys;
    const NSRESRECORD **pAdditionals;
} NSRRESULT, *PNSRRESULT;

typedef struct _NSRBASERECORD
{
    size_t nRawDataLen;
    const uint8_t *pRawData;
} NSRBASERECORD, *PNSRBASERECORD;

typedef struct _NSRARECORD
{
    in_addr_t address;
} NSRARECORD, *PNSRARECORD;

typedef struct _NSRNSRECORD
{
    const char *szDomainName;
} NSRNSRECORD, *PNSRNSRECORD;

typedef struct _NSRCNAMERECORD
{
    const char *szDomainName;
} NSRCNAMERECORD, *PNSRCNAMERECORD;

typedef struct _NSRMXRECORD
{
    uint16_t sPreference;
    const char *szDomainName;
} NSRMXRECORD, *PNSRMXRECORD;

int resolve(const char *szName, uint16_t sType, in_addr_t server, NSRRESULT **pResult, time_t timeout);
//int resolve(const char *szName, uint16_t sType, const char *szServer, NSRRESULT **pResult, time_t timeout);
void release(NSRRESULT *pResult);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRC_INCLUDE_NSRESOLVE_H_ */

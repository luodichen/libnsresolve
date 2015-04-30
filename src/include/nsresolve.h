/*
 * nsresolve.h
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#ifndef SRC_INCLUDE_NSRESOLVE_H_
#define SRC_INCLUDE_NSRESOLVE_H_

#include "../resolver/LDNSResolver.h"
#include <stdint.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
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
    const uint8_t *pResData;
} NSRESRECORD, *PNSRESRECORD;

typedef struct _NSRRESULT
{
    const NSRESRECORD **pAnswers;
    const NSRESRECORD **pAuthoritys;
    const NSRESRECORD **pAdditionals;
} NSRRESULT, *PNSRRESULT;

int resolve(const char *szName, uint16_t sType, in_addr_t server, NSRRESULT **pResult, time_t timeout);
//int resolve(const char *szName, uint16_t sType, const char *szServer, NSRRESULT **pResult, time_t timeout);
void release(NSRRESULT *pResult);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SRC_INCLUDE_NSRESOLVE_H_ */

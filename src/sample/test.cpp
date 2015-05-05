/*
 * test.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "../include/nsresolve.h"
#include "../common/constants.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main()
{
    in_addr_t addr = inet_addr("114.114.114.114");
    NSRRESULT *pResult = NULL;
    int result = resolve("www.baidu.com", QTYPE::A, addr, &pResult, 15);
    printf("result:%d\n", result);

    const NSRESRECORD **ppAnswers = pResult->pAnswers;
    while (NULL != *ppAnswers)
    {
        struct in_addr ip;
        const NSRESRECORD *pRecord = *ppAnswers;

        printf("%s: ", pRecord->szDomainName);
        printf("TTL=%d, type=%d, ", pRecord->nTTL, pRecord->sType);

        if (QTYPE::A == pRecord->sType)
        {
            memcpy((void *)&ip, pRecord->pResData, sizeof(ip));
            printf("IP=%s\n", inet_ntoa(ip));
        }
        else
        {
            printf("data=%s\n", pRecord->pResData);
        }
        ppAnswers++;
    }

    release(pResult);

    return 0;
}

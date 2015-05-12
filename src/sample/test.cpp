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
 * test.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "../include/nsresolve.h"
#include "../common/constants.h"
#include <stdio.h>
#include <string.h>

void print_records(const NSRESRECORD **pRecords)
{
    while (NULL != *pRecords)
    {
        struct in_addr ip;
        const NSRESRECORD *pRecord = *pRecords;

        printf("%s: ", pRecord->szDomainName);
        printf("TTL=%d, type=%d, ", pRecord->nTTL, pRecord->sType);

        if (QTYPE::A == pRecord->sType)
        {
            PNSRARECORD pARecord = (PNSRARECORD)pRecord->pResData;
            in_addr inaddr;
            inaddr.s_addr = pARecord->address;
            printf("IP=%s\n", inet_ntoa(inaddr));
        }
        else if (QTYPE::NS == pRecord->sType)
        {
            PNSRNSRECORD pNSRecord = (PNSRNSRECORD)pRecord->pResData;
            printf("Name Server=%s\n", pNSRecord->szDomainName);
        }
        else if (QTYPE::MX == pRecord->sType)
        {
            PNSRMXRECORD pMXRecord = (PNSRMXRECORD)pRecord->pResData;
            printf("MX Preference=%d, Server=%s\n", pMXRecord->sPreference, pMXRecord->szDomainName);
        }
        else if (QTYPE::CNAME == pRecord->sType)
        {
            PNSRCNAMERECORD pCNAMERecord = (PNSRCNAMERECORD)pRecord->pResData;
            printf("CNAME=%s\n", pCNAMERecord->szDomainName);
        }
        else
        {
            printf("unknown\n");
        }
        pRecords++;
    }
}

int main()
{
    in_addr_t addr = inet_addr("114.114.114.114");
    NSRRESULT *pResult = NULL;
    int result = resolve("work.luodc.cn", QTYPE::A, addr, &pResult, 15);
    printf("result:%d\n", result);

    if (result < 0)
        return -1;

    printf("-------- answers --------\n");
    print_records(pResult->pAnswers);
    printf("-------- authoritys --------\n");
    print_records(pResult->pAuthoritys);
    printf("-------- additionals --------\n");
    print_records(pResult->pAdditionals);

    release(pResult);

    return 0;
}

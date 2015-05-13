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
#include "getopt.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <ctype.h>

static class _TypeMap : public std::map<std::string, uint16_t>
{
public:
    _TypeMap()
        : std::map<std::string, uint16_t>()
    {
        (*this)["A"] = QTYPE::A;
        (*this)["NS"] = QTYPE::NS;
        (*this)["CNAME"] = QTYPE::CNAME;
        (*this)["MX"] = QTYPE::MX;
    }
} gTypeMap;

static void print_records(const NSRESRECORD **pRecords)
{
    if (NULL == *pRecords)
    {
        printf("(no records)\n");
    }

    while (NULL != *pRecords)
    {
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
            printf("not supported\n");
        }
        pRecords++;
    }
}

uint16_t get_type(const char *szArg)
{
    std::string strArg = "";
    while (*szArg != '\0')
    {
        strArg.push_back(toupper(*(szArg++)));
    }

    std::map<std::string, uint16_t>::iterator iter = gTypeMap.find(strArg);
    return iter == gTypeMap.end() ? -1 : iter->second;
}

int main(int argc, char *argv[])
{
    in_addr_t addr = inet_addr("8.8.8.8");
    uint16_t type = QTYPE::A;
    std::string strDomainName = "";
    int nChar = 0;

    int opt = 0;
    while (-1 != (opt = getopt(argc, argv, "h:t:")))
    {
        switch (opt)
        {
        case 'h':
            addr = inet_addr(optarg);
            break;

        case 't':
            if ((uint16_t)-1 == (type = get_type(optarg)))
            {
                printf("type '%s' not supported\n", optarg);
                exit(1);
            }

            break;

        default:
            printf("unrecognized option '%s'\n", optarg);
            exit(1);
            break;
        }
    }

    printf(">");
    while (EOF != (nChar = getchar()))
    {
        if (('\n' == nChar || '\r' == nChar) && strDomainName.length() > 0)
        {
            NSRRESULT *pResult = NULL;
            int result = resolve(strDomainName.c_str(), type, addr, &pResult, 15);

            if (result < 0)
            {
                printf("error:%d\n>", result);
                strDomainName = "";
                continue;
            }

            in_addr in;
            in.S_un.S_addr = addr;
            printf("Ask domain name %s from %s:\n", strDomainName.c_str(), inet_ntoa(in));
            printf("-------- answers --------\n");
            print_records(pResult->pAnswers);
            printf("-------- authoritys --------\n");
            print_records(pResult->pAuthoritys);
            printf("-------- additionals --------\n");
            print_records(pResult->pAdditionals);

            release(pResult);
            pResult = NULL;

            strDomainName = "";

            printf("\n>");
        }
        else if ('\n' != nChar && '\r' != nChar)
        {
            strDomainName.push_back(nChar);
        }
        else
        {
            printf(">");
        }
    }

    return 0;
}

/*
 * unit_test.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "LDNSResolver.h"
#include "../common/constants.h"
#include "../parser/LDNSParser.h"
#include <string.h>

int main()
{
    LDNSResolver test("114.114.114.114");
    printf("result:%d\n", test.Query("luodichen.com", QTYPE::A));
    const LDNSParser *pParser = test.GetParser();
    printf("%s:", pParser->GetAnswers()[0]->GetDomainName().c_str());
    uint8_t ip[4];
    size_t size;
    memcpy((void *)ip, pParser->GetAnswers()[0]->GetBuffer(&size), sizeof(ip));
    printf("%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
    
    return 0;
}

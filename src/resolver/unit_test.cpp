/*
 * unit_test.cpp
 *
 *  Created on: Apr 30, 2015
 *      Author: luodichen
 */

#include "LDNSResolver.h"
#include "../common/constants.h"

int main()
{
    LDNSResolver test("114.114.114.114");
    printf("result:%d\n", test.Query("luodichen.com", QTYPE::A));
    return 0;
}

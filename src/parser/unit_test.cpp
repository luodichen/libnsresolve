/*
 * unit_test.cpp
 *
 *  Created on: 2015年4月28日
 *      Author: luodichen
 */

#include "LDNSParser.h"
#include <stdio.h>
#include <string.h>

int main()
{
    LDNSParser parser;
    int c = 0;
    int i = 0;
    
    while (EOF != (c = getchar()))
    {
        i++;
        int e = parser.StreamInput(c);
        if (e < 0)
        {
            printf("Error %d, i = %d\n", e, i);
            break;
        }
        else if (e == 0)
        {
            printf("completed. i = %d\n", i);
            std::vector<const LResourceRecord *> answers = parser.GetAnswers();
            std::vector<const LResourceRecord *>::iterator iter = answers.begin();
            for (; iter != answers.end(); iter++)
            {
                unsigned char ip[4] = {0};
                size_t size = 0;
                memcpy(ip, (*iter)->GetBuffer(&size), sizeof(ip));
                printf("%s:%u.%u.%u.%u\n", (*iter)->GetDomainName().c_str()
                        , ip[0], ip[1], ip[2], ip[3]);
            }
            break;
        }
    }
    
    return 0;
}

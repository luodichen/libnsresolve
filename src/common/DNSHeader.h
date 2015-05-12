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
 * DNSHeader.h
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#ifndef SRC_PARSER_DNSHEADER_H_
#define SRC_PARSER_DNSHEADER_H_

#include <stdlib.h>
#include <stdint.h>

#pragma pack(1)

struct DNSHEADER 
{
    int16_t     wTransId;
    int16_t     wFlag;
    int16_t     wQuestions;
    int16_t     wAnswerRRs;
    int16_t     wAuthorityRRs;
    int16_t     wAdditionalRRs;
    
    DNSHEADER() 
        : wTransId(0)
        , wFlag(0)
        , wQuestions(0)
        , wAnswerRRs(0)
        , wAuthorityRRs(0)
        , wAdditionalRRs(0)
    {
        
    }
};

struct _HEADER_FLAG
{
    uint16_t rcode:4;
    uint16_t reserved:3;
    uint16_t ra:1;

    uint16_t rd:1;
    uint16_t tc:1;
    uint16_t aa:1;
    uint16_t opcode;
    uint16_t qr;
};

union HEADER_FLAG
{
    uint16_t wFlag;
    _HEADER_FLAG sFlag;
};

#pragma pack()

#endif /* SRC_PARSER_DNSHEADER_H_ */

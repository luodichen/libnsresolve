/*
 * DNSHeader.h
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#ifndef SRC_PARSER_DNSHEADER_H_
#define SRC_PARSER_DNSHEADER_H_

#include <stdlib.h>

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

#pragma pack()

#endif /* SRC_PARSER_DNSHEADER_H_ */

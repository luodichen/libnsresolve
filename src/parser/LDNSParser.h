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
 * LDNSParser.h
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LDNSPARSER_H_
#define SRC_PARSER_LDNSPARSER_H_

#include "../common/errcode.h"
#include "../include/nsresolve.h"
#include "LResourceRecord.h"
#include "LQuestion.h"
#include <stdint.h>
#include <vector>

class LDNSParser
{
public:
    LDNSParser();
    virtual ~LDNSParser();

public:
    int StreamInput(const uint8_t *pBuffer, int nSize);
    int StreamInput(uint8_t word);
    std::vector<const LResourceRecord *> GetAnswers() const;
    std::vector<const LResourceRecord *> GetAuthoritys() const;
    std::vector<const LResourceRecord *> GetAdditionals() const;
    DNSHEADER GetHeader() const;
    
private:
    LDNSParser(const LDNSParser &);
    LDNSParser &operator=(const LDNSParser &);
    
private:
    void ClearRecords(std::vector<LResourceRecord *> &);
    void SetError(int errcode);
    void ReadHeader();
    std::vector<const LResourceRecord *> GetResources(const std::vector<LResourceRecord *> &) const;

private:
    DNSHEADER m_header;
    std::vector<uint8_t> m_buffer;
    LQuestion *m_pQuestion;
    std::vector<LResourceRecord *> m_vAnswerRRs;
    std::vector<LResourceRecord *> m_vAuthorityRRs;
    std::vector<LResourceRecord *> m_vAdditionalRRs;
    
    uint16_t m_sAnswer;
    uint16_t m_sAuthority;
    uint16_t m_sAdditional;
    
    enum STATUS 
    {
        WAIT_HEADER = 0,
        WAIT_QUESTION = 1,
        WAIT_RESOURCES = 2,
        STOPED = 3,
        ERR = -1
    } m_status;
    
    struct CUR
    {
        size_t nHeaderWords;
        size_t nAnswerCount;
        size_t nAuthorityCount;
        size_t nAdditinalCount;
        
        LResourceRecord *pResourceRecord;
        
        CUR()
            : nHeaderWords(0)
            , nAnswerCount(0)
            , nAuthorityCount(0)
            , nAdditinalCount(0)
            , pResourceRecord(NULL)
        {
            
        }
    } m_cur;
    
    int m_nLastError;
};

#endif /* SRC_PARSER_LDNSPARSER_H_ */

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
 * LQuestion.h
 *
 *  Created on: Apr 26, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LQUESTION_H_
#define SRC_PARSER_LQUESTION_H_

#include "../common/errcode.h"
#include "../common/constants.h"
#include "LDomainNameParser.h"
//#include <arpa/inet.h>
#include "../common/socket.h"
#include <stdint.h>
#include <vector>
#include <string>

class LQuestion
{
public:
    LQuestion(const std::vector<uint8_t> &vBuffer);
    LQuestion(const char *szDomainName, uint16_t sType, uint16_t sClass);
    virtual ~LQuestion();
    
private:
    LQuestion(const LQuestion &);
    LQuestion &operator=(const LQuestion &);

public:
    int StreamInput(uint8_t word);
    int MakeBuffer(uint8_t *pBuffer, size_t *pSize);

private:
    std::string m_strDomainName;
    LDomainNameParser *m_pDomainNameParser;

private:
#pragma pack(1)
    struct HEADER
    {
        uint16_t sType;
        uint16_t sClass;

        HEADER()
            : sType(0)
            , sClass(0)
        {

        }
    } m_header;
#pragma pack()

    enum STATUS
    {
        PARSING_DOMAIN_NAME = 0,
        PARSING_HEADER = 1,
        STOPED = 2
    } m_status;

    int m_nBytesReaded;
};

#endif /* SRC_PARSER_LQUESTION_H_ */

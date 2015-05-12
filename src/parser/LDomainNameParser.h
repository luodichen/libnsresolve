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
 * LDomainNameParser.h
 *
 *  Created on: Apr 24, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LDOMAINNAMEPARSER_H_
#define SRC_PARSER_LDOMAINNAMEPARSER_H_

#include <stdint.h>
#include <vector>
#include <string>

class LDomainNameParser
{
public:
    LDomainNameParser(const std::vector<uint8_t> &vBuffer);
    virtual ~LDomainNameParser();

public:
    int StreamInput(uint8_t word);
    std::string GetDomainName() const;
    
private:
    LDomainNameParser(const LDomainNameParser &);
    LDomainNameParser &operator=(const LDomainNameParser &);

private:
    enum STATUS {
        WAIT_LENGTH = 0,
        WAIT_PTR = 1,
        WAIT_WORDS = 2,
        STOPED = 3
    } m_status;

private:
    int m_nSubLen;
    uint16_t m_ptr;
    const std::vector<uint8_t> &m_vBuffer;
    std::string m_strDomainName;
};

#endif /* SRC_PARSER_LDOMAINNAMEPARSER_H_ */

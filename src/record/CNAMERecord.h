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
 * CNAMERecord.h
 *
 *  Created on: 2015年5月7日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_CNAMERECORD_H_
#define SRC_RECORD_CNAMERECORD_H_

#include "BaseRecord.h"
#include "../parser/LDomainNameParser.h"
#include <vector>
#include <string>

class CNAMERecord : public BaseRecord
{
public:
    CNAMERecord(size_t size, const std::vector<uint8_t> &vBuffer);
    virtual ~CNAMERecord();
    
public:
    virtual int StreamInput(uint8_t word);
    std::string GetDomainName() const;
    virtual size_t GetDataLength() const;
    
private:
    LDomainNameParser m_parser;

public:
    static const uint16_t TYPE;
};

#endif /* SRC_RECORD_CNAMERECORD_H_ */

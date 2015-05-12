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
 * MXRecord.h
 *
 *  Created on: May 5, 2015
 *      Author: luodichen
 */

#ifndef SRC_RECORD_MXRECORD_H_
#define SRC_RECORD_MXRECORD_H_

#include "BaseRecord.h"
#include "../parser/LDomainNameParser.h"

class MXRecord : public BaseRecord
{
public:
    MXRecord(size_t size, const std::vector<uint8_t> &vBuffer);
    virtual ~MXRecord();

public:
    virtual int StreamInput(uint8_t word);
    std::string GetDomainName() const;
    virtual size_t GetDataLength() const;
    uint16_t GetPreference() const;

private:
    enum STATUS
    {
        READING_PREFERENCE,
        READING_DOMAIN_NAME
    } m_status;

private:
    LDomainNameParser m_parser;
    uint16_t m_sPreference;

    uint8_t *m_pWrite;

public:
    static const uint16_t TYPE;
};

#endif /* SRC_RECORD_MXRECORD_H_ */

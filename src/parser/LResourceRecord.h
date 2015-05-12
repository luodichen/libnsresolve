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
 * LResourceRecord.h
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LRESOURCERECORD_H_
#define SRC_PARSER_LRESOURCERECORD_H_

#include <string>
#include <vector>
#include <stdint.h>
#include "../common/errcode.h"
#include "../record/BaseRecord.h"
#include "../record/ARecord.h"
#include "../record/NSRecord.h"
#include "../record/MXRecord.h"
#include "../record/CNAMERecord.h"
#include "LDomainNameParser.h"

class LResourceRecord
{
public:
    LResourceRecord(const std::vector<uint8_t> &vBuffer);
    virtual ~LResourceRecord();

public:
    int StreamInput(uint8_t word);
    uint16_t GetType() const;
    uint16_t GetClass() const;
    uint32_t GetTTL() const;
    //const uint8_t *GetBuffer(size_t *pSize) const;
    const BaseRecord *GetRecord() const;
    std::string GetDomainName() const;
    size_t GetBufferSize() const;

private:
    LResourceRecord(const LResourceRecord &);
    LResourceRecord &operator=(const LResourceRecord &);

    BaseRecord *MakeRecord(uint16_t type);

protected:
    std::string m_strDomainName;
    uint16_t m_sType;
    uint16_t m_sClass;
    uint32_t m_nTTL;
    //uint8_t *m_pRecordBuffer;
    uint16_t m_sRecordLength;
    BaseRecord *m_pRecord;

private:
#pragma pack(1)
    struct HEADER
    {
        uint16_t sType;
        uint16_t sClass;
        uint32_t nTTL;
        uint16_t sDataLen;

        HEADER()
            : sType(0)
            , sClass(0)
            , nTTL(0)
            , sDataLen(0)
        {

        }
    } m_header;
#pragma pack()

    enum STATUS
    {
        PARSING_DOMAIN_NAME = 0,
        PARSING_HEADER = 1,
        READING_DATA = 2,
        STOPED = 3
    } m_status;

    LDomainNameParser m_dnp;
    //LDomainNameParser m_dnpRecord;
    uint8_t *m_pHeaderCur;
    //uint16_t m_sBytesReaded;

    const std::vector<uint8_t> m_vBuffer;
};

#endif /* SRC_PARSER_LRESOURCERECORD_H_ */

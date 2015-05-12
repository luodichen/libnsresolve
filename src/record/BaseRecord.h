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
 * BaseRecord.h
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_BASERECORD_H_
#define SRC_RECORD_BASERECORD_H_

#include <stdint.h>
#include <string.h>
#include "../common/errcode.h"

class BaseRecord
{
public:
    BaseRecord(size_t size);
    BaseRecord(const BaseRecord &);
    virtual ~BaseRecord();
    virtual BaseRecord &operator=(const BaseRecord &);
    
public:
    virtual int StreamInput(uint8_t word);
    const uint8_t *GetRawData() const;
    size_t GetRawDataLength() const;
    virtual size_t GetDataLength() const;
    virtual uint16_t GetType() const;
    
private:
    size_t m_nRawSize;
    uint8_t *m_pRawData;
    size_t m_nWordsReaded;

protected:
    uint16_t m_sType;

public:
    static const uint16_t TYPE;
};

#endif /* SRC_RECORD_BASERECORD_H_ */

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
 * ARecord.cpp
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#include "ARecord.h"
#include "../common/constants.h"
#include <string.h>

const uint16_t ARecord::TYPE = QTYPE::A;

ARecord::ARecord(size_t size)
    : BaseRecord(size)
{
    m_sType = TYPE;
}

ARecord::~ARecord()
{
    
}

in_addr_t ARecord::GetIPAddress() const
{
    in_addr_t ret = 0;
    memcpy((void *)&ret, GetRawData(), sizeof(ret));
    
    return ret;
}

size_t ARecord::GetDataLength() const
{
    return sizeof(in_addr_t);
}

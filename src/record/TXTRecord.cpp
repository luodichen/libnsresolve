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
 * TXTRecord.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: luodichen
 */

#include "TXTRecord.h"
#include "../common/constants.h"

const uint16_t TXTRecord::TYPE = QTYPE::TXT;

TXTRecord::TXTRecord(size_t size)
    : BaseRecord(size)
{
    m_sType = TYPE;
}

TXTRecord::~TXTRecord()
{
    
}

std::string TXTRecord::GetTXTString() const
{
    std::string ret = "";
    ret.append((char *)GetRawData(), GetDataLength());
    return ret;
}

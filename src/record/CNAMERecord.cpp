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
 * CNAMERecord.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: luodichen
 */

#include "CNAMERecord.h"
#include "../common/constants.h"

const uint16_t CNAMERecord::TYPE = QTYPE::CNAME;

CNAMERecord::CNAMERecord(size_t size, const std::vector<uint8_t> &vBuffer)
    : BaseRecord(size)
    , m_parser(vBuffer)
{
    m_sType = TYPE;
}

CNAMERecord::~CNAMERecord()
{
    
}

int CNAMERecord::StreamInput(uint8_t word)
{
    int ret = 0;
    if ((ret = BaseRecord::StreamInput(word)) < 0)
        return ret;
    
   ret = m_parser.StreamInput(word);
   
   return ret;
}

std::string CNAMERecord::GetDomainName() const
{
    return m_parser.GetDomainName();
}

size_t CNAMERecord::GetDataLength() const
{
    return GetDomainName().length() + 1;
}

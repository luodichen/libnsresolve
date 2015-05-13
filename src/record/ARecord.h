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
 * ARecord.h
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_ARECORD_H_
#define SRC_RECORD_ARECORD_H_

#include "BaseRecord.h"
#include "../common/socket.h"

class ARecord : public BaseRecord
{
public:
    ARecord(size_t size);
    virtual ~ARecord();
    
public:
    in_addr_t GetIPAddress() const;
    virtual size_t GetDataLength() const;

public:
    static const uint16_t TYPE;
};

#endif /* SRC_RECORD_ARECORD_H_ */

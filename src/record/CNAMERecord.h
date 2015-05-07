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

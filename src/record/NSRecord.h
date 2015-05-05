/*
 * NSRecord.h
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_NSRECORD_H_
#define SRC_RECORD_NSRECORD_H_

#include "BaseRecord.h"
#include "../parser/LDomainNameParser.h"
#include <vector>
#include <string>

class NSRecord : public BaseRecord 
{
public:
    NSRecord(size_t size, const std::vector<uint8_t> &vBuffer);
    virtual ~NSRecord();
    
public:
    virtual int StreamInput(uint8_t word);
    std::string GetDomainName();
    
private:
    LDomainNameParser m_parser;
};

#endif /* SRC_RECORD_NSRECORD_H_ */

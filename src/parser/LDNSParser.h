/*
 * LDNSParser.h
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LDNSPARSER_H_
#define SRC_PARSER_LDNSPARSER_H_

#include "DNSHeader.h"

class LDNSParser
{
public:
    LDNSParser();
    virtual ~LDNSParser();

public:
    void StreamInput(const uint8_t *pBuffer, int nSize);

private:
    DNSHEADER m_header;
};

#endif /* SRC_PARSER_LDNSPARSER_H_ */

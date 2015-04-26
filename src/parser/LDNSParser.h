/*
 * LDNSParser.h
 *
 *  Created on: 2015年4月23日
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LDNSPARSER_H_
#define SRC_PARSER_LDNSPARSER_H_

#include "DNSHeader.h"
#include <stdint.h>
#include <vector>

class LDNSParser
{
public:
    LDNSParser();
    virtual ~LDNSParser();

public:
    void StreamInput(const uint8_t *pBuffer, int nSize);
    void StreamInput(uint8_t word);

private:
    DNSHEADER m_header;
    std::vector<uint8_t> m_buffer;
};

#endif /* SRC_PARSER_LDNSPARSER_H_ */

/*
 * LDomainNameParser.h
 *
 *  Created on: Apr 24, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LDOMAINNAMEPARSER_H_
#define SRC_PARSER_LDOMAINNAMEPARSER_H_

class LDomainNameParser
{
public:
    LDomainNameParser(const std::vector<uint8_t> &vBuffer);
    virtual ~LDomainNameParser();

public:
    int StreamInput(uint8_t word);

private:
    enum STATUS {
        WAIT_LENGTH = 0,
        WAIT_WORDS = 1,
        STOPED = 2
    } m_status;

private:
    int m_nSubLen;
    std::vector<uint8_t> &m_vBuffer;
    std::string m_strDomainName;
};

#endif /* SRC_PARSER_LDOMAINNAMEPARSER_H_ */

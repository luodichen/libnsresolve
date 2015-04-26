/*
 * LQuestion.h
 *
 *  Created on: Apr 26, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LQUESTION_H_
#define SRC_PARSER_LQUESTION_H_

#include "../common/errcode.h"
#include "../common/constants.h"
#include "LDomainNameParser.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <vector>
#include <string>

class LQuestion
{
public:
    LQuestion(const std::vector<uint8_t> &vBuffer);
    LQuestion(const char *szDomainName, uint16_t sType, uint16_t sClass);
    virtual ~LQuestion();

public:
    int StreamInput(uint8_t word);

private:
    std::string m_strDomainName;
    LDomainNameParser *m_pDomainNameParser;

private:
#pragma pack(1)
    struct HEADER
    {
        uint16_t sType;
        uint16_t sClass;

        HEADER()
            : sType(0)
            , sClass(0)
        {

        }
    } m_header;
#pragma pack()

    enum STATUS
    {
        PARSING_DOMAIN_NAME = 0,
        PARSING_HEADER = 1,
        STOPED = 2
    } m_status;

    int m_nBytesReaded;
};

#endif /* SRC_PARSER_LQUESTION_H_ */

/*
 * LResourceRecord.h
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LRESOURCERECORD_H_
#define SRC_PARSER_LRESOURCERECORD_H_

#include <string>
#include <vector>
#include <stdint.h>
#include "../common/errcode.h"
#include "LDomainNameParser.h"

class LResourceRecord
{
public:
	LResourceRecord(const std::vector<uint8_t> &vBuffer);
	virtual ~LResourceRecord();

public:
	int StreamInput(uint8_t word);
	uint16_t GetType() const;
	uint16_t GetClass() const;
	uint32_t GetTTL() const;
	const uint8_t *GetBuffer(size_t *pSize) const;
	std::string GetDomainName() const;
	size_t GetBufferSize() const;
	
private:
	LResourceRecord(const LResourceRecord &);
	LResourceRecord &operator=(const LResourceRecord &);

protected:
	std::string m_strDomainName;
	uint16_t m_sType;
	uint16_t m_sClass;
	uint32_t m_nTTL;
	uint8_t *m_pRecordBuffer;
	uint16_t m_sRecordLength;

private:
#pragma pack(1)
	struct HEADER
	{
	    uint16_t sType;
	    uint16_t sClass;
	    uint32_t nTTL;
	    uint16_t sDataLen;

	    HEADER()
	        : sType(0)
	        , sClass(0)
	        , nTTL(0)
	        , sDataLen(0)
	    {

	    }
	} m_header;
#pragma pack()

	enum STATUS
	{
	    PARSING_DOMAIN_NAME = 0,
	    PARSING_HEADER = 1,
	    READING_DATA = 2,
	    STOPED = 3
	} m_status;

	LDomainNameParser m_dnp;
	uint8_t *m_pHeaderCur;
	uint16_t m_sBytesReaded;
};

#endif /* SRC_PARSER_LRESOURCERECORD_H_ */

/*
 * LResourceRecord.h
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#ifndef SRC_PARSER_LRESOURCERECORD_H_
#define SRC_PARSER_LRESOURCERECORD_H_

#include <string>

class LResourceRecord
{
public:
	LResourceRecord();
	virtual ~LResourceRecord();

protected:
	std::string m_strDomainName;
	uint16_t m_sType;
	uint16_t m_sClass;
	uint32_t m_nTTL;
	uint8_t m_pRecordBuffer;
};

#endif /* SRC_PARSER_LRESOURCERECORD_H_ */

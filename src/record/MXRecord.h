/*
 * MXRecord.h
 *
 *  Created on: May 5, 2015
 *      Author: luodichen
 */

#ifndef SRC_RECORD_MXRECORD_H_
#define SRC_RECORD_MXRECORD_H_

#include "BaseRecord.h"
#include "../parser/LDomainNameParser.h"

class MXRecord : public BaseRecord
{
public:
    MXRecord(size_t size, const std::vector<uint8_t> &vBuffer);
    virtual ~MXRecord();

public:
    virtual int StreamInput(uint8_t word);
    std::string GetDomainName() const;
    virtual size_t GetDataLength() const;

private:
    enum STATUS
    {
        READING_PREFERENCE,
        READING_DOMAIN_NAME
    } m_status;

private:
    LDomainNameParser m_parser;
    uint16_t m_sPreference;

    uint8_t *m_pWrite;
};

#endif /* SRC_RECORD_MXRECORD_H_ */

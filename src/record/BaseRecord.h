/*
 * BaseRecord.h
 *
 *  Created on: 2015年5月5日
 *      Author: luodichen
 */

#ifndef SRC_RECORD_BASERECORD_H_
#define SRC_RECORD_BASERECORD_H_

#include <stdint.h>
#include <string.h>
#include "../common/errcode.h"

class BaseRecord
{
public:
    BaseRecord(size_t size);
    BaseRecord(const BaseRecord &);
    virtual ~BaseRecord();
    virtual BaseRecord &operator=(const BaseRecord &);
    
public:
    virtual int StreamInput(uint8_t word);
    const uint8_t *GetRawData() const;
    size_t GetRawDataLength() const;
    virtual size_t GetDataLength() const;
    
private:
    size_t m_nRawSize;
    uint8_t *m_pRawData;
    size_t m_nWordsReaded;
};

#endif /* SRC_RECORD_BASERECORD_H_ */

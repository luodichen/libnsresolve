/*
 * LResourceRecord.cpp
 *
 *  Created on: Apr 23, 2015
 *      Author: luodichen
 */

#include "LResourceRecord.h"

LResourceRecord::LResourceRecord()
    : m_strDomainName("")
    , m_sType(0)
    , m_sClass(0)
    , m_nTTL(0)
    , m_pRecordBuffer(NULL)
{

}

LResourceRecord::~LResourceRecord()
{
    if (NULL != m_pRecordBuffer)
    {
        delete [] m_pRecordBuffer;
        m_pRecordBuffer = NULL;
    }
}


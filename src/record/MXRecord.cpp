/*
 * MXRecord.cpp
 *
 *  Created on: May 5, 2015
 *      Author: luodichen
 */

#include "MXRecord.h"
#include "../common/constants.h"
//#include <arpa/inet.h>
#include "../common/socket.h"

const uint16_t MXRecord::TYPE = QTYPE::MX;

MXRecord::MXRecord(size_t size, const std::vector<uint8_t> &vBuffer)
    : BaseRecord(size)
    , m_status(READING_PREFERENCE)
    , m_parser(vBuffer)
    , m_sPreference(0)
    , m_pWrite((uint8_t *)&m_sPreference)
{
    m_sType = TYPE;
}

MXRecord::~MXRecord()
{

}

int MXRecord::StreamInput(uint8_t word)
{
    int result = BaseRecord::StreamInput(word);
    if (result < 0)
        return result;

    switch (m_status)
    {
    case READING_PREFERENCE:
        *m_pWrite = word;

        if (++m_pWrite - (uint8_t *)&m_sPreference >= sizeof(m_sPreference))
        {
            m_sPreference = ntohs(m_sPreference);
            m_status = READING_DOMAIN_NAME;
            m_pWrite = NULL;
        }
        break;

    case READING_DOMAIN_NAME:
        return m_parser.StreamInput(word);
        break;
    }

    return result;
}

std::string MXRecord::GetDomainName() const
{
    return m_parser.GetDomainName();
}

size_t MXRecord::GetDataLength() const
{
    return GetDomainName().length() + sizeof(m_sPreference) + 1;
}

uint16_t MXRecord::GetPreference() const
{
    return m_sPreference;
}

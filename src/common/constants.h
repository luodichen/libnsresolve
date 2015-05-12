/*
 * constants.h
 *
 *  Created on: Apr 26, 2015
 *      Author: luodichen
 */

#ifndef SRC_COMMON_CONSTANTS_H_
#define SRC_COMMON_CONSTANTS_H_

#include <stdint.h>

namespace QTYPE
{
    static const uint16_t A     = 0x0001; // Host (A) record
    static const uint16_t NS    = 0x0002; // Name server (NS) record
    static const uint16_t CNAME = 0x0005; // Alias (CNAME) record
    static const uint16_t PTR   = 0x000c; // Reverse-lookup (PTR) record
    static const uint16_t MX    = 0x000f; // Mail exchange (MX) record
    static const uint16_t SRV   = 0x0021; // Service (SRV) record
    static const uint16_t IXFR  = 0x00fb; // Incremental zone transfer (IXFR) record
    static const uint16_t AXFR  = 0x00fc; // Standard zone transfer (AXFR) record
    static const uint16_t ALL   = 0x00ff; // All records
}

namespace QCLASS
{
    static const uint16_t INET  = 0x0001; // Internet
}

namespace FLAG_QR
{
    static const uint8_t QUERY = 0;
    static const uint8_t RESPONSE = 1;
}

namespace FLAG_OPCODE
{
    static const uint8_t STANDARD = 0;
}

namespace FLAG_RCODE
{
    //static const uint8_t NOERROR = 0;
    static const uint8_t FORMAT_INCORRECT = 1;
    static const uint8_t SERVER_ERROR = 2;
    static const uint8_t NAME_ERROR = 3;
    static const uint8_t NOT_SUPPORTED = 4;
    static const uint8_t REJECTED = 5;
}

#endif /* SRC_COMMON_CONSTANTS_H_ */

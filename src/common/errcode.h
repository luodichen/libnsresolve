/*
 * errcode.h
 *
 *  Created on: Apr 24, 2015
 *      Author: luodichen
 */

#ifndef SRC_COMMON_ERRCODE_H_
#define SRC_COMMON_ERRCODE_H_

#define CHKERR(exp, code)                                                       \
{                                                                               \
    if (!(exp)) {                                                               \
        return (code);                                                          \
    }                                                                           \
}

namespace ERR
{
    static const int DOMAIN_NAME_FORMAT_INCORRECT   = -101;
    static const int HIT_THE_END_OF_DOMAIN_NAME     = -102;
    static const int HIT_THE_END_OF_RESOURCE_RECORD = -103;
    static const int HIT_THE_END_OF_QUESTION        = -104;
}

#endif /* SRC_COMMON_ERRCODE_H_ */

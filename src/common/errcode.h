/*
 * Copyright (C) 2015 luodichen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
    static const int ERR_NOERROR                    = 0;
    static const int DOMAIN_NAME_FORMAT_INCORRECT   = -101;
    static const int HIT_THE_END_OF_DOMAIN_NAME     = -102;
    static const int HIT_THE_END_OF_RESOURCE_RECORD = -103;
    static const int HIT_THE_END_OF_QUESTION        = -104;
    static const int NEED_LARGER_BUF                = -105;
    static const int INVALID_PARAM                  = -106;
    static const int NO_MORE_WORDS                  = -107;
    static const int TOO_MUCH_RESOURCE_RECORDS      = -108;
    static const int SOCKET_CONNECT_FAILED          = -109;
    static const int SEND_DATA_FAILED               = -110;
    static const int NOT_CONNECTED                  = -111;
    static const int RECV_FAILED                    = -112;
    static const int RECV_TIMEOUT                   = -113;
    static const int DOMAIN_NAME_INCORRECT          = -114;
    static const int DOMAIN_NAME_TOO_LONG           = -115;
}

#endif /* SRC_COMMON_ERRCODE_H_ */

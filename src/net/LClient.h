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
 * LClient.h
 *
 *  Created on: Apr 28, 2015
 *      Author: luodichen
 */

#ifndef SRC_NET_LCLIENT_H_
#define SRC_NET_LCLIENT_H_

#include "INetIO.h"
/*
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
*/
#include "../common/socket.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

class LClient : public INetIO
{
public:
    typedef enum
    {
        TCP = 0,
        UDP = 1
    } TYPE;

#ifdef _MSC_VER
    class WSAInit
    {
    public:
        WSAInit()
        {
            WSADATA wsaData = {0};
            WSAStartup(MAKEWORD(2, 2), &wsaData);
        }

        ~WSAInit()
        {
            WSACleanup();
        }
    };

    const static WSAInit s_wsainit;
#endif /* _MSC_VER */

public:
    LClient(in_addr_t address, uint16_t port, TYPE type);
    LClient(const char *szAddress, uint16_t port, TYPE type);
    virtual ~LClient();

public:
    virtual int Write(const uint8_t *pBuffer, size_t size, uint32_t timeout);
    virtual int Read(uint8_t *pBuffer, size_t max, uint32_t timeout);

private:
    int Init();
    void SetTimeout(int opt, uint32_t value);

private:
    in_addr m_address;
    uint16_t m_sPort;
    //uint32_t m_nTimeout;
    TYPE m_type;
    int m_socket;
    bool m_connected;
    sockaddr_in m_sockaddr;
};

#endif /* SRC_NET_LCLIENT_H_ */

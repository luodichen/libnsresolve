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
 * LClient.cpp
 *
 *  Created on: Apr 28, 2015
 *      Author: luodichen
 */

#include "../common/errcode.h"
#include "LClient.h"
#include <time.h>
#include <string.h>
//#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

static const int MAX_TIMEOUT = 8;
static const int MAX_RETRY = 10;

#ifdef _MSC_VER
const LClient::WSAInit LClient::s_wsainit;
#endif /* _MSC_VER */

LClient::LClient(in_addr_t address, uint16_t port, TYPE type, uint32_t timeout)
    : INetIO()
    , m_address()
    , m_sPort(port)
    , m_nTimeout(timeout)
    , m_type(type)
    , m_socket(-1)
    , m_connected(false)
    , m_sockaddr()
{
    m_address.s_addr = address;
    int result = Init();
    assert(0 == result);
}

LClient::LClient(const char *szAddress, uint16_t port, TYPE type, uint32_t timeout)
    : INetIO()
    , m_address()
    , m_sPort(port)
    , m_nTimeout(timeout)
    , m_type(type)
    , m_socket(-1)
    , m_connected(false)
    , m_sockaddr()
{
    m_address.s_addr = inet_addr(szAddress);
    int result = Init();
    assert(0 == result);
}

LClient::~LClient()
{
    if (m_socket >= 0)
    {
        //close(m_socket);
        close_socket(m_socket);
        m_socket = -1;
    }
}

int LClient::Init()
{
    int ret = 0;
    int type = (LClient::TCP == m_type) ? SOCK_STREAM : SOCK_DGRAM;
    int protocol = (LClient::TCP == m_type) ? IPPROTO_TCP : IPPROTO_UDP;
    m_socket = socket(AF_INET, type, protocol);
    if (-1 == m_socket)
        ret = -1;

    if (LClient::TCP == m_type)
    {
        struct timeval timeout;
        memset((void *)&timeout, 0, sizeof(timeout));
        timeout.tv_sec = m_nTimeout;

        setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, CONST_BUF(&timeout), sizeof(timeout));
        setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, CONST_BUF(&timeout), sizeof(timeout));
    }

    memset((void *)&m_sockaddr, 0, sizeof(m_sockaddr));
    m_sockaddr.sin_family = AF_INET;
    m_sockaddr.sin_port = htons(m_sPort);
    m_sockaddr.sin_addr = m_address;

    return ret;
}

int LClient::Write(const uint8_t *pBuffer, size_t size)
{
    if (!m_connected
            && 0 != connect(m_socket, (sockaddr*)&m_sockaddr, sizeof(m_sockaddr)))
    {
        return ERR::SOCKET_CONNECT_FAILED;
    }
    m_connected = true;

    if (-1 == send(m_socket, CONST_BUF(pBuffer), size, 0))
    {
        return ERR::SEND_DATA_FAILED;
    }

    return 0;
}

int LClient::Read(uint8_t *pBuffer, size_t max)
{
    int ret = 0;
    if (!m_connected)
        return ERR::NOT_CONNECTED;

    if (LClient::TCP == m_type)
    {
        int count = recv(m_socket, RECV_BUF(pBuffer), max, 0);
        if (-1 == count)
        {
            ret = ERR::RECV_FAILED;
        }
        ret = count;
    }
    else
    {
        struct timeval timeout;
        memset((void *)&timeout, 0, sizeof(timeout));
        timeout.tv_sec = 1;
        time_t tBegin = time(NULL);

        int count = 0;
        int nTryCount = 0;
        ret = ERR::RECV_TIMEOUT;
        while ((time(NULL) - tBegin < m_nTimeout) && nTryCount++ < MAX_RETRY)
        {
            setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, CONST_BUF(&timeout), sizeof(timeout));
            count = recv(m_socket, RECV_BUF(pBuffer), max, 0);

            if (-1 == count)
            {
                timeout.tv_sec = ((timeout.tv_sec * 2 <= MAX_TIMEOUT) ? 2 : 1) * timeout.tv_sec;
            }
            else if (count >= 0)
            {
                ret = count;
                break;
            }
            else
            {
                ret = ERR::RECV_FAILED;
                break;
            }
        }
    }
    return ret;
}

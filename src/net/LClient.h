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
    LClient(in_addr_t address, uint16_t port = 53, TYPE type = UDP, uint32_t timeout = 15);
    LClient(const char *szAddress, uint16_t port = 53, TYPE type = UDP, uint32_t timeout = 15);
    virtual ~LClient();

public:
    virtual int Write(const uint8_t *pBuffer, size_t size);
    virtual int Read(uint8_t *pBuffer, size_t max);

private:
    int Init();

private:
    in_addr m_address;
    uint16_t m_sPort;
    uint32_t m_nTimeout;
    TYPE m_type;
    int m_socket;
    bool m_connected;
    sockaddr_in m_sockaddr;
};

#endif /* SRC_NET_LCLIENT_H_ */

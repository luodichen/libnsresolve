/*
 * socket.h
 *
 *  Created on: May 12, 2015
 *      Author: luodichen
 */

#ifndef _L_SOCKET_H
#define _L_SOCKET_H
#ifdef _MSC_VER
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define in_addr_t uint32_t
#define close_socket(s) closesocket(s)
#define CONST_BUF(b) ((const char *)(b))
#define RECV_BUF(b) ((char *)(b))
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define close_socket(s) close(s)
#define CONST_BUF(b) ((void *)(b))
#define RECV_BUF(b) ((void *)(b))
#endif /* _MSC_VER */
#endif /* _L_SOCKET_H */

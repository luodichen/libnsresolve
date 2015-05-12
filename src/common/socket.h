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

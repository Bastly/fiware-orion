#ifndef CLIENT_SOCKET_HTTP_H
#define CLIENT_SOCKET_HTTP_H

/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* iot_support at tid dot es
*
* Author: developer
*/
#include <string>
#include <vector>

#include "ConnectionInfo.h"

#define URI_BUF          (256)
#define TAM_BUF          (8 * 1024)            // 8 KB  (for HTTP responses and pre-payload bytes in request, which will be very small)
#define MAX_STA_MSG_SIZE (20 * 1024)           // 20 KB (HTTP request static buffer)
#define MAX_DYN_MSG_SIZE (8 * 1024 * 1024)     // 8 MB  (maximum length of the HTTP request dynamic buffer)

/***************************************************************************
*
* socketHttpConnect -
*/
extern int socketHttpConnect(const std::string& host, unsigned short port);


/* ****************************************************************************
*
* sendHttpSocket - 
*/
extern std::string sendHttpSocket
(
  const std::string&     ip,
  unsigned short         port, 
  const std::string&     protocol,
  const std::string&     verb,
  const std::string&     tenant,
  const std::string&     servicePath,
  const std::string&     xauthToken,
  const std::string&     resource, 
  const std::string&     content_type, 
  const std::string&     content,
  bool                   useRush,
  bool                   waitForResponse
);

#endif

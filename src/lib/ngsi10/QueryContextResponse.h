#ifndef QUERY_CONTEXT_RESPONSE_H
#define QUERY_CONTEXT_RESPONSE_H

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
* Author: Ken Zangelin
*/
#include <string>
#include <iostream>
#include <sstream>

#include "ngsi/ContextElementResponseVector.h"
#include "common/Format.h"
#include "ngsi/StatusCode.h"
#include "rest/ConnectionInfo.h"



/* ****************************************************************************
*
* QueryContextResponse - 
*
* NOTE
* Both contextElementResponseV and statusCode are optional, but a response
* MUST contain either contextElementResponseV OR statusCode.
*/
typedef struct QueryContextResponse
{
  ContextElementResponseVector  contextElementResponseVector;  // Optional
  StatusCode                    errorCode;                     // Optional

  QueryContextResponse();
  QueryContextResponse(StatusCode& _errorCode);
  ~QueryContextResponse();

  std::string   render(ConnectionInfo* ciP, RequestType requestType, const std::string& indent);  
  std::string   check(ConnectionInfo* ciP, RequestType requestType, const std::string& indent, const std::string& predetectedError, int counter);
  void          present(const std::string& indent);
  void          release(void);  
} QueryContextResponse;

#endif

#ifndef SRC_LIB_LOGMSG_TRACELEVELS_H_
#define SRC_LIB_LOGMSG_TRACELEVELS_H_

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



/* ****************************************************************************
*
* TraceLevels - 
*/
typedef enum TraceLevels
{
  /* The "tracelevels" group are largely based on the different processing levels used
   * at Context Broker */

  /* Rest (20-39) */
  LmtRest = 20,
  LmtRestCompare,
  LmtUrlParse,
  LmtHttpRequest,
  LmtHttpHeaders,
  LmtHttpDaemon = 25,
  LmtHttpUnsupportedHeader,
  LmtMhd,
  LmtSavedResponse,
  LmtIncompletePayload,
  LmtTenant = 30,
  LmtUriParams,
  LmtServicePath,
  LmtPagination,
  LmtCoap,
  LmtHttps = 35,
  LmtIpVersion,
  LmtCtxProviders,

  /* Parser (40-59) */
  LmtParse    = 40,
  LmtParsedPayload,
  LmtParseCheck,
  LmtPresent,
  LmtNew,
  LmtTreat = 45,
  LmtDump,
  LmtNullNode,
  LmtCompoundValue,
  LmtCompoundValueAdd,
  LmtCompoundValueLookup = 50,
  LmtCompoundValueRender,
  LmtCompoundValueRaw,
  LmtCompoundValueContainer,
  LmtCompoundValueStep,
  LmtCompoundValueShow = 55,
  LmtJsonAttributes,

  /* RestService and Service (60-79) */
  LmtService     = 60,
  LmtConvenience,

  /* ConvenienceMap (80-99) */
  LmtClone = 80,

  /* MongoBackend (100-119) */
  LmtMongo = 100,

  /* Cleanup (120-139) */
  LmtDestructor = 120,
  LmtRelease,

  /* Types (140-159) */
  LmtEntityId = 140,
  LmtRestriction,
  LmtScope,

  /* Notifications (160-179) */
  LmtNotifier = 160,
  LmtOntimeIntervalThreads,

  /* Input/Output payloads (180-199) */
  LmtServiceInputPayload = 180,
  LmtServiceOutPayload,
  LmtClientInputPayload,
  LmtClientOutputPayload = 183,  // Very important for harness test notification_different_sizes
  LmtPartialPayload,
  LmtClientOutputPayloadDump,

  /* Semaphores (200-202) */
  LmtReqSem = 200,
  LmtMongoSem,
  LmtTransSem,

  /* Others (>=210) */
  LmtCm = 210,
  LmtIotaXmlReg,
  LmtIotaXmlObs,
  LmtRush,
  LmtSoftError,
  LmtNotImplemented,

  LmtBug = 250
} TraceLevels;



/* ****************************************************************************
*
* traceLevelName - 
*/
extern char* traceLevelName(TraceLevels level);

#endif  // SRC_LIB_LOGMSG_TRACELEVELS_H_

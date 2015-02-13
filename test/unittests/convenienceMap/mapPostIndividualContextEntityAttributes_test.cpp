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
#include "gtest/gtest.h"

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"
#include "common/globals.h"

#include "convenience/RegisterProviderRequest.h"
#include "convenienceMap/mapPostIndividualContextEntityAttributes.h"
#include "mongoBackend/mongoRegisterContext.h"
#include "ngsi9/RegisterContextResponse.h"
#include "ngsi9/DiscoverContextAvailabilityResponse.h"
#include "rest/ConnectionInfo.h"

#include "testInit.h"



/* ****************************************************************************
*
* prepareDatabase -
*/
static void prepareDatabase(std::string id, std::string type)
{
  /* Set database */
  setupDatabase();

  DBClientBase* connection = getMongoConnection();

  /* We create one entity:
   *
   * - 'id', 'type' with four attributes
   *     A1: X
   *     A1: Y
   *     A2: Z
   *     A3: W
   */

  BSONObj en1 = BSON("_id" << BSON("id" << id << "type" << type) <<
                     "attrs" << BSON_ARRAY(
                        BSON("name" << "A1" << "type" << "TA1" << "value" << "X") <<
                        BSON("name" << "A1" << "type" << "TA1bis" << "value" << "Y") <<
                        BSON("name" << "A2" << "type" << "TA2" << "value" << "Z") <<
                        BSON("name" << "A3" << "type" << "TA3" << "value" << "W")
                        )
     );

  connection->insert(ENTITIES_COLL, en1);
}



/* ****************************************************************************
*
* newEntityThenNewEntity -
*/
TEST(mapPostIndividualContextEntityAttributes, newEntityThenNewEntity)
{
  HttpStatusCode                ms;
  AppendContextElementRequest   request;
  AppendContextElementResponse  response;
  ConnectionInfo                ci;

  /* Set timer */
  Timer* t = new Timer();
  setTimer(t);

  prepareDatabase("MPICE", "ttt");
  request.attributeDomainName.set("ad");

  ms = mapPostIndividualContextEntityAttributes("MPICE", &request, &response, &ci);
  EXPECT_EQ(SccOk, ms);
  EXPECT_EQ(SccOk, response.errorCode.code);

  ms = mapPostIndividualContextEntityAttributes("MPICE2", &request, &response, &ci);
  EXPECT_EQ(SccOk, ms);
  EXPECT_EQ(SccOk, response.errorCode.code);
}

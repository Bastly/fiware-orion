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
* fermin at tid dot es
*
* Author: Ken Zangelin
*/
#include <stdio.h>
#include <string>
#include <vector>

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "common/tag.h"
#include "ngsi/AttributeList.h"



/* ****************************************************************************
*
* render - 
*/
std::string AttributeList::render(Format format, const std::string& indent, bool comma)
{
  std::string  out = "";
  std::string  tag = "attributeList";

  if (attributeV.size() == 0)
    return "";

  out += startTag(indent, tag, format);

  for (unsigned int ix = 0; ix < attributeV.size(); ++ix)
    out += valueTag(indent + "  ", "attribute", attributeV[ix], format, ix != attributeV.size() - 1);

  out += endTag(indent, tag, format, comma);

  return out;
}



/* ****************************************************************************
*
* AttributeList::check - 
*/
std::string AttributeList::check
(
  RequestType         requestType,
  Format              format,
  const std::string&  indent,
  const std::string&  predetectedError,
  int                 counter
)
{
  for (unsigned int ix = 0; ix < attributeV.size(); ++ix)
  {
    if (attributeV[ix] == "")
      return "empty attribute name";
  }

  return "OK";
}



/* ****************************************************************************
*
* AttributeList::present - 
*/
void AttributeList::present(const std::string& indent)
{
  PRINTF("%sAttribute List\n",    indent.c_str());

  for (unsigned int ix = 0; ix < attributeV.size(); ++ix)
    PRINTF("%s  %s\n", indent.c_str(), attributeV[ix].c_str());
}



/* ****************************************************************************
*
* AttributeList::release - 
*/
void AttributeList::release(void)
{
  attributeV.clear();
}



/* ****************************************************************************
*
* push_back - 
*/
void AttributeList::push_back(const std::string& attributeName)
{
  attributeV.push_back(attributeName);
}



/* ****************************************************************************
*
* AttributeList::size - 
*/
unsigned int AttributeList::size(void)
{
  return attributeV.size();
}



/* ****************************************************************************
*
* AttributeList::get - 
*/
std::string AttributeList::get(int ix)
{
  return attributeV[ix];
}

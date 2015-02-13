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
#include <stdio.h>
#include <string>

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "common/tag.h"
#include "ngsi/ContextAttribute.h"
#include "rest/ConnectionInfo.h"
#include "rest/uriParamNames.h"



/* ****************************************************************************
*
* ContextAttribute::ContextAttribute - 
*/
ContextAttribute::ContextAttribute()
{
  LM_T(LmtClone, ("Creating a ContextAttribute 1"));
  name                  = "";
  type                  = "";
  value                 = "";
  compoundValueP        = NULL;
  typeFromXmlAttribute  = "";
}



/* ****************************************************************************
*
* ContextAttribute::ContextAttribute - 
*/
ContextAttribute::ContextAttribute(ContextAttribute* caP)
{
  name                  = caP->name;
  type                  = caP->type;
  value                 = caP->value;
  compoundValueP        = (caP->compoundValueP)? caP->compoundValueP->clone() : NULL;
  typeFromXmlAttribute  = "";

  LM_T(LmtClone, ("Creating a ContextAttribute: compoundValueP at %p for attribute '%s' at %p",
                  compoundValueP,
                  name.c_str(),
                  this));

  // Cloning metadata
  for (unsigned int mIx = 0; mIx < caP->metadataVector.size(); ++mIx)
  {
    LM_T(LmtClone, ("Copying metadata %d", mIx));
    Metadata* mP = new Metadata(caP->metadataVector.get(mIx));
    metadataVector.push_back(mP);
  }
}



/* ****************************************************************************
*
* ContextAttribute::ContextAttribute - 
*/
ContextAttribute::ContextAttribute
(
  const std::string&  _name,
  const std::string&  _type,
  const std::string&  _value
)
{
  LM_T(LmtClone, ("Creating a ContextAttribute '%s':'%s':'%s', setting its compound to NULL",
                  _name.c_str(),
                  _type.c_str(),
                  _value.c_str()));

  name                  = _name;
  type                  = _type;
  value                 = _value;
  compoundValueP        = NULL;
  typeFromXmlAttribute  = "";
}



/* ****************************************************************************
*
* ContextAttribute::ContextAttribute -
*/
ContextAttribute::ContextAttribute
(
  const std::string&         _name,
  const std::string&         _type,
  orion::CompoundValueNode*  _compoundValueP
)
{
  LM_T(LmtClone, ("Creating a ContextAttribute, maintaing a pointer to compound value (at %p)", _compoundValueP));

  name                  = _name;
  type                  = _type;
  compoundValueP        = _compoundValueP;
  typeFromXmlAttribute  = "";
}



/* ****************************************************************************
*
* ContextAttribute::getId() -
*/
std::string ContextAttribute::getId(void)
{
  for (unsigned int ix = 0; ix < metadataVector.size(); ++ix)
  {
    if (metadataVector.get(ix)->name == NGSI_MD_ID)
    {
      return metadataVector.get(ix)->value;
    }
  }

  return "";
}



/* ****************************************************************************
*
* ContextAttribute::getLocation() -
*/
std::string ContextAttribute::getLocation()
{
  for (unsigned int ix = 0; ix < metadataVector.size(); ++ix)
  {
    if (metadataVector.get(ix)->name == NGSI_MD_LOCATION)
    {
      return metadataVector.get(ix)->value;
    }
  }

  return "";
}



/* ****************************************************************************
*
* renderAsJsonObject - 
*/
std::string ContextAttribute::renderAsJsonObject
(
  ConnectionInfo*     ciP,
  const std::string&  indent,
  bool                comma,
  bool                omitValue
)
{
  std::string  out                    = "";
  std::string  jsonTag                = name;
  bool         commaAfterContextValue = metadataVector.size() != 0;
  bool         commaAfterType         = !omitValue || commaAfterContextValue;

  out += startTag(indent, "", jsonTag, ciP->outFormat, false, true);
  out += valueTag(indent + "  ", "type",         type,  ciP->outFormat, commaAfterType);

  if (compoundValueP == NULL)
  {
    if (omitValue == false)
    {
      out += valueTag(indent + "  ", ((ciP->outFormat == XML)? "contextValue" : "value"),
                      value, ciP->outFormat, commaAfterContextValue);
    }
  }
  else
  {
    bool isCompoundVector = false;

    if ((compoundValueP != NULL) && (compoundValueP->type == orion::CompoundValueNode::Vector))
    {
      isCompoundVector = true;
    }

    out += startTag(indent + "  ", "contextValue", "value", ciP->outFormat, isCompoundVector, true, isCompoundVector);
    out += compoundValueP->render(ciP->outFormat, indent + "    ");
    out += endTag(indent + "  ", "contextValue", ciP->outFormat, commaAfterContextValue, isCompoundVector);
  }

  if (omitValue == false)
  {
    out += metadataVector.render(ciP->outFormat, indent + "  ", false);
  }

  out += endTag(indent, "", ciP->outFormat, comma);

  return out;
}



/* ****************************************************************************
*
* render - 
*/
std::string ContextAttribute::render
(
  ConnectionInfo*     ciP,
  const std::string&  indent,
  bool                comma,
  bool                omitValue
)
{
  std::string  out                    = "";
  std::string  xmlTag                 = "contextAttribute";
  std::string  jsonTag                = "attribute";
  bool         commaAfterContextValue = metadataVector.size() != 0;
  bool         commaAfterType         = !omitValue || commaAfterContextValue;
  bool         commaAfterName         = commaAfterType || (type != "");

  metadataVector.tagSet("metadata");

  if ((ciP->uriParam[URI_PARAM_ATTRIBUTE_FORMAT] == "object") && (ciP->outFormat == JSON))
  {
    return renderAsJsonObject(ciP, indent, comma, omitValue);
  }

  out += startTag(indent, xmlTag, jsonTag, ciP->outFormat, false, false);
  out += valueTag(indent + "  ", "name",         name,  ciP->outFormat, commaAfterName);
  out += valueTag(indent + "  ", "type",         type,  ciP->outFormat, commaAfterType);

  if (compoundValueP == NULL)
  {
    if (omitValue == false)
    {
      out += valueTag(indent + "  ", ((ciP->outFormat == XML)? "contextValue" : "value"),
                      value, ciP->outFormat, commaAfterContextValue);
    }
  }
  else
  {
    bool isCompoundVector = false;

    if ((compoundValueP != NULL) && (compoundValueP->type == orion::CompoundValueNode::Vector))
    {
      isCompoundVector = true;
    }

    out += startTag(indent + "  ", "contextValue", "value", ciP->outFormat, isCompoundVector, true, isCompoundVector);
    out += compoundValueP->render(ciP->outFormat, indent + "    ");
    out += endTag(indent + "  ", "contextValue", ciP->outFormat, commaAfterContextValue, isCompoundVector);
  }

  out += metadataVector.render(ciP->outFormat, indent + "  ", false);
  out += endTag(indent, xmlTag, ciP->outFormat, comma);

  return out;
}



/* ****************************************************************************
*
* ContextAttribute::check - 
*/
std::string ContextAttribute::check
(
  RequestType         requestType,
  Format              format,
  const std::string&  indent,
  const std::string&  predetectedError,
  int                 counter
)
{
  if (name == "")
  {
    return "missing attribute name";
  }

  if ((compoundValueP != NULL) && (compoundValueP->childV.size() != 0))
  {
    // FIXME P9: Use CompoundValueNode::check here and stop calling it from where it is called right now.
    //           Also, change CompoundValueNode::check to return std::string
    return "OK";
  }

  return "OK";
}



/* ****************************************************************************
*
* ContextAttribute::present - 
*/
void ContextAttribute::present(const std::string& indent, int ix)
{
  PRINTF("%sAttribute %d:\n",    indent.c_str(), ix);
  PRINTF("%s  Name:       %s\n", indent.c_str(), name.c_str());
  PRINTF("%s  Type:       %s\n", indent.c_str(), type.c_str());

  if (compoundValueP == NULL)
  {
    PRINTF("%s  Value:      %s\n", indent.c_str(), value.c_str());
  }
  else
  {
    compoundValueP->show(indent + "  ");
  }

  metadataVector.present("Attribute", indent + "  ");
}



/* ****************************************************************************
*
* ContextAttribute::release - 
*/
void ContextAttribute::release(void)
{
  if (compoundValueP != NULL)
  {
    delete compoundValueP;
    compoundValueP = NULL;
  }

  metadataVector.release();
}



/* ****************************************************************************
*
* toString - 
*/
std::string ContextAttribute::toString(void)
{
  return name;
}

/*
*
*  Copyright 2011 by the CALATK development team
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*
*/

#ifndef C_PROCESS_BASE_H
#define C_PROCESS_BASE_H

#include "json/json-forwards.h"
#include "json/json.h"
#include "CALATKCommon.h"
#include "CObject.h"
#include "JSONParameterUtils.h"

namespace CALATK
{

/**
 * \class CProcessBase 
 * Base class for all classes that do processing (and have parameters)
 * so that they can provide functionality to auto-initialize based on a JSON configuration description
 */
class CProcessBase: public CObject
{
public:
  /** Standard class typedefs. */
  typedef CProcessBase                    Self;
  typedef CObject                         Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  CProcessBase();

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

  const Json::Value* GetAutoConfigurationIn();

  const Json::Value* GetAutoConfigurationOut();

  void SetPrintConfiguration( bool bPrint );

  bool GetPrintConfiguration();

  void SetAllowHelpComments( bool bAllow );

  bool GetAllowHelpComments();

protected:
  CJSONConfiguration m_jsonConfigIn;
  CJSONConfiguration m_jsonConfigOut;

private:
  // if true the Json configurations will be printed
  bool m_PrintConfiguration;

  // if true help comments will be added to the JSON configuration file
  bool m_AllowHelpComments;
};

} // end namespace

#endif

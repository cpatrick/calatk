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
#include "JSONParameterUtils.h"

namespace CALATK
{

/**
 * CBase.h -- Base class for all classes that do processing (and have parameters)
 * so that they can provide functionality to auto-initialize based on a JSON configuration description
 */

class CProcessBase
{
public:
  CProcessBase()
    : DefaultPrintConfiguration( true ), m_ExternallySetPrintConfiguration( false )
  {
      m_PrintConfiguration = DefaultPrintConfiguration;
  }

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
  {
    m_jsonConfigIn.SetRootReference( ConfValueIn );
    m_jsonConfigOut.SetRootReference( ConfValueOut );

    // this is the input file that is being parsed; only the fresh values (in out) are printed as needed
    m_jsonConfigOut.PrintSettingsOff();
    m_jsonConfigOut.AllowHelpCommentsOn();

    if ( this->GetPrintConfiguration() )
      {
      m_jsonConfigIn.PrintSettingsOn();
      }
    else
      {
      m_jsonConfigIn.PrintSettingsOff();
      }
  }

  const Json::Value* GetAutoConfigurationIn()
  {
    return m_jsonConfigIn.GetRootPointer();
  }

  const Json::Value* GetAutoConfigurationOut()
  {
    return m_jsonConfigOut.GetRootPointer();
  }

  SetMacro( PrintConfiguration, bool );
  GetMacro( PrintConfiguration, bool );

protected:
  CJSONConfiguration m_jsonConfigIn;
  CJSONConfiguration m_jsonConfigOut;

private:
  // if true the Json configurations will be printed
  bool m_PrintConfiguration;
  bool DefaultPrintConfiguration;
  bool m_ExternallySetPrintConfiguration;

};


} // end namespace

#endif

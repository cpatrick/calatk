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

#include "CProcessBase.h"

namespace CALATK
{

CProcessBase::CProcessBase():
  m_PrintConfiguration( true ),
  m_AllowHelpComments( false )
{
  this->m_jsonConfigOut.PrintSettingsOff();
  this->m_jsonConfigIn.PrintSettingsOn();

  this->m_jsonConfigOut.AllowHelpCommentsOff();
  this->m_jsonConfigIn.AllowHelpCommentsOff();
}

void CProcessBase::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  this->m_jsonConfigIn.SetRootReference( ConfValueIn );
  this->m_jsonConfigOut.SetRootReference( ConfValueOut );
}

const Json::Value* CProcessBase::GetAutoConfigurationIn()
{
  return this->m_jsonConfigIn.GetRootPointer();
}

const Json::Value* CProcessBase::GetAutoConfigurationOut()
{
  return this->m_jsonConfigOut.GetRootPointer();
}

void CProcessBase::SetPrintConfiguration( bool bPrint )
{
  this->m_PrintConfiguration = bPrint;
  if ( this->m_PrintConfiguration )
  {
    this->m_jsonConfigIn.PrintSettingsOn();
  }
  else
  {
    this->m_jsonConfigIn.PrintSettingsOff();
  }
}

bool CProcessBase::GetPrintConfiguration()
{
  return this->m_PrintConfiguration;
}

void CProcessBase::SetAllowHelpComments( bool bAllow )
{
  this->m_AllowHelpComments = bAllow;
  if ( this->m_AllowHelpComments )
  {
    this->m_jsonConfigIn.AllowHelpCommentsOn();
    this->m_jsonConfigOut.AllowHelpCommentsOn();
  }
  else
  {
    this->m_jsonConfigIn.AllowHelpCommentsOff();
    this->m_jsonConfigOut.AllowHelpCommentsOff();
  }
}

bool CProcessBase::GetAllowHelpComments()
{
  return this->m_AllowHelpComments;
}

} // end namespace CALATK

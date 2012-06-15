/*
*
*  Copyright 2011, 2012 by the CALATK development team
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
  m_AutoConfigurationSet( false ),
  m_PrintConfiguration( true ),
  m_AllowHelpComments( false )
{
  this->m_CombinedJSONConfig = new CJSONConfiguration;
  this->m_CleanedJSONConfig = new CJSONConfiguration;

  this->m_CleanedJSONConfig->PrintSettingsOff();
  this->m_CombinedJSONConfig->PrintSettingsOn();

  this->m_CleanedJSONConfig->AllowHelpCommentsOff();
  this->m_CombinedJSONConfig->AllowHelpCommentsOff();
}

void CProcessBase::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  this->m_CombinedJSONConfig = combined;
  this->m_CleanedJSONConfig  = cleaned;
  this->m_AutoConfigurationSet = true;
}


const CJSONConfiguration * CProcessBase::GetJSONConfigurationCombined()
{
  return this->m_CombinedJSONConfig.GetPointer();
}


const CJSONConfiguration * CProcessBase::GetJSONConfigurationCleaned()
{
  return this->m_CleanedJSONConfig.GetPointer();
}


void CProcessBase::SetPrintConfiguration( bool bPrint )
{
  this->m_PrintConfiguration = bPrint;
  if ( this->m_PrintConfiguration )
    {
    this->m_CombinedJSONConfig->PrintSettingsOn();
    }
  else
    {
    this->m_CombinedJSONConfig->PrintSettingsOff();
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
    this->m_CombinedJSONConfig->AllowHelpCommentsOn();
    this->m_CleanedJSONConfig->AllowHelpCommentsOn();
    }
  else
    {
    this->m_CombinedJSONConfig->AllowHelpCommentsOff();
    this->m_CleanedJSONConfig->AllowHelpCommentsOff();
    }
}


bool CProcessBase::GetAllowHelpComments()
{
  return this->m_AllowHelpComments;
}

} // end namespace CALATK

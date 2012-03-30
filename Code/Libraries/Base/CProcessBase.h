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
#include "CBase.h"
#include "CJSONConfiguration.h"

namespace CALATK
{

/**
 * \class CProcessBase
 * Base class for all classes that do processing (and have parameters)
 * so that they can provide functionality to auto-initialize based on a JSON configuration description
 */
class CProcessBase: public CBase
{
public:
  /** Standard class typedefs. */
  typedef CProcessBase                    Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  CProcessBase();

  /** Set the CJSONConfiguration that will be used for processing.  The first
   * configuration should contain an inputs that will be used for processing.
   * Both configurations will have the values used and any defaults that were
   * used but not specified. */
  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

  /** Get the configuration that is a combination of the defaults that were used
   * and the input configuration, including the input configuration that not
   * used. */
  const CJSONConfiguration * GetJSONConfigurationCombined();
  /** Get the configuration that is a combination of the defaults that were used
   * and the input configuration, excluding the input configuration that not
   * used. */
  const CJSONConfiguration * GetJSONConfigurationCleaned();

  //! if true the Json configurations will be printed
  void SetPrintConfiguration( bool print );
  bool GetPrintConfiguration();

  //! if true help comments will be added to the JSON configuration file
  void SetAllowHelpComments( bool allow );
  bool GetAllowHelpComments();

protected:
  CJSONConfiguration::Pointer m_CombinedJSONConfig;
  CJSONConfiguration::Pointer m_CleanedJSONConfig;

  bool m_AutoConfigurationSet;

private:
  bool m_PrintConfiguration;
  bool m_AllowHelpComments;
};

} // end namespace

#endif

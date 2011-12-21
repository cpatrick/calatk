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

  virtual void SetAutoConfiguration( Json::Value& ConfValue )
  {
    m_jsonConfig.SetRootReference( ConfValue );
    if ( this->GetPrintConfiguration() )
      {
      m_jsonConfig.PrintSettingsOn();
      }
    else
      {
      m_jsonConfig.PrintSettingsOff();
      }
  }

  const Json::Value* GetAutoConfiguration()
  {
    return m_jsonConfig.GetRootPointer();
  }

  SetMacro( PrintConfiguration, bool );
  GetMacro( PrintConfiguration, bool );

protected:
  CJSONConfiguration m_jsonConfig;

private:
  // if true the Json configurations will be printed
  bool m_PrintConfiguration;
  bool DefaultPrintConfiguration;
  bool m_ExternallySetPrintConfiguration;

};


} // end namespace

#endif

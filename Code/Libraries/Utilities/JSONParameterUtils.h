#ifndef JSON_PARAMETER_UTILS
#define JSON_PARAMETER_UTILS

#include "json/json-forwards.h"
#include "json/json.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace CALATK
{

class CJSONConfiguration
{
public:
  CJSONConfiguration();
  ~CJSONConfiguration();

  void SetRootReference( Json::Value& vRoot );
  Json::Value* GetRootPointer();

  void InitializeEmptyRoot();
  bool ReadJSONFile( std::string sFileName );
  bool WriteCurrentConfigurationToJSONFile( std::string sFileName );

  Json::Value& GetFromKey( std::string sKey, Json::Value vDefault );
  Json::Value& GetFromKey( Json::Value& vSubTree, std::string sKey, Json::Value vDefault, bool bUseIndent = true );
  Json::Value& GetFromIndex( Json::Value& vSubTree, Json::ArrayIndex ind, bool bUseIndent = true );

  void PrintSettingsOn();
  void PrintSettingsOff();
  bool GetPrintSettings();

  void SetIndent( unsigned int uiIndent );
  unsigned int GetIndent();

protected:
  std::string ReadFileContentIntoString( std::string sFileName );
private:
  // intentionally not implemented
  CJSONConfiguration( const CJSONConfiguration & );
  CJSONConfiguration& operator=( const CJSONConfiguration & );

  unsigned int m_Indent;
  bool m_IsMasterNode;
  bool m_PrintSettings;
  Json::Value* m_ptrRoot;
};

} // end namespace

#endif

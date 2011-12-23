/**
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

/**
 * @brief Implements ways to extract information from a JSON data description.
 * This can be used to keep track of paramters and to read in configuration files.
 *
 */
class CJSONConfiguration
{
public:
  CJSONConfiguration();
  ~CJSONConfiguration();

  /**
   * @brief Sets the JSON configuration root by passing a JSON reference.
   *
   * @param vRoot The root of the JSON configuration
   */
  void SetRootReference( Json::Value& vRoot );

  /**
   * @brief Returns the root pointer.
   *
   * @return Pointer to the root
   */
  Json::Value* GetRootPointer();

  /**
   * @brief Intialize the root to an empty JSON decription. This is a useful initialization
   * for example when the JSON description is to be populated from scratch externally
   * (as when creating a JSON description of all possible parameters).
   *
   */
  void InitializeEmptyRoot();

  /**
   * @brief Reads a given JSON file and initializes the root node to point to the parsed content of this file.
   *
   * @param sFileName Input filename
   * @return bool Returns true if file could be read and false otherwise.
   */
  bool ReadJSONFile( std::string sFileName );

  /**
   * @brief Writes the JSON description (relative to the root) to a file.
   *
   * @param sFileName File to write to
   * @return bool Returns true if the writing was successful and false otherwise.
   */
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

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

#ifndef C_JSON_CONFIGURATION
#define C_JSON_CONFIGURATION

#include "json/json-forwards.h"
#include "json/json.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

namespace CALATK
{

/**
 * @brief Implements ways to extract information from a JSON data description.
 * This can be used to keep track of parameters and to read in configuration files.
 *
 */
class CJSONConfiguration
{
public:
  CJSONConfiguration( bool printSettings = false );
  ~CJSONConfiguration();

  typedef std::vector< double > VectorType;
  typedef std::vector< float > VectorFloatType;

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
   * @param fileName Input filename
   */
  void ReadJSONFile( const std::string & fileName );

  /**
   * @brief Writes the JSON description (relative to the root) to a file.
   *
   * @param fileName File to write to
   * @param rootCommentString header comment (first line)
   *
   * Throws an exception if an error occurs.
   */
  void  WriteCurrentConfigurationToJSONFile( const std::string & fileName, const std::string & rootCommentString="" );

  Json::Value& GetFromKey( std::string sKey, Json::Value vDefault = Json::nullValue );
  Json::Value& GetFromKey( Json::Value& vSubTree, std::string sKey, Json::Value vDefault = Json::nullValue, bool bUseIndent = true );
  Json::Value& GetFromIndex( Json::Value& vSubTree, Json::ArrayIndex ind, bool bUseIndent = true );
  VectorType GetFromKeyAsVector( Json::Value& vSubTree, std::string sKey, VectorType, bool bUseIndent = true );
  VectorFloatType GetFromKeyAsVector( Json::Value& vSubTree, std::string sKey, VectorFloatType, bool bUseIndent = true );

  void SetHelpForKey( Json::Value& vSubTree, std::string sKey, std::string sHelpString, Json::CommentPlacement commentPlacement = Json::commentAfterOnSameLine );
  void SetHelpForKey( std::string sKey, std::string sHelpString, Json::CommentPlacement commentPlacement = Json::commentAfterOnSameLine );

  void PrintSettingsOn();
  void PrintSettingsOff();
  bool GetPrintSettings();

  void AllowHelpCommentsOn();
  void AllowHelpCommentsOff();
  bool GetAllowHelpComments();

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
  bool m_AllowHelpComments;
  Json::Value* m_ptrRoot;
};

} // end namespace

#endif

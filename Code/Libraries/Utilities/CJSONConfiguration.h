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

#ifndef C_JSON_CONFIGURATION
#define C_JSON_CONFIGURATION

#include "CBase.h"

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
class CJSONConfiguration: public CBase
{
public:
  typedef CJSONConfiguration              Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Initializes an empty Json::Value master root by default. */
  CJSONConfiguration( bool printSettings = false );
  /** Initialize the root of this configuration to another node of a master. */
  CJSONConfiguration( Json::Value * node, bool printSettings = false );
  ~CJSONConfiguration();

  typedef std::vector< double > VectorType;
  typedef std::vector< float > VectorFloatType;

  /**
   * @brief Returns the root pointer.
   *
   * @return Pointer to the jsoncpp root
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
  void ReadJSONConfigurationFile( const std::string & fileName );

  /**
   * @brief Writes the JSON description (relative to the root) to a file.
   *
   * @param fileName File to write to
   * @param rootCommentString header comment (first line)
   *
   * Throws an exception if an error occurs.
   */
  void  WriteJSONConfigurationFile( const std::string & fileName, const std::string & rootCommentString="" );

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
  std::string ReadFileContentIntoString( const std::string & fileName );

private:
  // intentionally not implemented
  CJSONConfiguration( const CJSONConfiguration & );
  CJSONConfiguration& operator=( const CJSONConfiguration & );

  void DeleteRoot();

  unsigned int m_Indent;
  bool         m_PrintSettings;
  bool         m_AllowHelpComments;
  bool         m_IsMasterRoot;
  Json::Value* m_ptrRoot;
};

} // end namespace

#endif

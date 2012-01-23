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

#include "JSONParameterUtils.h"
#include <iomanip>

namespace CALATK
{

CJSONConfiguration::CJSONConfiguration( bool bPrintConfiguration )
  : m_Indent( 2 ),
    m_IsMasterNode( false ),
    m_AllowHelpComments( false ),
    m_ptrRoot( NULL )
{
  m_PrintSettings = bPrintConfiguration;
}

CJSONConfiguration::~CJSONConfiguration()
{
  if ( m_IsMasterNode )
    {
    // this is the master, need to release the memory
    if ( m_ptrRoot != NULL )
      {
      delete m_ptrRoot;
      m_ptrRoot = NULL;
      }
    }
}

void CJSONConfiguration::PrintSettingsOn()
{
  m_PrintSettings = true;
}

void CJSONConfiguration::PrintSettingsOff()
{
  m_PrintSettings = false;
}

bool CJSONConfiguration::GetPrintSettings()
{
  return m_PrintSettings;
}

void CJSONConfiguration::AllowHelpCommentsOn()
{
  m_AllowHelpComments = true;
}

void CJSONConfiguration::AllowHelpCommentsOff()
{
  m_AllowHelpComments = false;
}

bool CJSONConfiguration::GetAllowHelpComments()
{
  return m_AllowHelpComments;
}

void CJSONConfiguration::SetIndent( unsigned int uiIndent )
{
  m_Indent = uiIndent;
}

unsigned int CJSONConfiguration::GetIndent()
{
  return m_Indent;
}

void CJSONConfiguration::SetRootReference( Json::Value& vRoot )
{
  if ( m_ptrRoot != NULL )
    {
    if ( m_IsMasterNode )
        {
        std::cerr << "Writing the root node." << std::endl;
        }
        // we can just overwrite it, because everything is ruled by the master root node
        m_ptrRoot = &vRoot;
    }
  else
    {
    m_ptrRoot = &vRoot;
    m_IsMasterNode = false;
    }
}

bool CJSONConfiguration::WriteCurrentConfigurationToJSONFile( std::string sFileName, std::string commentString )
{
  std::ofstream outFile;
  outFile.open( sFileName.c_str() );

  if ( !outFile )
    {
    std::cerr << "Could not open " << sFileName << " for writing." << std::endl;
    return false;
    }

  // write a header unless there is already some form of it
  if ( !commentString.empty() )
  {
    if ( !m_ptrRoot->hasComment( Json::commentBefore) )
    {
      m_ptrRoot->setComment( commentString, Json::commentBefore );
    }
  }

  if ( m_ptrRoot != NULL )
    {
    outFile << *m_ptrRoot;
    }

  outFile.close();

  return true;
}

Json::Value* CJSONConfiguration::GetRootPointer()
{
  // if the root is NULL, generate an empty root node
  if ( m_ptrRoot==NULL )
  {
    std::cout << "JSON: Created an empty root for a request for the root pointer." << std::endl;
    InitializeEmptyRoot();
  }

  return m_ptrRoot;
}

void CJSONConfiguration::InitializeEmptyRoot()
{
    // if a root already exists, kill it and create a new one
    if ( m_ptrRoot != NULL )
    {
        delete m_ptrRoot;
        m_ptrRoot = NULL;
    }

    if ( m_ptrRoot == NULL )
    {
        m_ptrRoot = new Json::Value( Json::nullValue );
        m_IsMasterNode = true;
    }

}

CJSONConfiguration::VectorFloatType
CJSONConfiguration::GetFromKeyAsVector( Json::Value& vSubTree, std::string sKey, VectorFloatType vDefault, bool bUseIndent )
{
  VectorType vecD;
  for ( unsigned int iI=0; iI<vDefault.size(); ++iI )
  {
    vecD.push_back( vDefault[iI] );
  }
  VectorType vecRet = GetFromKeyAsVector( vSubTree, sKey, vecD, bUseIndent );

  VectorFloatType vecF;
  for ( unsigned int iI=0; iI<vecRet.size(); ++iI )
  {
    vecF.push_back( vecRet[ iI ] );
  }

  return vecF;
}

CJSONConfiguration::VectorType
CJSONConfiguration::GetFromKeyAsVector( Json::Value& vSubTree, std::string sKey, VectorType vDefault, bool bUseIndent )
{
  unsigned int uiIndent;
  if ( bUseIndent )
  {
    uiIndent = m_Indent;
  }
  else
  {
    uiIndent = 0;
  }

  std::vector<double> vecReturn = vDefault;

  // check if it is a member, if not make one
  if ( !vSubTree.isMember( sKey ) )
  {
    for ( unsigned int iI=0; iI<vDefault.size(); ++iI )
    {
      vSubTree[ sKey ][ iI ] = vDefault[ iI ];
    }
  }
  else // was in tree, read out the values
  {
    if ( vSubTree[ sKey ].type() != Json::arrayValue )
    {
      std::cerr << "ERROR: json, expected array for key \"" << sKey << "\" , but did not see an array. Using default value." << std::endl;
    }
    else // found array, extract the values
    {
      vecReturn.clear();
      unsigned int uiNrOfElements = vSubTree[ sKey ].size();
      for ( unsigned int iI=0; iI< uiNrOfElements; ++iI )
      {
        vecReturn.push_back( vSubTree[ sKey ][ iI ].asDouble() );
      }
    }
  }

  if ( m_PrintSettings )
  {
    std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used value = [ ";
    for ( unsigned int iI=0; iI<vecReturn.size(); ++iI )
    {
      std::cout << vecReturn[iI ] << " ";
    }
    std::cout << "]" << std::endl;
  }

  return vecReturn;

}

void CJSONConfiguration::SetHelpForKey( Json::Value &vSubTree, std::string sKey, std::string sHelpString, Json::CommentPlacement commentPlacement )
{
  if ( this->GetAllowHelpComments() )
  {
    // need to suppress printing
    bool bPrintingState = GetPrintSettings();
    this->PrintSettingsOff();
    Json::Value& currentValue = GetFromKey( vSubTree, sKey );
    if ( bPrintingState ) this->PrintSettingsOn();
    currentValue.setComment( "// " + sHelpString, commentPlacement );
  } // otherwise do not do anything, because help comments are disabled
}

Json::Value& CJSONConfiguration::GetFromKey( std::string sKey, Json::Value vDefault )
{
  if ( m_ptrRoot == NULL )
    {
    std::cerr << "ERROR: root needs to be initialized before it can be accessed." << std::endl;
    std::cerr << "WARNING: Initializing empty root." << std::endl;
    InitializeEmptyRoot();
    }
  return GetFromKey( *m_ptrRoot, sKey, vDefault, false );
}

void CJSONConfiguration::SetHelpForKey( std::string sKey, std::string sHelpString, Json::CommentPlacement commentPlacement )
{
  if ( this->GetAllowHelpComments() )
  {
    // need to suppress printing
    bool bPrintingState = GetPrintSettings();
    this->PrintSettingsOff();
    Json::Value& currentValue = GetFromKey( sKey );
    if ( bPrintingState ) this->PrintSettingsOn();
    currentValue.setComment( "// " + sHelpString, commentPlacement );
  } // otherwise do not do anything, because help comments are disabled
}

Json::Value& CJSONConfiguration::GetFromIndex( Json::Value& vSubTree, Json::ArrayIndex ind, bool bUseIndent )
{
  unsigned int uiIndent;
  if ( bUseIndent )
    {
    uiIndent = m_Indent;
    }
  else
    {
    uiIndent = 0;
    }

  // check if we have such an index, if not, create one
  if ( vSubTree.get( ind, Json::nullValue ) == Json::nullValue )
    {
    // could not find it, create it
    vSubTree[ ind ] = Json::nullValue;
    if ( m_PrintSettings )
      {
      std::cout << std::string( uiIndent, '-' ) << "ind = \"" << ind << "\" : created default tree entry." << std::endl;
      }
    }
  return vSubTree[ ind ];
}

Json::Value& CJSONConfiguration::GetFromKey( Json::Value& vSubTree, std::string sKey, Json::Value vDefault, bool bUseIndent )
{
  unsigned int uiIndent;
  if ( bUseIndent )
    {
    uiIndent = m_Indent;
    }
  else
    {
    uiIndent = 0;
    }

  // check if it is a member, if not, make it one
  if ( !vSubTree.isMember( sKey ) )
    {
    vSubTree[ sKey ] = vDefault;
    if ( m_PrintSettings )
      {
      if ( vDefault.type() != Json::arrayValue && vDefault.type() != Json::objectValue )
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used default value = " << vDefault;
        }
      else
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : created default value." << std::endl;
        }
      }
    }
  else
    {
    if ( m_PrintSettings )
      {
      if ( vSubTree[ sKey ].type() != Json::arrayValue && vSubTree[ sKey ].type() != Json::objectValue )
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used JSON-specified value = " << vSubTree.get( sKey, vDefault );
        }
      else
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used JSON value." << std::endl;
        }
      }
    }
  return vSubTree[ sKey ];
}

std::string CJSONConfiguration::ReadFileContentIntoString( std::string sFileName )
{
  std::string outputString ="";
  std::ifstream inputFile;
  inputFile.open( sFileName.c_str() );

  if ( inputFile.is_open() )
    {
    // could successfully be opened, now read it
    std::string sLine;
    while ( inputFile.good() )
      {
      std::getline( inputFile, sLine );
      outputString += sLine + "\n";
      }
    inputFile.close();

    }
  else
    {
    throw std::runtime_error( "Could not open file " + sFileName );
    outputString = "";
    }

  return outputString;
}

bool CJSONConfiguration::ReadJSONFile( std::string sFileName )
{
  if ( m_ptrRoot != NULL )
    {
    if ( m_IsMasterNode )
      {
      // delete 
      delete m_ptrRoot;
      m_ptrRoot = NULL;
      }
    else
      {
      std::cerr << "Trying to read file into non-master node. FORBIDDEN." << std::endl;
      return false;
      }
    }

  m_ptrRoot = new Json::Value;

  std::cout << "Parsing input file " << sFileName << " ... ";
    
  Json::Reader reader;
  std::string config_doc = ReadFileContentIntoString( sFileName );
  bool parsingSuccessful = reader.parse( config_doc, *m_ptrRoot );
  
  if ( !parsingSuccessful )
    {
    std::cout << "failed." << std::endl;

    // report to the user the failure and their locations in the document.
    std::cout  << "Failed to parse configuration\n"
               << reader.getFormattedErrorMessages();

    return false;
    }
  std::cout << "succeeded." << std::endl;

  return true;
}

} // end namespace

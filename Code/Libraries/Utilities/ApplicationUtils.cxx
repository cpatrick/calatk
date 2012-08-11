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

#include "ApplicationUtils.h"

#include <stdio.h>
#include <stdlib.h>


#ifdef _MSC_VER
#include <direct.h>
#define getcwd(x,y) _getcwd(x,y)  // VC++ uses _isnan() instead of isnan()
#endif

namespace CALATK
{

/**
 * ApplicationUtils.cxx - non-algorithmic utilities for applications
 *
 */

//
// Finds a filename in path
//
std::string ApplicationUtils::findFileNameInPath( std::string fileNameOrig, std::vector< std::string > pathNames )
{
  std::string fileName = fileNameOrig;

  // check if we have an absolute path name here
  if ( itksys::SystemTools::FileIsFullPath( fileName.c_str() ))
  {
    // there is nothing to complete here, because a full path was specified
    return fileName;
  }
  else
  {
    // go through all possible path combinations (also works if it was relative)
    std::vector< std::string > foundFiles;
    typedef std::vector< std::string >::const_iterator IteratorType;
    for ( IteratorType iter = pathNames.begin(); iter != pathNames.end(); ++iter )
    {
      std::vector< std::string > vecOfStrings;
      vecOfStrings.push_back(""); // because the first two components are contained in *iter
      vecOfStrings.push_back( *iter );
      vecOfStrings.push_back( fileName );
      std::string fullPathFileName = itksys::SystemTools::JoinPath( vecOfStrings );
      std::string fullPathFileNameCollapsed = itksys::SystemTools::CollapseFullPath( fullPathFileName.c_str() );

      // check if it exists
      if ( itksys::SystemTools::FileExists( fullPathFileNameCollapsed.c_str(), true ) )
      {
        foundFiles.push_back( fullPathFileNameCollapsed );
      }
    }

    if ( foundFiles.size() == 0 )
    {
      // no file found (even with expansion), let ITK deal with it
      return fileNameOrig;
    }
    else if ( foundFiles.size()>1 )
    {
      std::cout << std::endl;
      std::cout << "Detected ambiguous filename expansions:" << std::endl;

      for ( int iI=0; iI < foundFiles.size(); ++iI )
      {
        std::cout << iI << ": " << foundFiles[ iI ] << std::endl;
      }

      std::cout << "Refusing to expand and using original filename = " << fileNameOrig << std::endl;
      return fileNameOrig;
    }
    else
    {
      // only one. This is non-ambiguous
      return foundFiles[ 0 ];
    }

  }
}

std::string ApplicationUtils::findDataFileName( std::string fileName )
{
  CQueryEnvironmentVariables env;
  std::vector< std::string > jsonPath = env.GetDataPath();

  return findFileNameInPath( fileName, jsonPath );
}

std::string ApplicationUtils::findJSONFileName( std::string fileName )
{
  CQueryEnvironmentVariables env;
  std::vector< std::string > jsonPath = env.GetJSONPath();

  return findFileNameInPath( fileName, jsonPath );
}

//
// Function to check if a string ends in the specified character
//
bool ApplicationUtils::endsWith(std::string str, char c) {
  return (str.size()-1 == str.find_last_of(c));
}

//
// Function to check if a string ends in the specified string suffix
//
bool ApplicationUtils::endsWith(std::string str, std::string sfx) {
  if (sfx.size() > str.size()) {
  return false;
  }
  std::string ending = str.substr(str.size()-sfx.size(), sfx.size());
  return (strcmp(ending.c_str(), sfx.c_str()) == 0);
}

//
// Function to check if a string starts with the specified character
//
bool ApplicationUtils::startsWith(std::string str, char c) {
  return (str.c_str()[0] == 0);
}

//
// Function to check if a string ends in the specified string suffix
//
bool ApplicationUtils::startsWith(std::string str, std::string pfx) {
  if (pfx.size() > str.size()) {
  return false;
  }
  std::string start = str.substr(0, pfx.size());
  return (strcmp(start.c_str(), pfx.c_str()) == 0);
}

} // end namespace


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

#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include "CALATKCommon.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <itksys/SystemTools.hxx>
#include "CQueryEnvironmentVariables.h"

namespace CALATK
{

/**
 * ApplicationUtils.cxx - non-algorithmic utilities for applications
 *
 * This provides a set of non-algorithmic utility functions that are useful for executables
 * such as string parsing and command line argument manipulation.
 */
class ApplicationUtils
{

public:

  /**
    * Combines a path and a filename, e.g., on unix path/fileName
    */
  static std::string combinePathAndFileName( std::string path, std::string fileName );

  /**
    * Finds a filename given a set of paths and returns the first one found
    */
  static std::string findFileNameInPath( std::string fileName, std::vector< std::string > pathNames );

  static std::string findDataFileName( std::string fileName );

  static std::string findJSONFileName( std::string fileName );


  /**
   * Function to check if a string ends in the specified character
   */
  static bool endsWith(std::string str, char c);
  
  /**
   * Function to check if a string ends in the specified string suffix
   */
  static bool endsWith(std::string str, std::string sfx);
  
  /**
   * Function to check if a string starts with the specified character
   */
  static bool startsWith(std::string str, char c);
  
  /**
   * Function to check if a string starts with the specified string prefix
   */
  static bool startsWith(std::string str, std::string pfx);
};

} // end namespace

#endif
















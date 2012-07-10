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
// Split a string by a delimiting character
//
std::vector<std::string> ApplicationUtils::splitString(std::string str, char delim) {
  
  std::vector<std::string> out(0);
  
  std::size_t pos1 = 0;
  std::size_t pos2 = 0;
  bool reachedEnd = false;
  while(!reachedEnd) {
  pos2 = str.find(delim, pos1);
  
  if (pos2 == std::string::npos) {
    reachedEnd = true;    
    if (pos1 != str.size()) {
    out.push_back(str.substr(pos1, str.size()-pos1));
    }
  } else {
    out.push_back(str.substr(pos1, pos2-pos1));
  }
  
  pos1 = pos2+1;
  }
  
  return out;
  
}

//
// Function to get the base directory from a file path
//
std::string ApplicationUtils::getBaseDir(std::string str) {

  size_t pos = str.find_last_of('/');
  std::string out = str.substr(0, pos);
  
  if (out.size() == 0) {
  out = "./";
  }
  
  return out;
  
}

//
// Function to check the validity of a path
//
bool ApplicationUtils::validPath(std::string str) {
  struct stat st;
  if(stat(str.c_str(),&st) == 0) {
    return true;
  }
  return false;
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

//
// Function to get the type of time series file
//
int ApplicationUtils::getTStype(std::string path) {
  if (!validPath(path)) {
  return TS_BAD;
  }
  std::vector<std::string> split = splitString(path, '.');
  if (strcmp(split[split.size()-1].c_str(),"ts2") == 0) {
  return TS_2D;
  } else if (strcmp(split[split.size()-1].c_str(),"ts3") == 0) {
  return TS_3D;
  } else {
  return TS_BAD;
  }
}

//
// Function to get the command line arguments for a sub function
//
std::vector<std::string> ApplicationUtils::getSubArgv(int argc, char** argv) {
  
  std::vector<std::string> subArgv(argc-1);
  
  for (int i = 1; i < argc; i++) {
  subArgv[i-1] = std::string(argv[i]);
  }
  
  return subArgv;
}

//
// getNextGoodLine
//
bool ApplicationUtils::getNextGoodLine(std::ifstream* reader, std::string* out) {
  bool goodLine = false;
  bool lineExists = true;
  std::string temp;
  while (!goodLine && lineExists) {
  lineExists = std::getline(*reader, temp);
  if(temp.size() > 0 && temp[0] != '#') {
    goodLine = true;
  }
  }
  
  *out = temp;
  return lineExists;
}

//
// getCWD
//
#if CALATK_SYSTEM_WINDOWS
#include <direct.h>
#endif
std::string ApplicationUtils::getCWD() {
  
  const unsigned int maxSize = 4096;
  char buffer[maxSize];

  char* temp = getcwd(buffer, maxSize);
  (void) temp; // Supress unused warning
  
  std::string out(buffer);
  
  return out;
}


//
// dirExists
//
bool ApplicationUtils::dirExists(std::string path) {
  path = removeDoubleSlashes(path);
  struct stat st;
  return (stat(path.c_str(), &st) == 0);
}

//
// removeDoubleSlashes
//
std::string ApplicationUtils::removeDoubleSlashes(std::string path) {

  // first replace "//" with "/"
  std::string out = path;
  size_t pos = out.find("//");
  while (pos != std::string::npos) {
    out.replace(pos, 2, "/");
    pos = out.find("//");
  }

  // next replace "\\" with "\"
  pos = out.find("\\\\");
  while (pos != std::string::npos) {
    out.replace(pos, 2, "\\");
    pos = out.find("\\\\");
  }

  return out;
}

std::string ApplicationUtils::removeLastSlash(std::string path) {

  // first replace "//" with "/"
  std::string out = path;

  if (out.length() > 0)
  {
    std::string::iterator it = out.end() - 1;
    if ( (*it == '/') || (*it == '\\'))
    {
      out.erase(it);
    }
  }

  return out;
}


//
// makeDirIfNeeded
//
bool ApplicationUtils::makeDirIfNeeded(std::string& path)
{
  path = removeDoubleSlashes(path);
  const std::string outpath = removeLastSlash(path);
  if (!dirExists(outpath))
    {
    bool dirCreated = false;
#if CALATK_SYSTEM_UNIX
    dirCreated = (mkdir(outpath.c_str(), 0755) == 0);
#elif CALATK_SYSTEM_WINDOWS
    dirCreated = (_mkdir(outpath.c_str()) == 0);
#else
    itkGenericExceptionMacro("Unkown Operating System. Cannot make directory");
#endif
    if (!dirCreated)
      {
      path = "";
      return false;
      }
    else
      {
      return true;
      }
    }
  return true;
}

} // end namespace


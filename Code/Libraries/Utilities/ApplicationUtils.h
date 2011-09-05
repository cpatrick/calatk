#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include "CALATKCommon.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace CALATK
{

#define TS_2D 2
#define TS_3D 3
#define TS_BAD 0

/**
 * ApplicationUtils.cxx - non-algorithmic utilities for applications
 *
 * This provides a set of non-algorithmic utility functions that are useful for executables
 * such as string parsing and command line argument manipulation.
 */
class ApplicationUtils {

public:
  /**
   * Split a string by a delimiting character
   */
  static std::vector<std::string> splitString(std::string str, char delim);
  
  /**
   * Function to get the base directory from a file path
   */
  static std::string getBaseDir(std::string str);
  
  /**
   * Function to check the validity of a path
  */
  static bool validPath(std::string str);
  
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
  
  /**
   * Function to get the type of time series file
   */
  static int getTStype(std::string path);
  
  /**
   * Function to get the command line arguments for a sub function
   */
  static std::vector<std::string> getSubArgv(int argc, char** argv);
  
  /**
   * Function to get the next non-comment line from a reader if it exists
   */
  static bool getNextGoodLine(std::ifstream* reader, std::string* out);
  
  /**
   * Function to get the current working directory as a string
   */
  static std::string getCWD();

  /**
   * Get the pointer to a portion of a std::vector
   */
  template <class T>
  static T** getPointerFromVec(std::vector<T*>& vec, int start = -1) {
    if (start < 0 || start > (int)vec.size()-1) {
      start = 0;
    }
   
    return &vec[start];
  }
 
  /**
   * Check to see if a directory exists
   *
   * @param path path to check for directory existence
   * @return true if dir exists, false otherwise
   */
  static bool dirExists(std::string path);

  /**
   * Replace any occurences of "//" with "/" in a path and occurences of "\\"
   * with "\"
   *
   * @param path to check for double slashes
   * @return path with double slashes removed
   */
  static std::string removeDoubleSlashes(std::string path);
  
  /**
   * Remove "\\" or "/" at the end of the path
   *
   * @param path to check for slash at the end
   * @return path with slashes removed
   */

  static std::string removeLastSlash(std::string path);

  /**
   * Create a directory if it doesn't already exist.
   *
   * @param path - path to new or existing directory
   * @return true if dir exists after call, false otherwise
   */
  static bool makeDirIfNeeded(std::string& path);
};

} // end namespace

#endif
















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

class JSONParameterUtils
{
public:
  /** 
   * Method that reads a text file content into a string, which can then be passed to a JSON parser.
   *
   * @param sFileName - file name of the input file
   */
  static std::string ReadFileContentIntoString( std::string sFileName );
  
  static Json::Value SaveGetFromKey( const Json::Value& value, std::string sKey, std::string sDefault, bool bPrint = true );
  static Json::Value SaveGetFromKey( const Json::Value& value, std::string sKey, double dDefault, bool bPrint = true );

};

} // end namespace

#endif

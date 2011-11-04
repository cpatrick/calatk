#include "JSONParameterUtils.h"

namespace CALATK
{

//
// Reading text file content into a string
//
std::string 
JSONParameterUtils::ReadFileContentIntoString( std::string sFileName )
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

// 
// Gets JSON value from key and prints a message if default values were used
//
Json::Value
JSONParameterUtils::SaveGetFromKey( const Json::Value& value, std::string sKey, std::string sDefault, bool bPrint )
{
  if ( !value.isMember( sKey ) && bPrint )
    {
    std::cout << "Key = \"" << sKey << "\" : used default value = " << sDefault << std::endl;
    }

  return value.get( sKey, sDefault );

}

// 
// Gets JSON value from key and prints a message if default values were used
//
Json::Value
JSONParameterUtils::SaveGetFromKey( const Json::Value& value, std::string sKey, double dDefault, bool bPrint )
{
  if ( !value.isMember( sKey ) && bPrint )
    {
    std::cout << "Key = \"" << sKey << "\" : used default value = " << dDefault << std::endl;
    }

  return value.get( sKey, dDefault );
}


} // end namespace

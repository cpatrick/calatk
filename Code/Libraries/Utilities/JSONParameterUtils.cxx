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
// Parses a JSON file
//
bool 
JSONParameterUtils::ParseJSONFile( std::string sFileName, Json::Value& root )
{
    Json::Reader reader;
    std::string config_doc = JSONParameterUtils::ReadFileContentIntoString( sFileName );
    bool parsingSuccessful = reader.parse( config_doc, root );

    if ( !parsingSuccessful )
      {
      // report to the user the failure and their locations in the document.
      std::cout  << "Failed to parse configuration\n"
                 << reader.getFormattedErrorMessages();
      return EXIT_FAILURE;
      }
    return EXIT_SUCCESS;
}

// 
// Gets JSON value from key and prints a message if default values were used
//
Json::Value
JSONParameterUtils::SaveGetFromKey( const Json::Value& value, std::string sKey, const Json::Value& vDefault, bool bPrint )
{
  Json::Value retValue = value;
  if ( retValue.empty() )
    {
    retValue[ sKey ] = vDefault;
    std::cout << "Key = \"" << sKey << "\" : used default value = " << vDefault;
    }

  if ( !retValue.isMember( sKey ) && bPrint )
    {
    std::cout << "Key = \"" << sKey << "\" : used default value = " << vDefault;
    }
  return retValue.get( sKey, vDefault );
}

} // end namespace

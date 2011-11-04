#include <iostream>
#include "JSONParameterUtils.h"

typedef float TFLOAT;

int main()
{

  std::string config_file = "test.json";
  std::string config_doc = CALATK::JSONParameterUtils::ReadFileContentIntoString( config_file );

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( config_doc, root );

  if ( !parsingSuccessful )
    {
    // report to the user the failure and their locations in the document.
    std::cout  << "Failed to parse configuration\n"
               << reader.getFormattedErrorMessages();
    return -1;
    }

  std::string solver = root.get( "solver", "" ).asString();

  std::cout << "Detected solver " << solver << std::endl;

  const Json::Value multiScaleSettings = root["multiscaleSettings"];
  
  std::cout << "Number of multiscale levels = " << multiScaleSettings.size() << std::endl;

  for ( unsigned int iI=0; iI<multiScaleSettings.size(); ++iI )
    {
    std::cout << "Level = " << iI << std::endl;
    std::cout << "Sigma = " << CALATK::JSONParameterUtils::SaveGetFromKey( multiScaleSettings[ iI ], "blurSigma", 0 ).asDouble() << std::endl;
    std::cout << "Factor = " << CALATK::JSONParameterUtils::SaveGetFromKey( multiScaleSettings[ iI ], "downsamplingFactor", 1 ).asDouble() << std::endl;

    std::cout << std::endl;
    }

  return 0;
}

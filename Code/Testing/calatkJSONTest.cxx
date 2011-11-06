#include <iostream>
#include "JSONParameterUtils.h"

typedef float TFLOAT;

int main( int argc, char* argv[] )
{

  if ( argc!=2 )
    {
    std::cout << "Usage calatkJSONTest fileName.json" << std::endl;
    return EXIT_FAILURE;
    }

  std::string config_file( argv[1] );
  std::string config_doc = CALATK::JSONParameterUtils::ReadFileContentIntoString( config_file );

  Json::Value root;   // will contains the root value after parsing.
  Json::Reader reader;
  bool parsingSuccessful = reader.parse( config_doc, root );

  if ( !parsingSuccessful )
    {
    // report to the user the failure and their locations in the document.
    std::cout  << "Failed to parse configuration\n"
               << reader.getFormattedErrorMessages();
    return EXIT_FAILURE;
    }

  std::string solver = root.get( "solver", "" ).asString();

  std::cout << "Detected solver " << solver << std::endl;

  const Json::Value multiScaleSettings = root["multiscaleSettings"];
  
  std::cout << "Number of multiscale levels = " << multiScaleSettings.size() << std::endl;

  for ( unsigned int iI=0; iI<multiScaleSettings.size(); ++iI )
    {
    std::cout << "Level = " << iI << std::endl;

    Json::Value currentDownsamplerConfiguration = CALATK::JSONParameterUtils::SaveGetFromKey( multiScaleSettings[ iI ], "downsampler", Json::nullValue );

    std::cout << "Sigma = " << CALATK::JSONParameterUtils::SaveGetFromKey( currentDownsamplerConfiguration, "blurSigma", 0 ).asDouble() << std::endl;
    std::cout << "Factor = " << CALATK::JSONParameterUtils::SaveGetFromKey( currentDownsamplerConfiguration, "downsamplingFactor", 1 ).asDouble() << std::endl;

    std::cout << std::endl;
    }

  return 0;
}

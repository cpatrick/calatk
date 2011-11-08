#include <iostream>
#include "JSONParameterUtils.h"
#include <fstream>

typedef float TFLOAT;

int main( int argc, char* argv[] )
{

  if ( argc!=3 )
    {
    std::cout << "Usage calatkJSONTest fileName.json fileNameOut.json" << std::endl;
    return EXIT_FAILURE;
    }

  std::string config_file( argv[1] );

  CALATK::CJSONConfiguration config;
  bool parsingSuccessful = config.ReadJSONFile( config_file );

  if ( !parsingSuccessful )
    {
    return EXIT_FAILURE;
    }

  Json::Value& valMultiScale = config.GetFromKey( "MultiscaleSettings", Json::nullValue );

  std::cout << "Multiscale settings = " << std::endl;
  std::cout << valMultiScale;

  Json::Value& valObj = config.GetFromKey( "ObjectiveFunction", Json::nullValue );
  
  valObj[0]["test"]=1;
  valObj[1]["test"]=2;

  config.WriteCurrentConfigurationToJSONFile( argv[2] );

  CALATK::CJSONConfiguration subConfig;
  subConfig.SetRootReference( valObj );
  
  std::cout << "SubConfig = " << std::endl;
  std::cout << *subConfig.GetRootPointer();

  return EXIT_SUCCESS;

}


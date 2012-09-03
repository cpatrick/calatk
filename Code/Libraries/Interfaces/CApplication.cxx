/*
i
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

#include "CApplication.h"

namespace CALATK
{

void CApplication::InstantiateConfigs()
{
  this->m_CombinedAlgorithmJSONConfig = new CJSONConfiguration;
  this->m_CombinedDataJSONConfig = new CJSONConfiguration;
  this->m_CombinedDataJSONConfig->InitializeEmptyRoot();
  this->m_CleanedAlgorithmJSONConfig = new CJSONConfiguration;
  this->m_CleanedDataJSONConfig = new CJSONConfiguration;
  JSONTransform=false;
}

CApplication::CApplication()
{
  this->InstantiateConfigs();
}


CApplication::CApplication( const int argc, char **argv )
{
  this->InstantiateConfigs();

  if( argc < 3 )
    {
    std::string usage = "Usage: CALATKCommandLine "
          "<given_algorithm_parameters_config.json> "
          "(<given_data_parameters_config.json> | <source_image_file> <target_image_file> <warped_source_output_file>) "
          "[<used_algorithm_parameters_config.json>] "
          "[<used_data_parameters_config.json>]\n";
    throw std::runtime_error( usage.c_str() );
    }
  int i=0;
  const std::string argv1( argv[i+1] );
  if( !(std::string( "GenerateJSONDisplacementVectorField" ).compare(argv1)))
  {
      JSONTransform=true;
      i=1;
  }
  std::cout<<"TESTOK"<<std::endl;
  this->SetGivenAlgorithmConfigurationFile( argv[i+1] );
  const std::string argv2( argv[i+2] );
  if( argv2.length() > 5 && std::string( ".json" ).compare( argv2.substr( argv2.length() - 5 )))
    {
    if( argc < i+4 )
      {
      throw std::runtime_error( "Insufficient number of arguments, must supply both source and target image." );
      }
    this->SetGivenSourceAndTargetImageFiles( argv[i+2], argv[i+3] );

    const std::string argv4( argv[i+4] );
    if( argc > i+4 && std::string( ".json" ).compare( argv4.substr( argv4.length() - 5 )))
      {
      this->SetGivenWarpedImageFile( argv[i+4] );
      if( argc > i+5 )
        {
        this->SetUsedAlgorithmConfigurationFile( argv[i+5] );
        }
      if( argc > i+6 )
        {
        this->SetUsedDataConfigurationFile( argv[i+6] );
        }
      }
    else
      {
      if( argc > i+4 )
        {
        this->SetUsedAlgorithmConfigurationFile( argv[i+4] );
        }
      if( argc > i+5 )
        {
        this->SetUsedDataConfigurationFile( argv[i+5] );
        }
      }
    }
  else
    {
    this->SetGivenDataConfigurationFile( argv2 );

    if( argc > i+3 )
      {
      this->SetUsedAlgorithmConfigurationFile( argv[i+3] );
      }

    if( argc > i+4 )
      {
      this->SetUsedDataConfigurationFile( argv[i+4] );
      }
    }
}


void CApplication::SetGivenAlgorithmConfigurationFile( const std::string & file )
{
  this->m_GivenAlgorithmConfigurationFile = file;
  this->m_CombinedAlgorithmJSONConfig->ReadJSONConfigurationFile( file );
}


std::string CApplication::GetGivenAlgorithmConfigurationFile() const
{
  return this->m_GivenAlgorithmConfigurationFile;
}


void CApplication::SetGivenSourceAndTargetImageFiles( const std::string & source, const std::string & target )
{
  // The source image goes into TimePoint 0.0.
  Json::Value sourceTimePoint( 0.0 );
  Json::Value sourceFile( source );
  Json::Value sourceTimeFile( Json::arrayValue );
  sourceTimeFile[0] = sourceTimePoint;
  sourceTimeFile[1] = sourceFile;

  // The target image goes into TimePoint 1.0.
  Json::Value targetTimePoint( 1.0 );
  Json::Value targetFile( target );
  Json::Value targetTimeFile( Json::arrayValue );
  targetTimeFile[0] = targetTimePoint;
  targetTimeFile[1] = targetFile;

  Json::Value timePoints( Json::arrayValue );
  timePoints[0] = sourceTimeFile;
  timePoints[1] = targetTimeFile;

  Json::Value subjects( Json::objectValue );
  subjects["subject1"] = timePoints;

  Json::Value & root = *(m_CombinedDataJSONConfig->GetRootPointer());
  root["Inputs"] = subjects;
}


void CApplication::SetGivenWarpedImageFile( const std::string & warped )
{
  // The warped image is at TimePoint 1.0.
  Json::Value warpedTimePoint( 1.0 );
  Json::Value warpedFile( warped );
  Json::Value warpedTimeFile( Json::arrayValue );
  warpedTimeFile[0] = warpedTimePoint;
  warpedTimeFile[1] = warpedFile;

  Json::Value timePoints( Json::arrayValue );
  timePoints[0] = warpedTimeFile;

  Json::Value subjects( Json::objectValue );
  subjects["subject1"] = timePoints;

  Json::Value & root = *(this->m_CombinedDataJSONConfig->GetRootPointer());
  root["Outputs"] = subjects;
}


void CApplication::SetGivenDataConfigurationFile( const std::string & file )
{
  this->m_GivenDataConfigurationFile = file;
  this->m_CombinedDataJSONConfig->ReadJSONConfigurationFile( file );
}


std::string CApplication::GetGivenDataConfigurationFile() const
{
  return this->m_GivenDataConfigurationFile;
}


void CApplication::SetUsedAlgorithmConfigurationFile( const std::string & file )
{
  this->m_UsedAlgorithmConfigurationFile = file;
}


std::string CApplication::GetUsedAlgorithmConfigurationFile() const
{
  return this->m_UsedAlgorithmConfigurationFile;
}


void CApplication::SetUsedDataConfigurationFile( const std::string & file )
{
  this->m_UsedDataConfigurationFile = file;
}


std::string CApplication::GetUsedDataConfigurationFile() const
{
  return this->m_UsedDataConfigurationFile;
}


void CApplication::Solve()
{
  // Check to make sure we have an algorithm and data input.
  Json::Value & combinedAlgorithmConfigRoot = *(this->m_CombinedAlgorithmJSONConfig->GetRootPointer());
  Json::Value & combinedDataConfigRoot = *(this->m_CombinedDataJSONConfig->GetRootPointer());
  if( &combinedAlgorithmConfigRoot == NULL )
    {
    throw std::logic_error( "Algorithm configuration was not set for CApplication" );
    }
  if( &combinedDataConfigRoot == NULL )
    {
    throw std::logic_error( "Data configuration was not set for CApplication" );
    }

  // Initialize the used JSON configurations.
  this->m_CleanedAlgorithmJSONConfig->InitializeEmptyRoot();
  this->m_CleanedDataJSONConfig->InitializeEmptyRoot();
  Json::Value & cleanedAlgorithmConfigRoot = *(this->m_CleanedAlgorithmJSONConfig->GetRootPointer());

  // Get the float type (double or float).
  // The default float type.
  static const std::string defaultFloatType = "double";
  std::string floatType = defaultFloatType;
  const Json::Value & givenAlgorithmConfig = combinedAlgorithmConfigRoot["Algorithm"];
  if( givenAlgorithmConfig != Json::nullValue && givenAlgorithmConfig["FloatType"] != Json::nullValue )
    {
    floatType = givenAlgorithmConfig["FloatType"].asString();
    cleanedAlgorithmConfigRoot["Algorithm"]["FloatType"] = floatType;
    }
  else
    {
    combinedAlgorithmConfigRoot["Algorithm"]["FloatType"] = defaultFloatType;
    cleanedAlgorithmConfigRoot["Algorithm"]["FloatType"] = defaultFloatType;
    }

  // Get the image dimension.
  unsigned int imageDimension = 3;
  if( givenAlgorithmConfig != Json::nullValue && givenAlgorithmConfig["ImageDimension"] != Json::nullValue )
    {
    imageDimension = givenAlgorithmConfig["ImageDimension"].asInt();
    cleanedAlgorithmConfigRoot["Algorithm"]["ImageDimension"] = imageDimension;
    }
  else
    {
    const Json::Value inputs = combinedDataConfigRoot["Inputs"];
    if( inputs != Json::nullValue )
      {
      // use the dimension from the first image of the first subject
      const Json::Value firstSubject = *(inputs.begin());
      if( firstSubject != Json::nullValue )
        {
        // first time point, the image file name is the second entry
        Json::Value firstImage = firstSubject[0][1];
        if( firstImage != Json::nullValue )
          {
          imageDimension = GetNonSingletonImageDimensionFromFile( firstImage.asString() );
          }
        }
      }
    combinedAlgorithmConfigRoot["Algorithm"]["ImageDimension"] = imageDimension;
    cleanedAlgorithmConfigRoot["Algorithm"]["ImageDimension"] = imageDimension;
    }

  if( floatType.compare( "float" ) == 0 )
    {
    switch( imageDimension )
      {
    case 1:
      this->InternalSolve< float, 1 >();
      break;
    case 2:
      this->InternalSolve< float, 2 >();
      break;
    case 3:
      this->InternalSolve< float, 3 >();
      break;
    default:
      std::ostringstream message;
      message << "Unsupported image dimension: " << imageDimension;
      throw std::runtime_error( message.str().c_str() );
      }
    }
  else if( floatType.compare( "double" ) == 0 )
    {
    switch( imageDimension )
      {
    case 1:
      this->InternalSolve< double, 1 >();
      break;
    case 2:
      this->InternalSolve< double, 2 >();
      break;
    case 3:
      this->InternalSolve< double, 3 >();
      break;
    default:
      std::ostringstream message;
      message << "Unsupported image dimension: " << imageDimension;
      throw std::runtime_error( message.str().c_str() );
      }
    }
  else
    {
    const std::string message = "Invalid Algorithm FloatType: " + floatType;
    throw std::runtime_error( message.c_str() );
    }

  if( this->m_UsedAlgorithmConfigurationFile.length() > 0 )
    {
    this->m_CleanedAlgorithmJSONConfig->WriteJSONConfigurationFile( this->m_UsedAlgorithmConfigurationFile );
    }

  if( this->m_UsedDataConfigurationFile.length() > 0 )
    {
    this->m_CleanedDataJSONConfig->WriteJSONConfigurationFile( this->m_UsedDataConfigurationFile );
    }

}


template< class TFloat, unsigned int VImageDimension >
void
CApplication::InternalSolve()
{
  // The default algorithm
  static const std::string defaultAlgorithmName = "LDDMMGrowthModelRegistration";
  std::string algorithmName = defaultAlgorithmName;
  Json::Value & combinedAlgorithmConfigRoot = *(this->m_CombinedAlgorithmJSONConfig->GetRootPointer());
  Json::Value & cleanedAlgorithmConfigRoot = *(this->m_CleanedAlgorithmJSONConfig->GetRootPointer());
  Json::Value & combinedAlgorithmConfig = combinedAlgorithmConfigRoot["Algorithm"];
  if( combinedAlgorithmConfig != Json::nullValue )
    {
    const Json::Value calatkAlgorithmName = combinedAlgorithmConfig["Name"];
    if( calatkAlgorithmName != Json::nullValue )
      {
      algorithmName = calatkAlgorithmName.asString();
      cleanedAlgorithmConfigRoot["Algorithm"]["Name"] = algorithmName;
      }
    }
  else
    {
    combinedAlgorithmConfigRoot["Algorithm"]["Name"] = algorithmName;
    cleanedAlgorithmConfigRoot["Algorithm"]["Name"] = algorithmName;
    }

  // Create the algorithm.
  typedef CAlgorithmFactory< TFloat, VImageDimension > AlgorithmFactoryType;
  typename AlgorithmFactoryType::Pointer algorithmFactory = new AlgorithmFactoryType;
  typedef typename AlgorithmFactoryType::AlgorithmBaseType AlgorithmBaseType;
  typename AlgorithmBaseType::Pointer algorithmBase = algorithmFactory->CreateNewAlgorithm( algorithmName );

  // Create the image manager.
  typedef CImageManager< TFloat, VImageDimension > ImageManagerType;
  typename ImageManagerType::Pointer imageManager;
  if( combinedAlgorithmConfigRoot["MultiScaleSettings"] != Json::nullValue )
    {
    typedef CImageManager< TFloat, VImageDimension > ImageManagerMultiScaleType;
    imageManager = new ImageManagerMultiScaleType;
    }
  else
    {
    typedef CImageManager< TFloat, VImageDimension > ImageManagerFullScaleType;
    imageManager = new ImageManagerFullScaleType;
    }
  algorithmBase->SetImageManagerPointer( imageManager );

  imageManager->SetAlgorithmAutoConfiguration( this->m_CombinedAlgorithmJSONConfig, this->m_CleanedAlgorithmJSONConfig );
  imageManager->SetDataAutoConfiguration( this->m_CombinedDataJSONConfig, this->m_CleanedDataJSONConfig );
  imageManager->ReadInputsFromDataJSONConfiguration();

  // Do it!
  algorithmBase->SetAutoConfiguration( this->m_CombinedAlgorithmJSONConfig, this->m_CleanedAlgorithmJSONConfig );
  algorithmBase->Solve();

  imageManager->WriteOutputsFromDataJSONConfiguration( algorithmBase );

  if(JSONTransform) imageManager->GetTransformsFromDataJSONConfiguration(algorithmBase);
}

} // end namespace CALATK

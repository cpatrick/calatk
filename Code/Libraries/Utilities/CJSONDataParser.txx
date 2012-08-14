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

#ifndef C_JSON_DATA_PARSER_TXX
#define C_JSON_DATA_PARSER_TXX

#include "CJSONDataParser.h"

namespace CALATK
{

template < class TFloat >
CJSONDataParser< TFloat >::CJSONDataParser()
{
}

template < class TFloat >
CJSONDataParser< TFloat >::~CJSONDataParser()
{
}

template < class TFloat >
void CJSONDataParser< TFloat >::ParseInputDataFromJSONConfiguration( std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned )
{
  if ( this->IsAdvancedDataConfigurationFormat( jsonConfiguration ) )
    {
      this->ParseDataFromAdvancedJSONConfigurationInternal( "Inputs", parsedData, jsonConfiguration, jsonConfigurationCleaned );
    }
  else
    {
      this->ParseDataFromBasicJSONConfigurationInternal( "Inputs", parsedData, jsonConfiguration, jsonConfigurationCleaned );
    }
}

template < class TFloat >
void CJSONDataParser< TFloat >::ParseOutputDataFromJSONConfiguration( std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned )
{
  if ( this->IsAdvancedDataConfigurationFormat( jsonConfiguration ) )
    {
      this->ParseDataFromAdvancedJSONConfigurationInternal( "Outputs", parsedData, jsonConfiguration, jsonConfigurationCleaned );
    }
  else
    {
      this->ParseDataFromBasicJSONConfigurationInternal( "Outputs", parsedData, jsonConfiguration, jsonConfigurationCleaned );
    }
}

template < class TFloat >
void CJSONDataParser< TFloat >::ParseDataFromBasicJSONConfigurationInternal( std::string dataType, std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned )
{
  parsedData.clear();

  Json::Value & dataCombinedConfigRoot = *( jsonConfiguration->GetRootPointer());
  Json::Value & combinedData = dataCombinedConfigRoot[ dataType ];

  if( combinedData == Json::nullValue )
    {
    std::cout << "No images of type " + dataType + " given." << std::endl;
    return;
    }
  Json::Value & combinedSubjects = *(combinedData.begin());
  if( combinedSubjects == Json::nullValue || combinedSubjects.size() == 0 )
    {
    throw std::runtime_error( "No subjects found." );
    }

  const Json::Value::Members subjects = combinedData.getMemberNames();
  for( unsigned int subjectId = 0; subjectId < combinedData.size(); ++subjectId )
    {
      Json::Value cleanedSubject( Json::arrayValue );
      const std::string subject = subjects[subjectId];
      Json::Value combinedSubject = combinedData[subject];

      for( unsigned int timePointIndex = 0; timePointIndex < combinedSubject.size(); ++timePointIndex )
        {
        Json::Value & combinedTimePoint = combinedSubject[timePointIndex];

        Json::Value cleanedTimePoint( Json::arrayValue );
        cleanedTimePoint[0] = combinedTimePoint[0];
        cleanedTimePoint[1] = combinedTimePoint[1];

        if( combinedTimePoint[0] == Json::nullValue )
          {
          throw std::runtime_error( "Expected time point not found in Basic data configuration file." );
          }
        if( combinedTimePoint[1] == Json::nullValue )
          {
          throw std::runtime_error( "Expected image file path not found in Basic data configuration file." );
          }

        const std::string nullTransformFileName( "" );

        SImageDatum imageDatum;
        imageDatum.timePoint = combinedTimePoint[0].asDouble();
        imageDatum.subjectId = subjectId;
        imageDatum.fileName = combinedTimePoint[1].asCString();
        imageDatum.transformFileName = nullTransformFileName;
        imageDatum.subjectString = subject;

        parsedData.push_back( imageDatum );

        cleanedSubject[timePointIndex] = cleanedTimePoint;

        }

      if ( jsonConfigurationCleaned != NULL )
        {
          Json::Value & dataCleanedConfigRoot  = *(jsonConfigurationCleaned->GetRootPointer());
          dataCleanedConfigRoot[ dataType ][ subject ] = cleanedSubject;
        }

      }
}

template < class TFloat >
void CJSONDataParser< TFloat >::ParseDataFromAdvancedJSONConfigurationInternal( std::string dataType, std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned )
{
  parsedData.clear();

  Json::Value & dataCombinedConfigRoot = *(jsonConfiguration->GetRootPointer());
  Json::Value & combinedData = dataCombinedConfigRoot[ dataType ];

  if( combinedData == Json::nullValue )
    {
    std::cout << "No images of type " + dataType + " given." << std::endl;
    return;
    }
  Json::Value combinedSubjects = combinedData["Subjects"];
  if( combinedSubjects == Json::nullValue || combinedSubjects.size() == 0 )
    {
    throw std::runtime_error( "No subjects found." );
    }

  Json::Value cleanedSubjects( Json::arrayValue );
  unsigned int subjectId = 0;
  for( Json::Value::iterator combinedSubject = combinedSubjects.begin();
         combinedSubject != combinedSubjects.end();
         ++combinedSubject, ++subjectId )
    {
    Json::Value cleanedSubject( Json::objectValue );

    Json::Value & subjectString = (*combinedSubject)["ID"];
    if( subjectString == Json::nullValue )
      {
      throw std::runtime_error( "Could not find expected subject Id." );
      }
    cleanedSubject["ID"] = subjectString;
    const std::string subject = subjectString.asString();

    Json::Value & combinedTimePoints = (*combinedSubject)["TimePoints"];
    Json::Value cleanedTimePoints( Json::arrayValue );
    for( unsigned int timePointIndex = 0; timePointIndex < combinedTimePoints.size(); ++timePointIndex )
      {
      Json::Value & combinedTimePoint = combinedTimePoints[timePointIndex];

      Json::Value cleanedTimePoint( Json::objectValue );

      Json::Value & time = combinedTimePoint["Time"];
      if( time == Json::nullValue )
        {
        throw std::runtime_error( "Expected time point not found in Advanced data configuration file." );
        }
      else
        {
        cleanedTimePoint["Time"] = time;
        }

      Json::Value & image = combinedTimePoint["Image"];
      if( image == Json::nullValue )
        {
        throw std::runtime_error( "Expected image file path not found in Advanced data configuration file." );
        }
      else
        {
        cleanedTimePoint["Image"] = image;
        }

      std::string transformFileName( "" );
      if( combinedTimePoint.isMember( "Transform" ) )
        {
        Json::Value & transform = combinedTimePoint["Transform"];
        cleanedTimePoint["Transform"] = transform;
        transformFileName = transform.asString();
        }

      SImageDatum imageDatum;
      imageDatum.timePoint = time.asDouble();
      imageDatum.subjectId = subjectId;
      imageDatum.fileName = image.asCString();
      imageDatum.transformFileName = transformFileName;
      imageDatum.subjectString = subject;

      parsedData.push_back( imageDatum );

      cleanedTimePoints[timePointIndex] = cleanedTimePoint;
      }
    cleanedSubject["TimePoints"] = cleanedTimePoints;

    cleanedSubjects[subjectId] = cleanedSubject;
    }

  if ( jsonConfigurationCleaned != NULL )
    {
      Json::Value & dataCleanedConfigRoot  = *(jsonConfigurationCleaned->GetRootPointer());
      dataCleanedConfigRoot[dataType]["Subjects"] = cleanedSubjects;
    }
}

template < class TFloat >
bool CJSONDataParser< TFloat >::IsAdvancedDataConfigurationFormat( CJSONConfiguration* jsonConfiguration )
{
  Json::Value & dataCombinedConfigRoot = *(jsonConfiguration->GetRootPointer());
  if( dataCombinedConfigRoot.isMember( "CalaTKDataConfigurationVersion" ) )
    {
    return true;
    }
  return false;
}

} // end namespace CALATK

#endif

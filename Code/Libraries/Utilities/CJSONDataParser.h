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

#ifndef C_JSON_DATA_PARSER_H
#define C_JSON_DATA_PARSER_H

#include "CJSONConfiguration.h"

namespace CALATK
{

/**
  * @brief Provides functionality to parse the basic and advanced data JSON formats.
  * This is functionality needed for example by the image-manager and to extract one image to determine image dimension on the fly.
  *
  */

template < class TFloat >
class CJSONDataParser
{
public:
  CJSONDataParser();
  ~CJSONDataParser();

  typedef TFloat FloatType;

  struct SImageDatum
  {
    FloatType timePoint;
    int subjectId;
    int timeSeriesIndex;
    std::string fileName;
    std::string transformFileName;
    std::string subjectString;
  };

  void ParseInputDataFromJSONConfiguration( std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned = NULL );
  void ParseOutputDataFromJSONConfiguration( std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned = NULL );
  bool IsAdvancedDataConfigurationFormat( CJSONConfiguration* jsonConfiguration );

protected:

  /** Parses the given input images and their associated metadata into the
   * ImageManager memory when the JSON configuration file is in the Basic
   * format. */
  void ParseDataFromBasicJSONConfigurationInternal( std::string dataType, std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned = NULL );

  /** Parses the given input images and their associated metadata into the
   * ImageManager memory when the JSON configuration file is in the Advanced
   * format. The Advanced data configuration format is more descriptive, can
   * accept more optional fields, and is distinguished from the Basic format by
   * the presence of a "CalaTKDataConfigurationVersion" entry. */
  void ParseDataFromAdvancedJSONConfigurationInternal( std::string dataType, std::vector< SImageDatum > &parsedData, CJSONConfiguration *jsonConfiguration, CJSONConfiguration *jsonConfigurationCleaned = NULL );

  /** Determined by the presence of a "CalaTKDataConfigurationVersion" entry. */
  bool IsAdvancedDataConfigurationFormat();

};

} // end namespace CALATK

#endif // C_JSON_DATA_PARSER_H

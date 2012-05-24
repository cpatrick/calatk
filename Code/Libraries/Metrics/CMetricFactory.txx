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

#ifndef C_METRIC_FACTORY_TXX
#define C_METRIC_FACTORY_TXX

template < class T, unsigned int VImageDimension >
CMetricFactory< T, VImageDimension >::CMetricFactory()
{
}

template < class T, unsigned int VImageDimension >
CMetricFactory< T, VImageDimension >::~CMetricFactory()
{
}

template < class T, unsigned int VImageDimension >
typename CMetricFactory< T, VImageDimension >::MetricType*
CMetricFactory< T, VImageDimension >::CreateNewMetric( NumericMetricType metric )
{
  MetricType* ptrMetric = NULL;
  switch ( metric )
  {
  default:
    std::cout << "Unknown metric type = " << metric << "; defaulting to SSD." << std::endl;
  case SSDMetric:
    ptrMetric = new SSDMetricType;
    break;
  }

  return ptrMetric;
}

template < class T, unsigned int VImageDimension >
typename CMetricFactory< T, VImageDimension >::MetricType*
CMetricFactory< T, VImageDimension >::CreateNewMetric( std::string sMetric )
{
  NumericMetricType metricType = GetMetricTypeFromString( sMetric );
  return CreateNewMetric( metricType );
}

template < class T, unsigned int VImageDimension >
typename CMetricFactory< T, VImageDimension >::NumericMetricType
CMetricFactory< T, VImageDimension >::GetMetricTypeFromString( std::string sMetric )
{
  std::string sMetricLowerCase = sMetric;
  // convert to all lower case
  std::transform( sMetricLowerCase.begin(), sMetricLowerCase.end(), sMetricLowerCase.begin(), ::tolower );

  if ( sMetricLowerCase == "ssd" )
  {
    return SSDMetric;
  }
  else
  {
    std::cout << "Unknown metric type " << sMetric << "; defaulting to SSD." << std::endl;
    return SSDMetric;
  }
}

#endif

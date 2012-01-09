/*
*
*  Copyright 2011 by the CALATK development team
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

#ifndef C_METRIC_FACTORY_H
#define C_METRIC_FACTORY_H

#include "JSONParameterUtils.h"
#include "CALATKCommon.h"
#include "CMetric.h"
#include "CProcessBase.h"

// includes for all the supported metrics
#include "CMetricSSD.h"

#include <algorithm>

namespace CALATK
{
/**
  * Factory class to dynamically create different metrics.
  *
  */

template < class T, unsigned int VImageDimension=3 >
class CMetricFactory :
    public CProcessBase
{
public:

  // all the kernel typdefs
  typedef CMetric< T, VImageDimension > MetricType;
  typedef CMetricSSD< T, VImageDimension > SSDMetricType;

  enum NumericMetricType { SSDMetric };

  CMetricFactory();
  ~CMetricFactory();

  static MetricType* CreateNewMetric( std::string sMetric );
  static MetricType* CreateNewMetric( NumericMetricType metric );

protected:
  static NumericMetricType GetMetricTypeFromString( std::string sMetric );
};

#include "CMetricFactory.txx"

} // end namespace

#endif // C_METRIC_FACTORY_H

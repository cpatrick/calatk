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

#ifndef C_CONSTRAINED_OPTIMIZATION_INTERFACE_H
#define C_CONSTRAINED_OPTIMIZATION_INTERFACE_H

#include "VectorImage.h"

namespace CALATK
{

/** Base interface class that provides the functionality to deal with simple
  * image-based equality constrains using an augmented Lagrangian approach
  */
class CConstrainedOptimizationInterface
{
public:
  virtual void PrintOptimizationStatus() {};
};

} // end of namespace

#endif // C_CONSTRAINED_OPTIMIZATION_INTERFACE_H

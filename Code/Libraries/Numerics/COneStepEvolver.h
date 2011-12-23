/**
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

#ifndef C_ONESTEPEVOLVER_H
#define C_ONESTEPEVOLVER_H

#include <stdexcept>

#include "CProcessBase.h"
#include "JSONParameterUtils.h"

#include "VectorImage.h"
#include "VectorField.h"

/** 
 * COneStepEvolver.h -- Base class that defines the general interface for 
 * numerical solvers to perform one update step. There is no testing
 * for numerical stability. The step is simply executed. Choosing an 
 * appropriate step-size is the resposibility of the caller. If implemented
 * by the derived class ComputeMaximalUpdateStep() can be used to get 
 * a suggested step size.
 *
 * Memory allocation is assumed to be external, so that it can be optimized
 * on an algorithm by algorithm case.
 *
 * Provided, so we can easily subsitute different solvers for the high
 * level solution algorithms
 *
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TSpace = T >
class COneStepEvolver : public CProcessBase
{

public:

  /** Some useful typedefs */
  
  typedef VectorField< T, VImageDimension, TSpace > VectorFieldType;
  typedef VectorImage< T, VImageDimension, TSpace > VectorImageType;

  typedef CProcessBase Superclass;

  /********************************
   * Constructors and Destructors *
   ********************************/

  /**
   * Empty Constructor
   */
  COneStepEvolver();

  /**
   * Computes a maximial step size (implementation optional)
   *
   * @param v - vector field to be evolved by
   */
  virtual T ComputeMaximalUpdateStep( const VectorFieldType* v  ) const = 0;

  /**
   * Performs an update step, to be implemented by the derived class (pure virtual).
   *
   * @param v - vector field to be evolved by
   * @param In - initial image (may be scalar or vector values as for a map)
   * @param Inp1 - resulting evolved image
   * @param dt - time step
   */
  virtual void PerformStep( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt ) = 0;

  /**
   * Determines the number of desired threads for parallel implementations. 
   * Default is 1, i.e., no threading.
   *
   * @params uiThreads - number of threads
   */
  SetMacro( NumberOfThreads, unsigned int );

  /**
   * Returns the set number of desired threads for parallel implementations. 
   *
   */
  GetMacro( NumberOfThreads, unsigned int );

  virtual void SetAutoConfiguration( Json::Value& ConfValue );

private:

  unsigned int m_NumberOfThreads;
  unsigned int DefaultNumberOfThreads;

  bool m_ExternallySetNumberOfThreads;

};

#include "COneStepEvolver.txx"

} // end namespace

#endif

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

#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CEvolver.h"
#include "CKernel.h"

namespace CALATK
{

template < class TState >
class CVelocityFieldObjectiveFunction : public CObjectiveFunction< TState >
{
public:

  /* some useful typedefs */
  typedef CObjectiveFunction< TState > Superclass;
  typedef typename TState::TFloat T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef CEvolver< T, TState::VImageDimension >* ptrEvolverType;
  typedef CKernel< T, TState::VImageDimension >* ptrKernelType;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CVelocityFieldObjectiveFunction();

  void SetEvolverPointer( ptrEvolverType pEvolver );
  ptrEvolverType GetEvolverPointer();

  void SetKernelPointer( ptrKernelType pKernel );
  ptrKernelType GetKernelPointer();

protected:

  ptrEvolverType m_ptrEvolver;
  // TODO: FIXME, support an array, so we can use different types of kernels
  ptrKernelType m_ptrKernel;

private:

};

#include "CVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif

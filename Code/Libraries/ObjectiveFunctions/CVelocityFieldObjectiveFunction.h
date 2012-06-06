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
  /** Standard class typedefs. */
  typedef CVelocityFieldObjectiveFunction  Self;
  typedef CObjectiveFunction< TState >     Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  /* some useful typedefs */
  typedef typename TState::FloatType T;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef CEvolver< T, TState::ImageDimension > EvolverType;
  typedef CKernel< T, TState::ImageDimension >  KernelType;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  typedef CImageManager< T, TState::ImageDimension >         ImageManagerType;
  typedef typename ImageManagerType::TimeSeriesDataPointType TimeSeriesDataPointType;
  typedef CTimePoint< T, VectorImageType, VectorFieldType > STimePoint;

  CVelocityFieldObjectiveFunction();

  void SetEvolverPointer( EvolverType * pEvolver );
  EvolverType * GetEvolverPointer();

  void SetKernelPointer( KernelType * pKernel );
  KernelType * GetKernelPointer();

protected:

  typename EvolverType::Pointer m_ptrEvolver;
  // TODO: FIXME, support an array, so we can use different types of kernels
  typename KernelType::Pointer  m_ptrKernel;

private:

};

#include "CVelocityFieldObjectiveFunction.txx"

} // end namespace

#endif

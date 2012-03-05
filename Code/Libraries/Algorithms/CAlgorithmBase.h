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

#ifndef C_ALGORITHM_BASE_H
#define C_ALGORITHM_BASE_H

#include "CALATKCommon.h"
#include "CProcessBase.h"
#include "CMetric.h"
#include "CKernel.h"
#include "CObjectiveFunction.h"
#include "CImageManager.h"
#include "CEvolver.h"
#include "CSolver.h"
#include "VectorImage.h"
#include "VectorField.h"

#include "CProcessBase.h"
#include "JSONParameterUtils.h"

namespace CALATK
{

/**
 * CAlgorithm.h -- Base class for all the registration algorithms which is still unaware of the state type.
 * Provides the interface for automatic instantiation of default metrics, solvers, ...
 * and takes care of deleting these default settings if they were allocated.
 *
 */

template < class T, unsigned int VImageDimension >
class CAlgorithmBase : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CAlgorithmBase                  Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* some useful typedefs */
  typedef CEvolver< T, VImageDimension >       EvolverType;
  typedef CKernel< T, VImageDimension >        KernelType;
  typedef CMetric< T, VImageDimension >        MetricType;
  typedef CImageManager< T, VImageDimension >  ImageManagerType;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  typedef typename CImageManager< T, VImageDimension >::SImageInformation SImageInformation;

  CAlgorithmBase();
  virtual ~CAlgorithmBase();

  void SetImageManagerPointer( ImageManagerType * ptrImageManager );
  ImageManagerType * GetImageManagerPointer();

  void SetKernelPointer( KernelType * ptrKernel );
  KernelType * GetKernelPointer();

  void SetEvolverPointer( EvolverType * ptrEvolver );
  EvolverType * GetEvolverPointer();

  void SetMetricPointer( MetricType * ptrMetric );
  MetricType * GetMetricPointer();

  virtual void Solve() = 0;

  virtual const VectorFieldType* GetMap( T dTime ) = 0;
  virtual const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo ) = 0;
  virtual const VectorImageType* GetImage( T dTime ) = 0;

protected:

  virtual void SetDefaultsIfNeeded() = 0;

  typename MetricType::Pointer       m_ptrMetric;
  typename ImageManagerType::Pointer m_ptrImageManager;
  typename EvolverType::Pointer      m_ptrEvolver;
  typename KernelType::Pointer       m_ptrKernel;

  virtual void SetDefaultMetricPointer() = 0;
  virtual void SetDefaultImageManagerPointer() = 0;
  virtual void SetDefaultKernelPointer() = 0;
  virtual void SetDefaultEvolverPointer() = 0;

  typename VectorImageType::Pointer m_ptrIm;
  typename VectorFieldType::Pointer m_ptrMap;
};

#include "CAlgorithmBase.txx"

} // end namespace


#endif // C_ALGORITHM_BASE_H

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

#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H

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
 * CAlgorithm.h -- Base class for all the registration algorithms.
 * Provides the interface for automatic instantiation of default metrics, solvers, ...
 * and takes care of deleting these default settings if they were allocated.
 *
 */

template < class TState >
class CAlgorithm : public CProcessBase
{
public:

  /** some useful typedefs */

  typedef typename TState::TFloat T;

  typedef CEvolver< T, TState::VImageDimension >* ptrEvolverType;
  typedef CKernel< T, TState::VImageDimension >* ptrKernelType;
  typedef CObjectiveFunction< TState >* ptrObjectiveFunctionType;
  typedef CMetric< T, TState::VImageDimension >* ptrMetricType;
  typedef CImageManager< T, TState::VImageDimension >* ptrImageManagerType;
  typedef CSolver< TState >* ptrSolverType;

  typedef VectorImage< T, TState::VImageDimension > VectorImageType;
  typedef VectorField< T, TState::VImageDimension > VectorFieldType;

  typedef CProcessBase Superclass;

  typedef typename CImageManager< T, TState::VImageDimension >::SImageInformation SImageInformation;

  CAlgorithm();
  virtual ~CAlgorithm();

  void SetImageManagerPointer( ptrImageManagerType ptrImageManager );
  ptrImageManagerType GetImageManagerPointer();

  void SetKernelPointer( ptrKernelType ptrKernel );
  ptrKernelType GetKernelPointer();

  void SetEvolverPointer( ptrEvolverType ptrEvolver );
  ptrEvolverType GetEvolverPointer();

  void SetObjectiveFunctionPointer( ptrObjectiveFunctionType ptrObjectiveFunction );
  ptrObjectiveFunctionType GetObjectiveFunctionPointer();

  void SetSolverPointer( ptrSolverType ptrSolver );
  ptrSolverType GetSolverPointer();

  void SetMetricPointer( ptrMetricType ptrMetric );
  ptrMetricType GetMetricPointer();

  virtual void Solve();

  virtual const VectorFieldType* GetMap( T dTime ) = 0;
  virtual const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo ) = 0;
  virtual const VectorImageType* GetImage( T dTime ) = 0;

protected:

  virtual void SetDefaultsIfNeeded();

  ptrMetricType m_ptrMetric;
  ptrImageManagerType m_ptrImageManager;
  ptrEvolverType m_ptrEvolver;
  ptrKernelType m_ptrKernel;
  ptrObjectiveFunctionType m_ptrObjectiveFunction;
  ptrSolverType m_ptrSolver;

  virtual void SetDefaultMetricPointer() = 0;
  virtual void SetDefaultImageManagerPointer() = 0;
  virtual void SetDefaultKernelPointer() = 0;
  virtual void SetDefaultEvolverPointer() = 0;
  virtual void SetDefaultObjectiveFunctionPointer() = 0;
  virtual void SetDefaultSolverPointer() = 0;

  VectorImageType* m_ptrIm;
  VectorFieldType* m_ptrMap;

private:

  bool m_bSetDefaultMetric;
  bool m_bSetDefaultImageManager;
  bool m_bSetDefaultEvolver;
  bool m_bSetDefaultKernel;
  bool m_bSetDefaultObjectiveFunction;
  bool m_bSetDefaultSolver;

};

#include "CAlgorithm.txx"

} // end namespace

#endif

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

#ifndef C_ALGORITHM_BASE_H
#define C_ALGORITHM_BASE_H

#include "CALATKCommon.h"
#include "CProcessBase.h"
#include "CMetric.h"
#include "CKernel.h"
#include "CObjectiveFunction.h"
#include "CImageManager.h"
#include "CEvolver.h"
#include "COneStepEvolver.h"
#include "CSolver.h"
#include "VectorImage.h"
#include "VectorField.h"

#include "CProcessBase.h"

namespace CALATK
{

/**
 * \class CAlgorithmBase
 *
 * \brief Base class for all the registration algorithms which is still unaware of the state type.
 *
 * Provides the interface for automatic instantiation of default metrics, solvers, ...
 */
template < class TFloat, unsigned int VImageDimension >
class CAlgorithmBase : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CAlgorithmBase                  Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /* some useful typedefs */
  typedef CEvolver< TFloat, VImageDimension >        EvolverType;
  typedef COneStepEvolver< TFloat, VImageDimension > OneStepEvolverType;
  typedef CKernel< TFloat, VImageDimension >         KernelType;
  typedef CMetric< TFloat, VImageDimension >         MetricType;
  typedef CImageManager< TFloat, VImageDimension >   ImageManagerType;

  typedef VectorImage< TFloat, VImageDimension >    VectorImageType;
  typedef VectorField< TFloat, VImageDimension >    VectorFieldType;

  CAlgorithmBase();
  virtual ~CAlgorithmBase();

  void SetImageManagerPointer( ImageManagerType * ptrImageManager );
  ImageManagerType * GetImageManagerPointer();

  virtual void SetKernelPointer( KernelType * ptrKernel );
  virtual KernelType * GetKernelPointer();

  virtual void SetEvolverPointer( EvolverType * ptrEvolver );
  virtual EvolverType * GetEvolverPointer();

  virtual void SetMetricPointer( MetricType * ptrMetric );
  virtual MetricType * GetMetricPointer();

  virtual void Solve() = 0;

  // Method which is executed before every sub-iteration
  virtual void PreSubIterationSolve();

  // Method which is executed before the inital solution
  virtual void PreFirstSolve();

  virtual const VectorFieldType* GetMap( TFloat dTime ) = 0;
  virtual const VectorFieldType* GetMapFromTo( TFloat dTimeFrom, TFloat dTimeTo ) = 0;
  virtual const VectorImageType* GetImage( TFloat dTime ) = 0;

  void SetAllowJSONHelpComments( bool bCreateJSONHelp );
  bool GetAllowJSONHelpComments();

  virtual void SetAutoConfiguration( CJSONConfiguration * configValueIn, CJSONConfiguration * configValueOut );

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

  unsigned int GetMultiScaleNumberOfScales() const ;
  TFloat       GetMultiScaleScale( unsigned int scaleIdx ) const;

  // Multi-scale settings for the image-manager
  typedef std::vector< TFloat > MultiScaleScalesType;

  MultiScaleScalesType m_MultiScaleScales;
};

} // end namespace


#endif // C_ALGORITHM_BASE_H

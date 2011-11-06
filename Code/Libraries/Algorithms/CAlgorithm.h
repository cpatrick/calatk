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

template <class T, unsigned int VImageDimension, class TState >
class CAlgorithm : public CProcessBase
{
public:

  /** some useful typedefs */

  typedef CEvolver< T, VImageDimension >* ptrEvolverType;
  typedef CKernel< T, VImageDimension >* ptrKernelType;
  typedef CObjectiveFunction< T, VImageDimension, TState >* ptrObjectiveFunctionType;
  typedef CMetric< T, VImageDimension >* ptrMetricType;
  typedef CImageManager< T, VImageDimension>* ptrImageManagerType;
  typedef CSolver< T, VImageDimension, TState>* ptrSolverType;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  typedef CProcessBase Superclass;

  typedef typename CImageManager< T, VImageDimension >::SImageInformation SImageInformation;

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

  virtual void SetAutoConfiguration( const Json::Value& ConfValue );

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

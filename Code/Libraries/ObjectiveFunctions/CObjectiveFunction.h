#ifndef C_OBJECTIVE_FUNCTION_H
#define C_OBJECTIVE_FUNCTION_H

#include "CALATKCommon.h"
#include "CMetric.h"
#include "CImageManager.h"
#include "CKernel.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CObjectiveFunction
{
public:

  /** some useful typedefs */
  
  typedef TState* ptrStateType;

  typedef CMetric< T, VImageDimension >* ptrMetricType;
  typedef CKernel< T, VImageDimension >* ptrKernelType;
  typedef CImageManager< T, VImageDimension >* ptrImageManagerType;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

  CObjectiveFunction();
  virtual ~CObjectiveFunction();

  /* Returns the energy of the objective function */
  virtual T GetCurrentEnergy() = 0;

  /* Compute the gradient of the objective function and store it in the gradient member variable */
  virtual void ComputeGradient();

  /* Gets the map at a specific time, assuming that the map for the initial image is the identity */
  virtual void GetMap( VectorFieldType* ptrMap, T dTime ) = 0;

  /* Gets the map between specific time points */
  virtual void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo ) = 0;

  /* Gets the image at a specific time point */
  virtual void GetImage( VectorImageType* ptrIm, T dTime ) = 0;

  virtual void InitializeState() = 0;
  virtual void InitializeState( TState* ) = 0;

  void SetImageManagerPointer( ptrImageManagerType ptrImageManager )
  {
    m_ptrImageManager = ptrImageManager;
  };

  ptrImageManagerType GetImageManagerPointer() const
  {
    return m_ptrImageManager;
  };

  void SetStatePointer( ptrStateType pState )
  {
    m_pState = pState;
  };

  ptrStateType GetStatePointer() const
  {
    return m_pState;
  };

  void SetGradientPointer( ptrStateType pGradient )
  {
    m_pGradient = pGradient;
  };

  ptrStateType GetGradientPointer() const
  {
    return m_pGradient;
  };

  void SetMetricPointer( ptrMetricType pMetric )
  {
    m_pMetric = pMetric;
  };

  ptrMetricType GetMetricPointer() const
  {
    return m_pMetric;
  };

protected:

  ptrStateType m_pState;
  ptrStateType m_pGradient;

  // TODO: FIXME, needs to be an array, so we can in principle support registrations at every measurement
  ptrMetricType m_pMetric;
  ptrImageManagerType m_ptrImageManager;

private:



};

#include "CObjectiveFunction.txx"

} // end namespace

#endif

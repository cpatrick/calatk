#ifndef C_OBJECTIVE_FUNCTION_H
#define C_OBJECTIVE_FUNCTION_H

#include "CALATKCommon.h"
#include "CMetric.h"
#include "CImageManager.h"
//#include "CKernelManager.h"

namespace CALATK
{

template <class T, class TState, unsigned int VImageDimension=3 >
class CObjectiveFunction
{
public:
  
  typedef TState* ptrStateType;
  typedef CMetric< T, VImageDimension >* ptrMetricType;

  typedef CImageManager< T, VImageDimension >* ptrImageManagerType;

  CObjectiveFunction();
  virtual ~CObjectiveFunction();

  virtual T GetCurrentEnergy() = 0;
  virtual void ComputeGradient();
  
  virtual void InitializeState() = 0;
  virtual void InitializeDataStructures() = 0;
  virtual void InitializeDataStructuresFromState( TState* ) = 0;

  void SetImageManagerPointer( ptrImageManagerType ptrImageManager )
  {
    m_ptrImageManager = ptrImageManager;
  };

  ptrImageMangerType GetImageManagerPointer()
  {
    return m_ptrImageManager;
  };

/*  TODO: Implement a kernel manager, so we can choose different kernels here
    and also so that we can easily interface with the multi-scale manager
    void SetKernelManagerPointer( ptrImageManagerType ptrImageManager )
  {
    m_ptrKernelManager = ptrKernelManager;
  };

  ptrImageMangerType GetKernelManagerPointer()
  {
    return m_ptrKernelManager;
    };*/

  void SetStatePointer( ptrStateType pState )
  {
    m_pState = pState;
  };

  ptrStateType GetStatePointer()
  {
    return m_pState;
  };

  void SetGradientPointer( ptrStateType pGradient )
  {
    m_pGradient = pGradient;
  };

  ptrStateType GetGradientPointer()
  {
    return m_pGradient;
  };

  void SetMetric( ptrMetricType pMetric )
  {
    m_pMetric = pMetric;
  };

  ptrMetricType GetMetric()
  {
    return m_pMetric;
  };

protected:

  ptrStateType m_pState;
  ptrStateType m_pGradient;

  // FIXME, needs to be an array, so we can in principle support registrations at every measurement
  ptrMetricType m_pMetric;

  ptrImageManagerType m_ptrImageManager;

private:



};

#include "CObjectiveFunction.txx"

} // end namespace

#endif

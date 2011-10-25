#ifndef C_LDDMM_GROWTH_MODEL_REGISTRATION_TXX
#define C_LDDMM_GROWTH_MODEL_REGISTRATION_TXX

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::CLDDMMGrowthModelRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::~CLDDMMGrowthModelRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::VectorFieldType* 
CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::GetMap( T dTime )
{
  this->m_ptrObjectiveFunction->GetMap( this->m_ptrMap, dTime );
  return this->m_ptrMap;
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::VectorImageType* 
CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::GetImage( T dTime )
{
  this->m_ptrObjectiveFunction->GetImage( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelRegistration< T, VImageDimension, TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState > CLDDMMType;
  CLDDMMType* plddmm = new CLDDMMType;
  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );
  this->m_ptrObjectiveFunction = plddmm;

}

#endif

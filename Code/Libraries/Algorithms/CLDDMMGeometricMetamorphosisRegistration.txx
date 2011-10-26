#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::CLDDMMGeometricMetamorphosisRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::~CLDDMMGeometricMetamorphosisRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::VectorImageType* 
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::GetImageT( T dTime )
{
  dynamic_cast< CLDDMMGeometricMetamorphosisObjectiveFunction<T, VImageDimension, TState>* >(this->m_ptrObjectiveFunction)->GetImageT( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::SetDefaultObjectiveFunctionPointer()
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

  typedef CLDDMMGeometricMetamorphosisObjectiveFunction< T, VImageDimension, TState > CLDDMMType;
  CLDDMMType* plddmm = new CLDDMMType;
  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );
  this->m_ptrObjectiveFunction = plddmm;

}

#endif



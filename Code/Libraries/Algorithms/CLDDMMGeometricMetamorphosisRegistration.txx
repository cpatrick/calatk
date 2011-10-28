#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::CLDDMMGeometricMetamorphosisRegistration()
{
  m_ptrMaskKernel = NULL;
  m_bSetDefaultMaskKernel = false;
}

template < class T, unsigned int VImageDimension, class TState >
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::~CLDDMMGeometricMetamorphosisRegistration()
{
  if ( m_bSetDefaultMaskKernel )
    {
    delete m_ptrMaskKernel;
    m_ptrMaskKernel = NULL;
    }
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::SetMaskKernelPointer( ptrKernelType ptrKernel )
{
  m_ptrMaskKernel = ptrKernel;
}

template < class T, unsigned int VImageDimension, class TState >
CKernel< T, VImageDimension >*
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::GetMaskKernelPointer()
{
  if ( m_ptrMaskKernel == NULL )
    {
    this->SetDefaultMaskKernelPointer();
    m_bSetDefaultMaskKernel = true;
    }
  return m_ptrMaskKernel;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::SetDefaultMaskKernelPointer()
{
  CHelmholtzKernel< T, VImageDimension >* ptrHH = new CHelmholtzKernel< T, VImageDimension >;
  this->m_ptrMaskKernel = ptrHH;

  // multiply alpha by 0.25 to make the mask more fluid as default behavior
  ptrHH->SetAlpha( 0.25*ptrHH->GetAlpha() );
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::VectorImageType* 
CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::GetImageT( T dTime )
{
  dynamic_cast< CLDDMMGeometricMetamorphosisObjectiveFunction<T, VImageDimension, TState>* >(this->m_ptrObjectiveFunction)->GetImageT( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::SetDefaultsIfNeeded()
{
  std::cout << "Called the default mask kernel function" << std::endl;
  if ( m_ptrMaskKernel == NULL )
    {
    SetDefaultMaskKernelPointer();
    m_bSetDefaultMaskKernel = true;
    }
  Superclass::SetDefaultsIfNeeded();
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMGeometricMetamorphosisRegistration< T, VImageDimension, TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMaskKernel == NULL )
    {
    throw std::runtime_error( "Mask kernel needs to be defined before default objective function can be created." );
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
  plddmm->SetMaskKernelPointer( this->m_ptrMaskKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );
  this->m_ptrObjectiveFunction = plddmm;

}

#endif



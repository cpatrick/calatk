#ifndef C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX
#define C_LDDMM_GEODESIC_SHOOTING_INITIAL_IMAGE_MOMENTUM_REGISTRATION_TXX

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::CLDDMMGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::~CLDDMMGeodesicShootingInitialImageMomentumRegistration()
{
}

template < class TState >
void CLDDMMGeodesicShootingInitialImageMomentumRegistration< TState >::SetDefaultObjectiveFunctionPointer()
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

  typedef CLDDMMGeodesicShootingInitialImageMomentumObjectiveFunction< TState > CLDDMMType;
  CLDDMMType* plddmm = new CLDDMMType;
  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  plddmm->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );

  this->m_ptrObjectiveFunction = plddmm;

}

#endif


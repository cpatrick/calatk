#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_TXX
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_REGISTRATION_TXX

template < class T, unsigned int VImageDimension, class TState >
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::CLDDMMSpatioTemporalVelocityFieldRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::~CLDDMMSpatioTemporalVelocityFieldRegistration()
{
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::VectorFieldType* 
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::GetMap( T dTime )
{
  this->m_ptrObjectiveFunction->GetMap( this->m_ptrMap, dTime );
  return this->m_ptrMap;
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::VectorImageType* 
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::GetImage( T dTime )
{
  this->m_ptrObjectiveFunction->GetImage( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultMetricPointer()
{
  this->m_ptrMetric = new CMetricSSD< T, VImageDimension >;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultImageManagerPointer()
{
  this->m_ptrImageManager = new CImageManagerFullScale< T, VImageDimension >;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultKernelPointer()
{
  this->m_ptrKernel = new CHelmholtzKernel< T, VImageDimension >;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultEvolverPointer()
{
  this->m_ptrEvolver = new CStationaryEvolver< T, VImageDimension >;
  this->m_ptrEvolver->SetOneStepEvolverPointer( &oneStepDefaultEvolver );
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrSolver == NULL )
    {
    throw std::runtime_error( "Solver needs to be defined before default objective function can be created." );
    }

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

  this->m_ptrObjectiveFunction = new CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< T, VImageDimension, TState >;
  this->SetEvolverPointer( this->m_ptrEvolver );
  this->SetSolverPointer( this->m_ptrSolver );
  this->SetKernelPointer( this->m_ptrKernel );
  this->SetMetricPointer( this->m_ptrMetric );
  this->SetImageManagerPointer( this->m_ptrImageManager );
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultSolverPointer()
{
  this->m_ptrSolver = new CSolverLineSearch< T, VImageDimension, TState >;
}



#endif

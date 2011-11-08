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
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultMetricPointer()
{
  this->m_ptrMetric = new CMetricSSD< T, VImageDimension >;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultImageManagerPointer()
{
  //this->m_ptrImageManager = new CImageManagerFullScale< T, VImageDimension >;
  this->m_ptrImageManager = new CImageManagerMultiScale< T, VImageDimension >;
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultKernelPointer()
{
  this->m_ptrKernel = new CHelmholtzKernel< T, VImageDimension >;
  this->m_ptrKernel->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultEvolverPointer()
{
  this->m_ptrEvolver = new CStationaryEvolver< T, VImageDimension >;
  this->m_ptrEvolver->SetOneStepEvolverPointer( &oneStepDefaultEvolver );
  this->m_ptrEvolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class T, unsigned int VImageDimension, class TState >
void CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::SetDefaultSolverPointer()
{
  //this->m_ptrSolver = new CSolverLineSearch< T, VImageDimension, TState >;
  this->m_ptrSolver = new CSolverMultiScale< T, VImageDimension, TState >;
  this->m_ptrSolver->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::VectorFieldType* 
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::GetMap( T dTime )
{
  this->m_ptrObjectiveFunction->GetMap( this->m_ptrMap, dTime );
  return this->m_ptrMap;
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::VectorFieldType* 
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::GetMapFromTo( T dTimeFrom, T dTimeTo )
{
  this->m_ptrObjectiveFunction->GetMapFromTo( this->m_ptrMap, dTimeFrom, dTimeTo );
  return this->m_ptrMap;
}

template < class T, unsigned int VImageDimension, class TState >
const typename CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::VectorImageType* 
CLDDMMSpatioTemporalVelocityFieldRegistration< T, VImageDimension, TState >::GetImage( T dTime )
{
  this->m_ptrObjectiveFunction->GetImage( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}



#endif

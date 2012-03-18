/*
*
*  Copyright 2011 by the CALATK development team
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

#ifndef C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_SPATIO_TEMPORAL_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
  : DefaultNumberOfDiscretizationVolumesPerUnitTime( 10.0 ), m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime( false )
{
  m_NumberOfDiscretizationVolumesPerUnitTime = DefaultNumberOfDiscretizationVolumesPerUnitTime;
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::DeleteData()
{
  this->m_ptrKernel->DeallocateMemory();

  this->m_ptrState = NULL;
  this->m_ptrGradient = NULL;

  m_vecTimeIncrements.clear();
  m_vecTimeDiscretization.clear();
}

template < class TState >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  DeleteData();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "SpatioTemporalVelocityField", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "SpatioTemporalVelocityField", Json::nullValue );

  SetJSONHelpForRootKey( SpatioTemporalVelocityField, "settings for the spatio-temporal velocity field" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, NumberOfDiscretizationVolumesPerUnitTime );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, NumberOfDiscretizationVolumesPerUnitTime,
                     "number of time discretization steps used for a unit time period" );
}


template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CreateTimeDiscretization()
{

  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    throw std::runtime_error( "ERROR: No image manager specified." );
    return;
    }

  std::vector< STimePoint > vecTimePointData;
  this->DetermineTimePointData( vecTimePointData );

  CALATK::LDDMMUtils< T, TState::VImageDimension >::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, m_NumberOfDiscretizationVolumesPerUnitTime );

  // the time discretization vector has all the N timepoint. There will be N-1 vector fields in between

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CreateNewStateStructures()
{

  // Images and adjoints will be saved at all time-points
  // velcoity fields have one less entry (because they 'live' in between the measurement points)

  assert( this->m_ptrState.GetPointer() == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate memory for the state

  std::vector< VectorFieldPointerType > vecState;

  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pGraftIm );
    ptrCurrentVectorField->SetToConstant( 0 );
    vecState.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the state
  this->m_ptrState = new TState( &vecState );
    
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CreateNewGradientStructures()
{
  assert( this->m_ptrGradient.GetPointer() == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate the memory for the gradient

  std::vector< VectorFieldPointerType > vecGradient;
  
  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pGraftIm );
    ptrCurrentVectorField->SetToConstant( 0 );
    vecGradient.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the gradient
  this->m_ptrGradient = new TState( &vecGradient );

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::ShallowCopyStateStructures( TState* ptrState )
{

  assert( this->m_ptrState.GetPointer() == NULL );

  std::vector< VectorFieldPointerType > vecState;

  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = ptrState->GetVectorFieldPointer( iI );
    vecState.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the state
  this->m_ptrState = new TState( &vecState );

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeDataStructuresFromState( TState* ptrState )
{
  this->DeleteAuxiliaryStructures();

  this->DeleteData();

  this->CreateTimeDiscretization();
  
  // shallow copy (i.e., we just take over the externally allocated memory)
  this->ShallowCopyStateStructures( ptrState );

  // new geadient structures
  this->CreateNewGradientStructures();

  // gradient and everything else
  this->CreateAuxiliaryStructures();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeDataStructures()
{
  this->DeleteAuxiliaryStructures();

  this->DeleteData();

  assert( this->m_ptrGradient.GetPointer() == NULL );
  assert( this->m_ptrState.GetPointer() == NULL );

  this->CreateTimeDiscretization();

  // allocate state structures
  this->CreateNewStateStructures();

  // allocate new gradient strcutures
  this->CreateNewGradientStructures();

  // gradient and everything else
  this->CreateAuxiliaryStructures();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{
  CALATK::LDDMMUtils< T, TState::VImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
        ptrMap,
        dTimeFrom,
        dTimeTo,
        this->m_vecTimeDiscretization,
        this->m_ptrState->GetVectorPointerToVectorFieldPointer(),
        this->m_ptrEvolver );
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeState()
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructures();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeState( TState* ptrState )
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructuresFromState( ptrState );
}

#endif

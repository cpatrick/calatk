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

  m_vecTimeIncrements.clear();
  m_vecTimeDiscretization.clear();

  SaveDelete< TState* >::Pointer( this->m_pState );
  SaveDelete< TState* >::Pointer( this->m_pGradient );

}

template < class TState >
CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::~CLDDMMSpatioTemporalVelocityFieldObjectiveFunction()
{
  DeleteData();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "SpatioTemporalVelocityField", Json::nullValue );

  SetJSONNumberOfDiscretizationVolumesPerUnitTime( this->m_jsonConfig.GetFromKey( currentConfiguration, "NumberOfDiscretizationVolumesPerUnitTime", GetExternalOrDefaultNumberOfDiscretizationVolumesPerUnitTime() ).asDouble() );
}


template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CreateTimeDiscretization()
{

  if ( this->m_ptrImageManager == NULL )
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

  assert( this->m_pState == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate memory for the state

  std::vector< VectorFieldPointerType > vecState;

  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pGraftIm );
    ptrCurrentVectorField->setConst( 0 );
    vecState.push_back( ptrCurrentVectorField );
    }

  // associate the allocated memory with the state
  this->m_pState = new TState( &vecState );
    
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::CreateNewGradientStructures()
{
  assert( this->m_pGradient == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate the memory for the gradient

  std::vector< VectorFieldPointerType > vecGradient;
  
  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( pGraftIm );
    ptrCurrentVectorField->setConst( 0 );
    vecGradient.push_back( ptrCurrentVectorField );
    }
  
  // associate the allocated memory with the gradient
  this->m_pGradient = new TState( &vecGradient );

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{

  assert( this->m_pState == NULL );

  std::vector< VectorFieldPointerType > vecState;
    
  // -1, because these are velocity fields
  for ( unsigned int iI=0; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    VectorFieldPointerType ptrCurrentVectorField = pState->GetVectorFieldPointer( iI );
    vecState.push_back( ptrCurrentVectorField );
    }
    
  // associate the allocated memory with the state
  this->m_pState = new TState( &vecState );

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeDataStructuresFromState( TState* pState )
{

  DeleteAuxiliaryStructures();

  DeleteData();

  CreateTimeDiscretization();
  
  // shallow copy (i.e., we just take over the externally allocated memory)
  ShallowCopyStateStructures( pState );

  // new geadient structures
  CreateNewGradientStructures();

  // gradient and everything else
  CreateAuxiliaryStructures();

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeDataStructures()
{

  DeleteAuxiliaryStructures();

  DeleteData();

  assert( this->m_pGradient == NULL );
  assert( this->m_pState == NULL );

  CreateTimeDiscretization();

  // allocate state structures
  CreateNewStateStructures();

  // allocate new gradient strcutures
  CreateNewGradientStructures();

  // gradient and everything else
  CreateAuxiliaryStructures();

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
  
  std::cout << "Computing map from " << dTimeFrom << " to " << dTimeTo << std::endl;

  if ( dTimeFrom < m_vecTimeDiscretization[0].dTime || dTimeTo > m_vecTimeDiscretization.back().dTime )
    {
    throw std::runtime_error("Requested map outside of valid time range.");
    return;
    }

  VectorFieldType* ptrMapOut = ptrMap;

  // create two additional maps to hold the solution
  VectorFieldType* ptrMapIn = new VectorFieldType( ptrMap );
  VectorFieldType* ptrMapTmp = new VectorFieldType( ptrMap );

  // get the map between two time points
  LDDMMUtils< T, TState::VImageDimension >::identityMap( ptrMapIn );


  T dCurrentTime = m_vecTimeDiscretization[0].dTime;
  unsigned int uiStart = 0;

  // we may need to fast forward to the beginning time point

  if ( dCurrentTime < dTimeFrom )
    {

    for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; ++iI )
      {
      if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] > dTimeFrom )
        {
        // evolve for an increment
        std::cout << "partially evolve for " << dTimeFrom - dCurrentTime << std::endl;
        this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), ptrMapIn, ptrMapOut, ptrMapTmp, dTimeFrom-dCurrentTime );
        // for next step, copy
        ptrMapIn->copy( ptrMapOut );
        uiStart = iI+1;
        dCurrentTime += this->m_vecTimeIncrements[ iI ];
        break;
        }
      else
        {
        // just skip ahead
        dCurrentTime += this->m_vecTimeIncrements[ iI ];
        uiStart = iI + 1;
        }
      if ( dCurrentTime >= dTimeFrom )
        {
        break;
        }
      }
    }

  std::cout << "fast forwarded to " << dCurrentTime << std::endl;
  std::cout << "starting from index " << uiStart << std::endl;

  // now we can move ahead

  for ( unsigned int iI = uiStart; iI < m_vecTimeDiscretization.size()-1; ++iI )
    {
    if ( dCurrentTime + this->m_vecTimeIncrements[ iI ] < dTimeTo )
      {
      std::cout << "evolved for " << this->m_vecTimeIncrements[ iI ] << std::endl;
      this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), ptrMapIn, ptrMapOut, ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      dCurrentTime += this->m_vecTimeIncrements[ iI ];
      }
    else
      {
      std::cout << "finally partially evolved for " << dTimeTo-dCurrentTime << std::endl;
      this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), ptrMapIn, ptrMapOut, ptrMapTmp, dTimeTo-dCurrentTime );
      dCurrentTime = dTimeTo;
      break;
      }
    // for next step, copy
    ptrMapIn->copy( ptrMapOut );
    }

  // get rid of the temporary memory
  delete ptrMapIn;
  delete ptrMapTmp;

}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeState()
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructures();
}

template < class TState >
void CLDDMMSpatioTemporalVelocityFieldObjectiveFunction< TState >::InitializeState( TState* pState )
{
  // TODO: Set all the velocities to zero and the initial image to the first image of the time series
  InitializeDataStructuresFromState( pState );
}

#endif

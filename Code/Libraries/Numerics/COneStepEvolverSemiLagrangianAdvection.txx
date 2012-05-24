/*
*
*  Copyright 2011, 2012 by the CALATK development team
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

#ifndef C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_TXX
#define C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_TXX

#include "COneStepEvolverSemiLagrangianAdvection.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension >
COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::COneStepEvolverSemiLagrangianAdvection()
  : DefaultTimeStepFactor( 1 ), m_ExternallySetTimeStepFactor( false ),
    DefaultNumberOfIterationsToDetermineFlowField( 2 ), m_ExternallySetNumberOfIterationsToDetermineFlowField( false )
{
  this->m_TimeStepFactor = DefaultTimeStepFactor;
  this->m_NumberOfIterationsToDetermineFlowField = DefaultNumberOfIterationsToDetermineFlowField;

  this->m_ptrAdjustedVectorField = NULL;
  this->m_ptrTmpVectorField = NULL;
}

template <class T, unsigned int VImageDimension >
COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::~COneStepEvolverSemiLagrangianAdvection()
{
}

template <class T, unsigned int VImageDimension >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "OneStepEvolverSemiLagrangianAdvection", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "OneStepEvolverSemiLagrangianAdvection", Json::nullValue );

  SetJSONHelpForRootKey( OneStepEvolverSemiLagrangianAdvection, "settings for the semi-Lagrangian advection solver" );

  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, NumberOfIterationsToDetermineFlowField );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, TimeStepFactor );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, NumberOfIterationsToDetermineFlowField,
                     "number of iterations used to determine the velocity which a particle had to end up at the current point" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, TimeStepFactor,
                     "factor the CFL timestep is multiplied by (can be larger than one for a semi-Lagrangian scheme" );
}

//
// Performs a step for an image
//
template <class T, unsigned int VImageDimension >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::PerformStepWithGivenVectorField(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
{

  //
  // check to make sure all data is set properly
  //
  
#ifdef DEBUG
  // data defined
  if (In == NULL || v == NULL || Inp1 == NULL) {
    throw std::runtime_error("No Data");
  }
#endif
  
  
#ifdef DEBUG

  // check that the image sizes match
  if ( !VectorImageUtilsType::IsSameSize( v, In ) || !VectorImageUtils::IsSameSize( v, Inp1 ) )
  {
    throw std::invalid_argument("Dimension Mismatch");
  }
#endif
  interpolator.SetNumberOfThreads( this->GetNumberOfThreads() );
  interpolator.InterpolateNegativeVelocityPos( In, v, dt, Inp1 );
  
}


template <class T, unsigned int VImageDimension >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::PerformStepWithAdjustedVectorField(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
{
  ComputeAdjustedVectorFieldIfRequired( v, dt );
  interpolator.SetNumberOfThreads( this->GetNumberOfThreads() );
  interpolator.InterpolateNegativeVelocityPos( In, this->m_ptrAdjustedVectorField, dt, Inp1 );
}

template <class T, unsigned int VImageDimension >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::ComputeAdjustedVectorFieldIfRequired( const VectorFieldType* v, T dt )
{
  // we need to compute it in any of these cases
  // 1) the image dimensions have changed
  // 2) memory has not been allocated
  // 3) the initialization state is set to true

  bool bIsSameSize = ( VectorImageUtilsType::IsSameSize( v, this->m_ptrAdjustedVectorField ) ) &&
                     ( VectorImageUtilsType::IsSameSize( v, this->m_ptrTmpVectorField ) );

  bool bRecompute =  ( this->GetInitializeOneStepEvolverState() ) || ( !bIsSameSize );

  if ( bRecompute )
  {
    if ( !bIsSameSize )
    {
      this->m_ptrAdjustedVectorField = new VectorFieldType( v );
      this->m_ptrTmpVectorField = new VectorFieldType( v );
    }

    // now that we have all the memory that is required do the computation
    this->m_ptrAdjustedVectorField->Copy( v );

    interpolator.SetNumberOfThreads( this->GetNumberOfThreads() );

    for ( unsigned int iI=0; iI < m_NumberOfIterationsToDetermineFlowField; ++iI )
    {
      interpolator.InterpolateNegativeVelocityPos( this->m_ptrAdjustedVectorField, v, dt*0.5, this->m_ptrTmpVectorField );

      // swap the pointers, so we have the updated result in ptrAdjustedVectorField
      typename VectorFieldType::Pointer ptrSwap = this->m_ptrAdjustedVectorField;
      this->m_ptrAdjustedVectorField = this->m_ptrTmpVectorField;
      this->m_ptrTmpVectorField = ptrSwap;
    }

  }

  // certify that this initialization was done
  this->SetInitializeOneStepEvolverState( false );

}

template <class T, unsigned int VImageDimension >
void COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::PerformStep(  const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt )
{
  if ( m_NumberOfIterationsToDetermineFlowField > 0 )
  {
    PerformStepWithAdjustedVectorField( v, In, Inp1, dt );
  }
  else
  {
    PerformStepWithGivenVectorField( v, In, Inp1, dt );
  }
}


template <class T, unsigned int VImageDimension >
T COneStepEvolverSemiLagrangianAdvection<T, VImageDimension >::ComputeMaximalUpdateStep( const VectorFieldType* v ) const
{
  T dFact = m_TimeStepFactor;

  T vMax = VectorFieldUtils< T, VImageDimension >::absMaxAll( v );

  if ( vMax==0 ) vMax = 1; // if everything is zero use a default value

  // TODO: take this back out, just for testing
  T dtx = dFact*v->GetSpacingX()/vMax;

  T dty = std::numeric_limits< T >::infinity();
  T dtz = std::numeric_limits< T >::infinity();

  if ( VImageDimension>1 )
    {
    dty = dFact*v->GetSpacingY()/vMax;
    }

  if ( VImageDimension>2 )
    {
    dtz = dFact*v->GetSpacingZ()/vMax;
    }

  T minDT = std::min( dtx, dty );
  minDT = std::min( minDT, dtz );

  return minDT;
}

} // end namespace CALATK

#endif

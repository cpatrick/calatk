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

#ifndef C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMGeodesicShootingObjectiveFunction< TState >::CLDDMMGeodesicShootingObjectiveFunction()
    : DefaultNumberOfDiscretizationVolumesPerUnitTime( 10.0 ), m_ExternallySetNumberOfDiscretizationVolumesPerUnitTime( false ),
      DefaultEstimateInitialImage( false ), m_ExternallySetEstimateInitialImage( false )
{
    m_NumberOfDiscretizationVolumesPerUnitTime = DefaultNumberOfDiscretizationVolumesPerUnitTime;
    m_EstimateInitialImage = DefaultEstimateInitialImage;
}

template < class TState >
CLDDMMGeodesicShootingObjectiveFunction< TState >::~CLDDMMGeodesicShootingObjectiveFunction()
{
}

template < class TState >
void CLDDMMGeodesicShootingObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "LDDMMGeodesicShooting", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "LDDMMGeodesicShooting", Json::nullValue );

  SetJSONHelpForRootKey( LDDMMGeodesicShooting, "setting for LDDMM shooting implementation (depending only on initial image/momentum" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, NumberOfDiscretizationVolumesPerUnitTime );
  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, EstimateInitialImage );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, NumberOfDiscretizationVolumesPerUnitTime,
                     "if time is discretized, determines how many discretization steps are used for each unit of time" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, EstimateIntialImage,
                     "when true the initial image is jointly estimated with the initial momentum" );
}

template < class TState >
void CLDDMMGeodesicShootingObjectiveFunction< TState >::GetInitialMomentum( VectorImageType* ptrMomentum )
{
  ptrMomentum->Copy( this->m_ptrState->GetPointerToInitialMomentum() );
}

template < class TState >
void CLDDMMGeodesicShootingObjectiveFunction< TState >::ComputeVelocity( const VectorImageType  * ptrI, const VectorImageType * ptrP, VectorFieldType * ptrVout, VectorFieldType * ptrTmpField )
{
 /**
  * Computes the velocity given an image and a momentum
  * Computes
  \f[
    v = -K*(p\nabla I)
  \f]
  */

  unsigned int dim = ptrI->GetDimension();
  ptrVout->SetToConstant(0);

  for ( unsigned int iD = 0; iD<dim; iD++ )
  {
      VectorFieldUtilsType::computeCentralGradient( ptrI, iD, ptrTmpField );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrP, iD, ptrTmpField );
      ptrVout->AddCellwise( ptrTmpField );
  }

  this->m_ptrKernel->ConvolveWithKernel( ptrVout );
  ptrVout->MultiplyByConstant( -1.0 );
}

#endif

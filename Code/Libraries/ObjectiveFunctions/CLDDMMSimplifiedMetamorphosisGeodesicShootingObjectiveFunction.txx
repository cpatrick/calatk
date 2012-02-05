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

#ifndef C_LDDMM_SIMPIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_SIMPIFIED_METAMORPHOSIS_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction()
  : DefaultRho( 1 ),
    m_ExternallySetRho( false ),
    m_AugmentedLagrangianMu( 0.1 ),
    m_ptrImageLagrangianMultiplier( NULL )
{
  m_Rho = DefaultRho;

  // storage for the map
  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

  m_ptrDeterminantOfJacobian = NULL;

  m_ptrCurrentI = NULL;
  m_ptrCurrentP = NULL;
  m_ptrCurrentVelocity = NULL;

  m_ptrTmpField = NULL;
  m_ptrTmpFieldConv = NULL;
  m_ptrTmpImage = NULL;

  m_ptrCurrentBackMap = NULL;
  m_ptrMapIdentity = NULL;
  m_ptrMapIncremental = NULL;

  m_ptrCurrentFinalAdjoint = NULL;
  m_ptrWarpedFinalToInitialAdjoint = NULL;

  // convencience pointers to the source and the target image respectively
  ptrI0 = NULL;
  ptrI1 = NULL;

}

template< class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::DeleteData()
{
  this->m_ptrKernel->DeallocateMemory();

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentI );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentP );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrCurrentVelocity );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpField );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpFieldConv );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrTmpImage );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrCurrentBackMap );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIdentity );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIncremental );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentFinalAdjoint );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrWarpedFinalToInitialAdjoint );

  ptrI0 = NULL;
  ptrI1 = NULL;

  m_vecMeasurementTimepoints.clear();
  m_vecTimeDiscretization.clear();
  m_vecTimeIncrements.clear();

  SaveDelete< TState* >::Pointer( this->m_pState );
  SaveDelete< TState* >::Pointer( this->m_pGradient );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrImageLagrangianMultiplier );

}

template < class TState >
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::~CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction()
{
  DeleteData();
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );
  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "SimplifiedMetamorphosis", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "SimplifiedMetamorphosis", Json::nullValue );

  SetJSONHelpForRootKey( GrowthModel, "settings for the simplified metamorphosis model" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Rho );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Rho,
                     "rho is the weight of the appearance change penalty rho|q|^2" );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
{
    assert( this->m_pState == NULL );
    assert( m_vecTimeDiscretization.size() > 1 );

    // get the subject ids
    std::vector< unsigned int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures

    SImageInformation* pImInfo;
    // get information from the first image to figure out the dimensions and determine the source and target image
    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

    VectorImageType* ptrInitialImage = new VectorImageType( pImInfo->pIm );
    VectorImageType* ptrInitialMomentum = new VectorImageType( pImInfo->pIm );
    ptrInitialMomentum->setConst(0);

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template< class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{
    assert ( this->m_pState == NULL );

    VectorImageType* ptrInitialImage = pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = pState->GetPointerToInitialMomentum();

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
{

    // get the subject ids
    std::vector< unsigned int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures
    // and assign the convenience image pointer ptrI0, ptrI1
    SImageInformation* pImInfo;
    // get information from the first image to figure out the dimensions and determine the source and target image
    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );
    ptrI0 = pImInfo->pIm;

    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 1 );
    ptrI1 = pImInfo->pIm;

    // create the gradient
    VectorImageType* ptrI0Gradient = new VectorImageType( pImInfo->pIm );
    ptrI0Gradient->setConst(0);

    VectorImageType* ptrP0Gradient = new VectorImageType( pImInfo->pIm );
    ptrP0Gradient->setConst(0);

    this->m_pGradient = new TState( ptrI0Gradient, ptrP0Gradient );

    // storage for the maps

    m_ptrMapIn = new VectorFieldType( pImInfo->pIm );
    m_ptrMapOut = new VectorFieldType( pImInfo->pIm );
    m_ptrMapTmp = new VectorFieldType( pImInfo->pIm );

    // storage for the determinant of the Jacobian
    m_ptrDeterminantOfJacobian = new VectorImageType( pImInfo->pIm, 0.0, 1 );

    // storage for current state
    m_ptrCurrentI = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentP = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentVelocity = new VectorFieldType( pImInfo->pIm );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( pImInfo->pIm );
    m_ptrTmpFieldConv = new VectorFieldType( pImInfo->pIm );

    m_ptrTmpImage = new VectorImageType( pImInfo->pIm );

    // storage for the back map and the adjoint
    m_ptrCurrentBackMap = new VectorFieldType( pImInfo->pIm );
    m_ptrMapIdentity = new VectorFieldType( pImInfo->pIm );
    m_ptrMapIncremental = new VectorFieldType( pImInfo->pIm );

    m_ptrCurrentFinalAdjoint = new VectorImageType( pImInfo->pIm );
    m_ptrWarpedFinalToInitialAdjoint = new VectorImageType( pImInfo->pIm );

    // augmented Lagrangian
    m_ptrImageLagrangianMultiplier = new VectorImageType( pImInfo->pIm );

}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* pState )
{
    DeleteData();

    CreateTimeDiscretization();

    ShallowCopyStateStructures( pState );

    CreateGradientAndAuxiliaryStructures();

}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
{
    DeleteData();

    assert (this->m_pGradient == NULL );

    CreateTimeDiscretization();

    // allocate state structures
    CreateNewStateStructures();

    // gradient and everything else
    CreateGradientAndAuxiliaryStructures();
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::InitializeState()
{
  InitializeDataStructures();
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* pState)
{
  InitializeDataStructuresFromState( pState );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->multElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // This is more complicated than for the standard models, because we have an appearance change here
  // required an integration forward in time
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromToAndImageAtFinalTime( NULL, ptrIm, dTimeFrom, dTime );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{
  GetMapFromToAndImageAtFinalTime( ptrMap, NULL, dTimeFrom, dTimeTo );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetMapFromToAndImageAtFinalTime( VectorFieldType* ptrMap, VectorImageType* ptrIm, T dTimeFrom, T dTimeTo )
{
   /**
    * Solves the EPDiff equation for metamorphosis forward in time using a map-based approach to compute the map between two time-points
    *
    * \f$ I_t + \nabla I^T v = p/rho, \f$
    * \f$ p_t + div( p v ) = 0, \f$
    * \f$ v = -K*(p\nabla I) \f$
    *
    */

  assert( dTimeTo >= dTimeFrom );
  std::cout << "Computing map from " << dTimeFrom << " to " << dTimeTo << std::endl;

  if ( dTimeFrom < m_vecTimeDiscretization[0].dTime || dTimeTo > m_vecTimeDiscretization.back().dTime )
    {
    throw std::runtime_error("Requested map outside of valid time range.");
    return;
    }

  if ( ptrMap != NULL )
  {
    LDDMMUtils< T, TState::VImageDimension >::identityMap( ptrMap );
  }

  // identity map if the time points are the same
  if ( dTimeFrom == dTimeTo )
  {
    if ( ptrIm != NULL )
    {
      ptrIm->copy( this->m_pState->GetPointerToInitialImage() );
    }
    return;
  }

  // create additional maps to hold the solution
  VectorFieldType* ptrMapIn = new VectorFieldType( m_ptrMapTmp );
  VectorFieldType* ptrMapTmp = new VectorFieldType( m_ptrMapTmp );
  VectorFieldType* ptrMapOut = new VectorFieldType( m_ptrMapTmp );
  VectorFieldType* ptrMapIncremental = new VectorFieldType( m_ptrMapTmp );
  VectorFieldType* ptrMapIdentity = new VectorFieldType( m_ptrMapTmp );
  VectorFieldType* ptrCurrentVelocity = new VectorFieldType( m_ptrMapTmp );

  VectorImageType* ptrDeterminantOfJacobian = new VectorImageType( m_ptrTmpImage, 0.0, 1 );
  VectorImageType* ptrTmpImage = new VectorImageType( m_ptrTmpImage );

  // create two new images to hold the image and the adjoint
  VectorImageType* ptrCurrentP = new VectorImageType( m_ptrCurrentP );
  VectorImageType* ptrCurrentI = new VectorImageType( m_ptrCurrentI );

  // get the map between the two timepoints
  LDDMMUtils< T, TState::VImageDimension>::identityMap( ptrMapIn );
  LDDMMUtils< T, TState::VImageDimension>::identityMap( ptrMapIdentity );

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  ptrCurrentI->copy( ptrInitialImage );
  ptrCurrentP->copy( ptrInitialMomentum );

  // first integrate all the way to the start

  bool bInitializedMap = false;
  bool bReachedLastTimeInterval = false;

  if ( ptrMap != NULL )
  {
    LDDMMUtils< T, TState::VImageDimension >::identityMap( ptrMap );
  }

  T dTimeEvolvedFor = 0;

  // the desired time is larger than the initial timepoint (so we can integrate forward)
  // TODO: also support backward integration

  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {

    T dCurrentDT;

    if ( m_vecTimeDiscretization[ iI+1 ].dTime >= dTimeTo )
    {
      dCurrentDT = dTimeTo-m_vecTimeDiscretization[ iI ].dTime;
      bReachedLastTimeInterval = true;
    }
    else
    {
      dCurrentDT = this->m_vecTimeIncrements[ iI ];
    }

    ComputeVelocity( ptrCurrentI, ptrCurrentP, ptrCurrentVelocity, ptrMapOut );

    // incremental map
    this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMapIdentity, ptrMapIncremental, ptrMapTmp, dCurrentDT );
    // full map
    LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapIncremental, ptrMapIn, ptrMapOut );

    dTimeEvolvedFor += dCurrentDT;

    // if output map was initialized then also propagate it for this time interval
    if ( bInitializedMap )
    {
      ptrMapTmp->copy( ptrMap );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapIncremental, ptrMapTmp, ptrMap );
    }

    // add p/rho*dt to I
    ptrTmpImage->copy( ptrCurrentP );
    ptrTmpImage->multConst( ( dCurrentDT )/m_Rho );
    ptrTmpImage->addCellwise( ptrCurrentI );

    LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapIncremental, ptrTmpImage, ptrCurrentI );
    LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapOut, ptrInitialMomentum, ptrCurrentP );

    LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( ptrMapOut, ptrDeterminantOfJacobian );
    ptrCurrentP->multElementwise( ptrDeterminantOfJacobian );

    ptrMapIn->copy( ptrMapOut );

    if ( ( m_vecTimeDiscretization[ iI+1 ].dTime > dTimeFrom ) && !bInitializedMap && ( ptrMap!=NULL ) )
    {
      T dCurrentMapDT;
      dCurrentMapDT = dTimeFrom - m_vecTimeDiscretization[ iI ].dTime;
      this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMapIdentity, ptrMap, ptrMapTmp, dCurrentMapDT );
      bInitializedMap = true;
    }

    if ( bReachedLastTimeInterval )
    {
      break; // exit from the loop, all work was done.
    }

  }

  if ( ptrIm != NULL )
  {
    ptrIm->copy( ptrCurrentI );
  }

  delete ptrMapOut;
  delete ptrMapTmp;
  delete ptrMapIn;
  delete ptrMapIncremental;
  delete ptrMapIdentity;
  delete ptrCurrentVelocity;

  delete ptrTmpImage;
  delete ptrDeterminantOfJacobian;

  delete ptrCurrentI;
  delete ptrCurrentP;

  std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

}

template< class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    unsigned int uiNrOfMeasurements = CALATK::LDDMMUtils< T, TState::VImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );

    if ( uiNrOfMeasurements != 2 )
    {
      throw std::runtime_error( "CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction only supports two measurements (one source and one target image" );
      return;
    }

    CALATK::LDDMMUtils< T, TState::VImageDimension >::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, this->m_NumberOfDiscretizationVolumesPerUnitTime );

    // now add the weights, default weights are all constants here
    // TODO: make this more flexible to support local geodesic regression

    typename std::vector< STimePoint >::iterator iter;
    for ( iter = m_vecTimeDiscretization.begin(); iter != m_vecTimeDiscretization.end(); ++iter )
    {
      iter->vecWeights.clear();
      for ( unsigned int iI=0; iI<iter->vecMeasurementImages.size(); ++iI )
      {
        // TODO: make this at least a constant variable
        iter->vecWeights.push_back( 1.0/this->m_SigmaSqr );
      }
    }
    // the time discretization vector has all the N timepoint. There will be N-1 vector fields in between
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( VectorImageType* ptrWarpedFinalToInitialAdjoint )
{
  /**
    * Solves the EPDiff equation forward in time using a map-based approach
    * and computes the map, mapping to the initial condition
    *
    * \f$ I_t + \nabla I^T v = p/rho, \f$
    * \f$ p_t + div( p v ) = 0, \f$
    * \f$ v = -K*(p\nabla I) \f$
    *
    */

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIn );

  m_ptrCurrentI->copy( ptrInitialImage );
  m_ptrCurrentP->copy( ptrInitialMomentum );

  LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrCurrentBackMap );
  LDDMMUtils< T, TState::VImageDimension>::identityMap( m_ptrMapIdentity );


  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {
      ComputeVelocity( m_ptrCurrentI, m_ptrCurrentP, m_ptrCurrentVelocity, m_ptrMapOut );

      // incremental map
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      // full map
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrMapIn, m_ptrMapOut );

      // add p/rho*dt to I
      m_ptrTmpImage->copy( m_ptrCurrentP );
      m_ptrTmpImage->multConst( ( this->m_vecTimeIncrements[ iI ] )/m_Rho );
      m_ptrTmpImage->addCellwise( m_ptrCurrentI );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapIncremental, m_ptrTmpImage, m_ptrCurrentI );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialMomentum, m_ptrCurrentP );

      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
      m_ptrCurrentP->multElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->copy( m_ptrMapOut );

      // keep track of the backward map
      // negate the velocity field
      m_ptrCurrentVelocity->multConst( -1.0 );

      // m_ptrMapOut here is the incremental part of the map within this time-step
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      m_ptrMapTmp->copy( m_ptrCurrentBackMap );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, m_ptrMapIncremental, m_ptrCurrentBackMap );
  }

  // TODO: implement this for multiple time points

  // now we have the back map and can compute the adjoint in the initial frame by pulling it back

  // the final condition here is: p(1) = r-\mu(I(1)-I_1)

  // I_1
  m_ptrCurrentFinalAdjoint->copy( ptrI1 );
  // -I(1)
  m_ptrCurrentFinalAdjoint->addCellwiseMultiple( m_ptrCurrentI, -1.0 );
  // \mu(I_1-I(1))
  m_ptrCurrentFinalAdjoint->multConst( m_AugmentedLagrangianMu );
  // add r
  m_ptrCurrentFinalAdjoint->addCellwise( m_ptrImageLagrangianMultiplier );

  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrCurrentBackMap, m_ptrCurrentFinalAdjoint, ptrWarpedFinalToInitialAdjoint );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::ComputeGradient()
{
  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  /**
    * The gradient is now simply
    *
    \f[
      \nabla_{I_0(t_0)}E = 0 // not used, I(0) is fixed
    \f]
    *
    \f[
      \nabla_{p(t_0)}E = (-\lambda(0)+p(0))
    \f]
    */

  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  VectorImageType* ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
  VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

  ptrP0Gradient->copy( m_ptrWarpedFinalToInitialAdjoint );
  ptrP0Gradient->multConst(-1);

  ptrP0Gradient->addCellwise( ptrInitialMomentum );
  ptrI0Gradient->setConst( 0.0 );

}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
{

  std::cout << "WARNING: initial unsmooth gradient computation is NOT adapted for metamorphosis. Results may be inaccurate. FIXME." << std::endl;

  // compute the unsmoothed velocity gradient; to be used to estimate weights for the multi-Gaussian kernels.
  // this is an approximation based on adding the augmented Lagrangian constraint to the original model (rather than the 2nd order for shooting)
  // v, r and p(0) is assumed zero here and the unsmoothed gradient is then, because I(t) = I(0) and p(t) = p(1)

  // \f$ \nabla E = \sum_i \mu(I_0-I_1)\nabla I_0  \f$
  // sum is over dimensions

  unsigned int dim = ptrI0->getDim();

  // initialize to 0
  VectorFieldType* ptrCurrentGradient = ptrInitialUnsmoothedVelocityGradient;
  ptrCurrentGradient->setConst( 0 );

  // \mu(I_0-I_1)
  m_ptrTmpImage->copy( ptrI0 );
  m_ptrTmpImage->addCellwiseMultiple( ptrI1, -1.0 );
  m_ptrTmpImage->multConst( m_AugmentedLagrangianMu );

  for ( unsigned int iD = 0; iD<dim; ++iD )
    {
    VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrI0, iD, m_ptrTmpField );
    VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( m_ptrTmpImage, iD, m_ptrTmpField );
    ptrCurrentGradient->addCellwise( m_ptrTmpField );
    }
}

template < class TState >
typename CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CEnergyValues
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState>::GetCurrentEnergy()
{
  /**
    * Computes the energy for the simplified shooting method.
    * Since everything is determined by the initial condition and the geodesics are explicitly
    * enforced we can compute the energy just as for the full adjoint formulation
    *
    \f[
      E = 0.5 \langle p(t_0)\nabla I(t_0),K*(p(t_0)\nabla I(t_0)\rangle + 0.5 rho \langle p(t_0), p(t_0) \rangle -\langle r, I_1-I(1)\rangle + \frac{\mu}{2}\|I_1-I(1)\|^2
    \f]
    */

  T dEnergy = 0;

  // computing \f$ 0.5\langle p(t_0) \nabla I(t_0) +  K*( p(t_0)\nabla I(t_0) ) \rangle \f$
  // this is done dimension for dimension (i.e., if we have a multidimensional image, we have as many of these terms as we have dimensions)

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  unsigned int dim = ptrInitialImage->getDim();

  for ( unsigned int iD=0; iD<dim; iD++ )
  {
      // nabla I
      VectorFieldUtils< T, TState::VImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i\nabla I
      VectorImageUtils< T, TState::VImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel, K*(pi\nabla I)
      m_ptrTmpFieldConv->copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the inner product
      dEnergy += m_ptrTmpField->computeInnerProduct( m_ptrTmpFieldConv );
  }

  // now add the simply the squared norm of the initial momentum
  dEnergy += m_Rho*ptrInitialMomentum->computeSquareNorm();

  // multiply the full energy by 0.5
  unsigned int uiNrOfDiscretizationPoints = m_vecTimeDiscretization.size();
  T dTimeDuration = this->m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].dTime - this->m_vecTimeDiscretization[0].dTime;

  dEnergy *= 0.5*dTimeDuration;

  T dVelocitySquareNorm = dEnergy;

  // add the contributions of the data terms

  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  // computing I(1)-I_1
  m_ptrTmpImage->copy( ptrI1 );
  m_ptrTmpImage->multConst( -1.0 );
  m_ptrTmpImage->addCellwise( m_ptrCurrentI );

  T dImageNorm = m_ptrTmpImage->computeSquareNorm();

  // +\mu/2\|I(1)-I_1\|^2
  T dAugmentedLagrangianNorm = 0.5*m_AugmentedLagrangianMu*dImageNorm;

  // -<r,I(1)-I_1>
  dAugmentedLagrangianNorm -= m_ptrTmpImage->computeInnerProduct( m_ptrImageLagrangianMultiplier );

  dEnergy += dAugmentedLagrangianNorm;

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  return energyValues;
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  std::cout << "Saving state information at iteration " << uiIter << std::endl;

  outputPrefix = outputPrefix + "Shooting-";

  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();
  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();

  VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

  ptrP0Gradient->copy( ptrInitialMomentum );
  ptrP0Gradient->multConst(-1);

  ptrP0Gradient->addCellwise( m_ptrWarpedFinalToInitialAdjoint );

  std::string suffix = "-iter-" + CreateIntegerString( uiIter, 3 ) + ".nrrd";

  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrWarpedFinalToInitialAdjoint, outputPrefix + "warpedLam" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrCurrentFinalAdjoint, outputPrefix + "Lam1" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrCurrentBackMap, outputPrefix + "backMap" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrMapIn, outputPrefix + "fwdMap" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrInitialMomentum, outputPrefix + "p0" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrInitialImage, outputPrefix + "sI0" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrP0Gradient, outputPrefix + "gradp0" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrI0, outputPrefix + "I0" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( ptrI1, outputPrefix + "I1" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrCurrentI, outputPrefix + "wI0" + suffix );
  VectorImageUtils< T, TState::VImageDimension >::writeFileITK( m_ptrCurrentP, outputPrefix + "wP0" + suffix );

}

/// functionality for augmented Lagrangian
template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::SetSquaredPenaltyScalarWeight( T dWeight )
{
  m_AugmentedLagrangianMu = dWeight;
}

template < class TState >
typename TState::TFloat
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetSquaredPenaltyScalarWeight()
{
  return m_AugmentedLagrangianMu;
}

template < class TState >
typename CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::VectorImageType*
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetPointerToImageLagrangianMultiplier()
{
  return m_ptrImageLagrangianMultiplier;
}

template < class TState >
const typename CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::VectorImageType*
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetPointerToCurrentImageResidual()
{
  // TODO: extend to support multiple time-points (how?)
  // just look at the last time point for now here
  unsigned int uiNrOfTimePoints= m_vecTimeDiscretization.size();
  unsigned int uiNrOfMeasuredImagesAtFinalTimePoint = m_vecTimeDiscretization[ uiNrOfTimePoints - 1].vecMeasurementImages.size();

  // for now we can only deal with one image (to be fixed)
  assert( uiNrOfMeasuredImagesAtFinalTimePoint==1 );

  // TODO: may not be necessary to recompute this here
  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  // computing I(1)-I_1
  m_ptrTmpImage->copy( ptrI1 );
  m_ptrTmpImage->multConst( -1.0 );
  m_ptrTmpImage->addCellwise( m_ptrCurrentI );

  return m_ptrTmpImage;

}


#endif

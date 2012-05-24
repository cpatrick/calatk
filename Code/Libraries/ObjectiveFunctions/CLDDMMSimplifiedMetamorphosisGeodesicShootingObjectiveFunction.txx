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

  ptrI0 = NULL;
  ptrI1 = NULL;

  m_vecMeasurementTimepoints.clear();
  m_vecTimeDiscretization.clear();
  m_vecTimeIncrements.clear();

  this->m_ptrState = NULL;
  this->m_ptrGradient = NULL;
}

template < class TState >
CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::~CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction()
{
  DeleteData();
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "SimplifiedMetamorphosis", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "SimplifiedMetamorphosis", Json::nullValue );

  SetJSONHelpForRootKey( GrowthModel, "settings for the simplified metamorphosis model" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Rho );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Rho,
                     "rho is the weight of the appearance change penalty rho|q|^2" );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
{
  assert( this->m_ptrState.GetPointer() == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // obtain image from which to graft the image information for the data structures

  ImageInformation* pImInfo;
  // get information from the first image to figure out the dimensions and determine the source and target image
  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

  this->m_ptrState = new TState( pImInfo->Image );
  this->m_ptrState->GetPointerToInitialMomentum()->SetToConstant( 0 );
}

template< class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState* ptrState )
{
  assert ( this->m_ptrState.GetPointer() == NULL );
  this->m_ptrState = ptrState;
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
  ImageInformation* pImInfo;
  // get information from the first image to figure out the dimensions and determine the source and target image
  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );
  ptrI0 = pImInfo->Image;

  this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 1 );
  ptrI1 = pImInfo->Image;

  // create the gradient
  this->m_ptrGradient = new TState( pImInfo->Image );
  this->m_ptrGradient->GetPointerToInitialImage()->SetToConstant( 0 );
  this->m_ptrGradient->GetPointerToInitialMomentum()->SetToConstant( 0 );

  // storage for the maps

  m_ptrMapIn = new VectorFieldType( pImInfo->Image );
  m_ptrMapOut = new VectorFieldType( pImInfo->Image );
  m_ptrMapTmp = new VectorFieldType( pImInfo->Image );

  // storage for the determinant of the Jacobian
  m_ptrDeterminantOfJacobian = new VectorImageType( pImInfo->Image, 0.0, 1 );

  // storage for current state
  m_ptrCurrentI = new VectorImageType( pImInfo->Image );
  m_ptrCurrentP = new VectorImageType( pImInfo->Image );
  m_ptrCurrentVelocity = new VectorFieldType( pImInfo->Image );

  // temporary storage
  m_ptrTmpField = new VectorFieldType( pImInfo->Image );
  m_ptrTmpFieldConv = new VectorFieldType( pImInfo->Image );

  m_ptrTmpImage = new VectorImageType( pImInfo->Image );

  // storage for the back map and the adjoint
  m_ptrCurrentBackMap = new VectorFieldType( pImInfo->Image );
  m_ptrMapIdentity = new VectorFieldType( pImInfo->Image );
  m_ptrMapIncremental = new VectorFieldType( pImInfo->Image );

  m_ptrCurrentFinalAdjoint = new VectorImageType( pImInfo->Image );
  m_ptrWarpedFinalToInitialAdjoint = new VectorImageType( pImInfo->Image );

  // augmented Lagrangian
  m_ptrImageLagrangianMultiplier = new VectorImageType( pImInfo->Image );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* ptrState )
{
  DeleteData();

  CreateTimeDiscretization();

  ShallowCopyStateStructures( ptrState );

  CreateGradientAndAuxiliaryStructures();
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
{
  DeleteData();

  assert ( this->m_ptrGradient.GetPointer() == NULL );

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
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* ptrState)
{
  InitializeDataStructuresFromState( ptrState );
}

template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();
  LDDMMUtilsType::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );

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
    LDDMMUtilsType::identityMap( ptrMap );
  }

  // identity map if the time points are the same
  if ( dTimeFrom == dTimeTo )
  {
    if ( ptrIm != NULL )
    {
      ptrIm->Copy( this->m_ptrState->GetPointerToInitialImage() );
    }
    return;
  }

  // create additional maps to hold the solution
  typename VectorFieldType::Pointer ptrMapIn = new VectorFieldType( m_ptrMapTmp );
  typename VectorFieldType::Pointer ptrMapTmp = new VectorFieldType( m_ptrMapTmp );
  typename VectorFieldType::Pointer ptrMapOut = new VectorFieldType( m_ptrMapTmp );
  typename VectorFieldType::Pointer ptrMapIncremental = new VectorFieldType( m_ptrMapTmp );
  typename VectorFieldType::Pointer ptrMapIdentity = new VectorFieldType( m_ptrMapTmp );
  typename VectorFieldType::Pointer ptrCurrentVelocity = new VectorFieldType( m_ptrMapTmp );

  typename VectorImageType::Pointer ptrDeterminantOfJacobian = new VectorImageType( m_ptrTmpImage, 0.0, 1 );
  typename VectorImageType::Pointer ptrTmpImage = new VectorImageType( m_ptrTmpImage );

  // create two new images to hold the image and the adjoint
  typename VectorImageType::Pointer ptrCurrentP = new VectorImageType( m_ptrCurrentP );
  typename VectorImageType::Pointer ptrCurrentI = new VectorImageType( m_ptrCurrentI );

  // get the map between the two timepoints
  LDDMMUtilsType::identityMap( ptrMapIn );
  LDDMMUtilsType::identityMap( ptrMapIdentity );

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  ptrCurrentI->Copy( ptrInitialImage );
  ptrCurrentP->Copy( ptrInitialMomentum );

  // first integrate all the way to the start

  bool bInitializedMap = false;
  bool bReachedLastTimeInterval = false;

  if ( ptrMap != NULL )
  {
    LDDMMUtilsType::identityMap( ptrMap );
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

    this->ComputeVelocity( ptrCurrentI, ptrCurrentP, ptrCurrentVelocity, ptrMapOut );

    // incremental map
    this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMapIdentity, ptrMapIncremental, ptrMapTmp, dCurrentDT );
    // full map
    LDDMMUtilsType::applyMap( ptrMapIncremental, ptrMapIn, ptrMapOut );

    dTimeEvolvedFor += dCurrentDT;

    // if output map was initialized then also propagate it for this time interval
    if ( bInitializedMap )
    {
      ptrMapTmp->Copy( ptrMap );
      LDDMMUtilsType::applyMap( ptrMapIncremental, ptrMapTmp, ptrMap );
    }

    // add p/rho*dt to I
    ptrTmpImage->Copy( ptrCurrentP );
    ptrTmpImage->MultiplyByConstant( ( dCurrentDT )/m_Rho );
    ptrTmpImage->AddCellwise( ptrCurrentI );

    LDDMMUtilsType::applyMap( ptrMapIncremental, ptrTmpImage, ptrCurrentI );
    LDDMMUtilsType::applyMap( ptrMapOut, ptrInitialMomentum, ptrCurrentP );

    LDDMMUtilsType::computeDeterminantOfJacobian( ptrMapOut, ptrDeterminantOfJacobian );
    ptrCurrentP->MultiplyElementwise( ptrDeterminantOfJacobian );

    ptrMapIn->Copy( ptrMapOut );

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
    ptrIm->Copy( ptrCurrentI );
  }

  std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

}

template< class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    unsigned int uiNrOfMeasurements = CALATK::LDDMMUtils< T, TState::ImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );

    if ( uiNrOfMeasurements != 2 )
    {
      throw std::runtime_error( "CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction only supports two measurements (one source and one target image" );
      return;
    }

    CALATK::LDDMMUtils< T, TState::ImageDimension >::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, this->m_NumberOfDiscretizationVolumesPerUnitTime );

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

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  LDDMMUtilsType::identityMap( m_ptrMapIn );

  m_ptrCurrentI->Copy( ptrInitialImage );
  m_ptrCurrentP->Copy( ptrInitialMomentum );

  LDDMMUtilsType::identityMap( m_ptrCurrentBackMap );
  LDDMMUtilsType::identityMap( m_ptrMapIdentity );


  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {
      this->ComputeVelocity( m_ptrCurrentI, m_ptrCurrentP, m_ptrCurrentVelocity, m_ptrMapOut );

      // incremental map
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      // full map
      LDDMMUtilsType::applyMap( m_ptrMapIncremental, m_ptrMapIn, m_ptrMapOut );

      // add p/rho*dt to I
      m_ptrTmpImage->Copy( m_ptrCurrentP );
      m_ptrTmpImage->MultiplyByConstant( ( this->m_vecTimeIncrements[ iI ] )/m_Rho );
      m_ptrTmpImage->AddCellwise( m_ptrCurrentI );

      LDDMMUtilsType::applyMap( m_ptrMapIncremental, m_ptrTmpImage, m_ptrCurrentI );
      LDDMMUtilsType::applyMap( m_ptrMapOut, ptrInitialMomentum, m_ptrCurrentP );

      LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
      m_ptrCurrentP->MultiplyElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->Copy( m_ptrMapOut );

      // keep track of the backward map
      // negate the velocity field
      m_ptrCurrentVelocity->MultiplyByConstant( -1.0 );

      // m_ptrMapOut here is the incremental part of the map within this time-step
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapIncremental, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      m_ptrMapTmp->Copy( m_ptrCurrentBackMap );
      LDDMMUtilsType::applyMap( m_ptrMapTmp, m_ptrMapIncremental, m_ptrCurrentBackMap );
  }

  // TODO: implement this for multiple time points

  // now we have the back map and can compute the adjoint in the initial frame by pulling it back

  // the final condition here is: p(1) = r-\mu(I(1)-I_1)

  // I_1
  m_ptrCurrentFinalAdjoint->Copy( ptrI1 );
  // -I(1)
  m_ptrCurrentFinalAdjoint->AddCellwiseMultiply( m_ptrCurrentI, -1.0 );
  // \mu(I_1-I(1))
  m_ptrCurrentFinalAdjoint->MultiplyByConstant( m_AugmentedLagrangianMu );
  // add r
  m_ptrCurrentFinalAdjoint->AddCellwise( m_ptrImageLagrangianMultiplier );

  LDDMMUtilsType::applyMap( m_ptrCurrentBackMap, m_ptrCurrentFinalAdjoint, ptrWarpedFinalToInitialAdjoint );
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

  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  VectorImageType* ptrI0Gradient = this->m_ptrGradient->GetPointerToInitialImage();
  VectorImageType* ptrP0Gradient = this->m_ptrGradient->GetPointerToInitialMomentum();

  ptrP0Gradient->Copy( m_ptrWarpedFinalToInitialAdjoint );
  ptrP0Gradient->MultiplyByConstant(-1);

  ptrP0Gradient->AddCellwise( ptrInitialMomentum );
  ptrI0Gradient->SetToConstant( 0.0 );

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

  unsigned int dim = ptrI0->GetDimension();

  // initialize to 0
  VectorFieldType* ptrCurrentGradient = ptrInitialUnsmoothedVelocityGradient;
  ptrCurrentGradient->SetToConstant( 0 );

  // \mu(I_0-I_1)
  m_ptrTmpImage->Copy( ptrI0 );
  m_ptrTmpImage->AddCellwiseMultiply( ptrI1, -1.0 );
  m_ptrTmpImage->MultiplyByConstant( m_AugmentedLagrangianMu );

  for ( unsigned int iD = 0; iD<dim; ++iD )
    {
    VectorFieldUtilsType::computeCentralGradient( ptrI0, iD, m_ptrTmpField );
    VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrTmpImage, iD, m_ptrTmpField );
    ptrCurrentGradient->AddCellwise( m_ptrTmpField );
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

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  unsigned int dim = ptrInitialImage->GetDimension();

  for ( unsigned int iD=0; iD<dim; iD++ )
  {
      // nabla I
      VectorFieldUtilsType::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i\nabla I
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel, K*(pi\nabla I)
      m_ptrTmpFieldConv->Copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the inner product
      dEnergy += m_ptrTmpField->ComputeInnerProduct( m_ptrTmpFieldConv );
  }

  // now add the simply the squared norm of the initial momentum
  dEnergy += m_Rho*ptrInitialMomentum->ComputeSquaredNorm();

  // multiply the full energy by 0.5
  unsigned int uiNrOfDiscretizationPoints = m_vecTimeDiscretization.size();
  T dTimeDuration = this->m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].dTime - this->m_vecTimeDiscretization[0].dTime;

  dEnergy *= 0.5*dTimeDuration;

  T dVelocitySquareNorm = dEnergy;

  // add the contributions of the data terms

  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  // computing I(1)-I_1
  m_ptrTmpImage->Copy( ptrI1 );
  m_ptrTmpImage->MultiplyByConstant( -1.0 );
  m_ptrTmpImage->AddCellwise( m_ptrCurrentI );

  T dImageNorm = m_ptrTmpImage->ComputeSquaredNorm();

  // +\mu/2\|I(1)-I_1\|^2
  T dAugmentedLagrangianNorm = 0.5*m_AugmentedLagrangianMu*dImageNorm;

  // -<r,I(1)-I_1>
  dAugmentedLagrangianNorm -= m_ptrTmpImage->ComputeInnerProduct( m_ptrImageLagrangianMultiplier );

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

  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();
  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();

  VectorImageType* ptrP0Gradient = this->m_ptrGradient->GetPointerToInitialMomentum();

  ptrP0Gradient->Copy( ptrInitialMomentum );
  ptrP0Gradient->MultiplyByConstant(-1);

  ptrP0Gradient->AddCellwise( m_ptrWarpedFinalToInitialAdjoint );

  std::string suffix = "-iter-" + CreateIntegerString( uiIter, 3 ) + ".nrrd";

  VectorImageUtilsType::writeFileITK( m_ptrWarpedFinalToInitialAdjoint, outputPrefix + "warpedLam" + suffix );
  VectorImageUtilsType::writeFileITK( m_ptrCurrentFinalAdjoint, outputPrefix + "Lam1" + suffix );
  VectorImageUtilsType::writeFileITK( m_ptrCurrentBackMap, outputPrefix + "backMap" + suffix );
  VectorImageUtilsType::writeFileITK( m_ptrMapIn, outputPrefix + "fwdMap" + suffix );
  VectorImageUtilsType::writeFileITK( ptrInitialMomentum, outputPrefix + "p0" + suffix );
  VectorImageUtilsType::writeFileITK( ptrInitialImage, outputPrefix + "sI0" + suffix );
  VectorImageUtilsType::writeFileITK( ptrP0Gradient, outputPrefix + "gradp0" + suffix );
  VectorImageUtilsType::writeFileITK( ptrI0, outputPrefix + "I0" + suffix );
  VectorImageUtilsType::writeFileITK( ptrI1, outputPrefix + "I1" + suffix );
  VectorImageUtilsType::writeFileITK( m_ptrCurrentI, outputPrefix + "wI0" + suffix );
  VectorImageUtilsType::writeFileITK( m_ptrCurrentP, outputPrefix + "wP0" + suffix );

}

/// functionality for augmented Lagrangian
template < class TState >
void CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::SetSquaredPenaltyScalarWeight( T dWeight )
{
  m_AugmentedLagrangianMu = dWeight;
}

template < class TState >
typename CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction< TState >::FloatType
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
  m_ptrTmpImage->Copy( ptrI1 );
  m_ptrTmpImage->MultiplyByConstant( -1.0 );
  m_ptrTmpImage->AddCellwise( m_ptrCurrentI );

  return m_ptrTmpImage;

}


#endif

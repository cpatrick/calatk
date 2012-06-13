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

#ifndef C_LDDMM_SIMPIFIED_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_SIMPIFIED_GEODESIC_SHOOTING_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::CLDDMMSimplifiedGeodesicShootingObjectiveFunction()
{
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

  m_ptrCurrentBackMap = NULL;
  m_ptrMapIdentity = NULL;

  m_ptrCurrentFinalAdjoint = NULL;
  m_ptrWarpedFinalToInitialAdjoint = NULL;

  // convencience pointers to the source and the target image respectively
  ptrI0 = NULL;
  ptrI1 = NULL;

}

template< class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::DeleteData()
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
CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::~CLDDMMSimplifiedGeodesicShootingObjectiveFunction()
{
  this->DeleteData();
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
{
  assert( this->m_ptrState.GetPointer() == NULL );
  assert( m_vecTimeDiscretization.size() > 1 );

  // get the subject ids
  std::vector< int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // obtain image from which to graft the image information for the data structures

  // get information from the first image to figure out the dimensions and determine the source and target image
  this->m_ptrState = new TState( this->m_ptrImageManager->GetGraftImagePointer( this->GetActiveSubjectId() ) );
  this->m_ptrState->GetPointerToInitialMomentum()->SetToConstant( 0 );
}

template< class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState* ptrState )
{
  assert ( this->m_ptrState.GetPointer() == NULL );  
  this->m_ptrState = ptrState;
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
{

    // get the subject ids
    std::vector< int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()==1 );

    // obtain image from which to graft the image information for the data structures
    // and assign the convenience image pointer ptrI0, ptrI1
    std::vector< TimeSeriesDataPointType > timeseries;
    this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeseries, this->GetActiveSubjectId() );

    if ( timeseries.size()!=2 )
    {
      throw std::runtime_error( "Time series should contain exactly two images." );
      return;
    }

    // get information from the first image to figure out the dimensions and determine the source and target image
    ptrI0 = timeseries[0].GetImage();
    ptrI1 = timeseries[1].GetImage();

    // use the first image here to graft from, but any other image would do
    const VectorImageType* graftImage = ptrI0;

    // create the gradient
    this->m_ptrGradient = new TState( graftImage );
    this->m_ptrGradient->GetPointerToInitialImage()->SetToConstant( 0 );
    this->m_ptrGradient->GetPointerToInitialMomentum()->SetToConstant( 0 );

    // storage for the maps

    m_ptrMapIn = new VectorFieldType( graftImage );
    m_ptrMapOut = new VectorFieldType( graftImage );
    m_ptrMapTmp = new VectorFieldType( graftImage );

    // storage for the determinant of the Jacobian (is a scalar)
    m_ptrDeterminantOfJacobian = new VectorImageType( graftImage, 0.0, 1 );

    // storage for current state
    m_ptrCurrentI = new VectorImageType( graftImage );
    m_ptrCurrentP = new VectorImageType( graftImage );
    m_ptrCurrentVelocity = new VectorFieldType( graftImage );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( graftImage );
    m_ptrTmpFieldConv = new VectorFieldType( graftImage );

    // storage for the back map and the adjoint
    m_ptrCurrentBackMap = new VectorFieldType( graftImage );
    m_ptrMapIdentity = new VectorFieldType( graftImage );

    m_ptrCurrentFinalAdjoint = new VectorImageType( graftImage );
    m_ptrWarpedFinalToInitialAdjoint = new VectorImageType( graftImage );

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* ptrState )
{
    DeleteData();

    CreateTimeDiscretization();

    ShallowCopyStateStructures( ptrState );

    CreateGradientAndAuxiliaryStructures();

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
{
    DeleteData();

    assert (this->m_ptrGradient.GetPointer() == NULL );

    CreateTimeDiscretization();

    // allocate state structures
    CreateNewStateStructures();

    // gradient and everything else
    CreateGradientAndAuxiliaryStructures();
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeState()
{
  InitializeDataStructures();
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* ptrState)
{
  InitializeDataStructuresFromState( ptrState );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  T dTimeFrom = m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{
   /**
    * Solves the EPDiff equation forward in time using a map-based approach to compute the map between two time-points
    *
    * \f$ I_t + \nabla I^T v = 0, \f$
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

  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( ptrMap );
  // identity map if the time points are the same
  if ( dTimeFrom == dTimeTo )
  {
    return;
  }

  // create four additional maps to hold the solution
  typename VectorFieldType::Pointer ptrMapIn = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrMapTmp = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrMapOut = new VectorFieldType( ptrMap );
  typename VectorFieldType::Pointer ptrCurrentVelocity = new VectorFieldType( ptrMap );

  // create two new images to hold the image and the adjoint
  typename VectorImageType::Pointer ptrCurrentP = new VectorImageType( m_ptrCurrentP );
  typename VectorImageType::Pointer ptrCurrentI = new VectorImageType( m_ptrCurrentI );

  // get the map between the two timepoints
  LDDMMUtilsType::identityMap( ptrMapIn );

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  ptrCurrentI->Copy( ptrInitialImage );
  ptrCurrentP->Copy( ptrInitialMomentum );

  T dCurrentTime = m_vecTimeDiscretization[0].dTime;

  // first integrate all the way to the start

  bool bInitializedMap = false;

  T dTimeEvolvedFor = 0;

  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {

    std::cout << "current time = " << dCurrentTime << std::endl;

    T dCurrentDT = m_vecTimeDiscretization[ iI+1 ].dTime - m_vecTimeDiscretization[ iI ].dTime;

    this->ComputeVelocity( ptrCurrentI, ptrCurrentP, ptrCurrentVelocity, ptrMapOut );
    std::cout << "evolving overall map for " << m_vecTimeIncrements[ iI ] << std::endl;
    this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMapIn, ptrMapOut, ptrMapTmp, m_vecTimeIncrements[ iI ] );

    LDDMMUtilsType::applyMap( ptrMapOut, ptrInitialImage, ptrCurrentI );
    LDDMMUtilsType::applyMap( ptrMapOut, ptrInitialMomentum, ptrCurrentP );

    LDDMMUtilsType::computeDeterminantOfJacobian( ptrMapOut, m_ptrDeterminantOfJacobian );

    ptrCurrentP->MultiplyElementwise( m_ptrDeterminantOfJacobian );

    ptrMapIn->Copy( ptrMapOut );

    if ( bInitializedMap )
    {
      if ( dCurrentTime + dCurrentDT >= dTimeTo )
      {
        // the full map is in between this time interval so just integrate it for a little bit
        std::cout << "1: evolving map for " << dTimeTo - dCurrentTime << std::endl;
        dTimeEvolvedFor += dTimeTo - dCurrentTime;
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, dTimeTo - dCurrentTime );
        ptrMap->Copy( ptrMapOut );

        std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

        return; // done because everything was in this interval
      }
      else
      {
         // integrate it for the full interval
        std::cout << "2: evolving map for " << m_vecTimeIncrements[ iI ] << std::endl;
        dTimeEvolvedFor += m_vecTimeIncrements[ iI ];
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, m_vecTimeIncrements[ iI ] );
        ptrMap->Copy( ptrMapOut );
      }
    }

    // determine if we need to perform a partial step for the map to be computed
    if ( dCurrentTime + dCurrentDT > dTimeFrom && !bInitializedMap )
    {
      if ( dCurrentTime + dCurrentDT > dTimeTo )
      {
        // the full map is in between this time interval so just integrate it for a little bit
        std::cout << "3: evolving map for " << dTimeTo - dTimeFrom << std::endl;
        dTimeEvolvedFor += dTimeTo = dTimeFrom;
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, dTimeTo - dTimeFrom );
        ptrMap->Copy( ptrMapOut );

        std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

        return; // done because everything was in this interval
      }
      else
      {
        // integrate the map until the end of this interval
        std::cout << "4: evolving map for " << dCurrentDT << std::endl;
        dTimeEvolvedFor += dCurrentDT;
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, dCurrentDT );
        ptrMap->Copy( ptrMapOut );
        bInitializedMap = true;
      }

    }

    dCurrentTime += dCurrentDT;

  }

  std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

}

template< class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::CreateTimeDiscretization()
{
    if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
    unsigned int uiNrOfMeasurements = LDDMMUtilsType::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, this->GetActiveSubjectId(), vecTimePointData );

    if ( uiNrOfMeasurements != 2 )
    {
      throw std::runtime_error( "CLDDMMSimplifiedGeodesicShootingObjectiveFunction only supports two measurements (one source and one target image" );
      return;
    }

    LDDMMUtilsType::CreateTimeDiscretization( vecTimePointData, m_vecTimeDiscretization, m_vecTimeIncrements, this->m_NumberOfDiscretizationVolumesPerUnitTime );

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
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( VectorImageType* ptrWarpedFinalToInitialAdjoint )
{
  /**
    * Solves the EPDiff equation forward in time using a map-based approach
    * and computes the map, mapping to the initial condition
    *
    * \f$ I_t + \nabla I^T v = 0, \f$
    * \f$ p_t + div( p v ) = 0, \f$
    * \f$ v = -K*(p\nabla I) \f$
    *
    */

  VectorImageType* ptrInitialImage = this->m_ptrState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_ptrState->GetPointerToInitialMomentum();

  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( m_ptrMapIn );

  m_ptrCurrentI->Copy( ptrInitialImage );
  m_ptrCurrentP->Copy( ptrInitialMomentum );

  LDDMMUtilsType::identityMap( m_ptrCurrentBackMap );
  LDDMMUtilsType::identityMap( m_ptrMapIdentity );


  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {
      this->ComputeVelocity( m_ptrCurrentI, m_ptrCurrentP, m_ptrCurrentVelocity, m_ptrMapOut );

      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      LDDMMUtilsType::applyMap( m_ptrMapOut, ptrInitialImage, m_ptrCurrentI );
      LDDMMUtilsType::applyMap( m_ptrMapOut, ptrInitialMomentum, m_ptrCurrentP );

      LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );

      m_ptrCurrentP->MultiplyElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->Copy( m_ptrMapOut );

      // keep track of the backward map
      // negate the velocity field
      m_ptrCurrentVelocity->MultiplyByConstant( -1.0 );

      // m_ptrMapOut here is the incremental part of the map within this time-step
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      m_ptrMapTmp->Copy( m_ptrCurrentBackMap );
      LDDMMUtilsType::applyMap( m_ptrMapTmp, m_ptrMapOut, m_ptrCurrentBackMap );
  }

  // TODO: implement this for multiple time points

  // now we have the back map and can compute the adjoint in the initial frame by pulling it back

  unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();

  this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentFinalAdjoint, m_ptrCurrentI, ptrI1 );
  m_ptrCurrentFinalAdjoint->MultiplyByConstant( m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecWeights[ 0 ] );

  LDDMMUtilsType::applyMap( m_ptrCurrentBackMap, m_ptrCurrentFinalAdjoint, ptrWarpedFinalToInitialAdjoint );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::ComputeGradient()
{
  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  /**
    * The gradient is now simply
    *
    \f[
      \nabla_{I_0(t_0)}E = -\lambda(0) -\frac{1}{\sigma^2}\nabla d^2(I(0),I_0)
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

  if ( this->m_EstimateInitialImage )
  {
    ptrI0Gradient->Copy( m_ptrWarpedFinalToInitialAdjoint );
    ptrI0Gradient->MultiplyByConstant(-1);
  }
  else
  {
    ptrI0Gradient->SetToConstant( 0.0 );
  }
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
{
  // compute the unsmoothed velocity gradient; to be used to estimate weights for the multi-Gaussian kernels.
  // v and p(0) is assumed zero here and the unsmoothed gradient is then
  // \f$ \sum_i -\lambda_i(0)\nabla I_0 \f$
  // where the \lambda_i(0) is the warped adjoint from the final state warped to the initial image

  unsigned int dim = ptrI0->GetDimension();

  unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();

  typename VectorImageType::Pointer ptrCurrentAdjointDifference = new VectorImageType( ptrI0 );

  this->m_ptrMetric->GetAdjointMatchingDifferenceImage( ptrCurrentAdjointDifference, ptrI0, ptrI1 );
  ptrCurrentAdjointDifference->MultiplyByConstant( m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecWeights[ 0 ] );

  // initialize to 0
  VectorFieldType* ptrCurrentGradient = ptrInitialUnsmoothedVelocityGradient;
  ptrCurrentGradient->SetToConstant( 0 );

  for ( unsigned int iD = 0; iD<dim; ++iD )
    {
    VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient( ptrI0, iD, m_ptrTmpField );
    VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( ptrCurrentAdjointDifference, iD, m_ptrTmpField );
    ptrCurrentGradient->AddCellwise( m_ptrTmpField );
    }

  ptrCurrentGradient->MultiplyByConstant( 1.0 );
}

template < class TState >
typename CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::CEnergyValues
CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::GetCurrentEnergy()
{
  /**
    * Computes the energy for the simplified shooting method.
    * Since everything is determined by the initial condition and the geodesics are explicitly
    * enforced we can compute the energy just as for the full adjoint formulation
    *
    \f[
      E = 0.5 \langle p(t_0)\nabla I(t_0),K*(p(t_0)\nabla I(t_0)\rangle + \frac{1}{\sigma}^2 d^2(I(t_1),Y)
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
      VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient( ptrInitialImage, iD, m_ptrTmpField );
      // multiply with initial momentum corresponding to this dimension of the image; p_i\nabla I
      VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( ptrInitialMomentum, iD, m_ptrTmpField );

      // convolve with kernel, K*(pi\nabla I)
      m_ptrTmpFieldConv->Copy( m_ptrTmpField );
      this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpFieldConv );

      // now we need to compute the inner product
      dEnergy += m_ptrTmpField->ComputeInnerProduct( m_ptrTmpFieldConv );
  }

  // multiply the full energy by 0.5
  unsigned int uiNrOfDiscretizationPoints = m_vecTimeDiscretization.size();
  T dTimeDuration = this->m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].dTime - this->m_vecTimeDiscretization[0].dTime;

  dEnergy *= 0.5*dTimeDuration;

  T dVelocitySquareNorm = dEnergy;

  // add the contributions of the data terms

  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  T dImageNorm = 0;

  // we only have two timepoints here

  dImageNorm += m_vecTimeDiscretization[ 0 ].vecWeights[ 0 ] * this->m_ptrMetric->GetMetric( ptrI0, ptrInitialImage );
  dImageNorm += m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].vecWeights[ 0 ] * this->m_ptrMetric->GetMetric( ptrI1, m_ptrCurrentI );

  dEnergy += dImageNorm;

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  return energyValues;
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
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

  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
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

#endif

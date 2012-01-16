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

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentI );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentP );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrCurrentVelocity );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpField );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpFieldConv );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrCurrentBackMap );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIdentity );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentFinalAdjoint );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrWarpedFinalToInitialAdjoint );

  ptrI0 = NULL;
  ptrI1 = NULL;

  m_vecMeasurementTimepoints.clear();
  m_vecTimeDiscretization.clear();
  m_vecTimeIncrements.clear();

  SaveDelete< TState* >::Pointer( this->m_pState );
  SaveDelete< TState* >::Pointer( this->m_pGradient );
}

template < class TState >
CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::~CLDDMMSimplifiedGeodesicShootingObjectiveFunction()
{
  DeleteData();
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::CreateNewStateStructures()
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
    ptrI0 = pImInfo->pIm;

    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 1 );
    ptrI1 = pImInfo->pIm;

    VectorImageType* ptrInitialImage = new VectorImageType( ptrI0 );
    VectorImageType* ptrInitialMomentum = new VectorImageType( pImInfo->pIm );
    ptrInitialMomentum->setConst(0);

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template< class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::ShallowCopyStateStructures( TState* pState )
{
    assert ( this->m_pState == NULL );

    VectorImageType* ptrInitialImage = pState->GetPointerToInitialImage();
    VectorImageType* ptrInitialMomentum = pState->GetPointerToInitialMomentum();

    this->m_pState = new TState( ptrInitialImage, ptrInitialMomentum );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::CreateGradientAndAuxiliaryStructures()
{

    // get the subject ids
    std::vector< unsigned int > vecSubjectIndices;
    this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

    assert( vecSubjectIndices.size()>0 );

    // obtain image from which to graft the image information for the data structures

    SImageInformation* pImInfo;
    // get information from the first image to figure out the dimensions
    this->m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[0], 0 );

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
    m_ptrDeterminantOfJacobian = new VectorImageType( pImInfo->pIm );

    // storage for current state
    m_ptrCurrentI = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentP = new VectorImageType( pImInfo->pIm );
    m_ptrCurrentVelocity = new VectorFieldType( pImInfo->pIm );

    // temporary storage
    m_ptrTmpField = new VectorFieldType( pImInfo->pIm );
    m_ptrTmpFieldConv = new VectorFieldType( pImInfo->pIm );

    // storage for the back map and the adjoint
    m_ptrCurrentBackMap = new VectorFieldType( pImInfo->pIm );
    m_ptrMapIdentity = new VectorFieldType( pImInfo->pIm );

    m_ptrCurrentFinalAdjoint = new VectorImageType( pImInfo->pIm );
    m_ptrWarpedFinalToInitialAdjoint = new VectorImageType( pImInfo->pIm );

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::InitializeDataStructuresFromState( TState* pState )
{
    DeleteData();

    CreateTimeDiscretization();

    ShallowCopyStateStructures( pState );

    CreateGradientAndAuxiliaryStructures();

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeDataStructures()
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
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeState()
{
  InitializeDataStructures();
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::InitializeState(TState* pState)
{
  InitializeDataStructuresFromState( pState );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  GetMap( m_ptrMapTmp, dTime );
  // now compute the momentum by interpolation
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialMomentum, ptrMomentum );
  LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );
  ptrMomentum->multElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, ptrInitialImage, ptrIm );

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

  LDDMMUtils< T, TState::VImageDimension >::identityMap( ptrMap );
  // identity map if the time points are the same
  if ( dTimeFrom == dTimeTo )
  {
    return;
  }

  // create four additional maps to hold the solution
  VectorFieldType* ptrMapIn = new VectorFieldType( ptrMap );
  VectorFieldType* ptrMapTmp = new VectorFieldType( ptrMap );
  VectorFieldType* ptrMapOut = new VectorFieldType( ptrMap );
  VectorFieldType* ptrCurrentVelocity = new VectorFieldType( ptrMap );

  // create two new images to hold the image and the adjoint
  VectorImageType* ptrCurrentP = new VectorImageType( m_ptrCurrentP );
  VectorImageType* ptrCurrentI = new VectorImageType( m_ptrCurrentI );

  // get the map between the two timepoints
  LDDMMUtils< T, TState::VImageDimension>::identityMap( ptrMapIn );

  VectorImageType* ptrInitialImage = this->m_pState->GetPointerToInitialImage();
  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  ptrCurrentI->copy( ptrInitialImage );
  ptrCurrentP->copy( ptrInitialMomentum );

  T dCurrentTime = m_vecTimeDiscretization[0].dTime;

  // first integrate all the way to the start

  bool bInitializedMap = false;

  T dTimeEvolvedFor = 0;

  for ( unsigned int iI = 0; iI < m_vecTimeDiscretization.size()-1; iI++ )
  {

    std::cout << "current time = " << dCurrentTime << std::endl;

    T dCurrentDT = m_vecTimeDiscretization[ iI+1 ].dTime - m_vecTimeDiscretization[ iI ].dTime;

    ComputeVelocity( ptrCurrentI, ptrCurrentP, ptrCurrentVelocity, ptrMapOut );
    std::cout << "evolving overall map for " << m_vecTimeIncrements[ iI ] << std::endl;
    this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMapIn, ptrMapOut, ptrMapTmp, m_vecTimeIncrements[ iI ] );

    LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapOut, ptrInitialImage, ptrCurrentI );
    LDDMMUtils< T, TState::VImageDimension >::applyMap( ptrMapOut, ptrInitialMomentum, ptrCurrentP );

    LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( ptrMapOut, m_ptrDeterminantOfJacobian );

    ptrCurrentP->multElementwise( m_ptrDeterminantOfJacobian );

    ptrMapIn->copy( ptrMapOut );

    if ( bInitializedMap )
    {
      if ( dCurrentTime + dCurrentDT >= dTimeTo )
      {
        // the full map is in between this time interval so just integrate it for a little bit
        std::cout << "1: evolving map for " << dTimeTo - dCurrentTime << std::endl;
        dTimeEvolvedFor += dTimeTo - dCurrentTime;
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, dTimeTo - dCurrentTime );
        ptrMap->copy( ptrMapOut );

        delete ptrMapOut;
        delete ptrMapTmp;
        delete ptrMapIn;
        delete ptrCurrentVelocity;

        delete ptrCurrentI;
        delete ptrCurrentP;

        std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

        return; // done because everything was in this interval
      }
      else
      {
         // integrate it for the full interval
        std::cout << "2: evolving map for " << m_vecTimeIncrements[ iI ] << std::endl;
        dTimeEvolvedFor += m_vecTimeIncrements[ iI ];
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, m_vecTimeIncrements[ iI ] );
        ptrMap->copy( ptrMapOut );
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
        ptrMap->copy( ptrMapOut );

        std::cout << "Overall time evolved for = " << dTimeEvolvedFor << std::endl;

        delete ptrMapOut;
        delete ptrMapTmp;
        delete ptrMapIn;
        delete ptrCurrentVelocity;

        delete ptrCurrentI;
        delete ptrCurrentP;

        return; // done because everything was in this interval
      }
      else
      {
        // integrate the map until the end of this interval
        std::cout << "4: evolving map for " << dCurrentDT << std::endl;
        dTimeEvolvedFor += dCurrentDT;
        this->m_ptrEvolver->SolveForward( ptrCurrentVelocity, ptrMap, ptrMapOut, ptrMapTmp, dCurrentDT );
        ptrMap->copy( ptrMapOut );
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
    if ( this->m_ptrImageManager == NULL )
    {
      throw std::runtime_error( "ERROR: No image manager specified." );
      return;
    }

    std::vector< STimePoint > vecTimePointData;
    unsigned int uiNrOfMeasurements = CALATK::LDDMMUtils< T, TState::VImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );

    if ( uiNrOfMeasurements != 2 )
    {
      throw std::runtime_error( "CLDDMMSimplifiedGeodesicShootingObjectiveFunction only supports two measurements (one source and one target image" );
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

      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialImage, m_ptrCurrentI );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapOut, ptrInitialMomentum, m_ptrCurrentP );

      LDDMMUtils< T, TState::VImageDimension >::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );

      m_ptrCurrentP->multElementwise( m_ptrDeterminantOfJacobian );

      m_ptrMapIn->copy( m_ptrMapOut );

      // keep track of the backward map
      // negate the velocity field
      m_ptrCurrentVelocity->multConst( -1.0 );

      // m_ptrMapOut here is the incremental part of the map within this time-step
      this->m_ptrEvolver->SolveForward( m_ptrCurrentVelocity, m_ptrMapIdentity, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      m_ptrMapTmp->copy( m_ptrCurrentBackMap );
      LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrMapTmp, m_ptrMapOut, m_ptrCurrentBackMap );
  }

  // TODO: implement this for multiple time points

  // now we have the back map and can compute the adjoint in the initial frame by pulling it back

  unsigned int uiNrOfTimePoints = m_vecTimeDiscretization.size();

  this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentFinalAdjoint, m_ptrCurrentI, ptrI1 );
  m_ptrCurrentFinalAdjoint->multConst( m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecWeights[ 0 ] );

  LDDMMUtils< T, TState::VImageDimension >::applyMap( m_ptrCurrentBackMap, m_ptrCurrentFinalAdjoint, ptrWarpedFinalToInitialAdjoint );
}

template < class TState >
void CLDDMMSimplifiedGeodesicShootingObjectiveFunction< TState>::ComputeGradient()
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
      \nabla_{p(t_0)}E = (\lambda(0)-p(0))
    \f]
    */

  VectorImageType* ptrInitialMomentum = this->m_pState->GetPointerToInitialMomentum();

  VectorImageType* ptrI0Gradient = this->m_pGradient->GetPointerToInitialImage();
  VectorImageType* ptrP0Gradient = this->m_pGradient->GetPointerToInitialMomentum();

  ptrP0Gradient->copy( ptrInitialMomentum );
  ptrP0Gradient->multConst(-1);

  ptrP0Gradient->addCellwise( m_ptrWarpedFinalToInitialAdjoint );

  // debug
  ptrP0Gradient->multConst(-1);

  if ( this->m_EstimateInitialImage )
  {
    ptrI0Gradient->copy( m_ptrWarpedFinalToInitialAdjoint );
    ptrI0Gradient->multConst(-1);
  }
  else
  {
    ptrI0Gradient->setConst( 0.0 );
  }
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

  // multiply the full energy by 0.5
  unsigned int uiNrOfDiscretizationPoints = m_vecTimeDiscretization.size();
  T dTimeDuration = this->m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].dTime - this->m_vecTimeDiscretization[0].dTime;

  dEnergy *= 0.5*dTimeDuration;

  T dVelocitySquareNorm = dEnergy;

  // add the contributions of the data terms

  ComputeImageMomentumForwardAndFinalAdjointWarpedToInitialImage( m_ptrWarpedFinalToInitialAdjoint );

  T dImageNorm = 0;

  // we only have two timepoints here

  dImageNorm += m_vecTimeDiscretization[ 0 ].vecWeights[ 0 ] * this->m_pMetric->GetMetric( ptrI0, ptrInitialImage );
  dImageNorm += m_vecTimeDiscretization[ uiNrOfDiscretizationPoints-1 ].vecWeights[ 0 ] * this->m_pMetric->GetMetric( ptrI1, m_ptrCurrentI );

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

#endif

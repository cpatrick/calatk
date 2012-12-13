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

#ifndef C_LDDMM_MULTI_VELOCITY_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_MULTI_VELOCITY_OBJECTIVE_FUNCTION_TXX

#define DEBUG

template < class TState >
CLDDMMMultiVelocityObjectiveFunction< TState >::CLDDMMMultiVelocityObjectiveFunction()
{

  //std::cout << "CLDDMMMultiVelocityObjectiveFunction Constructor" << std::endl;

  m_ptrI0 = NULL;
  m_ptrTmpVelocityField = NULL;
  m_ptrTmpGradient = NULL;
  m_ptrCurrentAdjointDifference = NULL;
  m_ptrDeterminantOfJacobian = NULL;

  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

}


template < class TState >
CLDDMMMultiVelocityObjectiveFunction< TState >::~CLDDMMMultiVelocityObjectiveFunction()
{
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
}

/// Set the number of separate masks
template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::SetNumberOfMasks( unsigned int numMasks )
{
  m_ptrS0s = std::vector< VectorImagePointerType >( numMasks, NULL );
}

  /// Set a binary mask image at the given index
template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::SetMaskImage( VectorImageType* ptrMask, unsigned int index )
{
  m_ptrS0s[index] = new VectorImageType( ptrMask );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::CreateAuxiliaryStructures()
{

  //std::cout << "CreateAuxiliaryStructures() Start" << std::endl;

  // obtain image from which to graft the image information for the data structures
  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer( this->GetActiveSubjectId() );

  // Hardcoding some S images
  //std::cout << "CreateAuxiliaryStructures() Start Creating Masks" << std::endl;
  this->SetNumberOfMasks(2);
  m_ptrS0s[0] = new VectorImageType( pGraftIm );
  m_ptrS0s[1] = new VectorImageType( pGraftIm );
  m_ptrS0s[0]->MultiplyByConstant(0);
  m_ptrS0s[1]->MultiplyByConstant(0);
  //std::cout << "got here" << std::endl;
  for ( int i = 0; i < 32; ++i )
    {
      for ( int j = 0; j < 32; ++j )
        {
        if (j < 16 )
          {
          m_ptrS0s[0]->SetValue( i, j, 0, 1 );
          }
        else
          {
          m_ptrS0s[1]->SetValue( i, j, 0, 1 );
          }
        }
    }
  //std::cout << "CreateAuxiliaryStructures() Done Creating Masks" << std::endl;

  m_Sigma2Sqr = 1.0;

  // get the subject ids
  std::vector< int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // allocate all the auxiliary data

  // image and adjoint time-series
  m_ptrIs.clear();
  m_ptrSs.clear();
  m_ptrLambdaIs.clear();
  m_ptrLambdaSs.clear();
  m_ptrVs.clear();
  m_ptrCurrentLambdaEnds.clear();
  m_ptrCurrentMaskLambdaEnds.clear();

  // storage for the initial image
  // and initialize it with the first image of the time-series
  m_ptrI0 = new VectorImageType( pGraftIm );
  m_ptrI1 = new VectorImageType( pGraftIm );
  this->GetTargetImage( m_ptrI1 );

  //std::cout << "CreateAuxiliaryStructures() Done Clearing vectors" << std::endl;

  //std::cout << "CreateAuxiliaryStructures() " << m_ptrS0s.size() << std::endl;

  // one more than for the velocity fields
  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex)
    {
    m_ptrVs.push_back(std::vector< VectorFieldPointerType >());
    m_ptrIs.push_back(std::vector< VectorImagePointerType >());
    m_ptrSs.push_back(std::vector< VectorImagePointerType >());
    m_ptrLambdaIs.push_back(std::vector< VectorImagePointerType >());
    m_ptrLambdaSs.push_back(std::vector< VectorImagePointerType >());

    typename VectorImageType::Pointer ptrCurrentVectorImage;
    ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrCurrentLambdaEnds.push_back( ptrCurrentVectorImage );
    ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrCurrentMaskLambdaEnds.push_back( ptrCurrentVectorImage );

    //std::cout << "outer loop" << std::endl;
    for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
      {
      //std::cout << "inner loop 0 " << std::endl;
      ptrCurrentVectorImage = new VectorImageType( pGraftIm );
      //std::cout << "inner loop 1" << std::endl;
      m_ptrIs[maskIndex].push_back( ptrCurrentVectorImage );
      ptrCurrentVectorImage = new VectorImageType( pGraftIm );
      m_ptrSs[maskIndex].push_back( ptrCurrentVectorImage );
      //std::cout << "inner loop 2" << std::endl;
      // bookkeeping to simplify metric computations
      // TODO cpatrick This might need to come back
      //this->m_vecTimeDiscretization[ iI ].vecEstimatedImages.push_back( ptrCurrentVectorImage );
//std::cout << "inner loop 3" << std::endl;
      typename VectorFieldType::Pointer ptrCurrentVectorField = new VectorFieldType( pGraftIm );
      //std::cout << "inner loop 4" << std::endl;
      m_ptrVs[maskIndex].push_back( ptrCurrentVectorField );
//std::cout << "inner loop 5" << std::endl;
      ptrCurrentVectorImage = new VectorImageType( pGraftIm );
      m_ptrLambdaIs[maskIndex].push_back( ptrCurrentVectorImage );
      ptrCurrentVectorImage = new VectorImageType( pGraftIm );
      m_ptrLambdaSs[maskIndex].push_back( ptrCurrentVectorImage );
      }
    }
  //std::cout << "Out of main loop" << std::endl;

  // storage for the maps

  m_ptrMapIn = new VectorFieldType( pGraftIm );
  m_ptrMapOut = new VectorFieldType( pGraftIm );
  m_ptrMapTmp = new VectorFieldType( pGraftIm );

  // storage for the adjoint difference

  m_ptrCurrentAdjointDifference = new VectorImageType( pGraftIm );
  m_ptrCurrentAdjointMaskDifference = new VectorImageType( pGraftIm );

  // storage for the determinant of Jacobian
  m_ptrDeterminantOfJacobian  = new VectorImageType( pGraftIm, 0.0, 1 );
  assert( m_ptrDeterminantOfJacobian->GetDimension() == 1 );

  // storage for the negated velocity field
  m_ptrTmpVelocityField = new VectorFieldType( pGraftIm );

  // storage for the temporary gradient
  m_ptrTmpGradient = new VectorFieldType( pGraftIm );

  //std::cout << "CreateAuxiliaryStructures() End" << std::endl;

}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::DeleteAuxiliaryStructures()
{

  //std::cout << "DeleteAuxiliaryStructures()" << std::endl;

  this->m_ptrMapIn  = NULL;
  this->m_ptrMapOut = NULL;
  this->m_ptrMapTmp = NULL;

  this->m_ptrTmpVelocityField = NULL;
  this->m_ptrTmpGradient      = NULL;

  this->m_ptrDeterminantOfJacobian    = NULL;
  this->m_ptrI0                       = NULL;
  //this->m_ptrCurrentLambdaEnd         = NULL;
  this->m_ptrCurrentAdjointDifference = NULL;

  this->m_ptrS0s.clear();

  this->m_ptrVs.clear();
  this->m_ptrIs.clear();
  this->m_ptrSs.clear();
  this->m_ptrLambdaIs.clear();
  this->m_ptrLambdaSs.clear();
  this->m_ptrCurrentLambdaEnds.clear();
  this->m_ptrCurrentMaskLambdaEnds.clear();
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, T dTime )
{
  //std::cout << "GetMap()" << std::endl;
  T dTimeFrom = this->m_vecTimeDiscretization[0].dTime;
  GetMapFromTo( ptrMap, dTimeFrom, dTime );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo )
{

  //std::cout << "GetMapFromTo()" << std::endl;

  std::vector< VectorFieldType* > tmpMaps( m_ptrS0s.size(), new VectorFieldType(ptrMap) );

  ptrMap->SetToConstant(0);

  for( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++ maskIndex)
    {
    std::vector<VectorFieldPointerType>* curVs = new std::vector<VectorFieldPointerType>();
    for ( unsigned int i = 0; i < m_ptrVs.size(); ++i )
      {
      curVs->push_back( m_ptrVs[maskIndex][i] );
      }
    CALATK::LDDMMUtils< T, TState::ImageDimension >::GetMapFromToFromSpatioTemporalVelocityField(
          tmpMaps[maskIndex],
          dTimeFrom,
          dTimeTo,
          this->m_vecTimeDiscretization,
          curVs,
          this->m_ptrEvolver );
    //tmpMaps[maskIndex]->MultiplyElementwise(m_ptrS0s[maskIndex]); // Maybe this isn't needed
    ptrMap->AddElementwise(tmpMaps[maskIndex]);
    }
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm )
{
  ptrIm->Copy( m_ptrI0 );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, m_ptrI0, ptrIm );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrIm )
{
  std::vector< TimeSeriesDataPointType > timeseries;
  this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeseries, this->GetActiveSubjectId() );
  unsigned int numberOfTimePoints = timeseries.size();

  ptrIm->Copy( timeseries[ numberOfTimePoints - 1 ].GetImage() );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrIm, T dTime )
{
  std::vector< TimeSeriesDataPointType > timeseries;
  this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeseries, this->GetActiveSubjectId() );
  unsigned int numberOfTimePoints = timeseries.size();

  this->GetMapFromTo( m_ptrMapTmp, timeseries[ numberOfTimePoints - 1 ].GetTimePoint(), dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, timeseries[ numberOfTimePoints - 1 ].GetImage(), ptrIm );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  //std::cout << "GetMomentum()" << std::endl;
  ComputeImagesForward();
  ComputeAdjointBackward();

  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, m_ptrLambdaIs[0][0], ptrMomentum );
  LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );

  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::GetInitialMomentum( VectorImageType* ptrMomentum )
{
  ptrMomentum->Copy( m_ptrLambdaIs[0][0] );
}

template  < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::DetermineTimePointData( std::vector< STimePoint >& vecTimePointData )
{
  CALATK::LDDMMUtils< T, TState::ImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, this->GetActiveSubjectId(), vecTimePointData );
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::ComputeImagesForward()
{

  //std::cout << "ComputeImagesForward()" << std::endl;

  LDDMMUtils< T, TState::ImageDimension >::identityMap( m_ptrMapIn );
  // FIXME: This is just to make things easier and to support estimating the initial image (todo) later
  m_ptrIs[0][0]->Copy( m_ptrI0 );
  //std::cout << "copy done" << std::endl;

  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex)
    {
    for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
      {
      //std::cout << "forward inner loop start" << std::endl;
      this->m_ptrEvolver->SolveForward(
        (VectorFieldType*) m_ptrVs[maskIndex][iI].GetPointer(), m_ptrMapIn,
        m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
      //std::cout << "forward inner loop after SolveForward" << std::endl;

      // for next step, copy
      m_ptrMapIn->Copy( m_ptrMapOut );
      //std::cout << "forward inner loop after copy" << std::endl;

      // now compute the image by interpolation
      LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapIn, m_ptrI0,
        m_ptrIs[maskIndex][ iI+1 ] );
      //std::cout << "forward inner loop after applyMap" << std::endl;

      // now compute the new masks by interpolation
      LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapIn, m_ptrI0,
        m_ptrSs[maskIndex][ iI+1 ] );
      //std::cout << "forward inner loop end" << std::endl;
      }
    }
  //std::cout << "ComputeImagesForward() end" << std::endl;
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::ComputeAdjointBackward()
{

  //std::cout << "ComputeAdjointBackward()" << std::endl;

  // Create R^c that ties the different velocity fields and masks together in the
  // adjoints
  // R^c = \sum^N_i=0 S^i(1)I^i(1) - I_1
  unsigned int lastTime = this->m_vecTimeDiscretization.size()-1;
  VectorImageType* ptrRc = new VectorImageType( m_ptrCurrentLambdaEnds[0] );
  ptrRc->SetToConstant( 0 );
  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex )
    {
      VectorImageType* curFactor = new VectorImageType( m_ptrSs[maskIndex][lastTime] );
      curFactor->MultiplyElementwise( m_ptrIs[maskIndex][lastTime] );
      ptrRc->AddElementwise( curFactor );
      delete curFactor;
    }
  VectorImageType* ptrTempTarget = new VectorImageType( m_ptrCurrentLambdaEnds[0] );
  this->GetTargetImage( ptrTempTarget );
  ptrTempTarget->MultiplyByConstant(-1);
  ptrRc->AddElementwise(ptrTempTarget);
  delete ptrTempTarget;


  // create the final condition
  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex )
    {
    m_ptrCurrentLambdaEnds[maskIndex]->SetToConstant( 0 );

    unsigned int uiNrOfTimePoints = this->m_vecTimeDiscretization.size();
    unsigned int uiNrOfMeasuredImagesAtTimePoint = 0;

    // Computing \lamba^i_I(1)
    m_ptrCurrentAdjointDifference->SetToConstant( 0 );
    m_ptrCurrentAdjointDifference->AddElementwise( m_ptrSs[maskIndex][lastTime] );
    m_ptrCurrentAdjointDifference->MultiplyByConstant( -2.0/this->m_SigmaSqr );
    m_ptrCurrentLambdaEnds[maskIndex]->AddCellwise( m_ptrCurrentAdjointDifference );
    m_ptrCurrentLambdaEnds[maskIndex]->MultiplyElementwise( ptrRc );

    // last adjoint; just for book-keeping, currently not really used in the algorithm
    m_ptrLambdaIs[maskIndex][ uiNrOfTimePoints-1 ]->Copy( m_ptrCurrentLambdaEnds[maskIndex] );

    //Computing \lambda^i_S(1)
    VectorImagePointerType ptrTmpAdjoint = new VectorImageType ( m_ptrCurrentAdjointMaskDifference );
    m_ptrCurrentAdjointMaskDifference->SetToConstant( 0 );
    m_ptrCurrentAdjointMaskDifference->AddElementwise( m_ptrIs[maskIndex][lastTime] );
    m_ptrCurrentAdjointMaskDifference->MultiplyElementwise( ptrRc );
    m_ptrCurrentAdjointDifference->MultiplyByConstant( -2.0/this->m_SigmaSqr );

    ptrTmpAdjoint->SetToConstant( 0 );
    ptrTmpAdjoint->AddElementwise( m_ptrSs[maskIndex][0] );
    ptrTmpAdjoint->MultiplyByConstant( -2.0/this->m_Sigma2Sqr );
    ptrTmpAdjoint->AddElementwise( m_ptrSs[maskIndex][lastTime] );
    m_ptrCurrentAdjointMaskDifference->AddElementwise( ptrTmpAdjoint );

    m_ptrCurrentMaskLambdaEnds[maskIndex]->SetToConstant( 0 );
    m_ptrCurrentMaskLambdaEnds[maskIndex]->AddElementwise( m_ptrCurrentAdjointMaskDifference );

    m_ptrLambdaIs[maskIndex][ uiNrOfTimePoints-1 ]->Copy( m_ptrCurrentLambdaEnds[maskIndex] );
    m_ptrLambdaSs[maskIndex][ uiNrOfTimePoints-1 ]->Copy( m_ptrCurrentMaskLambdaEnds[maskIndex] );

    // reset the map to flow backwards
    typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
    LDDMMUtilsType::identityMap( m_ptrMapIn );

    for ( int iI = (int)this->m_vecTimeDiscretization.size()-1-1; iI >= 0; --iI )
      {

      // need to reverse the velocity field, because we are evolving in the backward direction
      m_ptrTmpVelocityField->Copy( m_ptrVs[maskIndex][iI] );
      m_ptrTmpVelocityField->MultiplyByConstant( -1 );

      this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

      // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
      LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnds[maskIndex],
        m_ptrLambdaIs[maskIndex][ iI ] );

      // now compute the adjoint of the mask
      LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentMaskLambdaEnds[maskIndex],
        m_ptrLambdaSs[maskIndex][ iI ] );

      // compute det jacobian
      LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
      // multiply by the determinant of the Jacobian
      m_ptrLambdaIs[maskIndex][ iI ]->MultiplyElementwise( m_ptrDeterminantOfJacobian );
      m_ptrLambdaSs[maskIndex][ iI ]->MultiplyElementwise( m_ptrDeterminantOfJacobian );

      // for next step, copy
      m_ptrMapIn->Copy( m_ptrMapOut );

      }
    }
}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::ComputeGradient()
{
  //std::cout << "Compute Grad" << std::endl;
  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$
  //
  // Here the energy is defined as
  // \f$ E = 1/2 \int_0^1 \|v\|_L^2~dt + 1/(sigma^2)\|I(1)-I_1\|^2
  // and the gradient used is the Hilbert gradient

  unsigned int dim = m_ptrI0->GetDimension();

  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex )
    {
    for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
      {
      // initialize to 0
      VectorFieldType* ptrCurrentGradient = this->m_ptrGradient->GetVectorFieldPointer( iI );
      ptrCurrentGradient->SetToConstant( 0 );

      for ( unsigned int iD = 0; iD<dim; ++iD )
        {
        VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient(
          m_ptrIs[maskIndex][ iI ], iD, m_ptrTmpGradient );
        VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( m_ptrLambdaIs[maskIndex][ iI ], iD, m_ptrTmpGradient );
        ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
        }

      //VectorImageUtils< T, ImageDimension >::writeFileITK( ptrCurrentGradient, "curGradBeforeConv.nrrd" );

      this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

      //VectorImageUtils< T, ImageDimension >::writeFileITK( ptrCurrentGradient, "curGradAfterConv.nrrd" );

      // add v
      VectorFieldType* ptrCurrentVelocity = this->m_ptrState->GetVectorFieldPointer( iI );
      ptrCurrentGradient->AddCellwise( ptrCurrentVelocity );

      // multiply by global energy weight
      ptrCurrentGradient->MultiplyByConstant( this->m_EnergyWeight );

      }
    }
  //VectorFieldUtils< T, ImageDimension >::writeTimeDependantImagesITK( this->m_ptrGradient->GetVectorPointerToVectorFieldPointer(), "gradientAfterComputation.nrrd" );

}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
{
  // compute the unsmoothed velocity gradient; to be used to estimate weights for the multi-Gaussian kernels.
  // v is assumed zero here and the unsmoothed gradient is then
  // \f$ \nabla E = (\sum_i \lambda_i \nabla I_0 ) \f$
  // where the \lambda_i are the respective adjoints (based on the chosen metric)

  unsigned int dim = m_ptrI0->GetDimension();

  // compute the initial adjoint, assuming that there is only a zero velocity field
  typename VectorImageType::Pointer ptrLambda0 = new VectorImageType( m_ptrI0 );
  ptrLambda0->SetToConstant( 0.0 );

  typename VectorImageType::Pointer ptrCurrentAdjointDifference = new VectorImageType( m_ptrI0 );

  for ( unsigned int iI = 0; iI< this->m_vecTimeDiscretization.size(); ++iI )
    {
    // update if we need to jump at the current time-point
    if ( this->m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
      // account for all possible jumps of the adjoint at this time-point
      unsigned int uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
      for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
        this->m_ptrMetric->GetAdjointMatchingDifferenceImage( ptrCurrentAdjointDifference, m_ptrI0 , this->m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
        ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/this->m_SigmaSqr );
        ptrLambda0->AddCellwise( ptrCurrentAdjointDifference );
        }
      }
    }

  // initialize to 0
  VectorFieldType* ptrCurrentGradient = ptrInitialUnsmoothedVelocityGradient;
  ptrCurrentGradient->SetToConstant( 0 );

  for ( unsigned int iD = 0; iD<dim; ++iD )
    {
    VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient( m_ptrI0, iD, m_ptrTmpGradient );
    VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( ptrLambda0, iD, m_ptrTmpGradient );
    ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
    }

  ptrCurrentGradient->MultiplyByConstant( this->m_EnergyWeight );

}

template < class TState >
typename CLDDMMMultiVelocityObjectiveFunction< TState >::CEnergyValues
CLDDMMMultiVelocityObjectiveFunction< TState >::GetCurrentEnergy()
{
  //std::cout << "GetCurrentEnergy()" << std::endl;

  T dEnergy = 0;

  // computing the square velocity for this time step using the inverse kernel measuring non-smoothness (and not it's inverse)
  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex )
    {
    for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
      {
      //std::cout << "get energy inner loop start" << std::endl;
      // copy current velocity field (of the state)
      m_ptrTmpVelocityField->Copy( m_ptrVs[maskIndex][iI] );
      //std::cout << "get energy inner loop 1" << std::endl;
      // convolve it with the inverse kernel, L^\dagger L v
      this->m_ptrKernel->ConvolveWithInverseKernel( m_ptrTmpVelocityField );
//std::cout << "get energy inner loop 2" << std::endl;
      // now multiply it with v
      T dCurrentEnergy = 0.5*this->m_vecTimeIncrements[ iI ]*m_ptrTmpVelocityField->ComputeInnerProduct( m_ptrVs[maskIndex][iI] );
//std::cout << "get energy inner loop 3" << std::endl;
      // add energy increment, assuring that we have the correct spatio-temporal volume contribution
      dEnergy += dCurrentEnergy;
      //std::cout << "get energy inner loop end" << std::endl;
      }
  }

  dEnergy *= this->m_EnergyWeight;

  T dVelocitySquareNorm = dEnergy;

  // now add the contributions of the data terms

  // create the current images according to the current state
  // (in case the velocities were updated externally by the optimizer for example)

  //std::cout << "GetCurrentEnergy() right before compute images forward" << std::endl;
  ComputeImagesForward();

  T dImageNorm = 0;

  unsigned int lastTime = this->m_vecTimeDiscretization.size()-1;
  for ( unsigned int maskIndex = 0; maskIndex < m_ptrS0s.size(); ++maskIndex )
    {
      T dCurrentImageMetric = 1.0/this->m_SigmaSqr *
        this->m_ptrMetric->GetMetric( m_ptrIs[maskIndex][lastTime], m_ptrI1 );
      dImageNorm += dCurrentImageMetric;
    }

  dImageNorm *= this->m_EnergyWeight;

  dEnergy += dImageNorm;

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  //std::cout << "E(I) = " << dImageNorm << std::endl;

  return energyValues;

}

template < class TState >
void CLDDMMMultiVelocityObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  //std::cout << "saving gradient components at iteration " << uiIter << std::endl;

  outputPrefix = outputPrefix + "LDDMM-";

  ComputeImagesForward();
  ComputeAdjointBackward();

  unsigned int dim = m_ptrI0->GetDimension();

  typename VectorFieldType::Pointer ptrCurrentGradient = new VectorFieldType( m_ptrTmpGradient );

  std::string suffix = "-iter-" + CreateIntegerString( uiIter, 3 ) + ".nrrd";

  // write out the kernel
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
  VectorImageUtilsType::writeFileITK( this->m_ptrKernel->GetKernel(), outputPrefix + "Kernel" + CreateIntegerString( 0, 3 ) + suffix );

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    ptrCurrentGradient->SetToConstant( 0 );

    VectorImageUtilsType::writeFileITK( m_ptrIs[0][iI], outputPrefix + "I" + CreateIntegerString( iI, 3 ) + suffix );
    VectorImageUtilsType::writeFileITK( m_ptrLambdaIs[0][iI], outputPrefix + "lam" + CreateIntegerString( iI, 3 ) + suffix );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtilsType::computeCentralGradient( m_ptrIs[0][ iI ], iD, m_ptrTmpGradient );
      VectorImageUtilsType::writeFileITK( m_ptrTmpGradient, outputPrefix + "gradI" + CreateIntegerString( iI, 3 ) + suffix );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrLambdaIs[0][ iI ], iD, m_ptrTmpGradient );
      VectorImageUtilsType::writeFileITK( m_ptrTmpGradient, outputPrefix + "lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

      ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
      }

    this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    VectorImageUtilsType::writeFileITK( ptrCurrentGradient, outputPrefix + "conv_lam_x_gradI" + CreateIntegerString( iI, 3 ) + suffix );

    // add v
    VectorFieldType* ptrCurrentVelocity = this->m_ptrState->GetVectorFieldPointer( iI );
    VectorImageUtilsType::writeFileITK( ptrCurrentVelocity, outputPrefix + "v" + CreateIntegerString( iI, 3 ) + suffix );

    ptrCurrentGradient->AddCellwise( ptrCurrentVelocity );
    VectorImageUtilsType::writeFileITK( ptrCurrentGradient, outputPrefix + "gradv" + CreateIntegerString( iI, 3 ) + suffix );
    }
}

#endif

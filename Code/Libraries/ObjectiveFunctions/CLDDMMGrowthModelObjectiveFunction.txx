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

#ifndef C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_TXX

template < class TState >
CLDDMMGrowthModelObjectiveFunction< TState >::CLDDMMGrowthModelObjectiveFunction()
  : DefaultSigmaSqr( 0.01 ), m_ExternallySetSigmaSqr( false )
{
  m_SigmaSqr = DefaultSigmaSqr;

  m_ptrI = NULL;
  m_ptrLambda = NULL;

  m_ptrI0 = NULL;
  m_ptrTmpVelocityField = NULL;
  m_ptrTmpGradient = NULL;
  m_ptrCurrentLambdaEnd = NULL;
  m_ptrCurrentAdjointDifference = NULL;
  m_ptrDeterminantOfJacobian = NULL;

  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

}


template < class TState >
CLDDMMGrowthModelObjectiveFunction< TState >::~CLDDMMGrowthModelObjectiveFunction()
{
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "GrowthModel", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "GrowthModel", Json::nullValue );

  SetJSONHelpForRootKey( GrowthModel, "settings for the LDDMM growth model" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, SigmaSqr );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, SigmaSqr,
                     "1/SigmaSqr is the weight for the data match term" );
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::CreateAuxiliaryStructures()
{

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // obtain image from which to graft the image information for the data structures
  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate all the auxiliary data

  // image and adjoint time-series
  m_ptrI = new std::vector< typename VectorImageType::Pointer >;
  m_ptrLambda = new std::vector< VectorImagePointerType >;

  // storage for the initial image
  // and initialize it with the first image of the time-series
  m_ptrI0 = new VectorImageType( pGraftIm );

  // one more than for the velocity fields
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
    {
    typename VectorImageType::Pointer ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrI->push_back( ptrCurrentVectorImage );

    // bookkeeping to simplify metric computations
    this->m_vecTimeDiscretization[ iI ].vecEstimatedImages.push_back( ptrCurrentVectorImage );

    ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrLambda->push_back( ptrCurrentVectorImage );
    }

  // storage for the maps

  m_ptrMapIn = new VectorFieldType( pGraftIm );
  m_ptrMapOut = new VectorFieldType( pGraftIm );
  m_ptrMapTmp = new VectorFieldType( pGraftIm );

  // storage for the adjoint

  m_ptrCurrentLambdaEnd = new VectorImageType( pGraftIm );

  // storage for the adjoint difference

  m_ptrCurrentAdjointDifference = new VectorImageType( pGraftIm );

  // storage for the determinant of Jacobian
  m_ptrDeterminantOfJacobian  = new VectorImageType( pGraftIm, 0.0, 1 );
  assert( m_ptrDeterminantOfJacobian->GetDimension() == 1 );

  // storage for the negated velocity field
  m_ptrTmpVelocityField = new VectorFieldType( pGraftIm );

  // storage for the temporary gradient
  m_ptrTmpGradient = new VectorFieldType( pGraftIm );

}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::DeleteAuxiliaryStructures()
{
  this->m_ptrMapIn  = NULL;
  this->m_ptrMapOut = NULL;
  this->m_ptrMapTmp = NULL;

  this->m_ptrTmpVelocityField = NULL;
  this->m_ptrTmpGradient      = NULL;

  this->m_ptrI0                       = NULL;
  this->m_ptrCurrentLambdaEnd         = NULL;
  this->m_ptrCurrentAdjointDifference = NULL;
  this->m_ptrDeterminantOfJacobian    = NULL;

  if( this->m_ptrI )
    {
    this->m_ptrI->clear();
    }
  if( this->m_ptrLambda )
    {
    this->m_ptrLambda->clear();
    }
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, m_ptrI0, ptrIm );
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::GetInitialImage( VectorImageType* ptrIm )
{
  ptrIm->Copy( m_ptrI0 );
}

template < class TState >
const typename CLDDMMGrowthModelObjectiveFunction< TState >::VectorImageType*
CLDDMMGrowthModelObjectiveFunction< TState >::GetPointerToInitialImage() const
{
  return m_ptrI0;
}


template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  ComputeImagesForward();
  ComputeAdjointBackward();

  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, (*m_ptrLambda)[0], ptrMomentum );
  LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );

  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );

}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::GetInitialMomentum( VectorImageType* ptrMomentum )
{
  ptrMomentum->Copy( (*m_ptrLambda)[0] );
}

template  < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::DetermineTimePointData( std::vector< STimePoint >& vecTimePointData )
{
  CALATK::LDDMMUtils< T, TState::ImageDimension >::DetermineTimeSeriesTimePointData( this->m_ptrImageManager, 0, vecTimePointData );
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::ComputeImagesForward()
{
  LDDMMUtils< T, TState::ImageDimension >::identityMap( m_ptrMapIn );
  // FIXME: This is just to make things easier and to support estimating the initial image (todo) later
  (*m_ptrI)[ 0 ]->Copy( m_ptrI0 );

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {

    this->m_ptrEvolver->SolveForward( this->m_ptrState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    // now compute the image by interpolation
    LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapIn, m_ptrI0, (*m_ptrI)[ iI+1 ] );

    }
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::ComputeAdjointBackward()
{
  // create the final condition

  m_ptrCurrentLambdaEnd->SetToConstant( 0 );

  unsigned int uiNrOfTimePoints = this->m_vecTimeDiscretization.size();
  unsigned int uiNrOfMeasuredImagesAtTimePoint = 0;

  uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages.size();
  // first set the final condition
  for ( unsigned int iM = 0; iM <  uiNrOfMeasuredImagesAtTimePoint; ++iM )
    {
    this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecEstimatedImages[0] , this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages[ iM ] );
    m_ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_SigmaSqr );
    m_ptrCurrentLambdaEnd->AddCellwise( m_ptrCurrentAdjointDifference );
    }

  // last adjoint; just for book-keeping, currently not really used in the algorithm
  (*m_ptrLambda)[ uiNrOfTimePoints-1 ]->Copy( m_ptrCurrentLambdaEnd );

  // reset the map to flow backwards
  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( m_ptrMapIn );

  for ( int iI = (int)this->m_vecTimeDiscretization.size()-1-1; iI >= 0; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->Copy( this->m_ptrState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->MultiplyByConstant( -1 );

    this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnd, (*m_ptrLambda)[ iI ] );

    // compute det jacobian
    LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    (*m_ptrLambda)[ iI ]->MultiplyElementwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    // update if we need to jump at the current time-point
    if ( this->m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
      // CHANGED: affects \lambda directly at this time-point, lamda-end is then a copy

      // reset the map to flow backwards, because we update the current adjoint
      LDDMMUtilsType::identityMap( m_ptrMapIn );

      // account for all possible jumps of the adjoint at this time-point
      uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
      for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
        {
        this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, this->m_vecTimeDiscretization[ iI ].vecEstimatedImages[0] , this->m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
        m_ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_SigmaSqr );
        (*m_ptrLambda)[iI]->AddCellwise( m_ptrCurrentAdjointDifference );
        }
      // reset the current adjoint to the adjoint at current time point
      m_ptrCurrentLambdaEnd->Copy( (*m_ptrLambda)[ iI ] );
      }
    }
}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::ComputeGradient()
{

  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$
  //
  // Here the energy is defined as
  // \f$ E = 1/2 \int_0^1 \|v\|_L^2~dt + 1/(sigma^2)\|I(1)-I_1\|^2
  // and the gradient used is the Hilbert gradient

  unsigned int dim = m_ptrI0->GetDimension();

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    VectorFieldType* ptrCurrentGradient = this->m_ptrGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->SetToConstant( 0 );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtils< T, TState::ImageDimension >::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpGradient );
      VectorImageUtils< T, TState::ImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrLambda)[ iI ], iD, m_ptrTmpGradient );
      ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
      }

    //VectorImageUtils< T, ImageDimension >::writeFileITK( ptrCurrentGradient, "curGradBeforeConv.nrrd" );

    this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    //VectorImageUtils< T, ImageDimension >::writeFileITK( ptrCurrentGradient, "curGradAfterConv.nrrd" );

    // add v
    VectorFieldType* ptrCurrentVelocity = this->m_ptrState->GetVectorFieldPointer( iI );
    ptrCurrentGradient->AddCellwise( ptrCurrentVelocity );

    }

  //VectorFieldUtils< T, ImageDimension >::writeTimeDependantImagesITK( this->m_ptrGradient->GetVectorPointerToVectorFieldPointer(), "gradientAfterComputation.nrrd" );

}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
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
        this->m_pMetric->GetAdjointMatchingDifferenceImage( ptrCurrentAdjointDifference, m_ptrI0 , this->m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ] );
        ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_SigmaSqr );
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
}

template < class TState >
typename CLDDMMGrowthModelObjectiveFunction< TState >::CEnergyValues
CLDDMMGrowthModelObjectiveFunction< TState >::GetCurrentEnergy()
{

  T dEnergy = 0;

  // computing the square velocity for this time step using the inverse kernel measuring non-smoothness (and not it's inverse)
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // copy current velocity field (of the state)
    m_ptrTmpVelocityField->Copy( this->m_ptrState->GetVectorFieldPointer( iI ) );

    // convolve it with the inverse kernel, L^\dagger L v
    this->m_ptrKernel->ConvolveWithInverseKernel( m_ptrTmpVelocityField );

    // now multiply it with v
    T dCurrentEnergy = 0.5*this->m_vecTimeIncrements[ iI ]*m_ptrTmpVelocityField->ComputeInnerProduct( this->m_ptrState->GetVectorFieldPointer( iI ) );

    // add energy increment, assuring that we have the correct spatio-temporal volume contribution
    dEnergy += dCurrentEnergy;

    }

  T dVelocitySquareNorm = dEnergy;

  // now add the contributions of the data terms

  // create the current images according to the current state
  // (in case the velocities were updated externally by the optimizer for example)

  ComputeImagesForward();

  T dImageNorm = 0;

  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
    {
    // account for all possible measurements
    unsigned int uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
    for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM )
      {
      T dCurrentImageMetric = 1.0/m_SigmaSqr*this->m_pMetric->GetMetric( this->m_vecTimeDiscretization[ iI ].vecMeasurementImages[ iM ], this->m_vecTimeDiscretization[ iI ].vecEstimatedImages[0] );
      dImageNorm += dCurrentImageMetric;
      }

    }

  dEnergy += dImageNorm;

  //std::cout << "E = " << dEnergy << "; dV = " << dVelocitySquareNorm << "; dI = " << dImageNorm << std::endl;

  // write out the velocity, the image and the adjoint (everything basically)

  //VectorFieldUtils< T, ImageDimension >::writeTimeDependantImagesITK( this->m_ptrState->GetVectorPointerToVectorFieldPointer(), "vs.nrrd" );
  //VectorImageUtils< T, ImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "is.nrrd" );
  //VectorImageUtils< T, ImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "lambdas.nrrd" );
  //VectorImageUtils< T, ImageDimension >::writeFileITK( this->m_ptrKernel->GetKernel() , "kernel.nrrd");

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  return energyValues;

}

template < class TState >
void CLDDMMGrowthModelObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  std::cout << "saving gradient components at iteration " << uiIter << std::endl;

  outputPrefix = outputPrefix + "LDDMM-";

  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$
  //
  // Here the energy is defined as
  // \f$ E = 1/2 \int_0^1 \|v\|_L^2~dt + 1/(sigma^2)\|I(1)-I_1\|^2
  // and the gradient used is the Hilbert gradient

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

    VectorImageUtilsType::writeFileITK( (*m_ptrI)[iI], outputPrefix + "I" + CreateIntegerString( iI, 3 ) + suffix );
    VectorImageUtilsType::writeFileITK( (*m_ptrLambda)[iI], outputPrefix + "lam" + CreateIntegerString( iI, 3 ) + suffix );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtilsType::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpGradient );
      VectorImageUtilsType::writeFileITK( m_ptrTmpGradient, outputPrefix + "gradI" + CreateIntegerString( iI, 3 ) + suffix );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( (*m_ptrLambda)[ iI ], iD, m_ptrTmpGradient );
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

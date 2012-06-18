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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_OBJECTIVE_FUNCTON_TXX

template < class TState >
CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::CLDDMMGeometricMetamorphosisObjectiveFunction()
  : DefaultSigma1Sqr( 0.01 ),
    DefaultSigma2Sqr( 0.01 ),
    DefaultW( 0.5 ),
    m_ExternallySetSigma1Sqr( false ),
    m_ExternallySetSigma2Sqr( false ),
    m_ExternallySetW( false )
{
  m_Sigma1Sqr = DefaultSigma1Sqr;
  m_Sigma2Sqr = DefaultSigma2Sqr;
  m_W = DefaultW;

  // mask image at different time-points
  ptrT0 = NULL;
  ptrT2 = NULL;

  // measured images at different time-points
  ptrI0 = NULL;
  ptrI1 = NULL;

  // estimates at different time-points (subjected to the velocity field)
  ptrEstI1 = NULL;
  ptrEstT1 = NULL;
  ptrEstT2 = NULL;

  // storage difference images to control the image composition
  m_ptrT2M1 = NULL;
  m_ptrEstT1M1 = NULL;

  // temporary storage to compute maps
  m_ptrMapIn = NULL;
  m_ptrMapOut = NULL;
  m_ptrMapTmp = NULL;

  // temporary storage for adjoint differences
  m_ptrCurrentAdjointDifference = NULL;
  m_ptrCurrentAdjointDifferenceT = NULL;

  // temporary storage for adjoint values
  m_ptrCurrentLambdaEnd = NULL;
  m_ptrCurrentLambdaTEnd = NULL;
  
  // determinant of Jacobian
  m_ptrDeterminantOfJacobian = NULL;

  // temporary storage for composed images
  m_ptrI1Comp = NULL;
  m_ptrEstI1Comp = NULL;

  // temporary storage for velocity field and gradient
  m_ptrTmpVelocityField = NULL;
  m_ptrTmpGradient = NULL;

  // kernel pointer
  m_ptrMaskKernel = NULL;

}

template < class TState >
CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::~CLDDMMGeometricMetamorphosisObjectiveFunction()
{
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "GeometricMetamorphosis", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "GeometricMetamorphosis", Json::nullValue );

  SetJSONHelpForRootKey( GeometricMetamorphosis, "settings for geometric metamorphosis" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Sigma1Sqr );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, Sigma2Sqr );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, W );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigma1Sqr,
                     "1/Sigma1Sqr is the weight for the image match" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, Sigma2Sqr,
                     "1/Sigma2Sqr is the weight for the mask match" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, W,
                     "W is the weight between the energy terms in [0,1] and [1,2]" );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::SetMaskKernelPointer( KernelType * pKernel )
{
  m_ptrMaskKernel = pKernel;
}

template < class TState >
typename CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::KernelType *
CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetMaskKernelPointer()
{
  return m_ptrMaskKernel;
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::DeleteAuxiliaryStructures()
{
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::CreateAuxiliaryStructures()
{
// get the subject id's, there should only be two here!!
  std::vector< int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  // assumes a given set of subject indices for image (0) and mask (1)
  if ( !( vecSubjectIndices[ 0 ] == 0 && vecSubjectIndices[ 1 ] == 1 ) )
  {
    throw std::runtime_error( "Geometric metamorphosis requires two time-series, one with subject index 0 and the other one with subject index 1." );
    return;
  }

  // get the full time-course information for subject index 0 and 1
  std::vector< TimeSeriesDataPointType > timeSeries0;
  std::vector< TimeSeriesDataPointType > timeSeries1;

  this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeSeries0, 0 );
  this->m_ptrImageManager->GetTimeSeriesWithSubjectIndex( timeSeries1, 1 );

  // make sure that we only have two
  if ( !( timeSeries0.size()==2 && timeSeries1.size()==2 ) )
  {
    throw std::runtime_error( "Geometric metamorphosis requires exactly two time-points for each time-series." );
    return;
  }

  m_ImageInformation0 = timeSeries0[ 0 ];
  m_ImageInformation1 = timeSeries0[ 1 ];
  m_MaskInformation0 = timeSeries1[ 0 ];
  m_MaskInformation1 = timeSeries1[ 1 ];

  m_uiTimeIndexOfTimePoint1 = DetermineTimeIndexOfTimePoint1();

  // allocate all the auxiliary data structures
  
  // get images to graft the sizes for the images and the masks from
  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer( 0 );
  const VectorImageType* pGraftT = this->m_ptrImageManager->GetGraftImagePointer( 1 );

  // TODO: Fixme: Make sure that pGraftT is a scalar image
  assert( pGraftT->GetDimension() == 1 );

  // image and adjoint time-series
  m_ptrI.clear();
  m_ptrLambda.clear();

  // one more than for the velocity fields (but only as many as we need to time-point 1
  for ( unsigned int iI=0; iI <= m_uiTimeIndexOfTimePoint1 ; ++iI )
    {
    VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrI.push_back( ptrCurrentVectorImage );

    ptrCurrentVectorImage = new VectorImageType( pGraftIm );
    m_ptrLambda.push_back( ptrCurrentVectorImage );
    }
  
  // mask and adjoint time-series
  m_ptrT.clear();
  m_ptrLambdaT.clear();

  // one more than for the velocity fields
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
    {
    VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pGraftT );
    m_ptrT.push_back( ptrCurrentVectorImage );

    ptrCurrentVectorImage = new VectorImageType( pGraftT );
    m_ptrLambdaT.push_back( ptrCurrentVectorImage );
    }

  // storage difference images to control the image composition
  m_ptrT2M1 = new VectorImageType( pGraftT );
  m_ptrEstT1M1 = new VectorImageType( pGraftIm );

  // temporary storage to compute maps
  m_ptrMapIn = new VectorFieldType( pGraftIm );
  m_ptrMapOut = new VectorFieldType( pGraftIm );
  m_ptrMapTmp = new VectorFieldType( pGraftIm );

  // storage for intermediate values
  m_ptrCurrentAdjointDifference = new VectorImageType( pGraftIm );
  m_ptrCurrentAdjointDifferenceT = new VectorImageType( pGraftT );
  
  m_ptrCurrentLambdaEnd = new VectorImageType( pGraftIm );
  m_ptrCurrentLambdaTEnd = new VectorImageType( pGraftT );

  m_ptrDeterminantOfJacobian = new VectorImageType( pGraftT, 0.0, 1 );
  assert( m_ptrDeterminantOfJacobian->GetDimension() == 1 );

  m_ptrTmpVelocityField = new VectorFieldType( pGraftIm );
  m_ptrTmpGradient = new VectorFieldType( pGraftIm );

  m_ptrI1Comp = new VectorImageType( pGraftIm );
  m_ptrEstI1Comp = new VectorImageType( pGraftIm );

  // now make all the associations for the convenience pointers
  
  // mask image at different time-points
  ptrT0 = m_MaskInformation0.GetImage();
  ptrT2 = m_MaskInformation1.GetImage();

  // measured images at different time-points
  ptrI0 = m_ImageInformation0.GetImage();
  ptrI1 = m_ImageInformation1.GetImage();

  // estimates at different time-points (subjected to the velocity field)
  ptrEstI1 = m_ptrI[ m_uiTimeIndexOfTimePoint1 ];
  ptrEstT1 = m_ptrT[ m_uiTimeIndexOfTimePoint1 ];
  ptrEstT2 = m_ptrT[ this->m_vecTimeDiscretization.size()-1 ];

}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm )
{
  ptrIm->Copy( ptrI0 );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm, T dTime )
{
  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrI0, ptrIm );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrIm )
{
  ptrIm->Copy( ptrI1 );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrIm, T dTime )
{
  this->GetMapFromTo( m_ptrMapTmp, 2.0, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrI1, ptrIm );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetInitialImage( VectorImageType* ptrIm )
{
  ptrIm->Copy( ptrI0 );
}

template < class TState >
const typename CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::VectorImageType*
CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetPointerToInitialImage() const
{
  return ptrI0;
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetMomentum( VectorImageType* ptrMomentum, T dTime )
{
  ComputeImagesForward();
  ComputeAdjointBackward();

  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, m_ptrLambda[0], ptrMomentum );
  LDDMMUtils< T, TState::ImageDimension >::computeDeterminantOfJacobian( m_ptrMapTmp, m_ptrDeterminantOfJacobian );

  ptrMomentum->MultiplyElementwise( m_ptrDeterminantOfJacobian );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetInitialMomentum( VectorImageType* ptrMomentum )
{
  ptrMomentum->Copy( m_ptrLambda[0] );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetImageT( VectorImageType* ptrIm, T dTime )
{
  this->GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, TState::ImageDimension >::applyMap( m_ptrMapTmp, ptrT0, ptrIm );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::DetermineTimePointData( std::vector< STimePoint >& vecTimePointData )
{
  // get the subject ids
  std::vector< int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  unsigned int uiNumberOfDifferentSubjects = vecSubjectIndices.size();
  
  if ( uiNumberOfDifferentSubjects != 2 )
    {
    throw std::runtime_error( "CLDDMMGeometricMetamorphosisObjectiveFunction currently only supports two time-series at a time." );
    return;
    }

  // make sure we have exactly two time-points per time-series 
  // (the first one will be the image, the second one the mask)

  std::vector< T > vecTimepoints;

  this->m_ptrImageManager->GetTimepointsForSubjectIndex( vecTimepoints, vecSubjectIndices[ 0 ] );
  assert( vecTimepoints.size() == 2 );

  this->m_ptrImageManager->GetTimepointsForSubjectIndex( vecTimepoints, vecSubjectIndices[ 1 ] );
  assert( vecTimepoints.size() == 2 );

  // clear the time-point information vector

  vecTimePointData.clear();

  // now enter all the time-point information, assuming 0, 1, and 2 timepoints
  // do not enter the measured information, becasue we will strore this seperately for the geomet case

  STimePoint timePoint0;
  timePoint0.bIsMeasurementPoint = true;
  timePoint0.dTime = 0;

  vecTimePointData.push_back( timePoint0 );

  STimePoint timePoint1;
  timePoint1.bIsMeasurementPoint = true;
  timePoint1.dTime = 1;

  vecTimePointData.push_back( timePoint1 );

  STimePoint timePoint2;
  timePoint2.bIsMeasurementPoint = true;
  timePoint2.dTime = 2;

  vecTimePointData.push_back( timePoint2 );

}

template < class TState >
unsigned int CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::DetermineTimeIndexOfTimePoint1()
{
  unsigned int uiTimeIndexOfTimePoint1 = 0;
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
    {
    if ( this->m_vecTimeDiscretization[ iI ].dTime == 1 )
      {
      uiTimeIndexOfTimePoint1 = iI;
      break;
      }
    }
  return uiTimeIndexOfTimePoint1;
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeImagesForward()
{
  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( m_ptrMapIn );
  // FIXME: This is just to make things easier and to support estimating the initial image (todo) later
  m_ptrI[ 0 ]->Copy( ptrI0 );
  m_ptrT[ 0 ]->Copy( ptrT0 );

  // first compute forward until time point 1 for the image and the mask image

  for ( unsigned int iI = 0; iI < m_uiTimeIndexOfTimePoint1; ++iI )
    {
    this->m_ptrEvolver->SolveForward( this->m_ptrState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    // now compute the image by interpolation
    LDDMMUtilsType::applyMap( m_ptrMapIn, ptrI0, m_ptrI[ iI+1 ] );

    // now compute the mask image by interpolation
    LDDMMUtilsType::applyMap( m_ptrMapIn, ptrT0, m_ptrT[ iI+1 ] );
    }


  // keep on integrating for the mask image

  for ( unsigned int iI = m_uiTimeIndexOfTimePoint1; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    this->m_ptrEvolver->SolveForward( this->m_ptrState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    // now compute the mask image by interpolation
    LDDMMUtilsType::applyMap( m_ptrMapIn, ptrT0, m_ptrT[ iI+1 ] );
    }

}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeInvertedMask( const VectorImageType* ptrImIn, VectorImageType* ptrImOut )
{
  ptrImOut->Copy( ptrImIn );
  ptrImOut->AddConstant( -1 );
}


template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeCompositedImage( const VectorImageType* ptrImIn, const VectorImageType* ptrInvMask1, const VectorImageType* ptrInvMask2, VectorImageType* ptrImComp )
{
  ptrImComp->Copy( ptrImIn );
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInvMask1, 0, ptrImComp );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrInvMask2, 0, ptrImComp );
}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeAdjointBackward()
{

  // TODO: Fixme: Check how the gradients would change if other similiarity measures than SSD are applied

  // create the initial final conditions for the two adjoints

  m_ptrCurrentLambdaEnd->SetToConstant( 0 );
  m_ptrCurrentLambdaTEnd->SetToConstant( 0 );

  // first set the final condition for lambda T

  this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifferenceT, ptrEstT2, ptrT2 );
  m_ptrCurrentAdjointDifferenceT->MultiplyByConstant( 1.0/m_Sigma2Sqr );
  m_ptrCurrentLambdaTEnd->AddCellwise( m_ptrCurrentAdjointDifferenceT );

  // last adjoint; just for book-keeping, currently not really used in the algorithm
  m_ptrLambdaT[ this->m_vecTimeDiscretization.size()-1 ]->Copy( m_ptrCurrentLambdaTEnd );

  // now propagate it backward until timepoint 1

  // reset the map to flow backwards
  typedef LDDMMUtils< T, TState::ImageDimension > LDDMMUtilsType;
  LDDMMUtilsType::identityMap( m_ptrMapIn );

  for ( int iI = (int)this->m_vecTimeDiscretization.size()-1-1; iI >= (int)m_uiTimeIndexOfTimePoint1; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->Copy( this->m_ptrState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->MultiplyByConstant( -1 );

    this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentLambdaTEnd, m_ptrLambdaT[ iI ] );

    // compute det jacobian
    LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    m_ptrLambdaT[iI]->MultiplyElementwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    }

  // now we need to jump here

  // reset the current adjoint to the adjoint at current time point
  m_ptrCurrentLambdaTEnd->Copy( m_ptrLambdaT[ m_uiTimeIndexOfTimePoint1 ] );

  // compute the mask contributions: (T2-1) and (IT(1)-1)
  ComputeInvertedMask( ptrT2, m_ptrT2M1 );
  ComputeInvertedMask( ptrEstT1, m_ptrEstT1M1 );

  // now add the jump to currentlambdaTEnd
  // \lambda^\tau(1-) = \lambda^\tau(1+) -2/sigma1^2(I(1)-I_1)^2*(T2-1)^2(I^tau(1)-1)
  this->m_ptrMetric->GetLocalizedMetric( m_ptrCurrentAdjointDifferenceT, ptrEstI1, ptrI1 );
  m_ptrCurrentAdjointDifferenceT->MultiplyByConstant( 1.0/m_Sigma1Sqr );
  
  // now multiply it with (T2-1)^2 and (IT(1)-1) to get the final desired result for \lambda(1)
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifferenceT );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifferenceT );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrEstT1M1, 0, m_ptrCurrentAdjointDifferenceT );

  // add this to the currentLambdaTEnd to get the lambdaT with jump
  m_ptrCurrentLambdaTEnd->AddCellwise( m_ptrCurrentAdjointDifferenceT );

  // now compute the final condition for \lambda
  // \lambda(1) = -2/sigma1^2*(I(1)-I_1)*(T2-1)^2*(IT(1)-1)^2
  this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, ptrEstI1, ptrI1 );
  m_ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_Sigma1Sqr );

  // now multiply it with (T2-1)^2 and (IT(1)-1)^2 to get the final desired result for \lambda(1)
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifference );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifference );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrEstT1M1, 0, m_ptrCurrentAdjointDifference );
  VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrEstT1M1, 0, m_ptrCurrentAdjointDifference );

  // copy this to curentLambdaEnd to get the initial condition 
  m_ptrCurrentLambdaEnd->AddCellwise( m_ptrCurrentAdjointDifference );

  // reset the map to flow backwards, because we updated the current adjoint
  LDDMMUtilsType::identityMap( m_ptrMapIn );

  // now propagate from timepoint 1 back to timepoint 0 for both adjoints (for image and the mask)
  for ( int iI = (int)m_uiTimeIndexOfTimePoint1-1; iI >=0; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->Copy( this->m_ptrState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->MultiplyByConstant( -1 );

    this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
    
    // now compute the adjoints by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentLambdaTEnd, m_ptrLambdaT[ iI ] );
    LDDMMUtilsType::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnd, m_ptrLambda[ iI ] );
    
    // compute det jacobian
    LDDMMUtilsType::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    // TODO: Change the cellwise multiplication to support vector-valued adjoints
    m_ptrLambdaT[iI]->MultiplyElementwise( m_ptrDeterminantOfJacobian );
    m_ptrLambda[iI]->MultiplyElementwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->Copy( m_ptrMapOut );

    }

}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeGradient()
{

  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = 2(1-w)v + (L^\dagger L)^{-1}( (\sum_i \lambda_i \nabla I_i) + \lambda^tau\nabla I^\tau ); t\in[0,1)\f$
  // \f$ \nabla E = 2w v + (L^\dagger L)^{-1}( \lambda^\tau\nabla I^\tau ); t\in[1,2]\f$
  //
  // Here the energy is defined as
  // \f$ E = (1-w)\int_0^1 \|v\|_L^2~dt + w\int_1^2 \|v\|_L^2~dt 
  // + 1/(sigma1^2)Sim(I^c(I_1,I^\tau(1)(1),T_2),I^c(I(1),I^\tau(1),T_2)) + 1/(sigma2^2)Sim(I^\tau(2),T_2) \f$
  // and the gradient used is the Hilbert gradient

  unsigned int dim = ptrI0->GetDimension();

  typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  // first the inverval from [0,1)
  for ( unsigned int iI = 0; iI < m_uiTimeIndexOfTimePoint1; ++iI )
    {
    
    // initialize to 0
    VectorFieldType* ptrCurrentGradient = this->m_ptrGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->SetToConstant( 0.0 );

    // first add the components from the image
    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtilsType::computeCentralGradient( m_ptrI[ iI ], iD, m_ptrTmpGradient );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrLambda[ iI ], iD, m_ptrTmpGradient );
      ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
      }

    // now the one from the mask
    VectorFieldUtilsType::computeCentralGradient( m_ptrT[ iI ], 0, m_ptrTmpGradient );
    VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrLambdaT[ iI ], 0, m_ptrTmpGradient );
    ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );

    this->m_ptrKernel->ConvolveWithKernel( ptrCurrentGradient );

    // add 2*(1-w)v
    VectorFieldType* ptrCurrentVelocity = this->m_ptrState->GetVectorFieldPointer( iI );
    m_ptrTmpGradient->Copy( ptrCurrentVelocity );
    m_ptrTmpGradient->MultiplyByConstant( 2*(1-m_W) );
    ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );

    }

  // now the inverval from [1,2]
  for ( unsigned int iI = m_uiTimeIndexOfTimePoint1; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    VectorFieldType* ptrCurrentGradient = this->m_ptrGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->SetToConstant( 0 );

    // influence of the mask
    VectorFieldUtilsType::computeCentralGradient( m_ptrT[ iI ], 0, m_ptrTmpGradient );
    VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrLambdaT[ iI ], 0, m_ptrTmpGradient );
    ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );

    this->m_ptrMaskKernel->ConvolveWithKernel( ptrCurrentGradient );

    // add 2*w*v
    VectorFieldType* ptrCurrentVelocity = this->m_ptrState->GetVectorFieldPointer( iI );
    m_ptrTmpGradient->Copy( ptrCurrentVelocity );
    m_ptrTmpGradient->MultiplyByConstant( 2*m_W );
    ptrCurrentGradient->AddCellwise( m_ptrTmpGradient );
    }
}


template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
{
  /// uiKernelNumber == 0 -> kernel for the image-terms t\in[0,1]
  /// uiKernelNumber == 1 -> kernel for the mask-terms t\in(1,2]

  // shorthand for convenience
  VectorFieldType* ptrGradient = ptrInitialUnsmoothedVelocityGradient;

  typedef VectorFieldUtils< T, TState::ImageDimension > VectorFieldUtilsType;
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  if ( uiKernelNumber == 0 )
    {
      /** This is for the image-kernel.
        * For L2 and assuming v=0 initially the unsmoothed gradient becomes
        *
        \f[
        [2/\sigma1^2(I_0-I_1)^2(1-T_2)^2(1-T_1)-2/\sigma_2^2(T_1-T_2)]\nabla T_1 + \nabla I_0 \sum_i 2/\sigma_1^2(I_1-I_0)_i(1-T_1)^2(1-T_2)^2
        \f]
        *
        */
      ptrGradient->SetToConstant( 0.0 );
      unsigned int dim = ptrI0->GetDimension();

      // first add the components from the image

      // compute the adjoint related to the image, i.e., -\sigma_1^2(I_0-I_1)(1-T_1)^2(1-T_2)^2
      this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, ptrI0, ptrI1 );
      m_ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_Sigma1Sqr );

      // \lambda(1) = -2/sigma1^2*(I_0-I_1)*(T2-1)^2*(IT(1)-1)^2
      this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, ptrI0, ptrI1 );
      m_ptrCurrentAdjointDifference->MultiplyByConstant( 1.0/m_Sigma1Sqr );

      // for convencience of notation, use a scalar image as temporary storage
      VectorImageType* ptrTmp = m_ptrCurrentAdjointDifferenceT;

      // now multiply it with (T2-1)^2 and (T1-1)^2 to get the final desired result for \lambda(1)
      // compute the mask contribution: (T1-1)
      ComputeInvertedMask( ptrT0, ptrTmp );

      // multiply with (T1-1)^2
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrTmp, 0, m_ptrCurrentAdjointDifference );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrTmp, 0, m_ptrCurrentAdjointDifference );

      // compute the mask contribution: (T2-1)
      ComputeInvertedMask( ptrT2, ptrTmp );

      // multiply with (T2-1)^2
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrTmp, 0, m_ptrCurrentAdjointDifference );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( ptrTmp, 0, m_ptrCurrentAdjointDifference );

      // now do the multiplication dimension by dimension
      for ( unsigned int iD = 0; iD<dim; ++iD )
        {
        VectorFieldUtilsType::computeCentralGradient( ptrI0, iD, m_ptrTmpGradient );
        VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrCurrentAdjointDifference, iD, m_ptrTmpGradient );
        ptrGradient->AddCellwise( m_ptrTmpGradient );
        }

      // now add the mask component, i.e., [2/\sigma1^2(I_0-I_1)^2(1-T_2)^2(1-T_1)-2/\sigma_2^2(T_1-T_2)]\nabla T_1
      VectorFieldType* m_ptrTmpGradient2 = m_ptrTmpVelocityField; // convenience mapping

      VectorFieldUtilsType::computeCentralGradient( ptrT0, 0, m_ptrTmpGradient );
      m_ptrTmpGradient2->Copy( m_ptrTmpGradient );

      // now compute the final condition for \lambda
      // \lambda(1) = -2/sigma2^2*(T1-T2)
      this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifferenceT, ptrT0, ptrT2 );
      m_ptrCurrentAdjointDifferenceT->MultiplyByConstant( 1.0/m_Sigma2Sqr );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrCurrentAdjointDifferenceT, 0, m_ptrTmpGradient2 );

      ptrGradient->AddCellwise( m_ptrTmpGradient2 );

      // compute the mask contributions: (T2-1) and (IT(1)-1)
      ComputeInvertedMask( ptrT2, m_ptrT2M1 );
      ComputeInvertedMask( ptrT0, m_ptrEstT1M1 );

      // now add the jump to currentlambdaTEnd
      // \lambda^\tau(1-) = \lambda^\tau(1+) -2/sigma1^2(I0-I_1)^2*(T2-1)^2(T1-1)
      this->m_ptrMetric->GetLocalizedMetric( m_ptrCurrentAdjointDifferenceT, ptrI0, ptrI1 );
      m_ptrCurrentAdjointDifferenceT->MultiplyByConstant( 1.0/m_Sigma1Sqr );

      // now multiply it with (T2-1)^2 and (IT(1)-1) to get the final desired result for \lambda(1)
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifferenceT );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrT2M1, 0, m_ptrCurrentAdjointDifferenceT );
      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrEstT1M1, 0, m_ptrCurrentAdjointDifferenceT );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrCurrentAdjointDifferenceT, 0, m_ptrTmpGradient );

      ptrGradient->AddCellwise( m_ptrTmpGradient );

    }
  else if ( uiKernelNumber == 1 )
    {
      /** This is for the mask kernel.
      * For L2 and assuming v=0 initially the unsmoothed gradient becomes:
      \f[
      \nabla E = \lambda^\tau \nabla I^\tau = -\frac{2}{\sigma_2^2}(T_1-T_2)\nabla T_1
      \f]
      */
      // influence of the mask
      VectorFieldUtilsType::computeCentralGradient( ptrT0, 0, ptrGradient );

      // now compute the final condition for \lambda
      // \lambda(1) = -2/sigma2^2*(T1-T2)
      this->m_ptrMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifferenceT, ptrT0, ptrT2 );
      m_ptrCurrentAdjointDifferenceT->MultiplyByConstant( 1.0/m_Sigma2Sqr );

      VectorImageUtilsType::multiplyVectorByImageDimensionInPlace( m_ptrCurrentAdjointDifferenceT, 0, ptrGradient );
    }
  else
    {
      std::cerr << "WARNING: Unknown kernel number: returning 0 gradient field." << std::endl;
      ptrGradient->SetToConstant( 0.0 );
    }

}

template < class TState >
typename CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::CEnergyValues
CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::GetCurrentEnergy()
{
  // Here the energy is defined as
  // \f$ E = (1-w)\int_0^1 \|v\|_L^2~dt + w\int_1^2 \|v\|_L^2~dt
  // + 1/(sigma1^2)Sim(I^c(I_1,I^\tau(1)(1),T_2),I^c(I(1),I^\tau(1),T_2)) + 1/(sigma2^2)Sim(I^\tau(2),T_2) \f$

  T dEnergy = 0;

  // first the inverval from [0,1)
  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // copy current velocity field (of the state)
    m_ptrTmpVelocityField->Copy( this->m_ptrState->GetVectorFieldPointer( iI ) );

    // convolve it with the inverse kernel, L^\dagger L v
    if ( iI < m_uiTimeIndexOfTimePoint1 )
      {
      this->m_ptrKernel->ConvolveWithInverseKernel( m_ptrTmpVelocityField );
      }
    else
      {
      this->m_ptrMaskKernel->ConvolveWithInverseKernel( m_ptrTmpVelocityField );
      }

    // now multiply it with v and (1-w)
    T dCurrentEnergy = this->m_vecTimeIncrements[ iI ]*m_ptrTmpVelocityField->ComputeInnerProduct( this->m_ptrState->GetVectorFieldPointer( iI ) );
    if ( iI < m_uiTimeIndexOfTimePoint1 )
      {
      dCurrentEnergy *= (1-m_W);
      }
    else
      {
      dCurrentEnergy *= m_W;
      }

    // add energy increment, assuring that we have the correct spatio-temporal volume contribution
    dEnergy += dCurrentEnergy;

    }

  T dVelocitySquareNorm = dEnergy;

  // now add the contributions of the data terms
  
  // create the current images according to the current state 
  // (in case the velocities were updated externally by the optimizer for example)

  ComputeImagesForward();

  T dImageNorm = 0;

  // ptrI1, ptrT2, ptrEstI1, ptrEstT1, ptrEstT2

  // compute the mask contributions: (T2-1) and (IT(1)-1)
  ComputeInvertedMask( ptrT2, m_ptrT2M1 );
  ComputeInvertedMask( ptrEstT1, m_ptrEstT1M1 );

  // multiply I1 and I(1) with it to get the "masked" images, on which we can compute the image similarity term
  ComputeCompositedImage( ptrI1, m_ptrEstT1M1, m_ptrT2M1, m_ptrI1Comp );
  ComputeCompositedImage( ptrEstI1, m_ptrEstT1M1, m_ptrT2M1, m_ptrEstI1Comp );

  // foreground part

  T dCurrentImageMetric = 1.0/m_Sigma1Sqr*this->m_ptrMetric->GetMetric( m_ptrI1Comp, m_ptrEstI1Comp );

  dImageNorm += dCurrentImageMetric;

  // and background part
  dCurrentImageMetric = 1.0/m_Sigma2Sqr*this->m_ptrMetric->GetMetric( ptrEstT2, ptrT2 );

  dImageNorm += dCurrentImageMetric;

  dEnergy += dImageNorm;

  CEnergyValues energyValues;
  energyValues.dEnergy = dEnergy;
  energyValues.dRegularizationEnergy = dVelocitySquareNorm;
  energyValues.dMatchingEnergy = dImageNorm;

  return energyValues;

}

template < class TState >
void CLDDMMGeometricMetamorphosisObjectiveFunction< TState >::OutputStateInformation( unsigned int uiIter, std::string outputPrefix )
{
  ComputeImagesForward();
  std::string sGeometrRes = "geometRes-";
  typedef VectorImageUtils< T, TState::ImageDimension > VectorImageUtilsType;
  VectorImageUtilsType::writeFileITK( ptrEstT1, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-Est-T1.nrrd" ) );
  VectorImageUtilsType::writeFileITK( ptrEstT2, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-Est-T2.nrrd" ) );
  VectorImageUtilsType::writeFileITK( ptrEstI1, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-Est-I1.nrrd" ) );

  // blurred images and masks
  VectorImageUtilsType::writeFileITK( ptrI0, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-I0-saved-orig.nrrd" ) );
  VectorImageUtilsType::writeFileITK( ptrI1, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-I1-saved-orig.nrrd" ) );
  VectorImageUtilsType::writeFileITK( ptrT0, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-T0-saved-orig.nrrd" ) );
  VectorImageUtilsType::writeFileITK( ptrT2, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-T2-saved-orig.nrrd" ) );

  // compute the mask contributions: (T2-1) and (IT(1)-1)
  ComputeInvertedMask( ptrT2, m_ptrT2M1 );
  ComputeInvertedMask( ptrEstT1, m_ptrEstT1M1 );

  // multiply I1 and I(1) with it to get the "masked" images, on which we can compute the image similarity term
  ComputeCompositedImage( ptrI1, m_ptrEstT1M1, m_ptrT2M1, m_ptrI1Comp );
  ComputeCompositedImage( ptrEstI1, m_ptrEstT1M1, m_ptrT2M1, m_ptrEstI1Comp );

  VectorImageUtilsType::writeFileITK( m_ptrI1Comp, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-I1-comp.nrrd" ) );
  VectorImageUtilsType::writeFileITK( m_ptrEstI1Comp, outputPrefix + CreateNumberedFileName( sGeometrRes, uiIter, "-Est-I1-comp.nrrd" ) );
}

#endif

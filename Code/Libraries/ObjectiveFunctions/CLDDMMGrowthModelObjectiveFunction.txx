#ifndef C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_TXX
#define C_LDDMM_GROWTH_MODEL_OBJECTIVE_FUNCTION_TXX

template <class T, unsigned int VImageDimension, class TState >
CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::CLDDMMGrowthModelObjectiveFunction()
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


template <class T, unsigned int VImageDimension, class TState >
CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::~CLDDMMGrowthModelObjectiveFunction()
{
  DeleteAuxiliaryStructures();
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::SetAutoConfiguration( Json::Value& ConfValue )
{
  Superclass::SetAutoConfiguration( ConfValue );
  Json::Value& currentConfiguration = this->m_jsonConfig.GetFromKey( "GrowthModel", Json::nullValue );

  SetJSONSigmaSqr( this->m_jsonConfig.GetFromKey( currentConfiguration, "SigmaSqr", GetExternalOrDefaultSigmaSqr() ).asDouble() );

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::DeleteAuxiliaryStructures()
{
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapIn );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapOut );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrMapTmp );

  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpVelocityField );
  SaveDelete< VectorFieldPointerType >::Pointer( m_ptrTmpGradient );

  SaveDelete< VectorImagePointerType >::Pointer( m_ptrI0 );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentLambdaEnd );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrCurrentAdjointDifference );
  SaveDelete< VectorImagePointerType >::Pointer( m_ptrDeterminantOfJacobian );

  SaveDelete< VectorImagePointerType >::PointerVector( m_ptrI );
  SaveDelete< VectorImagePointerType >::PointerVector( m_ptrLambda );
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::CreateAuxiliaryStructures()
{

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  assert( vecSubjectIndices.size()>0 );

  // obtain image from which to graft the image information for the data structures
  const VectorImageType* pGraftIm = this->m_ptrImageManager->GetGraftImagePointer();

  // allocate all the auxiliary data

  // image and adjoint time-series
  m_ptrI = new std::vector< VectorImagePointerType >;
  m_ptrLambda = new std::vector< VectorImagePointerType >;

  // storage for the initial image
  // and initialize it with the first image of the time-series
  m_ptrI0 = new VectorImageType( pGraftIm );

  // one more than for the velocity fields
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size(); ++iI )
    {
    VectorImagePointerType ptrCurrentVectorImage = new VectorImageType( pGraftIm ); 
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
  // TODO: Fixme: fix initialization if a vector-valued image is used, needs to be a scalar-image
  assert( pGraftIm->getDim() == 1 );
  m_ptrDeterminantOfJacobian  = new VectorImageType( pGraftIm );

  // storage for the negated velocity field
  m_ptrTmpVelocityField = new VectorFieldType( pGraftIm );

  // storage for the temporary gradient
  m_ptrTmpGradient = new VectorFieldType( pGraftIm );

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::GetImage( VectorImageType* ptrIm, T dTime )
{
  // TODO: account for appearance changes, based on closeby images
  GetMap( m_ptrMapTmp, dTime );
  // now compute the image by interpolation
  LDDMMUtils< T, VImageDimension >::applyMap( m_ptrMapTmp, m_ptrI0, ptrIm );
}

template  <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::DetermineTimePointData( std::vector< STimePoint >& vecTimePointData )
{

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  unsigned int uiNumberOfDifferentSubjects = vecSubjectIndices.size();
  
  if ( uiNumberOfDifferentSubjects != 1 )
    {
    throw std::runtime_error( "CLDDMMSpatioTemporalVelocityFieldObjectiveFunction currently only supports one subject at a time." );
    return;
    }

  std::vector< T > vecMeasurementTimepoints;
  // make sure we have at least two timepoints
  this->m_ptrImageManager->GetTimepointsForSubjectIndex( vecMeasurementTimepoints, vecSubjectIndices[ 0 ] );
  assert( vecMeasurementTimepoints.size() > 1 );

  std::cout << "Measurement timepoints = " << vecMeasurementTimepoints << std::endl;

  // get the full time-course information for the subject
  SubjectInformationType* pSubjectInfo;
  this->m_ptrImageManager->GetImagesWithSubjectIndex( pSubjectInfo, vecSubjectIndices[ 0 ] );

  // clear the time-point information vector

  vecTimePointData.clear();

  // now enter all the time-point information based on the subject information

  // go through all the timepoints and enter them into the vecTimeDiscretization structure

  typename SubjectInformationType::iterator iter;
 
  // TODO: just make the image manager return this type of data-structure by default

  T dLastTimePoint = 0;
  bool bFirstValue = true;

  for ( iter = pSubjectInfo->begin(); iter != pSubjectInfo->end(); ++iter )
    {

    if ( !bFirstValue && ( dLastTimePoint == (*iter)->timepoint ) )
      {
      // if there is a multiple measurement
      unsigned int uiLast = vecTimePointData.size()-1;
      vecTimePointData[ uiLast ].vecMeasurementImages.push_back( (*iter)->pIm );
      vecTimePointData[ uiLast ].vecMeasurementTransforms.push_back( (*iter)->pTransform );
      }
    else
      {
      // fill the time-point stucture
      STimePoint timePoint;
      timePoint.bIsMeasurementPoint = true; // all of them are measurements
      timePoint.dTime = (*iter)->timepoint;
      timePoint.vecMeasurementImages.push_back( (*iter)->pIm );
      timePoint.vecMeasurementTransforms.push_back( (*iter)->pTransform );

      vecTimePointData.push_back( timePoint );

      }

      dLastTimePoint = (*iter)->timepoint;
      bFirstValue = false;

    }

}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::ComputeImagesForward()
{
  LDDMMUtils< T, VImageDimension >::identityMap( m_ptrMapIn );
  // FIXME: This is just to make things easier and to support estimating the initial image (todo) later 
  (*m_ptrI)[ 0 ]->copy( m_ptrI0 );
  
  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {

    this->m_ptrEvolver->SolveForward( this->m_pState->GetVectorFieldPointer( iI ), m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );

    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );

    // now compute the image by interpolation
    LDDMMUtils< T, VImageDimension >::applyMap( m_ptrMapIn, m_ptrI0, (*m_ptrI)[ iI+1 ] );
    
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::ComputeAdjointBackward()
{
  // create the final condition

  m_ptrCurrentLambdaEnd->setConst( 0 );
  
  unsigned int uiNrOfTimePoints = this->m_vecTimeDiscretization.size();
  unsigned int uiNrOfMeasuredImagesAtTimePoint = 0;

  uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages.size();
  // first set the final condition
  for ( unsigned int iM = 0; iM <  uiNrOfMeasuredImagesAtTimePoint; ++iM ) 
    {
    this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecEstimatedImages[0] , this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages[ iM ] );
    m_ptrCurrentAdjointDifference->multConst( 1.0/m_SigmaSqr );
    m_ptrCurrentLambdaEnd->addCellwise( m_ptrCurrentAdjointDifference );
    }
  
  // last adjoint; just for book-keeping, currently not really used in the algorithm
  (*m_ptrLambda)[ uiNrOfTimePoints-1 ]->copy( m_ptrCurrentLambdaEnd );

  // reset the map to flow backwards
  LDDMMUtils<T,VImageDimension>::identityMap( m_ptrMapIn );

  for ( int iI = (int)this->m_vecTimeDiscretization.size()-1-1; iI >= 0; --iI )
    {

    // need to reverse the velocity field, because we are evolving in the backward direction
    m_ptrTmpVelocityField->copy( this->m_pState->GetVectorFieldPointer( iI ) );
    m_ptrTmpVelocityField->multConst( -1 );

    this->m_ptrEvolver->SolveForward( m_ptrTmpVelocityField, m_ptrMapIn, m_ptrMapOut, m_ptrMapTmp, this->m_vecTimeIncrements[ iI ] );
    
    // now compute the adjoint by interpolation and exploiting the determinant of the Jacobian
    LDDMMUtils<T,VImageDimension>::applyMap( m_ptrMapOut, m_ptrCurrentLambdaEnd, (*m_ptrLambda)[ iI ] );
    
    // compute det jacobian
    LDDMMUtils<T,VImageDimension>::computeDeterminantOfJacobian( m_ptrMapOut, m_ptrDeterminantOfJacobian );
    // multiply by the determinant of the Jacobian
    (*m_ptrLambda)[iI]->multCellwise( m_ptrDeterminantOfJacobian );

    // for next step, copy
    m_ptrMapIn->copy( m_ptrMapOut );

    // update if we need to jump at the current time-point
    if ( this->m_vecTimeDiscretization[ iI ].bIsMeasurementPoint )
      {
      // reset the current adjoint to the adjoint at current time point
      m_ptrCurrentLambdaEnd->copy( (*m_ptrLambda)[ iI ] );
      // reset the map to flow backwards, because we update the current adjoint
      LDDMMUtils<T,VImageDimension>::identityMap( m_ptrMapIn );
      
      // account for all possible jumps of the adjoint at this time-point
      uiNrOfMeasuredImagesAtTimePoint = this->m_vecTimeDiscretization[ iI ].vecMeasurementImages.size();
      for ( unsigned int iM = 0; iM < uiNrOfMeasuredImagesAtTimePoint; ++iM ) 
        {
        this->m_pMetric->GetAdjointMatchingDifferenceImage( m_ptrCurrentAdjointDifference, this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecEstimatedImages[0] , this->m_vecTimeDiscretization[ uiNrOfTimePoints-1 ].vecMeasurementImages[ iM ] );
        m_ptrCurrentAdjointDifference->multConst( 1.0/m_SigmaSqr );
        m_ptrCurrentLambdaEnd->addCellwise( m_ptrCurrentAdjointDifference );
        }
      }
    }
}

template <class T, unsigned int VImageDimension, class TState >
void CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::ComputeGradient()
{

  ComputeImagesForward();
  ComputeAdjointBackward();

  // can compute the gradient from this
  // \f$ \nabla E = v + (L^\dagger L)^{-1}(\sum_i \lambda_i \nabla I_i ) \f$
  //
  // Here the energy is defined as
  // \f$ E = 1/2 \int_0^1 \|v\|_L^2~dt + 1/(sigma^2)\|I(1)-I_1\|^2
  // and the gradient used is the Hilbert gradient

  unsigned int dim = m_ptrI0->getDim();

  for ( unsigned int iI = 0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // initialize to 0
    VectorFieldType* ptrCurrentGradient = this->m_pGradient->GetVectorFieldPointer( iI );
    ptrCurrentGradient->setConst( 0 );

    for ( unsigned int iD = 0; iD<dim; ++iD )
      {
      VectorFieldUtils< T, VImageDimension >::computeCentralGradient( (*m_ptrI)[ iI ], iD, m_ptrTmpGradient );
      VectorImageUtils< T, VImageDimension >::multiplyVectorByImageDimensionInPlace( (*m_ptrLambda)[ iI ], iD, m_ptrTmpGradient );
      ptrCurrentGradient->addCellwise( m_ptrTmpGradient );
      }

    //VectorImageUtils< T, VImageDimension >::writeFileITK( ptrCurrentGradient, "curGradBeforeConv.nrrd" );

    this->m_ptrKernel->ConvolveWithInverseKernel( ptrCurrentGradient );

    //VectorImageUtils< T, VImageDimension >::writeFileITK( ptrCurrentGradient, "curGradAfterConv.nrrd" );

    // add v 
    VectorFieldType* ptrCurrentVelocity = this->m_pState->GetVectorFieldPointer( iI );
    ptrCurrentGradient->addCellwise( ptrCurrentVelocity );

    }

  //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_pGradient->GetVectorPointerToVectorFieldPointer(), "gradientAfterComputation.nrrd" );

}

template <class T, unsigned int VImageDimension, class TState >
T CLDDMMGrowthModelObjectiveFunction< T, VImageDimension, TState >::GetCurrentEnergy()
{

  T dEnergy = 0;

  // computing the square velocity for this time step using the kernel (and not it's inverse)
  for ( unsigned int iI=0; iI < this->m_vecTimeDiscretization.size()-1; ++iI )
    {
    // copy current velocity field (of the state)
    m_ptrTmpVelocityField->copy( this->m_pState->GetVectorFieldPointer( iI ) );

    // convolve it with the kernel, L^\dagger L v
    this->m_ptrKernel->ConvolveWithKernel( m_ptrTmpVelocityField );

    // now multiply it with v
    T dCurrentEnergy = 0.5*this->m_vecTimeIncrements[ iI ]*m_ptrTmpVelocityField->computeInnerProduct( this->m_pState->GetVectorFieldPointer( iI ) );

    // add energy increment, assuring that we have the correct spatio-temporal volume contribution
    dEnergy += dCurrentEnergy;

    }

  //T dVelocitySquareNorm = dEnergy;

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

  //VectorFieldUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_pState->GetVectorPointerToVectorFieldPointer(), "vs.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "is.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeTimeDependantImagesITK( this->m_ptrI, "lambdas.nrrd" );
  //VectorImageUtils< T, VImageDimension >::writeFileITK( this->m_ptrKernel->GetKernel() , "kernel.nrrd");

  return dEnergy;

}



#endif

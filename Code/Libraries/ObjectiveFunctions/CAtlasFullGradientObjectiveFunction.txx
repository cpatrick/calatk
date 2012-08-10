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

#ifndef C_ATLAS_FULL_GRADIENT_OBJECTIVE_FUNCTION_TXX
#define C_ATLAS_FULL_GRADIENT_OBJECTIVE_FUNCTION_TXX

template < class TState >
CAtlasFullGradientObjectiveFunction< TState >::CAtlasFullGradientObjectiveFunction()
{
}

template < class TState >
CAtlasFullGradientObjectiveFunction< TState >::~CAtlasFullGradientObjectiveFunction()
{
}

template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
}


template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::InitializeState()
{
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers
  // TODO: Have a true vectorized implementation, so we can use it with the general optimizers
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      (*iter)->InitializeState();
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< typename IndividualStateType::Pointer > vecState;
  std::vector< typename IndividualStateType::Pointer > vecGradient;

  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      vecState.push_back( (*iter)->GetStatePointer() );
      vecGradient.push_back( (*iter)->GetGradientPointer() );
    }

  // we need the intial image, which is the current atlas image
  typename VectorImageType::Pointer currentAtlasImage = this->m_ptrImageManager->GetOnlyCommonTimePointSavely( )->GetImage();
  typename VectorImageType::Pointer zeroImage = new VectorImageType( currentAtlasImage );
  zeroImage->SetToConstant( 0 );

  // allocate memory for the state, the objective function takes full control over the
  // memory and is therefore also responsible to free the memory in the constructor
  // all the pointers are aready stored in m_VectorObjectiveFunctionPtrs

  std::cout << "DEBUG: Initialized with current atlas image!" << std::endl;

  // associate the allocated memory with the atlas state
  this->m_ptrState = new TState( currentAtlasImage, vecState );

  // associate the allocated memory with the atlas gradient
  this->m_ptrGradient = new TState( zeroImage, vecGradient );

}

template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::InitializeState( TState *ptrState )
{
  // need to initialize all the individual states (based on the input state) and then create a new joint state vector which will form the new atlas state
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers

  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  unsigned int iI;
  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(), iI=0; iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iI, ++iter )
    {
    (*iter)->InitializeState( ptrState->GetIndividualStatePointer( iI ) );
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< typename IndividualStateType::Pointer > vecState;
  std::vector< typename IndividualStateType::Pointer > vecGradient;

  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      vecState.push_back( (*iter)->GetStatePointer() );
      vecGradient.push_back( (*iter)->GetGradientPointer() );
    }

  // allocate memory for the state, the objective function takes full control over the
  // memory and is therefore also responsible to free the memory in the constructor
  // all the pointers are aready stored in m_VectorObjectiveFunctionPtrs

  typename VectorImageType::Pointer zeroImage = new VectorImageType( ptrState->GetImageState() );
  zeroImage->SetToConstant( 0 );

  // associate the allocated memory with the atlas state
  this->m_ptrState = new TState( ptrState->GetImageState(), vecState );

  // associate the allocated memory with the atlas gradient
  this->m_ptrGradient = new TState( zeroImage, vecGradient );
}

template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm )
{
  // this should be the initial image of the atlas-builder, same as for the individual registration
  // if the atlas-image is indeed at the center
  throw std::runtime_error( "GetSourceImage: not yet implemented" );
}

// this is the gradient given all the individual registrations, i.e., just the concatenation of all the individual gradients
// concatenation will be automatic, because the states of the objective functions have been associated with the atlas state previously
template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::ComputeGradient()
{
  // this depends it the atlas image is source or target image
  // In any case, we need the adjoint variables

  // compute them for each and create a state which contains pointers to all of the gradients
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      // weights for the energies have been set previously, so this gradient also includes the weighting terms
      (*iter)->ComputeGradient();  // this automatically ends up in the gradient vector and also computes the adjoints
    }

  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer imageGradient = this->m_ptrGradient->GetImageState();

  imageGradient->SetToConstant( 0.0 );

  if ( this->m_AtlasIsSourceImage )
  {
      typename VectorImageType::Pointer tmpImage = new VectorImageType( imageGradient );
      typename VectorImageType::Pointer tmpTargetImage = new VectorImageType( imageGradient );
      typename VectorImageType::Pointer determinantOfJacobian = new VectorImageType( imageGradient, 0.0, 1 );
      typename VectorFieldType::Pointer tmpMap = new VectorFieldType( imageGradient );

      for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
        {
          (*iter)->GetMapFromTo( tmpMap, 1.0, 0.0 );
          LDDMMUtilsType::computeDeterminantOfJacobian( tmpMap, determinantOfJacobian );

          (*iter)->GetTargetImage( tmpTargetImage );

          LDDMMUtilsType::applyMap( tmpMap, tmpTargetImage, tmpImage );

          tmpImage->SubtractCellwise( this->m_ptrState->GetImageState() );
          tmpImage->MultiplyElementwise( determinantOfJacobian );
          tmpImage->MultiplyByConstant( 2.0/(*iter)->GetSigmaSqr() );; // no - here, because order of subtraction includes it

          imageGradient->SubtractCellwise( tmpImage );

        }
  }
  else // atlas is target image
  {
    typename VectorImageType::Pointer tmpImage = new VectorImageType( imageGradient );

    int it = 0;
    for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
      {
        (*iter)->GetSourceImage( tmpImage, 1.0 );
        VectorImageUtilsType::writeFileITK( tmpImage, CreateNumberedFileName( "sourceImage", it, ".nrrd" ));
        tmpImage->SubtractCellwise( this->m_ptrState->GetImageState() );
        // now multiply by -2/sigma^2, because we assume SSD for the similarity measure to build the atlas
        tmpImage->MultiplyByConstant( -2.0/(*iter)->GetSigmaSqr() );

        imageGradient->AddCellwise( tmpImage );
        it++;
      }
  }

}

template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::PreSubIterationSolve()
{
  // nothing so far
}

template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::OutputStateInformation( unsigned int iter, std::string outputPrefix )
{
  std::string suffix = "-iter-" + CreateIntegerString( iter, 3 ) + ".nrrd";

  VectorImageType* currentAtlasImage = this->m_ptrState->GetImageState();
  typedef VectorImageUtils< FloatType, TState::ImageDimension > VectorImageUtilsType;
  VectorImageUtilsType::writeFileITK( currentAtlasImage, outputPrefix + "atlas" + suffix );
}

#endif


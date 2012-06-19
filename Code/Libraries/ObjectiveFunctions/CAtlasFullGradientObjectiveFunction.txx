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
void CAtlasFullGradientObjectiveFunction< TState >::GetInitialImage( VectorImageType* ptrIm )
{
  // this should be the initial image of the atlas-builder, same as for the individual registration
  // if the atlas-image is indeed at the center
  throw std::runtime_error( "GetInitialImage: not yet implemented" );
}

template < class TState >
const typename CAtlasFullGradientObjectiveFunction< TState >::VectorImageType*
CAtlasFullGradientObjectiveFunction< TState >::GetPointerToInitialImage() const
{
  throw std::runtime_error( "GetPointerToInitialImage: not yet implemented" );
  return NULL;
}

// this is the gradient given all the individual registrations, i.e., just the concatenation of all the individual gradients
// concatenation will be automatic, because the states of the objective functions have been associated with the atlas state previously
template < class TState >
void CAtlasFullGradientObjectiveFunction< TState >::ComputeGradient()
{
  // this depends it the atlas image is source or target image
  // In any case, we need the adjoint variables

  // compute them for each and create a state which contains pointers to all of the gradients
  // TODO: also multiply the gradients by the weights
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  typename std::vector< FloatType >::const_iterator iterWeights;
  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(), iterWeights=this->m_Weights.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter, ++iterWeights )
    {
      (*iter)->ComputeGradient();  // this automatically ends up in the gradient vector and also computes the adjoints
      // TODO: multiply by weight, this should be handled by the individual algorithms, don't want to modift values here
      //IndividualStateType* currentGradientPointer = (*iter)->GetGradientPointer();
      //*currentGradientPointer *= *iterWeights;
    }

  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer imageGradient = this->m_ptrGradient->GetImageState();
  typename VectorImageType::Pointer tmpMomentum = new VectorImageType( imageGradient );
  imageGradient->SetToConstant( 0.0 );

  if ( this->m_AtlasIsSourceImage )
  {
    for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(), iterWeights=this->m_Weights.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter, ++iterWeights )
      {
        (*iter)->GetInitialMomentum( tmpMomentum );
        imageGradient->SubtractCellwise( tmpMomentum );
      }
  }
  else
  {
    for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(), iterWeights=this->m_Weights.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter, ++iterWeights )
      {
        (*iter)->GetMomentum( tmpMomentum, 1.0 );
        imageGradient->SubtractCellwise( tmpMomentum );
      }
  }

  //VectorImageUtilsType::writeFileITK( imageGradient, "imageGradient.nrrd" );

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


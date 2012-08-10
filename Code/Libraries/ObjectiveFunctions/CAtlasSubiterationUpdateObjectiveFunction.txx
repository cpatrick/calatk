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

#ifndef C_ATLAS_SUBITERATION_UPDATE_OBJECTIVE_FUNCTION_TXX
#define C_ATLAS_SUBITERATION_UPDATE_OBJECTIVE_FUNCTION_TXX

template < class TState >
CAtlasSubiterationUpdateObjectiveFunction< TState >::CAtlasSubiterationUpdateObjectiveFunction()
{
}

template < class TState >
CAtlasSubiterationUpdateObjectiveFunction< TState >::~CAtlasSubiterationUpdateObjectiveFunction()
{
}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
}


template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::InitializeState()
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

  // allocate memory for the state, the objective function takes full control over the
  // memory and is therefore also responsible to free the memory in the constructor
  // all the pointers are aready stored in m_VectorObjectiveFunctionPtrs

  // associate the allocated memory with the atlas state
  this->m_ptrState = new TState( vecState );

  // associate the allocated memory with the atlas gradient
  this->m_ptrGradient = new TState( vecGradient );

}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::InitializeState( TState *ptrState )
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

  // associate the allocated memory with the atlas state
  this->m_ptrState = new TState( vecState );

  // associate the allocated memory with the atlas gradient
  this->m_ptrGradient = new TState( vecGradient );

}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrIm )
{
  // this should be the initial image of the atlas-builder, same as for the individual registration
  // if the atlas-image is indeed at the center
  throw std::runtime_error( "GetSourceImage: not yet implemented" );
}

// this is the gradient given all the individual registrations, i.e., just the concatenation of all the individual gradients
// concatenation will be automatic, because the states of the objective functions have been associated with the atlas state previously
template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::ComputeGradient()
{
  // compute them for each and create a state which contains pointers to all of the gradients
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;

  int iI=0;
  for ( iter=this->m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=this->m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      // weights are included in the gradient
      (*iter)->ComputeGradient();  // this automatically ends up in the gradient vector

      VectorImageUtilsType::writeFileITK( (*iter)->GetGradientPointer()->GetPointerToInitialMomentum(), CreateNumberedFileName( "gradient", iI, ".nrrd" ));

      iI++;
    }

  // We don't compute the gradient with respect to the atlas image. This is handled explicitly in an outside loop in the atlas-builder algorithm.
  // While this is not the full gradient with respect to all unknowns it allows for a generic atlas-building-framework compatible with all
  // the component algorithms.
  // TODO: Investigate if it would make a difference in practice to implement the full algorithm

}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::PreSubIterationSolve()
{
  // replace the source / target image by its weighted average, unless the atlas-image is considered as part of the gradient

  std::cout << "Executing subiteration solve." << std::endl;

  if ( this->m_AtlasIsSourceImage )
  {
    UpdateAtlasImageAsAverageOfTargetImages();
  }
  else
  {
    UpdateAtlasImageAsAverageOfSourceImages();
  }

}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::UpdateAtlasImageAsAverageOfTargetImages()
{
  std::cout << "Updating atlas image as average of target images." << std::endl;

  std::cerr << "WARNING: This atlas builder type does not stable. Use at your own risk." << std::endl;


  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer currentAtlasImage = this->m_ptrImageManager->GetOnlyCommonTimePointSavely( )->GetImage();

  typename VectorImageType::Pointer newAtlasImage = new VectorImageType( currentAtlasImage );

  unsigned int numberOfObjectiveFunctions = this->m_VectorIndividualObjectiveFunctionPtrs.size();

  newAtlasImage->SetToConstant( 0 );

  typename VectorImageType::Pointer determinantOfJacobian = new VectorImageType( newAtlasImage, 0.0, 1 );
  typename VectorImageType::Pointer tmpImage = new VectorImageType( newAtlasImage );
  typename VectorImageType::Pointer currentTargetImage = new VectorImageType( newAtlasImage );
  typename VectorFieldType::Pointer tmpMap = new VectorFieldType( newAtlasImage );
  typename VectorImageType::Pointer weightImage = new VectorImageType( newAtlasImage );

  weightImage->SetToConstant( 0 );

  for ( unsigned int iI=0; iI < numberOfObjectiveFunctions; ++iI )
  {   
    typename IndividualObjectiveFunctionType::Pointer currentObjectiveFunction = this->m_VectorIndividualObjectiveFunctionPtrs[ iI ];

    // get the inverse map (so we can move the target image to the source (the atlas image)
    currentObjectiveFunction->GetMapFromTo( tmpMap, 1.0, 0.0 );
    LDDMMUtilsType::computeDeterminantOfJacobian( tmpMap, determinantOfJacobian );

    currentObjectiveFunction->GetTargetImage( currentTargetImage );

    LDDMMUtilsType::applyMap( tmpMap, currentTargetImage, tmpImage );

    tmpImage->MultiplyElementwise( determinantOfJacobian );

    newAtlasImage->AddCellwise( tmpImage );

    weightImage->AddCellwise( determinantOfJacobian );

  }
  // now that we have added all of them we just need to divide by the weight image
  // (i.e., to obtain a local weighted average based on the local space deformation)

  newAtlasImage->DivideElementwise( weightImage );

  // now copy the new one into the old one
  currentAtlasImage->Copy( newAtlasImage );

}

template < class TState >
void CAtlasSubiterationUpdateObjectiveFunction< TState >::UpdateAtlasImageAsAverageOfSourceImages()
{
  std::cout << "Updating atlas image as average of source images." << std::endl;

  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer currentAtlasImage = this->m_ptrImageManager->GetOnlyCommonTimePointSavely( )->GetImage();

  typename VectorImageType::Pointer newAtlasImage = new VectorImageType( currentAtlasImage );

  newAtlasImage->SetToConstant( 0 );

  typename VectorImageType::Pointer tmpImage = new VectorImageType( newAtlasImage );

  unsigned int numberOfObjectiveFunctions = this->m_VectorIndividualObjectiveFunctionPtrs.size();

  for ( unsigned int iI=0; iI < numberOfObjectiveFunctions; ++iI )
  {
      typename IndividualObjectiveFunctionType::Pointer currentObjectiveFunction = this->m_VectorIndividualObjectiveFunctionPtrs[ iI ];
      currentObjectiveFunction->GetSourceImage( tmpImage, 1.0 );
      newAtlasImage->AddCellwise( tmpImage );
  }

  // now that we have added all of them we just need to divide to get the average image
  newAtlasImage->MultiplyByConstant( 1.0/numberOfObjectiveFunctions );

  currentAtlasImage->Copy( newAtlasImage );
}

#endif


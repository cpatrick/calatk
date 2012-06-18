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

#ifndef C_ATLAS_OBJECTIVE_FUNCTION_TXX
#define C_ATLAS_OBJECTIVE_FUNCTION_TXX

template < class TState >
CAtlasObjectiveFunction< TState >::CAtlasObjectiveFunction()
  : m_CurrentActiveObjectiveFunctionOutput( 0 ),
    DefaultAtlasIsSourceImage( true ),
    m_ExternallySetAtlasIsSourceImage( false )
{
  m_AtlasIsSourceImage = DefaultAtlasIsSourceImage;
}

template < class TState >
CAtlasObjectiveFunction< TState >::~CAtlasObjectiveFunction()
{
}

template < class TState >
void CAtlasObjectiveFunction< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "AtlasObjectiveFunction", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "AtlasObjectiveFunction", Json::nullValue );

  SetJSONHelpForRootKey( Atlas, "settings for the cross-sectional atlas builder" );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage,
                     "if set to true the atlas is the source (recommended), otherwise it is the target" );
}


template < class TState >
void CAtlasObjectiveFunction< TState >::InitializeState()
{
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers
  // TODO: Have a true vectorized implementation, so we can use it with the general optimizers
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      (*iter)->InitializeState();
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< typename IndividualStateType::Pointer > vecState;
  std::vector< typename IndividualStateType::Pointer > vecGradient;

  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
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
void CAtlasObjectiveFunction< TState >::InitializeState( TState *ptrState )
{
  // need to initialize all the individual states (based on the input state) and then create a new joint state vector which will form the new atlas state
  // need to initialize the individual states and then create a new joint state vector which will form the new atlas state
  // initialize the individual state components and create a state with the pointers

  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  unsigned int iI;
  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(), iI=0; iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iI, ++iter )
    {
    (*iter)->InitializeState( ptrState->GetIndividualStatePointer( iI ) );
    }

  // now all of the individual objective functions are initialized
  // Let's collect the pointers to the states and gradients and create new atlas state and gradient pointers

  std::vector< typename IndividualStateType::Pointer > vecState;
  std::vector< typename IndividualStateType::Pointer > vecGradient;

  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
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
unsigned int
CAtlasObjectiveFunction< TState >
::SetObjectiveFunctionAndWeight( IndividualObjectiveFunctionType* pObj, FloatType dWeight )
{
  m_VectorIndividualObjectiveFunctionPtrs.push_back( pObj );
  m_Weights.push_back( dWeight );

  assert( m_VectorIndividualObjectiveFunctionPtrs.size() == m_Weights.size() );

  return m_Weights.size()-1;
}

template < class TState >
void
CAtlasObjectiveFunction< TState >
::SetObjectiveFunctionAndWeight( unsigned int uiId, const IndividualObjectiveFunctionType* pObj, FloatType dWeight )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( uiId ) =  pObj;
  m_Weights.at( uiId ) = dWeight;
}

template < class TState >
void CAtlasObjectiveFunction< TState >
::ClearObjectiveFunctionPointersAndWeights()
{
  m_VectorIndividualObjectiveFunctionPtrs.clear();
  m_Weights.clear();
}

template < class TState >
unsigned int CAtlasObjectiveFunction< TState >
::GetNumberOfRegisteredObjectiveFunctions() const
{
  return m_VectorIndividualObjectiveFunctionPtrs.size();
}

template < class TState >
void CAtlasObjectiveFunction< TState >
::SetCurrentActiveObjectiveFunctionOutput( unsigned int uiActiveObjectiveFunctionOutput )
{
  m_CurrentActiveObjectiveFunctionOutput = uiActiveObjectiveFunctionOutput;
}

template < class TState >
unsigned int CAtlasObjectiveFunction< TState >
::GetCurrentActiveObjectiveFunctionOutput()
{
  return m_CurrentActiveObjectiveFunctionOutput;
}

// TODO: To implement
template < class TState >
void CAtlasObjectiveFunction< TState >::GetMap( VectorFieldType* ptrMap, FloatType dTime )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetMap( ptrMap, dTime );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::GetMapFromTo( VectorFieldType* ptrMap, FloatType dTimeFrom, FloatType dTimeTo )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetMapFromTo( ptrMap, dTimeFrom, dTimeTo );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrImage )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetSourceImage( ptrImage );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::GetSourceImage( VectorImageType* ptrImage, FloatType dTime )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetSourceImage( ptrImage, dTime );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrImage )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetTargetImage( ptrImage );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::GetTargetImage( VectorImageType* ptrImage, FloatType dTime )
{
  m_VectorIndividualObjectiveFunctionPtrs.at( m_CurrentActiveObjectiveFunctionOutput )->GetTargetImage( ptrImage, dTime );
}


template < class TState >
void CAtlasObjectiveFunction< TState >::GetInitialImage( VectorImageType* ptrIm )
{
  // this should be the initial image of the atlas-builder, same as for the individual registration
  // if the atlas-image is indeed at the center
  throw std::runtime_error( "GetInitialImage: not yet implemented" );
}

template < class TState >
const typename CAtlasObjectiveFunction< TState >::VectorImageType*
CAtlasObjectiveFunction< TState >::GetPointerToInitialImage() const
{
  throw std::runtime_error( "GetPointerToInitialImage: not yet implemented" );
  return NULL;
}

// this is simply the sum of the energy over all the individual components
template < class TState >
typename CAtlasObjectiveFunction< TState >::CEnergyValues
CAtlasObjectiveFunction< TState >::GetCurrentEnergy()
{
  CEnergyValues energyValues;
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(); iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter )
    {
      energyValues += (*iter)->GetCurrentEnergy();
    }
  return energyValues;
}

// this is the gradient given all the individual registrations, i.e., just the concatenation of all the individual gradients
// concatenation will be automatic, because the states of the objective functions have been associated with the atlas state previously
template < class TState >
void CAtlasObjectiveFunction< TState >::ComputeGradient()
{
  // compute them for each and create a state which contains pointers to all of the gradients
  // TODO: also multiply the gradients by the weights
  typename VectorIndividualObjectiveFunctionPointersType::iterator iter;
  typename std::vector< FloatType >::const_iterator iterWeights;
  for ( iter=m_VectorIndividualObjectiveFunctionPtrs.begin(), iterWeights=m_Weights.begin(); iter!=m_VectorIndividualObjectiveFunctionPtrs.end(); ++iter, ++iterWeights )
    {
      (*iter)->ComputeGradient();  // this automatically ends up in the gradient vector
      // TODO: multiply by weight, this should be handled by the individual algorithms, don't want to modift values here
      //IndividualStateType* currentGradientPointer = (*iter)->GetGradientPointer();
      //*currentGradientPointer *= *iterWeights;
    }

  // We don't compute the gradient with respect to the atlas image. This is handled explicitly in an outside loop in the atlas-builder algorithm.
  // While this is not the full gradient with respect to all unknowns it allows for a generic atlas-building-framework compatible with all
  // the component algorithms.
  // TODO: Investigate if it would make a difference in practice to implement the full algorithm

}

template < class TState >
void CAtlasObjectiveFunction< TState >::PreSubIterationSolve()
{
  // replace the source / target image by its weighted average, unless the atlas-image is considered as part of the gradient

  std::cout << "Executing subiteration solve." << std::endl;

  if ( m_AtlasIsSourceImage )
  {
    UpdateAtlasImageAsAverageOfTargetImages();
  }
  else
  {
    UpdateAtlasImageAsAverageOfSourceImages();
  }

}

template < class TState >
void CAtlasObjectiveFunction< TState >::UpdateAtlasImageAsAverageOfTargetImages()
{
  std::cout << "Updating atlas image as average of target images." << std::endl;

  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer currentAtlasImage = this->m_ptrImageManager->GetOnlyCommonTimePointSavely( )->GetImage();
  currentAtlasImage->SetToConstant( 0 );

  typename VectorImageType::Pointer determinantOfJacobian = new VectorImageType( currentAtlasImage, 0.0, 1 );
  typename VectorImageType::Pointer tmpImage = new VectorImageType( currentAtlasImage );
  typename VectorImageType::Pointer currentTargetImage = new VectorImageType( currentAtlasImage );
  typename VectorFieldType::Pointer tmpMap = new VectorFieldType( currentAtlasImage );
  typename VectorImageType::Pointer weightImage = new VectorImageType( currentAtlasImage );

  weightImage->SetToConstant( 0 );

  unsigned int numberOfObjectiveFunctions = m_VectorIndividualObjectiveFunctionPtrs.size();

  for ( unsigned int iI=0; iI < numberOfObjectiveFunctions; ++iI )
  {   
    typename IndividualObjectiveFunctionType::Pointer currentObjectiveFunction = m_VectorIndividualObjectiveFunctionPtrs[ iI ];

    // get the inverse map (so we can move the target image to the source (the atlas image)
    currentObjectiveFunction->GetMapFromTo( tmpMap, 1.0, 0.0 );
    currentObjectiveFunction->GetTargetImage( currentTargetImage );
    LDDMMUtilsType::applyMap( tmpMap, currentTargetImage, tmpImage );
    LDDMMUtilsType::computeDeterminantOfJacobian( tmpMap, determinantOfJacobian );

    tmpImage->MultiplyElementwise( determinantOfJacobian );
    currentAtlasImage->AddCellwise( tmpImage );

    weightImage->AddCellwise( determinantOfJacobian );

  }
  // now that we have added all of them we just need to divide by the weight image
  // (i.e., to obtain a local weighted average based on the local space deformation)
  currentAtlasImage->DivideElementwise( weightImage );
}

template < class TState >
void CAtlasObjectiveFunction< TState >::UpdateAtlasImageAsAverageOfSourceImages()
{
  std::cout << "Updating atlas image as average of source images." << std::endl;

  // need to use the atlas image at the current resolution (so it works properly in case of multi scaling)
  typename VectorImageType::Pointer currentAtlasImage = this->m_ptrImageManager->GetOnlyCommonTimePointSavely( )->GetImage();
  currentAtlasImage->SetToConstant( 0 );

  typename VectorImageType::Pointer tmpImage = new VectorImageType( currentAtlasImage );

  unsigned int numberOfObjectiveFunctions = m_VectorIndividualObjectiveFunctionPtrs.size();

  for ( unsigned int iI=0; iI < numberOfObjectiveFunctions; ++iI )
  {
      typename IndividualObjectiveFunctionType::Pointer currentObjectiveFunction = m_VectorIndividualObjectiveFunctionPtrs[ iI ];
      currentObjectiveFunction->GetSourceImage( tmpImage, 1.0 );
      currentAtlasImage->AddCellwise( tmpImage );
  }

  // now that we have added all of them we just need to divide to get the average image
  currentAtlasImage->MultiplyByConstant( 1.0/numberOfObjectiveFunctions );
}

#endif


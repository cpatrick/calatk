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

  SetJSONHelpForRootKey( AtlasObjectiveFunction, "settings for the cross-sectional atlas builder" );

  SetJSONFromKeyBool( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, AtlasIsSourceImage,
                     "if set to true the atlas is the source (recommended), otherwise it is the target" );
}

template < class TState >
unsigned int
CAtlasObjectiveFunction< TState >
::SetObjectiveFunctionAndWeight( IndividualObjectiveFunctionType* pObj, FloatType dWeight )
{
  m_VectorIndividualObjectiveFunctionPtrs.push_back( pObj );
  pObj->SetEnergyWeight( dWeight );
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
  pObj->SetEnergyWeight( dWeight );
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

#endif


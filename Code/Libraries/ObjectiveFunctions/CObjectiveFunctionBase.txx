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

#ifndef C_OBJECTIVE_FUNCTION_BASE_TXX
#define C_OBJECTIVE_FUNCTION_BASE_TXX

#include "CObjectiveFunctionBase.h"

namespace CALATK
{

//
// empty constructor
//
template < class T, unsigned int VImageDimension >
CObjectiveFunctionBase< T, VImageDimension >::CObjectiveFunctionBase()
  : m_CurrentlyActiveSubjectId( -1 ),
    DefaultSigmaSqr( 1.0 ),
    m_ExternallySetSigmaSqr( false ),
    DefaultEnergyWeight( 1.0 ),
    m_ExternallySetEnergyWeight( false )
{
  m_ptrMetric = NULL;
  m_ptrImageManager = NULL;

  m_SigmaSqr = DefaultSigmaSqr;
  m_EnergyWeight = DefaultEnergyWeight;
}

//
// destructor
//
template < class T, unsigned int VImageDimension >
CObjectiveFunctionBase< T, VImageDimension >::~CObjectiveFunctionBase()
{
}

template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "ObjectiveFunction", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "ObjectiveFunction", Json::nullValue );

  SetJSONHelpForRootKey( ObjectiveFunction, "general settings for the objective function" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, SigmaSqr );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, EnergyWeight );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, SigmaSqr,
                     "1/SigmaSqr is the weight for the data match term" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, EnergyWeight,
                     "EnergyWeight is multiplies the energy (useful for example for kernel regression)" );
}


template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::PreSubIterationSolve()
{
  // TODO: Put functionality here. Should call an externally specified method (maybe point to algorithm and have a PreSubIterationSolve method there)
}

//
// initial unsmoothed gradient computation (for example for kernel weight determination)
template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber )
{
  std::cerr << "ComputeInitialUnsmoothedVelocityGradient has not been implemented. Returning 0." << std::endl;
  ptrInitialUnsmoothedVelocityGradient->SetToConstant( 0.0 );
}

template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::SetImageManagerPointer( ImageManagerType * ptrImageManager )
{
  this->m_ptrImageManager = ptrImageManager;
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionBase< T, VImageDimension >::ImageManagerType*
CObjectiveFunctionBase< T, VImageDimension >::GetImageManagerPointer() const
{
  return this->m_ptrImageManager;
}

template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::SetMetricPointer( MetricType* ptrMetric )
{
  this->m_ptrMetric = ptrMetric;
}

template < class T, unsigned int VImageDimension >
typename CObjectiveFunctionBase< T, VImageDimension >::MetricType*
CObjectiveFunctionBase< T, VImageDimension >::GetMetricPointer() const
{
  return this->m_ptrMetric;
}

template < class T, unsigned int VImageDimension >
void CObjectiveFunctionBase< T, VImageDimension >::SetActiveSubjectId( int uid )
{
  this->m_CurrentlyActiveSubjectId = uid;
}

template < class T, unsigned int VImageDimension >
int CObjectiveFunctionBase< T, VImageDimension >::GetActiveSubjectId() const
{
  return this->m_CurrentlyActiveSubjectId;
}

} // end namespace

#endif

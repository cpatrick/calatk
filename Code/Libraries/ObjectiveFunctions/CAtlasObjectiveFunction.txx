/**
*
*  Copyright 2011 by the CALATK development team
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

template <class T, unsigned int VImageDimension, class TState >
CAtlasObjectiveFunction< T, VImageDimension, TState >::CAtlasObjectiveFunction()
{
}

template <class T, unsigned int VImageDimension, class TState >
CAtlasObjectiveFunction< T, VImageDimension, TState >::~CAtlasObjectiveFunction()
{
  this->DeleteAuxiliaryStructures();
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::DeleteAuxiliaryStructures()
{
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::CreateAuxiliaryStructures()
{
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >
::SetObjectiveFunctionPointerAndWeight( const ObjectiveFunctionType* pObj, T dWeight, unsigned int uiId )
{
}

template <class T, unsigned int VImageDimension, class TState >
T CAtlasObjectiveFunction< T, VImageDimension, TState >::GetCurrentEnergy()
{
}

template <class T, unsigned int VImageDimension, class TState >
void CAtlasObjectiveFunction< T, VImageDimension, TState >::ComputeGradient()
{
}






#endif


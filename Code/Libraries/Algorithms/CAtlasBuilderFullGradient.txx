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

#ifndef C_ATLAS_BUILDER_FULL_GRADIENT_TXX
#define C_ATLAS_BUILDER_FULL_GRADIENT_TXX

template < class TState >
CAtlasBuilderFullGradient< TState >::CAtlasBuilderFullGradient()
{
}

template < class TState >
CAtlasBuilderFullGradient< TState >::~CAtlasBuilderFullGradient()
{
}

template < class TState >
typename CAtlasBuilderFullGradient< TState >::AtlasObjectiveFunctionType*
CAtlasBuilderFullGradient< TState >::CreateAtlasObjectiveFunction()
{
  typedef CAtlasFullGradientObjectiveFunction< TState > CAtlasFullGradientType;
  AtlasObjectiveFunctionType* pAtlas = new CAtlasFullGradientType;

  return pAtlas;
}

#endif


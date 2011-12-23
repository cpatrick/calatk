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

#ifndef C_OBJECTIVE_FUNCTION_TXX
#define C_OBJECTIVE_FUNCTION_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension, class TState >
CObjectiveFunction<T, VImageDimension, TState >::CObjectiveFunction()
{
  m_pState = NULL;
  m_pGradient = NULL;
  m_pMetric = NULL;
  m_ptrImageManager = NULL;
}

//
// destructor
//
template <class T, unsigned int VImageDimension, class TState >
CObjectiveFunction<T, VImageDimension, TState >::~CObjectiveFunction()
{
}

//
// gradient function
//
template <class T, unsigned int VImageDimension, class TState >
void CObjectiveFunction<T, VImageDimension, TState >::ComputeGradient()
{
  if ( m_pGradient == NULL )
    {
    throw std::runtime_error( "Memory for gradient not allocated, nothing will be computed." );
    }
}

#endif

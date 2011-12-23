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

#ifndef C_EVOLVER_TXX
#define C_EVOLVER_TXX

template <class T, unsigned int VImageDimension >
CEvolver< T, VImageDimension >::CEvolver()
{
  m_ptrOneStepEvolver = NULL;
}

template <class T, unsigned int VImageDimension >
CEvolver< T, VImageDimension >::~CEvolver()
{
}

template <class T, unsigned int VImageDimension >
void CEvolver< T, VImageDimension >::SetOneStepEvolverPointer( ptrOneStepEvolverType pOneStepEvolver )
{
  m_ptrOneStepEvolver = pOneStepEvolver;
}

template <class T, unsigned int VImageDimension >
COneStepEvolver< T, VImageDimension >*
CEvolver< T, VImageDimension >::GetOneStepEvolverPointer()
{
  return m_ptrOneStepEvolver;
}

#endif

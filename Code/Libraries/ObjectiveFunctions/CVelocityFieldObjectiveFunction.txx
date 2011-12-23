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

#ifndef C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX
#define C_VELOCITY_FIELD_OBJECTIVE_FUNCTION_TXX

template <class T, unsigned int VImageDimension, class TState >
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::CVelocityFieldObjectiveFunction()
{
  m_ptrEvolver = NULL;
  m_ptrKernel = NULL;
}

template <class T, unsigned int VImageDimension, class TState >
void CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::SetEvolverPointer( ptrEvolverType pEvolver )
{
  m_ptrEvolver = pEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
CEvolver< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetEvolverPointer()
{
  return m_ptrEvolver;
}

template <class T, unsigned int VImageDimension, class TState >
void CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::SetKernelPointer( ptrKernelType pKernel )
{
  m_ptrKernel = pKernel;
}

template <class T, unsigned int VImageDimension, class TState >
CKernel< T, VImageDimension >*
CVelocityFieldObjectiveFunction< T, VImageDimension, TState >::GetKernelPointer()
{
  return m_ptrKernel;
}

#endif

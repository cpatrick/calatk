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

//
// empty constructor
//
template < class T, unsigned int VImageDimension >
CObjectiveFunctionBase< T, VImageDimension >::CObjectiveFunctionBase()
{
  m_pMetric = NULL;
  m_ptrImageManager = NULL;
}

//
// destructor
//
template < class T, unsigned int VImageDimension >
CObjectiveFunctionBase< T, VImageDimension >::~CObjectiveFunctionBase()
{
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

#endif

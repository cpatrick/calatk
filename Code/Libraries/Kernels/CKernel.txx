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

#ifndef C_KERNEL_TXX
#define C_KERNEL_TXX

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::CKernel()
  : m_KernelsNeedToBeComputed( true ),
    m_ptrL( NULL ),
    m_ptrLInv( NULL ),
    m_KernelNumber( 0 )
{
}

template <class T, unsigned int VImageDimension >
CKernel< T, VImageDimension >::~CKernel()
{
}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::AllocateMemoryForKernelAndInverseKernel( const VectorImageType * inputImage )
{
  const unsigned int szX = inputImage->GetSizeX();
  const unsigned int szY = inputImage->GetSizeY();
  const unsigned int szZ = inputImage->GetSizeZ();

  switch ( VImageDimension )
    {
    case 1:
      this->m_ptrL = new VectorImageType( szX, 1 );
      this->m_ptrLInv = new VectorImageType( szX, 1 );
      break;
    case 2:
      this->m_ptrL = new VectorImageType( szX, szY, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, 1 );
      break;
    case 3:
      this->m_ptrL = new VectorImageType( szX, szY, szZ, 1 );
      this->m_ptrLInv = new VectorImageType( szX, szY, szZ, 1 );
      break;
    default:
      throw std::runtime_error("Cannot allocate kernel memory of desired dimension.");
    }
}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::SetObjectiveFunction( ObjectiveFunctionBaseType* ptrObj )
{
  this->m_ptrObjectiveFunction = ptrObj;
}

template <class T, unsigned int VImageDimension >
typename CKernel< T, VImageDimension >::ObjectiveFunctionBaseType*
CKernel< T, VImageDimension >::GetObjectiveFunction()
{
  return this->m_ptrObjectiveFunction.GetPointer();
}

template <class T, unsigned int VImageDimension >
void CKernel< T, VImageDimension >::SetObjectiveFunctionKernelNumber( unsigned int uiKN )
{
  m_KernelNumber = uiKN;
}

template <class T, unsigned int VImageDimension >
unsigned int CKernel< T, VImageDimension >::GetObjectiveFunctionKernelNumber()
{
  return m_KernelNumber;
}

#endif

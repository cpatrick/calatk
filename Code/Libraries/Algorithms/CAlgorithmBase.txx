/*
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

#ifndef C_ALGORITHM_BASE_TXX
#define C_ALGORITHM_BASE_TXX

template < class T, unsigned int VImageDimension >
CAlgorithmBase< T, VImageDimension >::CAlgorithmBase()
{
  m_ptrMetric = NULL;
  m_ptrImageManager = NULL;
  m_ptrEvolver = NULL;
  m_ptrKernel = NULL;

  m_ptrIm = NULL;
  m_ptrMap = NULL;

  m_bSetDefaultMetric = false;
  m_bSetDefaultEvolver = false;
  m_bSetDefaultKernel = false;
  m_bSetDefaultImageManager = false;

}

template < class T, unsigned int VImageDimension >
CAlgorithmBase< T, VImageDimension >::~CAlgorithmBase()
{
  if ( m_bSetDefaultMetric )
    {
    if ( m_ptrMetric != NULL )
      {
      delete m_ptrMetric;
      m_ptrMetric = NULL;
      }
    }

  if ( m_bSetDefaultImageManager )
    {
    if ( m_ptrImageManager != NULL )
      {
      delete m_ptrImageManager;
      m_ptrImageManager = NULL;
      }
    }

  if ( m_bSetDefaultEvolver )
    {
    if ( m_ptrEvolver != NULL )
      {
      delete m_ptrEvolver;
      m_ptrEvolver = NULL;
      }
    }

  if ( m_bSetDefaultKernel )
    {
    if ( m_ptrKernel != NULL )
      {
      delete m_ptrKernel;
      m_ptrKernel = NULL;
      }
    }

  if ( m_ptrIm != NULL )
    {
    delete m_ptrIm;
    m_ptrIm = NULL;
    }

  if ( m_ptrMap != NULL )
    {
    delete m_ptrMap;
    m_ptrMap = NULL;
    }

}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetDefaultsIfNeeded()
{

  if ( m_ptrMetric == NULL )
    {
    SetDefaultMetricPointer();
    m_bSetDefaultMetric = true;
    }

  m_ptrMetric->SetPrintConfiguration( this->GetPrintConfiguration() );
  m_ptrMetric->SetAllowHelpComments( this->GetAllowHelpComments() );
  m_ptrMetric->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  if ( m_ptrEvolver == NULL )
    {
    SetDefaultEvolverPointer();
    m_bSetDefaultEvolver = true;
    }

  m_ptrEvolver->SetPrintConfiguration( this->GetPrintConfiguration() );
  m_ptrEvolver->SetAllowHelpComments( this->GetAllowHelpComments() );
  m_ptrEvolver->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  if ( m_ptrKernel == NULL )
    {
    SetDefaultKernelPointer();
    m_bSetDefaultKernel = true;
    }

  m_ptrKernel->SetPrintConfiguration( this->GetPrintConfiguration() );
  m_ptrKernel->SetAllowHelpComments( this->GetAllowHelpComments() );
  m_ptrKernel->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  // get the subject ids
  std::vector< unsigned int > vecSubjectIndices;
  this->m_ptrImageManager->GetAvailableSubjectIndices( vecSubjectIndices );

  this->m_ptrImageManager->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrImageManager->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrImageManager->SetAutoConfiguration( *this->m_jsonConfigIn.GetRootPointer(), *this->m_jsonConfigOut.GetRootPointer() );

  // also create the memory for the map and the image, so we can use it to return a map and an image at any time
  SImageInformation* pImInfo;
  m_ptrImageManager->GetPointerToSubjectImageInformationByIndex( pImInfo, vecSubjectIndices[ 0 ], 0 );

  assert( m_ptrIm == NULL );
  if ( m_ptrIm == NULL ) // this should be the default
    {
    m_ptrIm = new VectorImageType( pImInfo->pIm );
    m_ptrIm->setConst( 0 );
    }

  assert( m_ptrMap == NULL );
  if ( m_ptrMap == NULL )
    {
    m_ptrMap = new VectorFieldType( pImInfo->pIm );
    m_ptrMap->setConst( 0 );
    }

}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetImageManagerPointer( ptrImageManagerType ptrImageManager )
{
  m_ptrImageManager = ptrImageManager;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::ptrImageManagerType
CAlgorithmBase< T, VImageDimension >::GetImageManagerPointer()
{
  if ( m_ptrImageManager == NULL )
    {
    this->SetDefaultImageManagerPointer();
    m_bSetDefaultImageManager = true;
    }
  return m_ptrImageManager;
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetKernelPointer( ptrKernelType ptrKernel )
{
  m_ptrKernel = ptrKernel;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::ptrKernelType
CAlgorithmBase< T, VImageDimension >::GetKernelPointer()
{
  if ( m_ptrKernel == NULL )
    {
    this->SetDefaultKernelPointer();
    m_bSetDefaultKernel = true;
    }
  return m_ptrKernel;
}

template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetEvolverPointer( ptrEvolverType ptrEvolver )
{
  m_ptrEvolver = ptrEvolver;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::ptrEvolverType
CAlgorithmBase< T, VImageDimension >::GetEvolverPointer()
{
  if ( m_ptrEvolver == NULL )
    {
    this->SetDefaultEvolverPointer();
    m_bSetDefaultEvolver = true;
    }
  return m_ptrEvolver;
}


template < class T, unsigned int VImageDimension >
void CAlgorithmBase< T, VImageDimension >::SetMetricPointer( ptrMetricType ptrMetric )
{
  m_ptrMetric = ptrMetric;
}

template < class T, unsigned int VImageDimension >
typename CAlgorithmBase< T, VImageDimension >::ptrMetricType
CAlgorithmBase< T, VImageDimension >::GetMetricPointer()
{
  if ( m_ptrMetric == NULL )
    {
    this->SetDefaultMetricPointer();
    m_bSetDefaultMetric = true;
    }
  return m_ptrMetric;
}

#endif

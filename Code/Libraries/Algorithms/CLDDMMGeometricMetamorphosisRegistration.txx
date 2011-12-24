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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::CLDDMMGeometricMetamorphosisRegistration()
{
  m_ptrMaskKernel = NULL;
  m_bSetDefaultMaskKernel = false;
}

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::~CLDDMMGeometricMetamorphosisRegistration()
{
  if ( m_bSetDefaultMaskKernel )
    {
    delete m_ptrMaskKernel;
    m_ptrMaskKernel = NULL;
    // unregister it from the objective function
    typedef CLDDMMGeometricMetamorphosisObjectiveFunction< TState > CLDDMMType;
    CLDDMMType* plddmm = dynamic_cast< CLDDMMType *>( this->m_ptrObjectiveFunction );
    plddmm->SetMaskKernelPointer( this->m_ptrMaskKernel );    
    }
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetMaskKernelPointer( ptrKernelType ptrKernel )
{
  m_ptrMaskKernel = ptrKernel;
}

template < class TState >
CKernel< typename TState::TFloat, TState::VImageDimension >*
CLDDMMGeometricMetamorphosisRegistration< TState >::GetMaskKernelPointer()
{
  if ( m_ptrMaskKernel == NULL )
    {
    this->SetDefaultMaskKernelPointer();
    m_bSetDefaultMaskKernel = true;
    }
  return m_ptrMaskKernel;
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultMaskKernelPointer()
{
  CHelmholtzKernel< T, TState::VImageDimension >* ptrHH = new CHelmholtzKernel< T, TState::VImageDimension >;
  this->m_ptrMaskKernel = ptrHH;

  // multiply alpha by 0.25 to make the mask more fluid as default behavior
  //ptrHH->SetAlpha( 0.2*ptrHH->GetAlpha() );
  //std::cout << "WARNING: Check that this works properly and does not overwrite the previously set value if no other value is specified." << std::endl;

  ptrHH->SetAutoConfiguration( this->m_jsonConfig.GetFromKey( "MaskKernel", Json::nullValue ) );
  
}

template < class TState >
const typename CLDDMMGeometricMetamorphosisRegistration< TState >::VectorImageType*
CLDDMMGeometricMetamorphosisRegistration< TState >::GetImageT( T dTime )
{
  dynamic_cast< CLDDMMGeometricMetamorphosisObjectiveFunction< TState>* >(this->m_ptrObjectiveFunction)->GetImageT( this->m_ptrIm, dTime );
  return this->m_ptrIm;
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultsIfNeeded()
{
  std::cout << "Called the default mask kernel function" << std::endl;
  if ( m_ptrMaskKernel == NULL )
    {
    SetDefaultMaskKernelPointer();
    m_bSetDefaultMaskKernel = true;
    }
  Superclass::SetDefaultsIfNeeded();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMaskKernel == NULL )
    {
    throw std::runtime_error( "Mask kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CLDDMMGeometricMetamorphosisObjectiveFunction< TState > CLDDMMType;
  CLDDMMType* plddmm = new CLDDMMType;
  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMaskKernelPointer( this->m_ptrMaskKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  plddmm->SetAutoConfiguration( *this->m_jsonConfig.GetRootPointer() );
  
  this->m_ptrObjectiveFunction = plddmm;

}

#endif



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

#ifndef C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX
#define C_LDDMM_GEOMETRIC_METAMORPHOSIS_TXX

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::CLDDMMGeometricMetamorphosisRegistration()
  : DefaultMaskKernel( "HelmholtzKernel" ), m_ExternallySetMaskKernel( false )
{
  m_MaskKernel = DefaultMaskKernel;

  m_ptrMaskKernel = NULL;
}

template < class TState >
CLDDMMGeometricMetamorphosisRegistration< TState >::~CLDDMMGeometricMetamorphosisRegistration()
{
}

template< class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut )
{
  Superclass::SetAutoConfiguration( ConfValueIn, ConfValueOut );

  Json::Value& currentConfigurationIn = this->m_jsonConfigIn.GetFromKey( "GeneralRegistrationSettings", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_jsonConfigOut.GetFromKey( "GeneralRegistrationSettings", Json::nullValue );

  SetJSONHelpForRootKey( GeneralRegistrationSettings, "general setting for the registration" );

  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, MaskKernel );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, MaskKernel,
                     "kernel for the mask: GaussianKernel / HelmholtzKernel /  MultiGaussianKernel" );
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetMaskKernelPointer( KernelType * ptrKernel )
{
  this->m_ptrMaskKernel = ptrKernel;
}

template < class TState >
typename CLDDMMGeometricMetamorphosisRegistration< TState >::KernelType *
CLDDMMGeometricMetamorphosisRegistration< TState >::GetMaskKernelPointer()
{
  if ( m_ptrMaskKernel.GetPointer() == NULL )
    {
    this->SetDefaultMaskKernelPointer();
    }
  return this->m_ptrMaskKernel.GetPointer();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultMaskKernelPointer()
{
  this->m_ptrMaskKernel = CKernelFactory< T, TState::ImageDimension >::CreateNewKernel( m_MaskKernel );
}

template < class TState >
const typename CLDDMMGeometricMetamorphosisRegistration< TState >::VectorImageType*
CLDDMMGeometricMetamorphosisRegistration< TState >::GetImageT( T dTime )
{
  dynamic_cast< CLDDMMGeometricMetamorphosisObjectiveFunction< TState>* >(this->m_ptrObjectiveFunction.GetPointer() )->GetImageT( this->m_ptrIm, dTime );
  return this->m_ptrIm.GetPointer();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultsIfNeeded()
{
  std::cout << "Called the default mask kernel function" << std::endl;
  if ( m_ptrMaskKernel.GetPointer() == NULL )
    {
    SetDefaultMaskKernelPointer();
    }

  this->m_ptrMaskKernel->SetPrintConfiguration( this->GetPrintConfiguration() );
  this->m_ptrMaskKernel->SetAllowHelpComments( this->GetAllowHelpComments() );
  this->m_ptrMaskKernel->SetAutoConfiguration( this->m_jsonConfigIn.GetFromKey( "MaskKernel", Json::nullValue ), this->m_jsonConfigOut.GetFromKey( "MaskKernel", Json::nullValue ) );

  Superclass::SetDefaultsIfNeeded();
}

template < class TState >
void CLDDMMGeometricMetamorphosisRegistration< TState >::SetDefaultObjectiveFunctionPointer()
{
  // make sure that all we need has already been allocated

  if ( this->m_ptrKernel.GetPointer() == NULL )
    {
    throw std::runtime_error( "Kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMaskKernel.GetPointer() == NULL )
    {
    throw std::runtime_error( "Mask kernel needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrMetric.GetPointer() == NULL )
    {
    throw std::runtime_error( "Metric needs to be defined before default objective function can be created." );
    }

  if ( this->m_ptrImageManager.GetPointer() == NULL )
    {
    throw std::runtime_error( "Image manager needs to be defined before default objective function can be created." );
    }

  typedef CLDDMMGeometricMetamorphosisObjectiveFunction< TState > CLDDMMType;
  typename CLDDMMType::Pointer plddmm = new CLDDMMType;
  plddmm->SetEvolverPointer( this->m_ptrEvolver );
  plddmm->SetKernelPointer( this->m_ptrKernel );
  plddmm->SetMaskKernelPointer( this->m_ptrMaskKernel );
  plddmm->SetMetricPointer( this->m_ptrMetric );
  plddmm->SetImageManagerPointer( this->m_ptrImageManager );

  this->m_ptrObjectiveFunction = plddmm;
  // set the objective functions for the kernels and the kernel numbers
  this->m_ptrKernel->SetObjectiveFunction( plddmm );
  this->m_ptrKernel->SetObjectiveFunctionKernelNumber( 0 );
  this->m_ptrMaskKernel->SetObjectiveFunction( plddmm );
  this->m_ptrMaskKernel->SetObjectiveFunctionKernelNumber( 1 );
}

#endif



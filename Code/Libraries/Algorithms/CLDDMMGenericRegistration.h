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

#ifndef C_LDDMM_GENERIC_REGISTRATION_H
#define C_LDDMM_GENERIC_REGISTRATION_H

#include "CObjectiveFunctionFactory.h"
#include "CALATKCommon.h"
#include "CJSONConfiguration.h"
#include "CLDDMMVelocityFieldWithMomentumRegistration.h"

namespace CALATK
{

/** A generic registration driver which instantiates different LDDMM registration objective functions
  * through the objective function factory.
  */
template < class TState >
class CLDDMMGenericRegistration
    : public CLDDMMVelocityFieldWithMomentumRegistration< TState >
{
public:
  /** Standard class typedefs. */
  typedef CLDDMMGenericRegistration                             Self;
  typedef CLDDMMVelocityFieldWithMomentumRegistration< TState > Superclass;
  typedef itk::SmartPointer< Self >                             Pointer;
  typedef itk::SmartPointer< const Self >                       ConstPointer;

  /* some useful typedefs */
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CLDDMMGenericRegistration();
  ~CLDDMMGenericRegistration();

  SetMacro( ObjectiveFunction, std::string );
  GetMacro( ObjectiveFunction, std::string );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  void SetDefaultObjectiveFunctionPointer();

private:

  std::string m_ObjectiveFunction;
  const std::string DefaultObjectiveFunction;
  bool m_ExternallySetObjectiveFunction;

};

#include "CLDDMMGenericRegistration.txx"

} // end namespace

#endif // C_LDDMM_GENERIC_REGISTRATION_H

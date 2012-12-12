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

#ifndef C_OBJECTIVE_FUNCTION_FACTORY_H
#define C_OBJECTIVE_FUNCTION_FACTORY_H

#include "CJSONConfiguration.h"
#include "CALATKCommon.h"
#include "CObjectiveFunctionBase.h"
#include "CProcessBase.h"

// include for all the supported objective functions
#include "CLDDMMAdjointGeodesicShootingObjectiveFunction.h"
#include "CLDDMMSimplifiedGeodesicShootingObjectiveFunction.h"
#include "CLDDMMGrowthModelObjectiveFunction.h"
#include "CLDDMMGeometricMetamorphosisObjectiveFunction.h"
#include "CLDDMMSimplifiedMetamorphosisGeodesicShootingObjectiveFunction.h"
#include "CMetamorphosisAdjointGeodesicShootingObjectiveFunction.h"
#include "CLDDMMMultiVelocityObjectiveFunction.h"

#include "CStateInitialImageMomentum.h"
#include "CStateInitialMomentum.h"
#include "CStateSpatioTemporalVelocityField.h"

namespace CALATK
{
/**
  * Factory class to dynamically create different objective functions.
  *
  */

template < class T, unsigned int VImageDimension >
class CObjectiveFunctionFactory :
    public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CObjectiveFunctionFactory        Self;
  typedef CProcessBase                     Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  // all the objective function typedefs
  typedef CObjectiveFunctionBase< T, VImageDimension >             ObjectiveFunctionBaseType;

  typedef CStateInitialImageMomentum< T, VImageDimension >         InitialImageMomentumStateType;
  typedef CStateInitialMomentum< T, VImageDimension >              InitialMomentumStateType;
  typedef CStateSpatioTemporalVelocityField< T, VImageDimension >  SpatioTemporalVelocityFieldStateType;

  enum NumericObjectiveFunctionType
    {
    LDDMMAdjointGeodesicShooting,
    LDDMMAdjointGeodesicShootingInitialImage,
    LDDMMSimplifiedGeodesicShooting,
    LDDMMSimplifiedGeodesicShootingInitialImage,
    LDDMMGrowthModel,
    LDDMMGeometricMetamorphosis,
    LDDMMMetamorphosisAdjointGeodesicShooting,
    LDDMMMetamorphosisAdjointGeodesicShootingInitialImage,
    LDDMMSimplifiedMetamorphosisGeodesicShooting,
    LDDMMSimplifiedMetamorphosisGeodesicShootingInitialImage,
    LDDMMMultiVelocity
    };

  CObjectiveFunctionFactory();
  ~CObjectiveFunctionFactory();

  static ObjectiveFunctionBaseType* CreateNewObjectiveFunction( const std::string & objectiveFunctionName );
  static ObjectiveFunctionBaseType* CreateNewObjectiveFunction( NumericObjectiveFunctionType objectiveFunction );

protected:
  static NumericObjectiveFunctionType GetObjectiveFunctionTypeFromString( const std::string & objectiveFunctionName );
};

} // end namespace

//#include "CObjectiveFunctionFactory.txx"

#endif // C_OBJECTIVE_FUNCTION_FACTORY_H

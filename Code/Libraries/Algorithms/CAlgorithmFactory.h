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

#ifndef C_ALGORITHM_FACTORY_H
#define C_ALGORITHM_FACTORY_H

#include "CLDDMMGenericRegistration.h"
#include "CLDDMMGeometricMetamorphosisRegistration.h"

namespace CALATK
{

/**
 * \brief Factory class to dynamically create different algorithms.
 */
template< class TFloat, unsigned int VImageDimension=3 >
class CAlgorithmFactory: public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CAlgorithmFactory               Self;
  typedef CProcessBase                    Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef CAlgorithmBase< TFloat, VImageDimension >  AlgorithmBaseType;

  typedef CStateInitialImageMomentum< TFloat, VImageDimension >                            StateInitialImageMomentumType;
  typedef CStateSpatioTemporalVelocityField< TFloat, VImageDimension >                     StateSpatioTemporalVelocityFieldType;

  typedef CLDDMMGenericRegistration< StateInitialImageMomentumType >                       StateInitialImageMomentumLDDMMRegistrationType;
  typedef CLDDMMGenericRegistration< StateSpatioTemporalVelocityFieldType >                StateSpatioTemporalVelocityFieldLDDMMRegistrationType;
  typedef CLDDMMGeometricMetamorphosisRegistration< StateSpatioTemporalVelocityFieldType > GeometricMetamorphosisRegistrationType;

  enum CALATKAlgorithmName
    {
    LDDMMAdjointGeodesicShootingRegistration,
    LDDMMGrowthModelRegistration,
    LDDMMGeometricMetamorphosisRegistration,
    LDDMMSimplifiedGeodesicShootingRegistration,
    };

  CAlgorithmFactory();
  ~CAlgorithmFactory();

  static AlgorithmBaseType* CreateNewAlgorithm( const std::string & algorithmName );
  static AlgorithmBaseType* CreateNewAlgorithm( CALATKAlgorithmName algorithm );

protected:

  static CALATKAlgorithmName GetAlgorithmNameFromString( const std::string & algorithmName );
};

} // end namespace CALATK

#endif // C_ALGORITHM_FACTORY_H

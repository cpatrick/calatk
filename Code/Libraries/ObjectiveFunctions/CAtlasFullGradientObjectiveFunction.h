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

#ifndef C_ATLAS_FULL_GRADIENT_OBJECTIVE_FUNCTION_H
#define C_ATLAS_FULL_GRADIENT_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"
#include "VectorImageUtils.h"
#include "CAtlasObjectiveFunction.h"

namespace CALATK
{

template < class TState >
/**
 * @brief Objective function for atlas-building. Makes use of a multistate and manages multiple individual objective functions.
 *
 */
class CAtlasFullGradientObjectiveFunction
    : public CAtlasObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CAtlasFullGradientObjectiveFunction  Self;
  typedef CAtlasObjectiveFunction< TState >    Superclass;
  typedef itk::SmartPointer< Self >            Pointer;
  typedef itk::SmartPointer< const Self >      ConstPointer;

  /* Some useful typedefs */

  typedef typename TState::FloatType FloatType;
  typedef typename TState::IndividualStateType IndividualStateType;

  typedef CAtlasFullGradientObjectiveFunction< TState >                       ObjectiveFunctionType;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< IndividualStateType >  IndividualObjectiveFunctionType;
  typedef LDDMMUtils< FloatType, TState::ImageDimension > LDDMMUtilsType;
  typedef VectorImageUtils< FloatType, TState::ImageDimension > VectorImageUtilsType;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorIndividualObjectiveFunctionPointersType VectorIndividualObjectiveFunctionPointersType;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CAtlasFullGradientObjectiveFunction();
  virtual ~CAtlasFullGradientObjectiveFunction();

  void ComputeGradient();

  // TODO: needs to be implemented
  const VectorImageType* GetPointerToInitialImage() const;
  void GetInitialImage( VectorImageType* ptrIm );

  void PreSubIterationSolve();

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

  void OutputStateInformation( unsigned int iter, std::string outputPrefix );

protected:

  void InitializeState();
  void InitializeState( TState * ptrState );

private:
  // intentionally not implemented
  CAtlasFullGradientObjectiveFunction( const CAtlasFullGradientObjectiveFunction & );
  CAtlasFullGradientObjectiveFunction& operator=( const CAtlasFullGradientObjectiveFunction & );
};

#include "CAtlasFullGradientObjectiveFunction.txx"

} // end namespace

#endif // C_ATLAS_FULL_GRADIENT_OBJECTIVE_FUNCTION_H

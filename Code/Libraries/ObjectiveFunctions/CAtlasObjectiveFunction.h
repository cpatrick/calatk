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

#ifndef C_ATLAS_OBJECTIVE_FUNCTION_H
#define C_ATLAS_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CALATKCommon.h"

namespace CALATK
{

template < class TState>
class CAtlasObjectiveFunction
    : public CObjectiveFunction< TState >
{
public:
  /** Standard class typedefs. */
  typedef CAtlasObjectiveFunction          Self;
  typedef CObjectiveFunction< TState >     Superclass;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;

  /* Some useful typedefs */

  typedef CObjectiveFunction< TState > ObjectiveFunctionType;

  typedef typename TState::TFloat T;
  typedef typename TState::TIndividualState TIndividualState;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  CAtlasObjectiveFunction();
  virtual ~CAtlasObjectiveFunction();

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();

  /**
   * @brief Sets one of the objective functions for the registration between an image and the atlas
   *
   * @param pObj - pointer to the objective function
   * @param dWeight - weight in the atlas building
   */
  void SetObjectiveFunctionAndWeight( const ObjectiveFunctionType* pObj, T dWeight );

  /**
   * @brief Deletes the data structures used to store the pointers to the objective functions
   * and the weights. The data the pointers point to is *not* deallocated (this is the duty of
   * whoever created the pointers). Allows to start assigning the atlas building information from scratch.
   *
   */
  void ClearObjectiveFunctionPointersAndWeights();

protected:

  void DeleteAuxiliaryStructures();
  void CreateAuxiliaryStructures();

  void InitializeState();
  void InitializeState( TStateAtlas * ptrState );

private:
  // intentionally not implemented
  CAtlasObjectiveFunction( const CAtlasObjectiveFunction & );
  CAtlasObjectiveFunction& operator=( const CAtlasObjectiveFunction & );

  std::vector< T > vecWeights; ///< Contains the weights for each subject to atlas registration
  typedef std::vector< typename ObjectiveFunctionType::Pointer > VectorObjectiveFunctionPointersType;
  VectorObjectiveFunctionPointersType  m_VectorObjectiveFunctionPtrs;
};

#include "CAtlasObjectiveFunction.txx"

} // end namespace

#endif // C_ATLAS_OBJECTIVE_FUNCTION_H

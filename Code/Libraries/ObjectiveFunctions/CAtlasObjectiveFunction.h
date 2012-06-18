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

#ifndef C_ATLAS_OBJECTIVE_FUNCTION_H
#define C_ATLAS_OBJECTIVE_FUNCTION_H

#include "CObjectiveFunction.h"
#include "CVelocityFieldObjectiveFunctionWithMomentum.h"
#include "CALATKCommon.h"
#include "LDDMMUtils.h"
#include "VectorImageUtils.h"

namespace CALATK
{

template < class TState >
/**
 * @brief Objective function for atlas-building. Makes use of a multistate and manages multiple individual objective functions.
 *
 */
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

  typedef typename TState::FloatType FloatType;
  typedef typename TState::IndividualStateType IndividualStateType;

  typedef CAtlasObjectiveFunction< TState >                                   ObjectiveFunctionType;
  typedef CVelocityFieldObjectiveFunctionWithMomentum< IndividualStateType >  IndividualObjectiveFunctionType;
  typedef LDDMMUtils< FloatType, TState::ImageDimension > LDDMMUtilsType;
  typedef VectorImageUtils< FloatType, TState::ImageDimension > VectorImageUtilsType;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CAtlasObjectiveFunction();
  virtual ~CAtlasObjectiveFunction();

  CEnergyValues GetCurrentEnergy();
  void ComputeGradient();

  /**
   * @brief Sets one of the objective functions for the registration between an image and the atlas
   *
   * @returns --id of the individualobjectivefunction that was set (starts at 0 and is then incremented)
   * @param pObj - pointer to the objective function
   * @param dWeight - weight in the atlas building
   */
  unsigned int SetObjectiveFunctionAndWeight( IndividualObjectiveFunctionType* pObj, FloatType dWeight );

  /**
   * @brief Sets one of the objective functions for the registration between an image and the atlas, can be used to overwrite previously assigned individual objective functions
   *
   * @param uiId --id of the individualobjectivefunction (starts at 0, if not existant will be created, if not successive intermediate ids will be initialized to null)
   * @param pObj - pointer to the objective function
   * @param dWeight - weight in the atlas building
   */
  void SetObjectiveFunctionAndWeight( unsigned int uiId, const IndividualObjectiveFunctionType* pObj, FloatType dWeight );

  /**
   * @brief Deletes the data structures used to store the pointers to the objective functions
   * and the weights. The data the pointers point to is *not* deallocated (this is the duty of
   * whoever created the pointers -- or the respective smart pointer implementation).
   * Allows to start assigning the atlas building information from scratch.
   *
   */
  void ClearObjectiveFunctionPointersAndWeights();

  /**
   * @brief Allows to check how many individual objective functions have been registered with the atlas-builder.
   *
   * @return unsigned int -- number of registered objective functions
   */
  unsigned int GetNumberOfRegisteredObjectiveFunctions() const;

  /**
   * @brief The individual objective functions are numbered. To support the standard GetMap, GetMapFromTo, GetImage interface
   * this method allows to set a currently active individual objective function for the respective output. This has no effect on any
   * internal atlas-builder computations and is just provided for convenience to keep the interface.
   *
   * @param uiActiveObjectiveFunctionOutput -- number of individual objective function which will create subsequent output
   */
  void SetCurrentActiveObjectiveFunctionOutput( unsigned int uiActiveObjectiveFunctionOutput );

  /**
   * @brief Returns the current individual objective function active for output with GetMap, GetMapFromTo, GetImage
   *
   * @return unsigned int -- id of currently active individual objective function
   */
  unsigned int GetCurrentActiveObjectiveFunctionOutput();

  /**
   * @brief Assumes that the atlas image is the source image and updates it by a weighted average of the target images pulled back to the source
   *
   */
  void UpdateAtlasImageAsAverageOfTargetImages();

  /**
   * @brief Assumes that the atlas image is the target image and updates it by flowing the source images forward and averaging them.
   *
   */
  void UpdateAtlasImageAsAverageOfSourceImages();

  // Map and image output for the individual objective functions (selection by CurrentActiveObjectiveFunctionOutput)
  void GetMap( VectorFieldType* ptrMap, FloatType dTime );
  void GetMapFromTo( VectorFieldType* ptrMap, FloatType dTimeFrom, FloatType dTimeTo );
  void GetSourceImage( VectorImageType* ptrIm );
  void GetSourceImage( VectorImageType* ptrIm, FloatType dTime );
  void GetTargetImage( VectorImageType* ptrIm );
  void GetTargetImage( VectorImageType* ptrIm, FloatType dTime );

  // TODO: needs to be implemented
  const VectorImageType* GetPointerToInitialImage() const;
  void GetInitialImage( VectorImageType* ptrIm );

  void PreSubIterationSolve();

  SetMacro( AtlasIsSourceImage, bool );
  GetMacro( AtlasIsSourceImage, bool );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:

  void InitializeState();
  void InitializeState( TState * ptrState );

private:
  // intentionally not implemented
  CAtlasObjectiveFunction( const CAtlasObjectiveFunction & );
  CAtlasObjectiveFunction& operator=( const CAtlasObjectiveFunction & );

  std::vector< FloatType > m_Weights; ///< Contains the weights for each subject to atlas registration
  typedef std::vector< typename IndividualObjectiveFunctionType::Pointer > VectorIndividualObjectiveFunctionPointersType;

  VectorIndividualObjectiveFunctionPointersType  m_VectorIndividualObjectiveFunctionPtrs;

  unsigned int m_CurrentActiveObjectiveFunctionOutput;  /**< id for currently active individual objective function to create output */

  bool m_AtlasIsSourceImage; // TODO: Make this a proper choice

  const bool DefaultAtlasIsSourceImage;
  bool m_ExternallySetAtlasIsSourceImage;
};

#include "CAtlasObjectiveFunction.txx"

} // end namespace

#endif // C_ATLAS_OBJECTIVE_FUNCTION_H

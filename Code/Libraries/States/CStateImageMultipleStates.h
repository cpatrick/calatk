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

#ifndef C_STATE_IMAGE_MULTIPLE_STATES_H
#define C_STATE_IMAGE_MULTIPLE_STATES_H

#include "CStateImageMultipleStates.h"

namespace CALATK
{
/**
  * Implements the state for MultipleStates building.
  * This state is simply a collection of individual states of the individual registrations
  * between an image and the MultipleStates image. Templated over the state of these registrations.
  */
template < class TIndividualState >
class CStateImageMultipleStates : public CStateImageDomain< typename TIndividualState::FloatType, TIndividualState::ImageDimension >
{
public:
  /* some useful typedefs */
  typedef TIndividualState                           IndividualStateType;
  typedef typename IndividualStateType::FloatType    FloatType;

  static const unsigned int ImageDimension = TIndividualState::ImageDimension;

  /* Standard class typedefs. */
  typedef CStateImageMultipleStates                                Self;
  typedef itk::SmartPointer< Self >                                Pointer;
  typedef itk::SmartPointer< const Self >                          ConstPointer;
  typedef CStateImageDomain< TIndividualState, ImageDimension >    Superclass;

  typedef typename Superclass::VectorImageType                     VectorImageType;

  typedef std::vector< typename IndividualStateType::Pointer >     IndividualStatesCollectionType;

  /**
   * Empty constructor
   */
  CStateImageMultipleStates();

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateImageMultipleStates( const CStateImageMultipleStates & c );

  /**
    * copy constructor to initialize from a vector of pointers to states
    * assumes memory will be managed externally, i.e., only a shallow copy will be performed
    */
  CStateImageMultipleStates( const IndividualStatesCollectionType & individualStates );

  /**
   * Destructor, this class will involve dynamic memory allocation, so needs a destructor
   */
  ~CStateImageMultipleStates();

  /**
   * Allow for upsampling of the state
   */
  virtual Superclass* CreateUpsampledStateAndAllocateMemory( const VectorImageType* graftImage ) const;

  /// declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateImageMultipleStates & operator=( const CStateImageMultipleStates & p );

  CStateImageMultipleStates & operator+=(const CStateImageMultipleStates & p );

  CStateImageMultipleStates & operator-=(const CStateImageMultipleStates & p );

  CStateImageMultipleStates & operator*=(const FloatType & p );

  CStateImageMultipleStates operator+(const CStateImageMultipleStates & p ) const;

  CStateImageMultipleStates operator-(const CStateImageMultipleStates & p ) const;

  CStateImageMultipleStates operator*(const FloatType & p ) const;

  /**
   * @brief Returns the state pointer for one of the underlying objectuve functions of the MultipleStates builder
   *
   * @param idx - nr of the state to be returned, if it does not exist will return NULL
   * @return TState * - returned state pointer
   */
  IndividualStateType* GetIndividualStatePointer( unsigned int idx );
  
  /**
   * @brief Computes the square norm of the state. To be used for example in a line search method
   * to establish sufficient descrease of an objective function
   *
   * @return Returns the squared norm. For the MultipleStates, this is the sum of the squared norms of all
   * the components.
   */
  FloatType SquaredNorm();

  bool StateContainsInitialImage();

protected:
  void ClearDataStructure();

#warning not fully implemented, needs to hold an image as a state

  // holds the states of the individual registration algorithms
  IndividualStatesCollectionType  m_IndividualStatesCollection;

private:

};

} // end namespace

#include "CStateImageMultipleStates.txx"

#endif

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

#ifndef C_STATE_ATLAS_H
#define C_STATE_ATLAS_H

#include "CStateAtlas.h"

namespace CALATK
{
/**
  * Implements the state for atlas building.
  * This state is simply a collection of individual states of the individual registrations
  * between an image and the atlas image. Templated over the state of these registrations.
  */
template <class TState>
class CStateAtlas : public CStateImageDomain< typename TState::TFloat, typename TState::VImageDimension, typename TState::TResampler >
{
public:
  /* some useful typedefs */
  typedef typename TState::T               T;
  typedef typename TState                  TIndividualState;
  typedef typename TState::VImageDimension VImageDimension;
  typedef typename TState::TResampler      TResampler;

  /* Standard class typedefs. */
  typedef CStateAtlas                                              Self;
  typedef itk::SmartPointer< Self >                                Pointer;
  typedef itk::SmartPointer< const Self >                          ConstPointer;
  typedef CStateImageDomain< TState, VImageDimension, TResampler > Superclass;
  typedef Superclass                                               SuperclassTState;

  /**
   * Empty constructor
   */
  CStateAtlas();

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateAtlas( const CStateAtlas & c );

  /**
    * copy constructor to initialize from a vector of pointers to states
    * assumes memory will be managed externally, i.e., only a shallow copy will be performed
    */
  CStateAtlas( const std::vector< TState* > * pVec );

  /**
   * Destructor, this class will involve dynamic memory allocation, so needs a destructor
   */
  ~CStateAtlas();

  /**
   * Allow for upsampling of the state
   */
  SuperclassTState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const; 

  /// declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateAtlas & operator=( const CStateAtlas & p );

  CStateAtlas & operator+=(const CStateAtlas & p );

  CStateAtlas & operator-=(const CStateAtlas & p );

  CStateAtlas & operator*=(const T & p );

  CStateAtlas operator+(const CStateAtlas & p ) const;

  CStateAtlas operator-(const CStateAtlas & p ) const;

  CStateAtlas operator*(const T & p ) const;

  /**
   * @brief Returns the state pointer for one of the underlying objectuve functions of the atlas builder
   *
   * @param uiState - nr of the state to be returned, if it does not exist will return NULL
   * @return TState * - returned state pointer
   */
  TState* GetIndividualStatePointer( unsigned int uiState );
  
  /**
   * @brief Computes the square norm of the state. To be used for example in a line search method
   * to establish sufficient descrease of an objective function
   *
   * @return Returns the squared norm. For the atlas, this is the sum of the squared norms of all
   * the components.
   */
  T SquaredNorm();

protected:
  void ClearDataStructure();

  // holds the state vectors of the individual registration algorithms
  typedef std::vector< typename TState::Pointer > VectorIndividualStatesType;
  VectorIndividualStatesType  m_vecIndividualStates;

private:

};

#include "CStateAtlas.txx"

} // end namespace

#endif

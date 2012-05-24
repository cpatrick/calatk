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

#ifndef C_STATE_INITIAL_MOMENTUM_H
#define C_STATE_INITIAL_MOMENTUM_H

#include "CStateImageDomain.h"
#include "VectorField.h"

namespace CALATK
{

/**
 * \class CStateInitialMomentum
 *
 * \todo describe me
 */
template < class TFloat, unsigned int VImageDimension=3 >
class CStateInitialMomentum: public CStateImageDomain< TFloat, VImageDimension >
{
public:

  /* Standard class typedefs. */
  typedef CStateInitialMomentum                        Self;
  typedef CStateImageDomain< TFloat, VImageDimension > Superclass;
  typedef itk::SmartPointer< Self >                    Pointer;
  typedef itk::SmartPointer< const Self >              ConstPointer;

  static const unsigned int ImageDimension = VImageDimension;
  typedef TFloat                           FloatType;

  /* some useful typedefs */
  typedef VectorImage< FloatType, VImageDimension >  VectorImageType;

  /**
   * Empty constructor
   */
  CStateInitialMomentum();

  /**
   * Constructor which takes a pointer to a graft image, so that a state of appropriate dimension can be created.
   * Creates the memory (raw memory pointer) and also creates the appropriate state structures (image and momentum)
   * which make use of this memory block.
   * Will destroy the data in Destructor.
   */
  CStateInitialMomentum( VectorImageType* ptrGraftImage );

  /**
   * Constructor which takes a pointer to the initial momentum as well as to the raw memory (which the initial momentum wil point to);
   * Does not copy the data, just stores the pointers to it;
   * Will destroy the data in Destructor.
   */
  CStateInitialMomentum( FloatType* ptrRawData, VectorImageType* ptrInitialMomentum );

  /**
    * copy constructor, creation of the image and momentum for the first time, need to allocate memory
    */
  CStateInitialMomentum( const CStateInitialMomentum & c );

  /**
    * Destructor
    */
  ~CStateInitialMomentum();

  /*
   * Allow for upsampling of the state
   */
  virtual Superclass* CreateUpsampledStateAndAllocateMemory( const VectorImageType* ptrGraftImage ) const;

  // declare operators to be able to do computations with this state, which are needed in the numerical solvers
  /**
   * assignment
   */
  CStateInitialMomentum & operator=( const CStateInitialMomentum & p);

  CStateInitialMomentum & operator+=( const CStateInitialMomentum & p);

  CStateInitialMomentum & operator-=( const CStateInitialMomentum & p);

  CStateInitialMomentum & operator*=( const FloatType & p);

  CStateInitialMomentum operator+( const CStateInitialMomentum & p) const;

  CStateInitialMomentum operator-( const CStateInitialMomentum & p) const;

  CStateInitialMomentum operator*( const FloatType & p) const;

  VectorImageType * GetPointerToInitialMomentum() const;

  virtual long int GetNumberOfStateVectorElements();
  virtual FloatType* GetPointerToStateVector();

  virtual long int GetNumberOfStateVectorElementsToEstimate();
  virtual FloatType* GetPointerToStateVectorElementsToEstimate();

  virtual FloatType SquaredNorm();

  virtual bool StateContainsInitialImage();

protected:
  void ClearDataStructure();
  void CopyDataStructure( const VectorImageType * ptrMomentum );

private:
  typename VectorImageType::Pointer m_ptrInitialMomentum;

  long int m_NumberOfStateVectorElements;
  FloatType* m_ptrRawData;
};

} // end namespace

#endif // C_STATE_INITIAL_MOMENTUM_H

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

#ifndef C_STATE_INITIAL_IMAGE_MOMENTUM_H
#define C_STATE_INITIAL_IMAGE_MOMENTUM_H

#include "CStateImageDomain.h"
#include "VectorField.h"
#include "CResamplerLinear.h"

namespace CALATK
{

/**
 * \class CStateInitialImageMomentum
 *
 * \todo describe me
 */
template <class T, unsigned int VImageDimension=3, class TResampler=CResamplerLinear<T, VImageDimension> >
class CStateInitialImageMomentum: public CStateImageDomain< T, VImageDimension, TResampler >
{
public:
  typedef CStateInitialImageMomentum   TState;

  /* Standard class typedefs. */
  typedef CStateInitialImageMomentum                               Self;
  typedef CStateImageDomain< TState, VImageDimension, TResampler > Superclass;
  typedef itk::SmartPointer< Self >                                Pointer;
  typedef itk::SmartPointer< const Self >                          ConstPointer;

  /* some useful typedefs */
  typedef typename Superclass::TState  SuperclassTState;

  typedef VectorImage< T, VImageDimension >  VectorImageType;
  //typedef typename Superclass::VectorImageType VectorImageType;

  /**
   * Empty constructor
   */
  CStateInitialImageMomentum();

  /**
   * Constructor which takes a pointer to a graft image, so that a state of appropriate dimension can be created.
   * Creates the memory (raw memory pointer) and also creates the appropriate state structures (image and momentum)
   * which make use of this memory block.
   * Will destroy the data in Destructor.
   */
  CStateInitialImageMomentum( VectorImageType* ptrGraftImage );

  /**
   * Constructor which takes a pointer to the initial image and initial momentum as well as to the raw memory (which the initial image and the momentum wil point to);
   * Does not copy the data, just stores the pointers to it;
   * Will destroy the data in Destructor.
   */
  CStateInitialImageMomentum( T* ptrRawData, VectorImageType* ptrInitialImage, VectorImageType* ptrInitialMomentum );

  /**
    * copy constructor, creation of the image and momentum for the first time, need to allocate memory
    */
  CStateInitialImageMomentum( const CStateInitialImageMomentum & c );

  /**
    * Destructor
    */
  ~CStateInitialImageMomentum();

  /*
   * Allow for upsampling of the state
   */
  virtual TState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* ptrGraftImage ) const;

  // declare operators to be able to do computations with this state, which are needed in the numerical solvers
  /**
   * assignment
   */
  CStateInitialImageMomentum & operator=( const CStateInitialImageMomentum & p);

  CStateInitialImageMomentum & operator+=( const CStateInitialImageMomentum & p);

  CStateInitialImageMomentum & operator-=( const CStateInitialImageMomentum & p);

  CStateInitialImageMomentum & operator*=( const T & p);

  CStateInitialImageMomentum operator+( const CStateInitialImageMomentum & p) const;

  CStateInitialImageMomentum operator-( const CStateInitialImageMomentum & p) const;

  CStateInitialImageMomentum operator*( const T & p) const;

  VectorImageType * GetPointerToInitialImage() const;
  VectorImageType * GetPointerToInitialMomentum() const;

  virtual long int GetNumberOfStateVectorElements();
  virtual T* GetPointerToStateVector();

  virtual long int GetNumberOfStateVectorElementsToEstimate();
  virtual T* GetPointerToStateVectorElementsToEstimate();

  T SquaredNorm();

  GetMacro( EstimateInitialImage, bool );
  SetMacro( EstimateInitialImage, bool );

protected:
  void ClearDataStructure();
  void CopyDataStructure( const VectorImageType * ptrImage, const VectorImageType * ptrMomentum );

private:
  typename VectorImageType::Pointer m_ptrInitialImage;
  typename VectorImageType::Pointer m_ptrInitialMomentum;

  long int m_NumberOfStateVectorElements;
  T* m_ptrRawData;

  bool m_EstimateInitialImage;
  const bool DefaultEstimateInitialImage;
  bool m_ExternallySetEstimateInitialImage;

};

#include "CStateInitialImageMomentum.txx"

} // end namespace

#endif // C_STATE_INITIAL_IMAGE_MOMENTUM_H

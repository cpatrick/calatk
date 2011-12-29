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

#ifndef C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H
#define C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H

#include "CStateImageDomain.h"
#include "VectorField.h"
#include "CResamplerLinear.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3, class TResampler=CResamplerLinear<T, VImageDimension> >
class CStateSpatioTemporalVelocityField : public CStateImageDomain< T, VImageDimension, TResampler >
{
public:

  /* some useful typedefs */

  typedef CStateImageDomain< T, VImageDimension, TResampler > Superclass;
  typedef typename Superclass::TState SuperclassTState;
  typedef CStateSpatioTemporalVelocityField< T, VImageDimension, TResampler > TState;
  typedef typename Superclass::VectorImageType VectorImageType;
  
  typedef VectorField< T, VImageDimension > VectorFieldType;
  typedef VectorFieldType* VectorFieldPointerType;
  typedef std::vector< VectorFieldPointerType >* VectorPointerToVectorFieldPointerType;
  typedef const std::vector< VectorFieldPointerType >* ConstVectorPointerToVectorFieldPointerType;
  /**
   * Empty constructor
   */
  CStateSpatioTemporalVelocityField();

  /**
   * Constructor which takes a pointer of a vector of vector fields as an input.
   * Does not copy the data, but just stores the pointers to it. 
   * Destructor will destroy the vector fields though.
   */
  CStateSpatioTemporalVelocityField( ConstVectorPointerToVectorFieldPointerType pVecVecField );

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c );

  /**
   * Destructor, this class will involve dynamic memory allocation, so needs a destructor
   */
  ~CStateSpatioTemporalVelocityField();

  /*
   * Allow for upsampling of the state
   */
  SuperclassTState* CreateUpsampledStateAndAllocateMemory( const VectorImageType* pGraftImage ) const; 

  // declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateSpatioTemporalVelocityField & operator=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator+=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator-=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator*=( const T & p );

  CStateSpatioTemporalVelocityField operator+( const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator-( const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator*( const T & p ) const;

  ConstVectorPointerToVectorFieldPointerType GetVectorPointerToVectorFieldPointer() const;

  VectorFieldPointerType GetVectorFieldPointer( unsigned int iI );
  void SetVectorFieldPointer( unsigned int iI, VectorFieldPointerType ptrVecField );

  void SetSize( unsigned int iS );
  unsigned int GetSize();

  T SquaredNorm();

protected:

  void ClearDataStructure();
  void CopyDataStructure( ConstVectorPointerToVectorFieldPointerType ptrSource );

private:

  std::vector<VectorFieldPointerType> m_vecPtrSTVelocityField;

};

#include "CStateSpatioTemporalVelocityField.txx"

} // end namespace


#endif

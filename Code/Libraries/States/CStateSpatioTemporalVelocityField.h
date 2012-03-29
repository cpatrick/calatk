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

namespace CALATK
{

template < class TFloat, unsigned int VImageDimension=3 >
class CStateSpatioTemporalVelocityField : public CStateImageDomain< TFloat, VImageDimension >
{
public:
  /* Standard class typedefs. */
  typedef CStateSpatioTemporalVelocityField            Self;
  typedef itk::SmartPointer< Self >                    Pointer;
  typedef itk::SmartPointer< const Self >              ConstPointer;
  typedef CStateImageDomain< TFloat, VImageDimension > Superclass;

  /* some useful typedefs */
  typedef typename Superclass::VectorImageType  VectorImageType;

  typedef VectorField< TFloat, VImageDimension >            VectorFieldType;
  typedef std::vector< typename VectorFieldType::Pointer >  VectorFieldTimeSeriesType;

  /**
   * Empty constructor
   */
  CStateSpatioTemporalVelocityField();

  /**
   * Constructor which takes a VectorFieldTimeSeries as an input.
   * Does not copy the data, but just stores the pointers to it.
   */
  CStateSpatioTemporalVelocityField( const VectorFieldTimeSeriesType & vectorFieldTimeSeries );

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c );

  /**
   * Destructor
   */
  ~CStateSpatioTemporalVelocityField();

  /*
   * Allow for upsampling of the state.
   */
  Superclass* CreateUpsampledStateAndAllocateMemory( const VectorImageType* graftImage ) const;

  // declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateSpatioTemporalVelocityField & operator=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator+=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator-=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator*=( const TFloat & p );

  CStateSpatioTemporalVelocityField operator+( const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator-( const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator*( const TFloat & p ) const;

  const VectorFieldTimeSeriesType * GetVectorFieldTimeSeries() const;

  VectorFieldType * GetVectorFieldPointer( unsigned int ii );
  void SetVectorFieldPointer( unsigned int ii, VectorFieldType * ptrVecField );

  void SetSize( unsigned int ii );
  unsigned int GetSize();

  virtual TFloat SquaredNorm();

protected:
  void ClearDataStructure();
  void CopyDataStructure( const VectorFieldTimeSeriesType * ptrSource );

private:
  VectorFieldTimeSeriesType m_VectorFieldTimeSeries;
};

} // end namespace

#endif

#ifndef C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H
#define C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H

#include "CState.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CStateSpatioTemporalVelocityField : public CState< T >
{
public:
  
  /**
   * Empty constructor
   */
  CStateSpatioTemporalVelocityField();

  /**
   * copy constructor, creation of the image for the first time, need to allocate memory
   */
  CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c );

  /**
   * Destructor, this class will involve dynamic memory allocation, so needs a destructor
   */
  ~CStateSpatioTemporalVelocityField();

  // declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateSpatioTemporalVelocityField & operator=(const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator+=(const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator-=(const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField operator+(const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator-(const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator*(const T & p ) const;

protected:
private:
};

#include "CStateSpatioTemporalVelocityField.txx"

} // end namespace


#endif

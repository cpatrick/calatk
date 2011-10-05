#ifndef C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H
#define C_STATE_SPATIOTEMPORAL_VELOCITY_FIELD_H

#include "CState.h"
#include "VectorField.h"

namespace CALATK
{

template <class T, unsigned int VImageDimension=3 >
class CStateSpatioTemporalVelocityField : public CState< T >
{
public:

  /** some useful typedefs */

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

  // declare operators to be able to do some computations with this state, which are needed in the numerical solvers

  /**
   * assignment, memory already allocated, may need to be destroyed and re-allocated
   */
  CStateSpatioTemporalVelocityField & operator=( const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator+=(const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator-=(const CStateSpatioTemporalVelocityField & p );

  CStateSpatioTemporalVelocityField & operator*=(const T & p );

  CStateSpatioTemporalVelocityField operator+(const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator-(const CStateSpatioTemporalVelocityField & p ) const;

  CStateSpatioTemporalVelocityField operator*(const T & p ) const;

  ConstVectorPointerToVectorFieldPointerType GetVectorPointerToVectorFieldPointer() const;

  VectorFieldPointerType GetVectorFieldPointer( unsigned int iI );
  void SetVectorFieldPointer( unsigned int iI, VectorFieldPointerType ptrVecField );

  void SetSize( unsigned int iS );
  unsigned int GetSize();

protected:

  void ClearDataStructure();
  void CopyDataStructure( ConstVectorPointerToVectorFieldPointerType ptrSource );

private:

  std::vector<VectorFieldPointerType> m_vecPtrSTVelocityField;

};

#include "CStateSpatioTemporalVelocityField.txx"

} // end namespace


#endif

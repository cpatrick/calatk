#ifndef C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX
#define C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX


//
// empty constructor
//
template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField()
{
  m_vecPtrSTVelocityField.clear();
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension>  
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c ) 
{
  if ( this != &c )
    {
    ConstVectorPointerToVectorFieldPointerType ptrSource = c.GetVectorPointerToVectorFieldPointer();
    CopyDataStructure( ptrSource );
    }
}


//
// constructor which takes a vector of vectorfields as input
//
template <class T, unsigned int VImageDimension>  
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField( ConstVectorPointerToVectorFieldPointerType pVecVecField )
{
  ClearDataStructure(); // just in case we are overwriting something
  typename std::vector<VectorFieldPointerType>::const_iterator iter;
  for ( iter = pVecVecField->begin(); iter != pVecVecField->end(); ++iter )
    {
    m_vecPtrSTVelocityField.push_back( *iter );
    }
}

//
// clear data structure
//
template <class T, unsigned int VImageDimension>
void CStateSpatioTemporalVelocityField< T, VImageDimension>::ClearDataStructure()
{
  typename std::vector< VectorFieldPointerType >::iterator iter;
  for ( iter = m_vecPtrSTVelocityField.begin(); iter != m_vecPtrSTVelocityField.end(); ++iter )
    {
    // delete vector field
    if ( *iter != NULL )
      {
      delete *iter;
      }
    }
  m_vecPtrSTVelocityField.clear();
}

//
// copy data structures
//
template <class T, unsigned int VImageDimension>
void CStateSpatioTemporalVelocityField< T, VImageDimension>::CopyDataStructure( ConstVectorPointerToVectorFieldPointerType ptrSource )
{
    ClearDataStructure();

    if ( ptrSource != NULL )
      {
      // iterate through it and create a new vector field here
      typename std::vector< VectorFieldPointerType >::const_iterator iter;
      for ( iter = ptrSource->begin(); iter != ptrSource->end(); ++iter )
        {
        // create a vector and initialize its content 
        VectorFieldPointerType ptrCurrentVectorField = new VectorFieldType( *iter );
        m_vecPtrSTVelocityField.push_back( ptrCurrentVectorField );
        }
      }
}

//
// destructor
//
template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >::~CStateSpatioTemporalVelocityField()
{
  ClearDataStructure();
}

//
// sets the size of the datastructure, only needed if the vector field pointers should be assigned manually
// initializes all the entries to NULL
//
template <class T, unsigned int VImageDimension>
void CStateSpatioTemporalVelocityField< T, VImageDimension >::SetSize( unsigned int iS )
{
  ClearDataStructure();
  for ( unsigned int iI=0; iI<iS; ++iI )
    {
    m_vecPtrSTVelocityField.push_back( NULL );
    }
}

//
// returns the number of vector fields stored in the spatio temporal data structure
//
template <class T, unsigned int VImageDimension>
unsigned int CStateSpatioTemporalVelocityField< T, VImageDimension >::GetSize()
{
  return m_vecPtrSTVelocityField.size();
}

//
// returns the pointer to a particular vector field
//
template <class T, unsigned int VImageDimension>
typename CStateSpatioTemporalVelocityField< T, VImageDimension >::VectorFieldPointerType
CStateSpatioTemporalVelocityField< T, VImageDimension >::GetVectorFieldPointer( unsigned int iI )
{
  assert( iI>=0 && iI<m_vecPtrSTVelocityField.size() );
  return m_vecPtrSTVelocityField[ iI ];
}

//
// returns the pointer to all the vector field pointers
//
template <class T, unsigned int VImageDimension>
typename CStateSpatioTemporalVelocityField< T, VImageDimension >::ConstVectorPointerToVectorFieldPointerType
CStateSpatioTemporalVelocityField< T, VImageDimension >::GetVectorPointerToVectorFieldPointer() const
{
  return &m_vecPtrSTVelocityField;
}

/*
template <class T, unsigned int VImageDimension>
typename CStateSpatioTemporalVelocityField< T, VImageDimension >::VectorPointerToVectorFieldPointerType
CStateSpatioTemporalVelocityField< T, VImageDimension >::GetVectorPointerToVectorFieldPointer()
{
  return &m_vecPtrSTVelocityField;
}
*/

//
// sets a vector field as an element of the spatio temporal velocity field data structure
//
template <class T, unsigned int VImageDimension>
void CStateSpatioTemporalVelocityField< T, VImageDimension >::SetVectorFieldPointer( unsigned int iI, VectorFieldPointerType ptrVecField )
{
  assert( iI>=0 && iI<m_vecPtrSTVelocityField.size() );
  m_vecPtrSTVelocityField[ iI ] = ptrVecField;
}

// Here come the algebraic operators and assignment

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension > & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator=(const CStateSpatioTemporalVelocityField & p )
{
  if ( this != &p )
    {
    ConstVectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

    // now do a deep copy
    
    // check if we already have the same number of elements. If so overwrite, otherwise recreate
    if ( m_vecPtrSTVelocityField.size() == p.GetVectorPointerToVectorFieldPointer()->size() )
      {
      // already memory of appropriate size allocated, so just copy
      // iterate and copy
      
      typename std::vector< VectorFieldPointerType >::const_iterator iterSource;
      typename std::vector< VectorFieldPointerType >::iterator iterTarget;
      for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
            iterSource != ptrSource->end(), iterTarget != m_vecPtrSTVelocityField.end(); 
            ++iterSource, ++iterTarget )
        {
        // copy the current vector field
        (*iterTarget)->copy( *iterSource );
        }
      }
    else
      {
      // clear and then allocate, this should typically not be necessary
      std::cerr << "WARNING: reallocating memory, should already have been assigned." << std::endl;
      CopyDataStructure( ptrSource );      
      }
    return *this;
    }
  else
    {
    return *this;
    }
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension > & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+=(const CStateSpatioTemporalVelocityField & p )
{
  if ( m_vecPtrSTVelocityField.size() != p.GetVectorPointerToVectorFieldPointer()->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    }

  ConstVectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

  typename std::vector< VectorFieldPointerType >::const_iterator iterSource;
  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
        iterSource != ptrSource->end(), iterTarget != m_vecPtrSTVelocityField.end(); 
        ++iterSource, ++iterTarget )
    {
    // add the source to the target
    (*iterTarget)->addCellwise( *iterSource );
    }

  return *this;
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension > & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-=(const CStateSpatioTemporalVelocityField & p )
{

  if ( m_vecPtrSTVelocityField.size() != p.GetVectorPointerToVectorFieldPointer->size() )
    {
    throw std::runtime_error( "Size mismatch of vector of vector fields. ABORT." );
    return;
    }

  VectorPointerToVectorFieldPointerType ptrSource = p.GetVectorPointerToVectorFieldPointer();

  typename std::vector< VectorFieldPointerType >::iterator iterSource;
  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterSource = ptrSource->begin(), iterTarget = m_vecPtrSTVelocityField.begin(); 
        iterSource != ptrSource->end(), iterTarget != m_vecPtrSTVelocityField.end(); 
        ++iterSource, ++iterTarget )
    {
    // subtract the source from the target
    iterTarget->subtractCellwise( *iterSource );
    }

  return *this;
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension > & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator*=(const T & p )
{

  typename std::vector< VectorFieldPointerType >::iterator iterTarget;
  for ( iterTarget = m_vecPtrSTVelocityField.begin(); iterTarget != m_vecPtrSTVelocityField.end(); ++iterTarget )
    {
    // multiply by the value
    (*iterTarget)->multConst( p );
    }

  return *this;
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r += p;
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-(const CStateSpatioTemporalVelocityField & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;
  return r -= p;
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension > 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator*(const T & p ) const
{
  CStateSpatioTemporalVelocityField r = *this;

  // multiply all values with a scalar
  r.multConst( p );

  return r*= p;
}

#endif

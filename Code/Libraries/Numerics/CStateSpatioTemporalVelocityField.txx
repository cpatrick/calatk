#ifndef C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX
#define C_STATE_SPATIO_TEMPORAL_VELOCITYFIELD_TXX

//
// empty constructor
//
template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField()
{
}

//
// copy constructor
//
template <class T, unsigned int VImageDimension>  
CStateSpatioTemporalVelocityField< T, VImageDimension >::CStateSpatioTemporalVelocityField( const CStateSpatioTemporalVelocityField & c ) 
{
}

//
// destructor
//
template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField< T, VImageDimension >::~CStateSpatioTemporalVelocityField()
{
}

// Here come the algebraic operators and assignment

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator=(const CStateSpatioTemporalVelocityField & p )
{
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+=(const CStateSpatioTemporalVelocityField & p )
{
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField & 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-=(const CStateSpatioTemporalVelocityField & p )
{
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator+(const CStateSpatioTemporalVelocityField & p ) const
{
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator-(const CStateSpatioTemporalVelocityField & p ) const
{
}

template <class T, unsigned int VImageDimension>
CStateSpatioTemporalVelocityField 
CStateSpatioTemporalVelocityField< T, VImageDimension >::operator*(const T & p ) const
{
}

#endif

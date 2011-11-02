#ifndef C_STATE_H
#define C_STATE_H

/**
 * Base class for a state. So far does not do much other than declaring certain operators private
 * May want to add more common functionality later.
 */


namespace CALATK
{

template <class T, unsigned int VImageDimension, class TResampler >
class CState
{
public:
  // default empty constuctor
  CState() { }
protected:
private:
  // copy constructor private
  CState( const CState & c ) {}
  
  // operators private
  CState & operator=(const CState &p ) { }
  CState & operator+=(const CState &p ) { }
  CState & operator-=(const CState &p ) { }
  CState operator+(const CState &p ) const { }
  CState operator-(const CState &p ) const { }
  CState operator*(const T &p ) const { }

  virtual T SquaredNorm() = 0;

};

} // end namespace


#endif

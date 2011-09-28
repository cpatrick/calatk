#ifndef C_STATE
#define C_STATE

/**
 * Base class for a state. So far does not do much other than declaring certain operators private
 * May want to add more common functionality later.
 */


namespace CALATK
{

template <class T>
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

};

} // end namespace


#endif

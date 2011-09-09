#ifndef C_STATE_SCALAR_EXAMPLE_H
#define C_STATE_SCALAR_EXAMPLE_H

namespace CALATK
{

template <class T>
class CStateScalarExample 
{

public:
  CStateScalarExample()
  {
    val = 0;
  }

  // copy constructor, obvious for scalar case, this is just to show the principle
  CStateScalarExample( const CStateScalarExample& c ) : val( c.val ) { }

  // now declare obbious methdods, just to demonstrate how 
  // it would be done for a more complicated data structure

  // to support assignment
  CStateScalarExample & operator=(const CStateScalarExample &p )
  {
    if ( this==&p )
      return *this;
    this->val = p.val;
    return *this;
  }

  // to support +=
  CStateScalarExample & operator+=(const CStateScalarExample &p ) 
  {
    this->val += p.val;
    return *this;
  }

  // to support -=
  CStateScalarExample & operator-=(const CStateScalarExample &p ) 
  {
    this->val -= p.val;
    return *this;
  }

  // to support addition
  CStateScalarExample operator+(const CStateScalarExample &p ) const
  {
    CStateScalarExample r = *this;
    return r += p;
  }

  // to support subtraction
  CStateScalarExample operator-(const CStateScalarExample &p ) const
  {
    CStateScalarExample r = *this;
    return r -= p;
  }

  // to support right sided multiplication with a scalar
  CStateScalarExample operator*(const T &p ) const
  {
    CStateScalarExample r = *this;
    r.val *= p;
    return r;
  }

  inline void SetValue( T val )
  {
    this->val = val;
  }

  inline T GetValue()
  {
    return val;
  }

  private:

  T val;

};

// make sure we can output it
template < class T >
std::ostream &operator<<(std::ostream &stream, CStateScalarExample< T >& o)
{
  stream << o.GetValue();
  return stream; 
}

} // end namespace

#endif

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

#ifndef C_STATE_SCALAR_EXAMPLE_H
#define C_STATE_SCALAR_EXAMPLE_H

#include "CState.h"

namespace CALATK
{

/**
 * \class CStateScalarExample
 *
 * \todo describe me
 */
template <class T>
class CStateScalarExample : public CState< T, 0, T >
{
public:
  /* Standard class typedefs. */
  typedef CStateScalarExample              Self;
  typedef itk::SmartPointer< Self >        Pointer;
  typedef itk::SmartPointer< const Self >  ConstPointer;
  typedef CState< T, 0, T >                Superclass;

  CStateScalarExample()
  {
    val = 0;
  }

  // copy constructor, obvious for scalar case, this is just to show the principle
  CStateScalarExample( const CStateScalarExample& c ) : val( c.val ) { }

  // now declare obvious methdods, just to demonstrate how 
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

  // to support *=
  CStateScalarExample & operator*=(const T &p )
  {
    this->val *= p;
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
    return r *= p;
  }

  inline void SetValue( T val )
  {
    this->val = val;
  }

  inline T GetValue()
  {
    return val;
  }

  inline T SquaredNorm()
  {
    return val*val;
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

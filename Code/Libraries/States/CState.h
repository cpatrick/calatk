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

#ifndef C_STATE_H
#define C_STATE_H


#include "CBase.h"
#include "itkSmartPointer.h"


namespace CALATK
{

/**
 * \class CState
 *
 * Base class for a state. So far does not do much other than declaring certain operators private
 * May want to add more common functionality later.
 */
template <class T, unsigned int DIM, class TResamplerType >
class CState: public CBase
{
public:

  /* Useful typedefs and constants */
  typedef T                 TFloat;
  typedef TResamplerType    TResampler;
  static const unsigned int VImageDimension = DIM;

  /** Standard typedefs. */
  typedef CState                          Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;


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

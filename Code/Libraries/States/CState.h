/*
*
*  Copyright 2011, 2012 by the CALATK development team
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
#include <string>

namespace CALATK
{

/**
 * \class CState
 *
 * Base class for a state. So far does not do much.
 * May want to add more common functionality later.
 */
template < class TFloat >
class CState: public CBase
{
public:
  /* Useful typedefs and constants */
  typedef TFloat            FloatType;

  /** Standard typedefs. */
  typedef CState                          Self;
  typedef CBase                           Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;


  // default empty constuctor
  CState() { }

  virtual long int GetNumberOfStateVectorElements()
  {
    return 0;
  };

  virtual long int GetNumberOfStateVectorElementsToEstimate()
  {
    return 0;
  }

  virtual FloatType* GetPointerToStateVector()
  {
    return NULL;
  };

  virtual FloatType* GetPointerToStateVectorElementsToEstimate()
  {
    return NULL;
  };

  virtual void WriteStateToFile( std::string prefix, std::string postfix, int offset = 0 )
  {
    throw std::runtime_error( "Not yet implementded." );
  }

protected:
  virtual FloatType SquaredNorm() = 0;

private:
  // copy constructor private
  CState( const CState & c ) {}
};

} // end namespace

#endif

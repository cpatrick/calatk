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

#ifndef C_BASE_H
#define C_BASE_H

#include "itkSimpleFastMutexLock.h"

#if defined( _WIN32 )
// To get LONG defined
  #include "itkWindows.h"
#elif defined( __APPLE__ )
// To get MAC_OS_X_VERSION_MIN_REQUIRED defined
  #include <AvailabilityMacros.h>
#endif


namespace CALATK
{

/**
 * \class CBase
 * Base class that does reference counting.  Defines Register() and UnRegister()
 * methods, so an itk::SmartPointer can be created from the class.
 */
class CBase
{
public:
  CBase();
  virtual ~CBase();

  /** Increase the reference count (mark as used by another object).  */
  virtual void Register() const;

  /** Decrease the reference count (release by another object).  */
  virtual void UnRegister() const;

  /** Gets the reference count on this object. */
  virtual int GetReferenceCount() const
  { return static_cast< int >( m_ReferenceCount ); }

  /** Delete an itk object.  This method should always be used to delete an
   *  object when the new operator was used to create it. Using the C++
   *  delete method will not work with reference counting.  */
  virtual void Delete();

#ifdef _WIN32
  /** Used to avoid dll boundary problems.  */
  void * operator new(size_t);

  void * operator new[](size_t);

  void operator delete(void *);

  void operator delete[](void *, size_t);
#endif

protected:
  /** Define the type of the reference count according to the
      target. This allows the use of atomic operations */
#if ( defined( WIN32 ) || defined( _WIN32 ) )
  typedef LONG InternalReferenceCountType;
#elif defined( __APPLE__ ) && ( MAC_OS_X_VERSION_MIN_REQUIRED >= 1050 )
 #if defined ( __LP64__ ) && __LP64__
  typedef volatile int64_t InternalReferenceCountType;
 #else
  typedef volatile int32_t InternalReferenceCountType;
 #endif
#elif defined( __GLIBCPP__ ) || defined( __GLIBCXX__ )
  typedef _Atomic_word InternalReferenceCountType;
#else
  typedef int InternalReferenceCountType;
#endif

  /** Number of uses of this object by other objects. */
  mutable InternalReferenceCountType m_ReferenceCount;

  /** Mutex lock to protect modification to the reference count */
  mutable itk::SimpleFastMutexLock m_ReferenceCountLock;
};

}

#endif

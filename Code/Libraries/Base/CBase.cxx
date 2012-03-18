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

#include "CBase.h"

#if defined( __APPLE__ )
// OSAtomic.h optimizations only used in 10.5 and later
  #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
    #include <libkern/OSAtomic.h>
  #endif

#elif defined( __GLIBCPP__ ) || defined( __GLIBCXX__ )
  #if ( __GNUC__ > 4 ) || ( ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 2 ) )
  #include <ext/atomicity.h>
  #else
  #include <bits/atomicity.h>
  #endif

#endif
namespace CALATK
{

#if defined( __GLIBCXX__ ) // g++ 3.4+

using __gnu_cxx::__atomic_add;
using __gnu_cxx::__exchange_and_add;

#endif

CBase::CBase():
  m_ReferenceCount(1)
{}

void CBase::Delete()
{
  this->UnRegister();
}

#ifdef _WIN32
void * CBase::operator new(size_t n)
{
  return new char[n];
}

void * CBase::operator new[](size_t n)
{
  return new char[n];
}

void CBase::operator delete(void *m)
{
  delete[] (char *)m;
}

void CBase::operator delete[](void *m, size_t)
{
  delete[] (char *)m;
}
#endif // _WIN32

void CBase::Register() const
{
  // Windows optimization
#if ( defined( WIN32 ) || defined( _WIN32 ) )
  InterlockedIncrement(&m_ReferenceCount);

  // Mac optimization
#elif defined( __APPLE__ ) && ( MAC_OS_X_VERSION_MIN_REQUIRED >= 1050 )
 #if defined ( __LP64__ ) && __LP64__
  OSAtomicIncrement64Barrier(&m_ReferenceCount);
 #else
  OSAtomicIncrement32Barrier(&m_ReferenceCount);
 #endif

  // gcc optimization
#elif defined( __GLIBCPP__ ) || defined( __GLIBCXX__ )
  __atomic_add(&m_ReferenceCount, 1);

  // General case
#else
  m_ReferenceCountLock.Lock();
  m_ReferenceCount++;
  m_ReferenceCountLock.Unlock();
#endif
}

void CBase::UnRegister() const
{
  // As ReferenceCount gets unlocked, we may have a race condition
  // to delete the object.

  // Windows optimization
#if ( defined( WIN32 ) || defined( _WIN32 ) )
  if ( InterlockedDecrement(&m_ReferenceCount) <= 0 )
    {
    delete this;
    }

// Mac optimization
#elif defined( __APPLE__ ) && ( MAC_OS_X_VERSION_MIN_REQUIRED >= 1050 )
 #if defined ( __LP64__ ) && __LP64__
  if ( OSAtomicDecrement64Barrier(&m_ReferenceCount) <= 0 )
    {
    delete this;
    }
 #else
  if ( OSAtomicDecrement32Barrier(&m_ReferenceCount) <= 0 )
    {
    delete this;
    }
 #endif

// gcc optimization
#elif defined( __GLIBCPP__ ) || defined( __GLIBCXX__ )
  if ( __exchange_and_add(&m_ReferenceCount, -1) <= 1 )
    {
    delete this;
    }

// General case
#else
  m_ReferenceCountLock.Lock();
  InternalReferenceCountType tmpReferenceCount = --m_ReferenceCount;
  m_ReferenceCountLock.Unlock();

  if ( tmpReferenceCount <= 0 )
    {
    delete this;
    }
#endif
}

CBase::~CBase()
{
  /**
   * warn user if reference counting is on and the object is being referenced
   * by another object.
   * a call to uncaught_exception is necessary here to avoid throwing an
   * exception if one has been thrown already. This is likely to
   * happen when a subclass constructor (say B) is throwing an exception: at
   * that point, the stack unwinds by calling all superclass destructors back
   * to this method (~CBase): since the ref count is still 1, an
   * exception would be thrown again, causing the system to abort()!
   */
  if ( this->m_ReferenceCount > 0 && !std::uncaught_exception() )
    {
    // A general exception safety rule is that destructors should
    // never throw.  Something is wrong with a program that reaches
    // this point anyway.  Also this is the least-derived class so the
    // whole object has been destroyed by this point anyway.  Just
    // issue a warning.
    std::cerr << "Trying to delete object with non-zero reference count." << std::endl;
    }
}

} // end namespace CALATK

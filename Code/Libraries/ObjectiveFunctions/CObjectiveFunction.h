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

#ifndef C_OBJECTIVE_FUNCTION_H
#define C_OBJECTIVE_FUNCTION_H

#include "CALATKCommon.h"
#include "CMetric.h"
#include "CImageManager.h"
#include "CKernel.h"
#include "CProcessBase.h"
#include "JSONParameterUtils.h"

namespace CALATK
{

template < class TState >
class CObjectiveFunction : public CProcessBase
{
public:

  /* some useful typedefs */
  
  typedef typename TState::TFloat T;

  typedef TState* ptrStateType;

  typedef CMetric< T, TState::VImageDimension >* ptrMetricType;
  typedef CKernel< T, TState::VImageDimension >* ptrKernelType;
  typedef CImageManager< T, TState::VImageDimension >* ptrImageManagerType;

  typedef VectorImage< T, TState::VImageDimension > VectorImageType;
  typedef VectorField< T, TState::VImageDimension > VectorFieldType;

  // structure to return the energy
  class CEnergyValues
  {
  public:

    CEnergyValues()
      : dEnergy( 0 ), dRegularizationEnergy( 0 ), dMatchingEnergy( 0 )
    {};

    // copy constructor
    CEnergyValues( const CEnergyValues& c ) :
      dEnergy( c.dEnergy ),
      dRegularizationEnergy( c.dRegularizationEnergy ),
      dMatchingEnergy( c.dMatchingEnergy )
    {};

    // assignment
    CEnergyValues & operator=( const CEnergyValues &p )
    {
      if ( this==&p )
        return *this;
      this->dEnergy = p.dEnergy;
      this->dRegularizationEnergy = p.dRegularizationEnergy;
      this->dMatchingEnergy = p.dMatchingEnergy;
      return *this;
    };

    CEnergyValues & operator+=( const CEnergyValues & p )
    {
      this->dEnergy += p.dEnergy;
      this->dRegularizationEnergy += p.dRegularizationEnergy;
      this->dMatchingEnergy += p.dMatchingEnergy;
      return *this;
    };

    CEnergyValues & operator-=( const CEnergyValues & p )
    {
      this->dEnergy -= p.dEnergy;
      this->dRegularizationEnergy -= p.dRegularizationEnergy;
      this->dMatchingEnergy -= p.dMatchingEnergy;
      return *this;
    };

    CEnergyValues & operator*=( const CEnergyValues & p )
    {
      this->dEnergy *= p.dEnergy;
      this->dRegularizationEnergy *= p.dRegularizationEnergy;
      this->dMatchingEnergy *= p.dMatchingEnergy;
      return *this;
    };

    CEnergyValues operator+( const CEnergyValues & p ) const
    {
      CEnergyValues r = *this;
      return r += p;
    };

    CEnergyValues operator-( const CEnergyValues & p ) const
    {
      CEnergyValues r = *this;
      return r -= p;
    };

    CEnergyValues operator*( const CEnergyValues &p ) const
    {
      CEnergyValues r = *this;
      return r *= p;
    }

    T dEnergy;
    T dRegularizationEnergy;
    T dMatchingEnergy;
  };

  CObjectiveFunction();
  virtual ~CObjectiveFunction();

  /* Returns the energy of the objective function */
  virtual CEnergyValues GetCurrentEnergy() = 0;

  /* Compute the gradient of the objective function and store it in the gradient member variable */
  virtual void ComputeGradient();

  /* Gets the map at a specific time, assuming that the map for the initial image is the identity */
  virtual void GetMap( VectorFieldType* ptrMap, T dTime ) = 0;

  /* Gets the map between specific time points */
  virtual void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo ) = 0;

  /* Gets the image at a specific time point */
  virtual void GetImage( VectorImageType* ptrIm, T dTime ) = 0;

  /* Gets the initial image */
  virtual void GetInitialImage( VectorImageType* ptrIm ) = 0;

  virtual void InitializeState() = 0;
  virtual void InitializeState( TState* ) = 0;

  /* Can get called to create an output of a current state (input is iteration number) */
  virtual void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" )
  {
  };

  void SetImageManagerPointer( ptrImageManagerType ptrImageManager )
  {
    m_ptrImageManager = ptrImageManager;
  };

  ptrImageManagerType GetImageManagerPointer() const
  {
    return m_ptrImageManager;
  };

  void SetStatePointer( ptrStateType pState )
  {
    m_pState = pState;
  };

  ptrStateType GetStatePointer() const
  {
    return m_pState;
  };

  void SetGradientPointer( ptrStateType pGradient )
  {
    m_pGradient = pGradient;
  };

  ptrStateType GetGradientPointer() const
  {
    return m_pGradient;
  };

  void SetMetricPointer( ptrMetricType pMetric )
  {
    m_pMetric = pMetric;
  };

  ptrMetricType GetMetricPointer() const
  {
    return m_pMetric;
  };

protected:

  ptrStateType m_pState;
  ptrStateType m_pGradient;

  // TODO: FIXME, needs to be an array, so we can in principle support registrations at every measurement
  ptrMetricType m_pMetric;
  ptrImageManagerType m_ptrImageManager;

};

#include "CObjectiveFunction.txx"

} // end namespace

#endif

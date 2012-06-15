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

#ifndef C_OBJECTIVE_FUNCTION_BASE_H
#define C_OBJECTIVE_FUNCTION_BASE_H

#include "CALATKCommon.h"
#include "CProcessBase.h"

#include "CMetric.h"
#include "CKernel.h"

#include "CImageManager.h"

namespace CALATK
{

/** Base class for objective functions which is still unaware of the state type */

template < class T, unsigned int VImageDimension >
class CObjectiveFunctionBase
    : public CProcessBase
{
public:
  /** Standard class typedefs. */
  typedef CObjectiveFunctionBase            Self;
  typedef CProcessBase                      Superclass;
  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  typedef CMetric< T, VImageDimension >       MetricType;
  typedef CKernel< T, VImageDimension >       KernelType;
  typedef CImageManager< T, VImageDimension > ImageManagerType;

  typedef typename MetricType::Pointer PointerMetricType;
  typedef typename ImageManagerType::Pointer PointerImageManagerType;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorField< T, VImageDimension > VectorFieldType;

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

  CObjectiveFunctionBase();
  virtual ~CObjectiveFunctionBase();

  /* Returns the energy of the objective function */
  virtual CEnergyValues GetCurrentEnergy() = 0;

  /* Gets the map at a specific time, assuming that the map for the initial image is the identity */
  virtual void GetMap( VectorFieldType* ptrMap, T dTime ) = 0;

  /* Gets the map between specific time points */
  virtual void GetMapFromTo( VectorFieldType* ptrMap, T dTimeFrom, T dTimeTo ) = 0;

  /* Gets the source image at a specific time point */
  virtual void GetSourceImage( VectorImageType* ptrIm, T dTime ) = 0;

  /* Gets the target image at a specific time point */
  virtual void GetTargetImage( VectorImageType* ptrIm, T dTime ) = 0;

  /* Gets a pointer to the initial image */
  virtual const VectorImageType* GetPointerToInitialImage() const = 0;

  /* Gets the initial image */
  virtual void GetInitialImage( VectorImageType* ptrIm ) = 0;

  /* Can get called to create an output of a current state (input is iteration number) */
  virtual void OutputStateInformation( unsigned int uiIter, std::string outputPrefix="" )
  {
  };

  /**
   * @brief Sets the subject id which is then used in subsequent algorithms to select subject-specific timeseries
   *
   * @param uid  -- unique subject id
   */
  void SetActiveSubjectId( int uid );

  /**
   * @brief Returns the currently set subject id which is used to select timeseries
   *
   * @return int
   */
  int GetActiveSubjectId() const;

  void SetImageManagerPointer( ImageManagerType * ptrImageManager );
  ImageManagerType* GetImageManagerPointer() const;

  void SetMetricPointer( MetricType* pMetric );
  MetricType* GetMetricPointer() const;

  /**
  * Computes initial un-smoothed velocity gradient of the velocity.
  * This can be used to estimate weights for multi-Gaussian kernels for example.
  */
  virtual void ComputeInitialUnsmoothedVelocityGradient( VectorFieldType* ptrInitialUnsmoothedVelocityGradient, unsigned int uiKernelNumber = 0 );

  // method being called before a subiteration of a numerical solver for the multi-scale solver, overwrite
  virtual void PreSubIterationSolve();

protected:

  // TODO: FIXME, needs to be an array, so we can in principle support registrations at every measurement
  PointerMetricType m_ptrMetric;
  PointerImageManagerType m_ptrImageManager;

  int m_CurrentlyActiveSubjectId;
};

} // end namespace

#include "CObjectiveFunctionBase.txx"

#endif // C_OBJECTIVE_FUNCTION_BASE_H

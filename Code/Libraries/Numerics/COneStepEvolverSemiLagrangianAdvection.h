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

#ifndef C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H
#define C_ONESTEPEVOLVER_SEMILAGRANGIAN_ADVECTION_H

#include <stdexcept>

#include "CALATKCommon.h"
#include "COneStepEvolver.h"
#include "VectorFieldUtils.h"
#include "CFastLinearInterpolator.h"
#include "CLinearInterpolator.h"
#include "CCubicConvolutionInterpolator.h"
#include "VectorImageUtils.h"

namespace CALATK
{

/**
 * \class COneStepEvolverSemiLagrangianAdvection
 *
 * \brief Implementation of a one-step advection evolver using a semi-lagrangian approach
 *
 * The assumption is that the underlying velocity field is static for this step.
 * The approach followed is then a simplified version of the approach described by Stanisforth and Cote in
 *
 * A. Stanisforth and J. Cote, "Semi-Lagrangian Schemes for Atmospheric Models - A Review,"
 * Monthly Weather Review, vol 119, pp. 2206-2223, 1991.
 *
 * The approach simplifies in this setting because the velocity field is assumed to be static for a finite time interval.
 *
 * TODO: Implement shooting solvers which do no assume piecewise stationary velocity fields.
 * This will then require a modified Semi-Lagrangian solver.
 *
 */
template <class T, unsigned int VImageDimension=3 >
class COneStepEvolverSemiLagrangianAdvection : public COneStepEvolver< T, VImageDimension >
{
public:
  /** Standard class typedefs. */
  typedef COneStepEvolverSemiLagrangianAdvection  Self;
  typedef COneStepEvolver< T, VImageDimension >   Superclass;
  typedef itk::SmartPointer< Self >               Pointer;
  typedef itk::SmartPointer< const Self >         ConstPointer;

  typedef typename Superclass::VectorFieldType VectorFieldType;
  typedef typename Superclass::VectorImageType VectorImageType;

  typedef VectorImageUtils< T, VImageDimension > VectorImageUtilsType;

  /**
   * Empty Constructor
   */
  COneStepEvolverSemiLagrangianAdvection();
  ~COneStepEvolverSemiLagrangianAdvection();

  virtual void PerformStep( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );

  T ComputeMaximalUpdateStep( const VectorFieldType* v ) const;

  SetMacro( NumberOfIterationsToDetermineFlowField, unsigned int );
  GetMacro( NumberOfIterationsToDetermineFlowField, unsigned int );

  SetMacro( TimeStepFactor, T );
  GetMacro( TimeStepFactor, T );

   virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  void PerformStepWithGivenVectorField( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );
  void PerformStepWithAdjustedVectorField( const VectorFieldType* v, const VectorImageType* In, VectorImageType* Inp1, T dt );

private:

  void ComputeAdjustedVectorFieldIfRequired( const VectorFieldType* v, T dt );

  typename VectorFieldType::Pointer m_ptrAdjustedVectorField; ///< Vector field which should be used because it points back to the point of origin for each grid point
  typename VectorFieldType::Pointer m_ptrTmpVectorField; ///< holds intermediate iteration results

  T m_TimeStepFactor; ///< multiplier for the CFL timestep (timstep can be larger for a semi-Lagrangian scheme)
  const T DefaultTimeStepFactor;
  bool m_ExternallySetTimeStepFactor;

  unsigned int m_NumberOfIterationsToDetermineFlowField; ///< Number of iterations to determine the starting point for a tracjtory (iterations for alpha in Stanisforth paper)
  const unsigned int DefaultNumberOfIterationsToDetermineFlowField;
  bool m_ExternallySetNumberOfIterationsToDetermineFlowField;

  CFastLinearInterpolator< T, VImageDimension > interpolator;
};

} // end namespace

#endif

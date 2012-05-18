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

#ifndef C_ATLAS_BUILDER_H
#define C_ATLAS_BUILDER_H

#include "CAtlasObjectiveFunction.h"
#include "CAlgorithm.h"

namespace CALATK
{

/**
  * A reasonable default value for the state is
  * TState = CStateMultipleStates< CStateInitialMomentum< T, VImageDimension > >
  *
  */
template < class TState >
class CAtlasBuilder : public CAlgorithm< TState >
{
public:
  /** Standard class typedefs. */
  typedef CAtlasBuilder                     Self;
  typedef CAlgorithm< TState >              Superclass;

  typedef typename TState::TIndividualState TIndividualState;

  typedef itk::SmartPointer< Self >         Pointer;
  typedef itk::SmartPointer< const Self >   ConstPointer;

  /* some useful typedefs */
  typedef typename TState::TFloat T;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorFieldType VectorFieldType;

  CAtlasBuilder();
  ~CAtlasBuilder();

  virtual void Solve();
  virtual void PreSubIterationSolve();

  SetMacro( AtlasIsSourceImage, bool );
  GetMacro( AtlasIsSourceImage, bool );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

protected:

  /**
    * Sets the objective function if not objective function was specified externally.
    */
  void SetDefaultObjectiveFunctionPointer();

  // TODO: Implement

  const VectorFieldType* GetMap( T dTime );
  const VectorFieldType* GetMapFromTo( T dTimeFrom, T dTimeTo );
  const VectorImageType* GetImage( T dTime );

  void SetDefaultMetricPointer();
  void SetDefaultImageManagerPointer();
  void SetDefaultKernelPointer();
  void SetDefaultEvolverPointer();
  void SetDefaultSolverPointer();

private:

  bool m_AtlasIsSourceImage;
  const bool DefaultAtlasIsSourceImage;
  bool m_ExternallySetAtlasIsSourceImage;

};

#include "CAtlasBuilder.txx"

} // end namespace


#endif // C_ATLAS_BUILDER_H

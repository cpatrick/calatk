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

#ifndef C_SOLVER_MULTISCALE_H
#define C_SOLVER_MULTISCALE_H

#include "CSolver.h"
#include "CSolverLineSearchUnconstrained.h"
#include "CImageManagerMultiScale.h"
#include "VectorImage.h"
#include "VectorImageUtils.h"

namespace CALATK
{

template < class TState >
class CSolverMultiScale : public CSolver< TState >
{
public:
  
  typedef typename TState::TFloat T;
  static const unsigned int VImageDimension = TState::VImageDimension;

  typedef CSolver< TState > Superclass;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef CSolver< TState > SolverType;

  typedef VectorImage< T, VImageDimension > VectorImageType;
  typedef VectorImageUtils< T, VImageDimension > VectorImageUtilsType;
  typedef CImageManagerMultiScale< T, VImageDimension > ImageManagerMultiScaleType;
  typedef typename SolverType::ptrObjectiveFunctionType ptrObjectiveFunctionType;

  CSolverMultiScale();
  virtual ~CSolverMultiScale();

  // TODO: Make it such that we can have different solvers for the different scales
  // for now: One solver with identical settings

  void SetSingleScaleSolverPointer( const SolverType* ptrSolver );
  const SolverType* GetSingleScaleSolverPointer() const;

  // the objective function has an image manager associated with it

  virtual bool SolvePreInitialized();
  virtual bool Solve();

  // auto configuration

  virtual void SetAutoConfiguration( Json::Value &ConfValue );

protected:

  void SetDefaultSingleScaleSolver();
  void DeleteDefaultSingleScaleSolver();

  SolverType* m_ptrSolver;

private:

  bool m_bSetDefaultSingleScaleSolver;

};

#include "CSolverMultiScale.txx"

} // end namespace

#endif

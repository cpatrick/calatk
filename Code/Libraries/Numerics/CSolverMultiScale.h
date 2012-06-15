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

#ifndef C_SOLVER_MULTISCALE_H
#define C_SOLVER_MULTISCALE_H

#include "CSolver.h"
#include "CSolverFactory.h"
#include "CImageManager.h"
#include "VectorImage.h"
#include "VectorImageUtils.h"

namespace CALATK
{

template < class TState >
class CSolverMultiScale : public CSolver< TState >
{
public:
  /** Standard class typedefs. */
  typedef CSolverMultiScale               Self;
  typedef CSolver< TState >               Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  typedef typename TState::FloatType T;
  static const unsigned int ImageDimension = TState::ImageDimension;

  typedef typename Superclass::CEnergyValues CEnergyValues;

  typedef VectorImage< T, ImageDimension >             VectorImageType;
  typedef VectorImageUtils< T, ImageDimension >        VectorImageUtilsType;
  typedef CImageManager< T, ImageDimension >           ImageManagerType;
  typedef typename Superclass::ObjectiveFunctionType   ObjectiveFunctionType;

  CSolverMultiScale();
  virtual ~CSolverMultiScale();

  // TODO: Make it such that we can have different solvers for the different scales
  // for now: One solver with identical settings

  void SetSingleScaleSolverPointer( Superclass* ptrSolver );
  const Superclass* GetSingleScaleSolverPointer() const;

  // the objective function has an image manager associated with it

  virtual bool SolvePreInitialized();
  virtual bool Solve();
  virtual void PreSubIterationSolve();

  SetMacro( SingleScaleSolver, std::string );
  GetMacro( SingleScaleSolver, std::string );

  // Number of times we repeat using the solver
  // before each solver call PreSubIterationSolve is called which allows to specify a pre-condition
  SetMacro( NumberOfSubIterations, unsigned int );
  GetMacro( NumberOfSubIterations, unsigned int );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

protected:
  void SetDefaultSingleScaleSolver();

  typename Superclass::Pointer m_ptrSolver;

private:
  bool m_bSetDefaultSingleScaleSolver;

  std::string m_SingleScaleSolver;
  const std::string DefaultSingleScaleSolver;
  bool m_ExternallySetSingleScaleSolver;

  unsigned int m_NumberOfSubIterations;
  const unsigned int DefaultNumberOfSubIterations;
  bool m_ExternallySetNumberOfSubIterations;

};

#include "CSolverMultiScale.txx"

} // end namespace

#endif

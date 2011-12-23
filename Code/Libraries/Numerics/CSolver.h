/**
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

#ifndef C_SOLVER_H
#define C_SOLVER_H

#include "CObjectiveFunction.h"
#include "CALATKCommon.h"
#include "CProcessBase.h"
#include "JSONParameterUtils.h"

/** 
 * Base class for the registration solvers.
 */

namespace CALATK
{

template <class T, unsigned int VImageDimension, class TState >
class CSolver : public CProcessBase
{
public:

  typedef CObjectiveFunction< T, VImageDimension, TState>* ptrObjectiveFunctionType;

  CSolver();
  virtual ~CSolver();

  virtual void SetObjectiveFunctionPointer( ptrObjectiveFunctionType pObjectiveFunction );
  ptrObjectiveFunctionType GetObjectiveFunctionPointer();

  virtual bool Solve();
  virtual bool SolvePreInitialized() = 0;

  GetMacro(OutputStateInformation,bool);
  SetMacro(OutputStateInformation,bool);
  
  GetMacro(ExternalSolverState,unsigned int);
  SetMacro(ExternalSolverState,unsigned int);

protected:

  ptrObjectiveFunctionType m_pObjectiveFunction;
  bool m_OutputStateInformation;

private:

  // can be set externally, so the solver can create different outputs
  // for example for a multi-scale pyramid
  unsigned int m_ExternalSolverState;

  const bool DefaultOutputStateInformation;
  const unsigned int DefaultExternalSolverState;

  bool m_ExternallySetOutputStateInformation;
  bool m_ExternallySetExternalSolverState;

};

#include "CSolver.txx"

} // end namespace

#endif

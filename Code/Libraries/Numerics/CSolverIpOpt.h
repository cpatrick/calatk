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

#ifndef C_SOLVER_IPOPT_H
#define C_SOLVER_IPOPT_H

#include <string>
#include <math.h>

#include "IpStdCInterface.h"

namespace CALATK
{
/**
  * Interfaces with the Ipopt library.
  * Uses Ipopt's C-interface, because it simplifies the memory management here,
  * ie., we can pass the memory of the state to Ipopt easily.
  *
  * TODO: Maybe convert to nice C++ implementation.
  *
  */
template < class TState >
class CSolverIpOpt
    : public CSolver< TState >
{
public:

  typedef typename TState::TFloat T;
  typedef CSolver< TState > Superclass;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverIpOpt();
  ~CSolverIpOpt();

  bool SolvePreInitialized();

  // specializations
  bool SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements );
  bool SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements );

protected:

  struct SUserData
  {
    CSolverIpOpt< TState > *pSolver;
  };

  Bool AddNumOption( IpoptProblem& nlp, std::string sKey, Number dVal );
  Bool AddIntOption( IpoptProblem& nlp, std::string sKey, Int iVal );
  Bool AddStrOption( IpoptProblem& nlp, std::string sKey, std::string sVal );

  std::string GetStringForStatusCode( int ret );

  T* m_x;

  /* The actual callback functions */
  static Bool _eval_f(Index n, Number* x, Bool new_x, Number* obj_value, UserDataPtr user_data);
  static Bool _eval_grad_f(Index n, Number* x, Bool new_x, Number* grad_f, UserDataPtr user_data);
  static Bool _eval_g(Index n, Number* x, Bool new_x, Index m, Number* g, UserDataPtr user_data);

  static Bool _eval_jac_g(Index n, Number *x, Bool new_x,
                  Index m, Index nele_jac,
                  Index *iRow, Index *jCol, Number *values,
                  UserDataPtr user_data);

  static Bool _eval_h(Index n, Number *x, Bool new_x, Number obj_factor,
              Index m, Number *lambda, Bool new_lambda,
              Index nele_hess, Index *iRow, Index *jCol,
              Number *values, UserDataPtr user_data);

  /* Mapping to class methods */
  Bool eval_f(Index n, T* x, Bool new_x, T* obj_value, UserDataPtr user_data);
  Bool eval_grad_f(Index n, T* x, Bool new_x, T* grad_f, UserDataPtr user_data);
  Bool eval_g(Index n, T* x, Bool new_x, Index m, T* g, UserDataPtr user_data);

  Bool eval_jac_g(Index n, T *x, Bool new_x,
                  Index m, Index nele_jac,
                  Index *iRow, Index *jCol, T *values,
                  UserDataPtr user_data);

  Bool eval_h(Index n, T *x, Bool new_x, T obj_factor,
              Index m, T *lambda, Bool new_lambda,
              Index nele_hess, Index *iRow, Index *jCol,
              T *values, UserDataPtr user_data);


};

#include "CSolverIpOpt.txx"

} // end namespace

#endif // C_SOLVER_IPOPT_H

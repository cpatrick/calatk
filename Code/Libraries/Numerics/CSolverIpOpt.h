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
  typedef typename Superclass::ObjectiveFunctionType ObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverIpOpt();
  ~CSolverIpOpt();

  bool SolvePreInitialized();

  // specializations
  bool SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements );
  bool SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements );

  virtual void SetAutoConfiguration( Json::Value& ConfValueIn, Json::Value& ConfValueOut );

  SetMacro( tol, T );
  GetMacro( tol, T );

  SetMacro( print_level, int );
  GetMacro( print_level, int );

  SetMacro( limited_memory_max_history, int );
  GetMacro( limited_memory_max_history, int );

  SetMacro( hessian_approximation, std::string );
  GetMacro( hessian_approximation, std::string );

  SetMacro( eta_phi, T );
  GetMacro( eta_phi, T );

  SetMacro( watchdog_shortened_iter_trigger, int );
  GetMacro( watchdog_shortened_iter_trigger, int );

  SetMacro( accept_every_trial_step, std::string );
  GetMacro( accept_every_trial_step, std::string );

  SetMacro( output_file, std::string );
  GetMacro( output_file, std::string );

  SetMacro( derivative_test, std::string );
  GetMacro( derivative_test, std::string );

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

  // variables of the solver which can be set externally

  T m_tol;
  const T Defaulttol;
  bool m_ExternallySettol;

  int m_print_level;
  const int Defaultprint_level;
  bool m_ExternallySetprint_level;

  int m_limited_memory_max_history;
  const int Defaultlimited_memory_max_history;
  bool m_ExternallySetlimited_memory_max_history;

  std::string m_hessian_approximation;
  const std::string Defaulthessian_approximation;
  bool m_ExternallySethessian_approximation;

  T m_eta_phi;
  const T Defaulteta_phi;
  bool m_ExternallySeteta_phi;

  int m_watchdog_shortened_iter_trigger;
  const int Defaultwatchdog_shortened_iter_trigger;
  bool m_ExternallySetwatchdog_shortened_iter_trigger;

  std::string m_accept_every_trial_step;
  const std::string Defaultaccept_every_trial_step;
  bool m_ExternallySetaccept_every_trial_step;

  std::string m_output_file;
  const std::string Defaultoutput_file;
  bool m_ExternallySetoutput_file;

  std::string m_derivative_test;
  std::string Defaultderivative_test;
  bool m_ExternallySetderivative_test;

};

#include "CSolverIpOpt.txx"

} // end namespace

#endif // C_SOLVER_IPOPT_H

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

#ifndef C_SOLVER_NLOPT_H
#define C_SOLVER_NLOPT_H

#include <string>
#include <math.h>
#include <nlopt.h>

namespace CALATK
{

/**
  * Implements an LBFGS solver
  *
  */
template < class TState >
class CSolverNLOpt
    : public CSolver< TState >
{
public:
  typedef typename TState::TFloat T;
  typedef CSolver< TState > Superclass;
  typedef typename Superclass::ObjectiveFunctionType ObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverNLOpt();
  ~CSolverNLOpt();

  bool SolvePreInitialized();

  // specializations
  bool SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements );
  bool SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements );

  virtual void SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned );

  SetMacro( initial_step1, T );
  GetMacro( initial_step1, T );

  SetMacro( maxeval, unsigned int );
  GetMacro( maxeval, unsigned int );

  SetMacro( xtol_rel, T );
  GetMacro( xtol_rel, T );

  SetMacro( ftol_rel, T );
  GetMacro( ftol_rel, T );

  SetMacro( vector_storage, unsigned int );
  GetMacro( vector_storage, unsigned int );

protected:

  struct SUserData
  {
    CSolverNLOpt< TState > *pSolver;
  };

  std::string GetStringForStatusCode( int ret );

  T* m_x;

    // specialization for double
   static double _evaluate(
       unsigned n,
       const double *x,
       double *g,
       void *my_func_data
       );

    // specialization for float
    static float _evaluate(
        unsigned n,
        const float *x,
        float *g,
        void *my_func_data
        );

     // generic evaluate and progress methods

    T evaluate(
        unsigned n,
        const T *x,
        T *g,
        void *my_func_data
        );

    T m_initial_step1;
    const T Defaultinitial_step1;
    bool m_ExternallySetinitial_step1;

    unsigned int m_maxeval;
    const unsigned int Defaultmaxeval;
    bool m_ExternallySetmaxeval;

    T m_xtol_rel;
    const T Defaultxtol_rel;
    bool m_ExternallySetxtol_rel;

    T m_ftol_rel;
    const T Defaultftol_rel;
    bool m_ExternallySetftol_rel;

    unsigned int m_vector_storage;
    const unsigned int Defaultvector_storage;
    bool m_ExternallySetvector_storage;

};

#include "CSolverNLOpt.txx"

} // end namespace

#endif // C_SOLVER_NLOPT_H

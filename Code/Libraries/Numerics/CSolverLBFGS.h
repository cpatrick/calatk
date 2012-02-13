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

#ifndef C_SOLVER_LBFGS_H
#define C_SOLVER_LBFGS_H

#include "lbfgs.h"
#include <string>

namespace CALATK
{
/**
  * Implements an LBFGS solver
  *
  */
template < class TState >
class CSolverLBFGS
    : public CSolver< TState >
{
public:

  typedef typename TState::TFloat T;
  typedef CSolver< TState > Superclass;
  typedef typename Superclass::ptrObjectiveFunctionType ptrObjectiveFunctionType;
  typedef typename Superclass::CEnergyValues CEnergyValues;

  CSolverLBFGS();
  ~CSolverLBFGS();

  bool SolvePreInitialized();

  // specializations
  bool SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements );
  bool SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements );

protected:

  std::string GetStringForStatusCode( int ret );

  T* m_x;

    // specialization for double
   static double _evaluate(
        void *instance,
        const double *x,
        double *g,
        const int n,
        const double step
        );

    static int _progress(
        void *instance,
        const double *x,
        const double *g,
        const double fx,
        const double xnorm,
        const double gnorm,
        const double step,
        int n,
        int k,
        int ls
        );

    // specialization for float
    static float _evaluate(
         void *instance,
         const float *x,
         float *g,
         const int n,
         const float step
         );

     static int _progress(
         void *instance,
         const float *x,
         const float *g,
         const float fx,
         const float xnorm,
         const float gnorm,
         const float step,
         int n,
         int k,
         int ls
         );

     // generic evaluate and progress methods

    T evaluate(
        const T *x,
        T *g,
        const int n,
        const T step
        );

    int progress(
        const T *x,
        const T *g,
        const T fx,
        const T xnorm,
        const T gnorm,
        const T step,
        int n,
        int k,
        int ls
        );

};

#include "CSolverLBFGS.txx"

} // end namespace

#endif // C_SOLVER_LBFGS_H

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

#ifndef C_SOLVER_IPOPT_TXX
#define C_SOLVER_IPOPT_TXX

template < class TState >
CSolverIpOpt< TState >::CSolverIpOpt()
  : m_x( NULL )
{
}

template < class TState >
CSolverIpOpt< TState >::~CSolverIpOpt()
{
  if ( m_x != NULL )
  {
    delete [] m_x;
    m_x = NULL;
  }
}

template < class TState >
bool CSolverIpOpt< TState >::SolvePreInitialized()
{
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();

  // initialize

  T *ptrCurrentState = pObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  long int liNumberOfStateVectorElements = pObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

  // Probably no need to copy here. TODO, make this more efficient
  T *ptrMX = new T[ liNumberOfStateVectorElements ];

  if ( ptrMX == NULL )
  {
    throw std::runtime_error( "State has no memory allocated" );
  }

  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    ptrMX[ iI ] = ptrCurrentState[ iI ];
  }

  return SolvePreInitialized( ptrMX, liNumberOfStateVectorElements );

}

template < class TState >
Bool CSolverIpOpt< TState >::AddNumOption( IpoptProblem& nlp, std::string sKey, Number dVal )
{
  // ugly const cast; TODO can we do better here
  return AddIpoptNumOption(nlp, const_cast<char*>( sKey.c_str() ), dVal );
}

template < class TState >
Bool CSolverIpOpt< TState >::AddIntOption( IpoptProblem& nlp, std::string sKey, Int iVal )
{
  // ugly const cast; TODO can we do better here
  return AddIpoptIntOption(nlp, const_cast<char*>( sKey.c_str() ), iVal );
}

template < class TState >
Bool CSolverIpOpt< TState >::AddStrOption( IpoptProblem& nlp, std::string sKey, std::string sVal )
{
  // ugly const cast; TODO can we do better here
  return AddIpoptStrOption(nlp, const_cast<char*>( sKey.c_str() ), const_cast<char*>( sVal.c_str() ) );
}

template < class TState >
bool CSolverIpOpt< TState >::SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements )
{
  m_x = ptr;

  Index n=-1;                          /* number of variables */
  Index m=-1;                          /* number of constraints */
  Number* x_L = NULL;                  /* lower bounds on x */
  Number* x_U = NULL;                  /* upper bounds on x */
  Number* g_L = NULL;                  /* lower bounds on g */
  Number* g_U = NULL;                  /* upper bounds on g */

  IpoptProblem nlp = NULL;             /* IpoptProblem */
  enum ApplicationReturnStatus status; /* Solve return code */

  Number* mult_g = NULL;               /* constraint multipliers at the solution */
  Number* mult_x_L = NULL;             /* lower bound multipliers at the solution */
  Number* mult_x_U = NULL;             /* upper bound multipliers at the solution */
  Number obj;                          /* objective value */
  Index i;                             /* generic counter */

  /* Number of nonzeros in the Jacobian of the constraints */
  Index nele_jac = 0;

  /* Number of nonzeros in the Hessian of the Lagrangian (lower or upper triangual part only) */
  Index nele_hess = 0;

  /* indexing style for matrices */
  Index index_style = 0; /* C-style; start counting of rows and column indices at 0 */

  /* set the number of variables and allocate space for th bounds*/
  n=liNumberOfStateVectorElements;

  x_L = (Number*)malloc(sizeof(Number)*n);
  x_U = (Number*)malloc(sizeof(Number)*n);

  /* set the values for the variable bounds */
  for (i=0; i<n; ++i)
  {
    x_L[i] = -10000;;
    x_U[i] = 10000;
  }


  /* set the number of constraints and allocate space for the bounds */
  m=0;

  /* create the IpoptProblem */
  nlp = CreateIpoptProblem(n, x_L, x_U, m, g_L, g_U, nele_jac, nele_hess,
                            index_style, &_eval_f, &_eval_g, &_eval_grad_f,
                            &_eval_jac_g, &_eval_h);

  /* We can free the memory now - the values for the bounds have been
       copied internally in CreateIpoptProblem */
  free(x_L);
  free(x_U);

  /* Set some options.  Note the following ones are only examples,
     they might not be suitable for your problem. */
  AddNumOption(nlp, "tol", 1e-7);
  AddIntOption( nlp, "print_level", 5 );
  AddIntOption( nlp, "limited_memory_max_history", 3 );
  AddStrOption(nlp, "hessian_approximation", "limited-memory");
  AddNumOption(nlp, "eta_phi", 1e-20);
  AddIntOption(nlp, "watchdog shortened iter trigger", 2 );
  //AddStrOption(nlp, "accept_every_trial_step", "yes");

  AddStrOption(nlp, "output_file", "ipopt.out");
  // AddStrOption(nlp, "derivative_test", "first-order" ); // derivative check if we want it, good for debugging the gradient

  /* m_x holds the state, no need to allocate again */

  /* Initialize the user data */
  SUserData user_data;
  user_data.pSolver = this;

  /* Set the callback method for intermediate user-control.  This is
   * not required, just gives you some intermediate control in case
   * you need it. */
  /* SetIntermediateCallback(nlp, intermediate_cb); */

  /* allocate space to store the bound multipliers at the solution */
  mult_x_L = (Number*)malloc(sizeof(Number)*n);
  mult_x_U = (Number*)malloc(sizeof(Number)*n);

   /* solve the problem */
   status = IpoptSolve(nlp, m_x, NULL, &obj, mult_g, mult_x_L, mult_x_U, &user_data);

  if (status == Solve_Succeeded)
  {
    std::cout << "IpOpt: solution succeeded. Objective value = " << obj <<  std::endl;
  }
  else
  {
    std::cout << "IpOpt: solution DID NOT succeed. Error code = " << status << std::endl;
    std::cout << "IpOpt: Error = " << GetStringForStatusCode( status ) << std::endl;
  }

  /* free allocated memory */
  FreeIpoptProblem(nlp);

  free( mult_x_L );
  free( mult_x_U );

  return true;
}

template < class TState >
std::string CSolverIpOpt< TState >::GetStringForStatusCode( int ret )
{
  std::string retString;

  switch ( ret )
  {
  default:
    retString = "Unknown error code";
  }

  return retString;

}


template < class TState >
bool CSolverIpOpt< TState >::SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements )
{
  m_x = ptr;
  throw std::runtime_error( "Not implemented for type float. Try double.");
  return false;
}

template < class TState >
Bool CSolverIpOpt< TState >::_eval_f(Index n, Number* x, Bool new_x, Number* obj_value, UserDataPtr user_data)
{
  typedef CSolverIpOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)user_data;

  return pUserData->pSolver->eval_f( n, x, new_x, obj_value, user_data );
}

template < class TState >
Bool CSolverIpOpt< TState >::_eval_grad_f(Index n, Number* x, Bool new_x, Number* grad_f, UserDataPtr user_data)
{
  typedef CSolverIpOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)user_data;

  return pUserData->pSolver->eval_grad_f( n, x, new_x, grad_f, user_data );
}

template < class TState >
Bool CSolverIpOpt< TState >::_eval_g(Index n, Number* x, Bool new_x, Index m, Number* g, UserDataPtr user_data)
{
  typedef CSolverIpOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)user_data;

  return pUserData->pSolver->eval_g( n, x, new_x, m, g, user_data );

}

template < class TState >
Bool CSolverIpOpt< TState >::_eval_jac_g(Index n, Number *x, Bool new_x,
                Index m, Index nele_jac,
                Index *iRow, Index *jCol, Number *values,
                UserDataPtr user_data)
{
  typedef CSolverIpOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)user_data;

  return pUserData->pSolver->eval_jac_g( n, x, new_x, m, nele_jac, iRow, jCol, values, user_data );

}

template < class TState >
Bool CSolverIpOpt< TState >::_eval_h(Index n, Number *x, Bool new_x, Number obj_factor,
            Index m, Number *lambda, Bool new_lambda,
            Index nele_hess, Index *iRow, Index *jCol,
            Number *values, UserDataPtr user_data)
{
  typedef CSolverIpOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)user_data;

  return pUserData->pSolver->eval_h( n, x, new_x, obj_factor, m, lambda, new_lambda, nele_hess, iRow, jCol, values, user_data );

}

template < class TState >
Bool CSolverIpOpt< TState >::eval_f(Index n, T* x, Bool new_x, T* obj_value, UserDataPtr user_data)
{
  //assert(n == 4);

  // get pointer to the objective function
  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();
  long int liNumberOfStateVectorElements = pObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

  // initialize
  T *ptrCurrentState = pObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    ptrCurrentState[ iI ] = x[ iI ];
  }

  // determine current energy
  CEnergyValues ComputedEnergy = pObj->GetCurrentEnergy();

  //std::cout << "E = " << ComputedEnergy.dEnergy << std::endl;

  *obj_value = ComputedEnergy.dEnergy;

  return TRUE;
}

template < class TState >
Bool CSolverIpOpt< TState >::eval_grad_f(Index n, T* x, Bool new_x, T* grad_f, UserDataPtr user_data)
{
  //assert(n == 4);

  // evaluate gradient

  ptrObjectiveFunctionType pObj = this->GetObjectiveFunctionPointer();
  long int liNumberOfStateVectorElements = pObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

  // initialize
  T *ptrCurrentState = pObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    ptrCurrentState[ iI ] = x[ iI ];
  }

  // compute gradient
  pObj->ComputeGradient();

  // get current gradient
  TState *pCurrentGradient = pObj->GetGradientPointer();
  T* ptrCurrentGradient = pCurrentGradient->GetPointerToStateVectorElementsToEstimate();

  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    grad_f[ iI ] = ptrCurrentGradient[ iI ];
  }

  return TRUE;

}

template < class TState >
Bool CSolverIpOpt< TState >::eval_g(Index n, T* x, Bool new_x, Index m, T* g, UserDataPtr user_data)
{
  // these are the constraints, we don't have any

  //assert(n == 4);
  //assert(m == 2);

  return TRUE;

}

template < class TState >
Bool CSolverIpOpt< TState >::eval_jac_g(Index n, T *x, Bool new_x,
                Index m, Index nele_jac,
                Index *iRow, Index *jCol, T *values,
                UserDataPtr user_data)
{
  /* return the structure of the jacobian */
  // since we don't have any constraints, just return TRUE

  return TRUE;
}

template < class TState >
Bool CSolverIpOpt< TState >::eval_h(Index n, T *x, Bool new_x, T obj_factor,
            Index m, T *lambda, Bool new_lambda,
            Index nele_hess, Index *iRow, Index *jCol,
            T *values, UserDataPtr user_data)
{
  // This is to specify the Hessian. Not specified here, because we will use a low memory quasi-Newton method (L-BFGS) to estimate the Hessian

  return TRUE;
}

#endif


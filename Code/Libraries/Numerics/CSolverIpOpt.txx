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
  : m_x( NULL ),
    Defaulttol( 1e-7 ),
    m_ExternallySettol( false ),
    Defaultprint_level( 5 ),
    m_ExternallySetprint_level( false ),
    Defaultlimited_memory_max_history( 3 ),
    m_ExternallySetlimited_memory_max_history( false ),
    Defaulthessian_approximation( "limited_memory" ),
    m_ExternallySethessian_approximation( false ),
    Defaulteta_phi( 1e-20 ),
    m_ExternallySeteta_phi( false ),
    Defaultwatchdog_shortened_iter_trigger( 2 ),
    m_ExternallySetwatchdog_shortened_iter_trigger( false ),
    Defaultaccept_every_trial_step( "no" ),
    m_ExternallySetaccept_every_trial_step( false ),
    Defaultoutput_file( "ipopt.out" ),
    m_ExternallySetoutput_file( false ),
    Defaultderivative_test( "first_order" ),
    m_ExternallySetderivative_test( false )

{
  m_tol = Defaulttol;
  m_print_level = Defaultprint_level;
  m_limited_memory_max_history = Defaultlimited_memory_max_history;
  m_hessian_approximation = Defaulthessian_approximation;
  m_eta_phi = Defaulteta_phi;
  m_watchdog_shortened_iter_trigger = Defaultwatchdog_shortened_iter_trigger;
  m_accept_every_trial_step = Defaultaccept_every_trial_step;
  m_output_file = Defaultoutput_file;
  m_derivative_test = Defaultderivative_test;

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
void CSolverIpOpt< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "IpOpt", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "IpOpt", Json::nullValue );

  SetJSONHelpForRootKey( IpOpt, "Setting for the IpOpt optimizer" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, tol );
  SetJSONFromKeyInt( currentConfigurationIn, currentConfigurationOut, print_level );
  SetJSONFromKeyInt( currentConfigurationIn, currentConfigurationOut, limited_memory_max_history );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, hessian_approximation );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, eta_phi );
  SetJSONFromKeyInt( currentConfigurationIn, currentConfigurationOut, watchdog_shortened_iter_trigger );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, accept_every_trial_step );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, output_file );
  SetJSONFromKeyString( currentConfigurationIn, currentConfigurationOut, derivative_test );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, tol,
                     "tolerance for optimizer" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, print_level,
                     "how much information should be printed (higher->more, max 10)" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, limited_memory_max_history,
                     "how many gradients are stored for a quasi Newton method to approximate the Hessian" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, hessian_approximation,
                     "how the Hessian should be approximated" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, eta_phi,
                     "line search sufficient decrease" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, watchdog_shortened_iter_trigger,
                     "how many steps for watchdog" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, accept_every_trial_step,
                     "disable proper line search and accept every step (very aggressive)" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, output_file,
                     "file to write the optimization information to" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, derivative_test,
                     "how to test the derivative numerically" );
}

template < class TState >
bool CSolverIpOpt< TState >::SolvePreInitialized()
{
  ObjectiveFunctionType* ptrObj = this->GetObjectiveFunction();

  // initialize

  T *ptrCurrentState = ptrObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  long int liNumberOfStateVectorElements = ptrObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

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
  AddNumOption(nlp, "tol", m_tol );
  AddIntOption( nlp, "print_level", m_print_level );
  AddIntOption( nlp, "limited_memory_max_history", m_limited_memory_max_history );
  AddStrOption(nlp, "hessian_approximation", m_hessian_approximation );
  AddNumOption(nlp, "eta_phi", m_eta_phi );
  AddIntOption(nlp, "watchdog shortened iter trigger", m_watchdog_shortened_iter_trigger );
  AddStrOption(nlp, "accept_every_trial_step", m_accept_every_trial_step );

  AddStrOption(nlp, "output_file", m_output_file );
  AddStrOption(nlp, "derivative_test", m_derivative_test ); // derivative check if we want it, good for debugging the gradient

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
  ObjectiveFunctionType* ptrObj = this->GetObjectiveFunction();
  long int liNumberOfStateVectorElements = ptrObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

  // initialize
  T *ptrCurrentState = ptrObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    ptrCurrentState[ iI ] = x[ iI ];
  }

  // determine current energy
  CEnergyValues ComputedEnergy = ptrObj->GetCurrentEnergy();

  //std::cout << "E = " << ComputedEnergy.dEnergy << std::endl;

  *obj_value = ComputedEnergy.dEnergy;

  return TRUE;
}

template < class TState >
Bool CSolverIpOpt< TState >::eval_grad_f(Index n, T* x, Bool new_x, T* grad_f, UserDataPtr user_data)
{
  //assert(n == 4);

  // evaluate gradient

  ObjectiveFunctionType* ptrObj = this->GetObjectiveFunction();
  long int liNumberOfStateVectorElements = ptrObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

  // initialize
  T *ptrCurrentState = ptrObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  // copy it
  for ( long int iI=0; iI<liNumberOfStateVectorElements; ++iI )
  {
    ptrCurrentState[ iI ] = x[ iI ];
  }

  // compute gradient
  ptrObj->ComputeGradient();

  // get current gradient
  TState *pCurrentGradient = ptrObj->GetGradientPointer();
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


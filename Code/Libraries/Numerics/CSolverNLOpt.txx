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

#ifndef C_SOLVER_NLOPT_TXX
#define C_SOLVER_NLOPT_TXX

template < class TState >
CSolverNLOpt< TState >::CSolverNLOpt()
  : m_x( NULL ),
    Defaultinitial_step1( 1e-5 ),
    m_ExternallySetinitial_step1( false ),
    Defaultmaxeval( 1000 ),
    m_ExternallySetmaxeval( false ),
    Defaultxtol_rel( 1e-10 ),
    m_ExternallySetxtol_rel( false ),
    Defaultftol_rel( 1e-10 ),
    m_ExternallySetftol_rel( false ),
    Defaultvector_storage( 5 ),
    m_ExternallySetvector_storage( false )
{
  m_initial_step1 = Defaultinitial_step1;
  m_maxeval = Defaultmaxeval;
  m_xtol_rel = Defaultxtol_rel;
  m_ftol_rel = Defaultftol_rel;
  m_vector_storage = Defaultvector_storage;
}

template < class TState >
CSolverNLOpt< TState >::~CSolverNLOpt()
{
  if ( m_x != NULL )
  {
    delete [] m_x;
    m_x = NULL;
  }
}

template < class TState >
void CSolverNLOpt< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );
  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "NLOpt", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "NLOpt", Json::nullValue );

  SetJSONHelpForRootKey( IpOpt, "Setting for the NLOpt optimizer" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, initial_step1 );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, maxeval );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, xtol_rel );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, ftol_rel );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, vector_storage );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, initial_step1,
                     "initial step size for the line search" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, maxeval,
                     "maximum number of evaluations" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, xtol_rel,
                     "termnination tolerance for the variable" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, ftol_rel,
                     "termination tolerance for the function" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, vector_storage,
                     "number of gradients to store for the L-BFGS Hessian approximation " );
}

template < class TState >
bool CSolverNLOpt< TState >::SolvePreInitialized()
{
  ObjectiveFunctionType* ptrObj = this->GetObjectiveFunction();

  // initialize

  T *ptrCurrentState = ptrObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();
  long int liNumberOfStateVectorElements = ptrObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

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
bool CSolverNLOpt< TState >::SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements )
{
  m_x = ptr;

  nlopt_opt opt;
  opt = nlopt_create( NLOPT_LD_LBFGS, liNumberOfStateVectorElements ); /* algorithm and dimensionality */

  SUserData userData;
  userData.pSolver = this;

  nlopt_set_initial_step1( opt, m_initial_step1 );
  nlopt_set_maxeval( opt, m_maxeval );

  nlopt_set_min_objective(opt, _evaluate, (void*)&userData );
  nlopt_set_xtol_rel(opt, m_xtol_rel );
  nlopt_set_ftol_rel(opt, m_ftol_rel );

  nlopt_set_vector_storage( opt, m_vector_storage );

  T fx; /* the minimum objective value, upon return */

  /*
      Start the L-BFGS optimization; this will invoke the callback functions
      evaluate() and progress() when necessary.
   */

  int ret = nlopt_optimize(opt, m_x, &fx);

  if (  ret< 0)
  {
      std::cout << "nlopt failed!" << std::endl;
      std::cout << "nlopt status = " << GetStringForStatusCode( ret ) << std::endl;
  }
  else
  {
      std::cout << "Found minimum with value f = " << fx << std::endl;
  }

  nlopt_destroy(opt);

  return ( ret >=0 );

}

template < class TState >
std::string CSolverNLOpt< TState >::GetStringForStatusCode( int ret )
{
  std::string retString;

  switch ( ret )
  {
  case NLOPT_SUCCESS:
    retString = "NLOPT_SUCCESS";
    break;
  case NLOPT_STOPVAL_REACHED:
    retString = "NLOPT_STOPVAL_REACHED";
    break;
  case NLOPT_FTOL_REACHED:
    retString = "NLOPT_FTOL_REACHED";
    break;
  case NLOPT_XTOL_REACHED:
    retString = "NLOPT_XTOL_REACHED";
    break;
  case NLOPT_MAXEVAL_REACHED:
    retString = "NLOPT_MAXEVAL_REACHED";
    break;
  case NLOPT_MAXTIME_REACHED:
    retString = "NLOPT_MAXTIME_REACED";
    break;
  case NLOPT_FAILURE:
    retString = "NLOPT_FAILURE";
    break;
  case NLOPT_INVALID_ARGS:
    retString = "NLOPT_INVALID_ARGS";
    break;
  case NLOPT_OUT_OF_MEMORY:
    retString = "NLOPT_OUT_OF_MEMORY";
    break;
  case NLOPT_ROUNDOFF_LIMITED:
    retString = "NLOPT_ROUNDOFF_LIMIT";
    break;
  case NLOPT_FORCED_STOP:
    retString = "NLOPT_FORCED_STOP";
  break;
  default:
    retString = "Unknown error code";
  }

  return retString;

}


template < class TState >
bool CSolverNLOpt< TState >::SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements )
{
  m_x = ptr;
  throw std::runtime_error( "Not implemented for type float. Try double.");
  return false;
}

template < class TState >
typename CSolverNLOpt< TState >::T
CSolverNLOpt< TState >::evaluate( unsigned n,
                                  const T *x,
                                   T *g,
                                   void *my_func_data
                                 )
{

  // evaluate gradient and function value

  ObjectiveFunctionType* ptrObj = this->GetObjectiveFunction();
  // initialize
  T *ptrCurrentState = ptrObj->GetStatePointer()->GetPointerToStateVectorElementsToEstimate();

  long int liNumberOfStateVectorElements = ptrObj->GetStatePointer()->GetNumberOfStateVectorElementsToEstimate();

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
    g[ iI ] = ptrCurrentGradient[ iI ];
  }

  // determine current energy
  CEnergyValues ComputedEnergy = ptrObj->GetCurrentEnergy();

  std::cout << "E = " << ComputedEnergy.dEnergy << std::endl;

  return ComputedEnergy.dEnergy;
}

template < class TState >
double CSolverNLOpt< TState >::_evaluate( unsigned n,
                                          const double *x,
                                          double *g,
                                          void *my_func_data
                                          )
{
  typedef CSolverNLOpt< TState > objective_function;
  SUserData* pUserData = (SUserData*)my_func_data;

  return pUserData->pSolver->evaluate(n,x, g, my_func_data);
}

template < class TState >
float CSolverNLOpt< TState >::_evaluate( unsigned n,
                                         const float *x,
                                         float *g,
                                         void *my_func_data
                                        )
{
  throw std::runtime_error( "NLOpt not implemented for float type. Use double.");
}



#endif


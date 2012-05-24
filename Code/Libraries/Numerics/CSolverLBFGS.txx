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

#ifndef C_SOLVER_LBFGS_TXX
#define C_SOLVER_LBFGS_TXX

template < class TState >
CSolverLBFGS< TState >::CSolverLBFGS()
  : m_x( NULL ),
    Defaultepsilon( 1e-10 ),
    m_ExternallySetepsilon( false ),
    Defaultmin_step( 1e-10 ),
    m_ExternallySetmin_step( false ),
    Defaultmax_linesearch( 20 ),
    m_ExternallySetmax_linesearch( false ),
    Defaultm( 3 ),
    m_ExternallySetm( false ),
    Defaultftol( 0 ),
    m_ExternallySetftol( false ),
    Defaultgtol( 0 ),
    m_ExternallySetgtol( false )
{
  m_epsilon = Defaultepsilon;
  m_min_step = Defaultmin_step;
  m_max_linesearch = Defaultmax_linesearch;
  m_m = Defaultm;
  m_ftol = Defaultftol;
  m_gtol = Defaultgtol;
}

template < class TState >
CSolverLBFGS< TState >::~CSolverLBFGS()
{
  if ( m_x != NULL )
  {
    delete [] m_x;
    m_x = NULL;
  }
}

template < class TState >
void CSolverLBFGS< TState >::SetAutoConfiguration( CJSONConfiguration * combined, CJSONConfiguration * cleaned )
{
  Superclass::SetAutoConfiguration( combined, cleaned );

  Json::Value& currentConfigurationIn = this->m_CombinedJSONConfig->GetFromKey( "LBFGS", Json::nullValue );
  Json::Value& currentConfigurationOut = this->m_CleanedJSONConfig->GetFromKey( "LBFGS", Json::nullValue );

  SetJSONHelpForRootKey( IpOpt, "Setting for the libLBFGS optimizer" );

  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, epsilon );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, min_step );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, max_linesearch );
  SetJSONFromKeyUInt( currentConfigurationIn, currentConfigurationOut, m );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, ftol );
  SetJSONFromKeyDouble( currentConfigurationIn, currentConfigurationOut, gtol );

  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, epsilon,
                     "epsilon" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, min_step,
                     "minimal step for linesearch" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, max_linesearch,
                     "maximal linesearch tries" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, m,
                     "number of gradients to store for the Hessian approximation" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, ftol,
                     "f-tolerance" );
  SetJSONHelpForKey( currentConfigurationIn, currentConfigurationOut, gtol,
                     "g-tolerance" );
}

template < class TState >
bool CSolverLBFGS< TState >::SolvePreInitialized()
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
bool CSolverLBFGS< TState >::SolvePreInitialized( double* ptr, long int liNumberOfStateVectorElements )
{

  T fx;
  m_x = ptr;

  lbfgs_parameter_t param;

  /* Initialize the parameters for the L-BFGS optimization. */
  lbfgs_parameter_init( &param );
  param.linesearch = LBFGS_LINESEARCH_BACKTRACKING_ARMIJO;
  /*param.linesearch = LBFGS_LINESEARCH_BACKTRACKING;*/
  param.epsilon = m_epsilon;
  param.min_step = m_min_step;
  param.max_linesearch = m_max_linesearch;
  param.m = m_m;
  param.ftol = m_ftol;
  param.gtol = m_gtol;

  /*
      Start the L-BFGS optimization; this will invoke the callback functions
      evaluate() and progress() when necessary.
   */
  int ret = lbfgs( liNumberOfStateVectorElements, m_x, &fx, _evaluate, _progress, this, &param );

  /* Report the result. */
  std::cout << "L-BFGS optimization terminated with status code = " << ret << std::endl;

  std::cout << "Status code = " << GetStringForStatusCode( ret ) << std::endl;

  std::cout << "L-BFGS minimal function value = " << fx << std::endl;

  return ( ret == LBFGS_SUCCESS );

}

template < class TState >
std::string CSolverLBFGS< TState >::GetStringForStatusCode( int ret )
{
  std::string retString;

  switch ( ret )
  {
    case LBFGS_SUCCESS:
      retString = "LBFGS_SUCCESS";
    case LBFGS_STOP:
      retString = "LBFGS_STOP";
      break;
    case LBFGS_ALREADY_MINIMIZED:
      retString = "LBFGS_ALREADY_MINIMIZED";
      break;
    case LBFGSERR_UNKNOWNERROR:
      retString = "LBFGS_UNKNOWNERROR";
      break;
    case LBFGSERR_LOGICERROR:
      retString = "LBFGSERR_LOGICERROR";
      break;
    case LBFGSERR_OUTOFMEMORY:
      retString = "LBFGSERR_OUTOFMEMORY";
      break;
    case LBFGSERR_CANCELED:
      retString = "LBFGSERR_CANCELED";
      break;
    case LBFGSERR_INVALID_N:
      retString = "LBFGS_INVALID_N";
      break;
    case LBFGSERR_INVALID_N_SSE:
      retString = "LBFGSERR_INVALID_N_SSE";
      break;
    case LBFGSERR_INVALID_X_SSE:
      retString = "LBFGSERR_INVALID_X_SSE";
      break;
    case LBFGSERR_INVALID_EPSILON:
      retString = "LBFGSERR_INVALID_EPSILON";
      break;
    case LBFGSERR_INVALID_TESTPERIOD:
      retString = "LBFGSERR_INVALID_TESTPERIOD";
      break;
    case LBFGSERR_INVALID_DELTA:
      retString = "LBFGSERR_INVALID_DELTA";
      break;
    case LBFGSERR_INVALID_LINESEARCH:
      retString = "LBFGSERR_INVALID_LINESEARCH";
      break;
    case LBFGSERR_INVALID_MINSTEP:
      retString = "LBFGSERR_INVALID_MINSTEP";
      break;
    case LBFGSERR_INVALID_MAXSTEP:
      retString = "LBFGSERR_INVALID_MAXSTEP";
      break;
    case LBFGSERR_INVALID_FTOL:
      retString = "LBFGSERR_INVALID_FTOL";
      break;
    case LBFGSERR_INVALID_WOLFE:
      retString = "LBFGSERR_INVALID_WOLFE";
      break;
    case LBFGSERR_INVALID_GTOL:
      retString = "LBFGSERR_INVALID_GTOL";
      break;
    case LBFGSERR_INVALID_XTOL:
      retString = "LBFGSERR_INVALID_XTOL";
      break;
    case LBFGSERR_INVALID_MAXLINESEARCH:
      retString = "LBFGSERR_INVALID_MAXLINESEARCH";
      break;
    case LBFGSERR_INVALID_ORTHANTWISE:
      retString = "LBFGSERR_INVALID_ORTHANTWISE";
      break;
    case LBFGSERR_INVALID_ORTHANTWISE_START:
      retString = "LBFGSERR_INVALID_ORTHANTWISE_START";
      break;
    case LBFGSERR_INVALID_ORTHANTWISE_END:
      retString = "LBFGSERR_INVALID_ORTHANTWISE_END";
      break;
    case LBFGSERR_OUTOFINTERVAL:
      retString = "LBFGSERR_OUTOFINTERVAL";
      break;
    case LBFGSERR_INCORRECT_TMINMAX:
      retString = "LBFGSERR_INCORRECT_TMINMAX";
      break;
    case LBFGSERR_ROUNDING_ERROR:
      retString = "LBFGSERR_ROUNDING_ERROR";
      break;
    case LBFGSERR_MINIMUMSTEP:
      retString = "LBFGSERR_MINIMUMSTEP";
      break;
    case LBFGSERR_MAXIMUMSTEP:
      retString = "LBFGSERR_MAXIMUMSTEP";
      break;
    case LBFGSERR_MAXIMUMLINESEARCH:
      retString = "LBFSGERR_MAXIMUMLINESEARCH";
      break;
    case LBFGSERR_MAXIMUMITERATION:
      retString = "LBFGSERR_MAXIMUMITERATION";
      break;
    case LBFGSERR_WIDTHTOOSMALL:
      retString = "LBFGSERR_WIDTHTOOSMALL";
      break;
    case LBFGSERR_INVALIDPARAMETERS:
      retString = "LBFGSERR_INVALIDPARAMETERS";
      break;
    case LBFGSERR_INCREASEGRADIENT:
      retString = "LBFGSERR_INCREASEGRADIENT";
      break;
    default:
      retString = "Unknown error code";
  }

  return retString;

}


template < class TState >
bool CSolverLBFGS< TState >::SolvePreInitialized( float* ptr, long int liNumberOfStateVectorElements )
{
  m_x = ptr;
  throw std::runtime_error( "Not implemented for type float. Try double.");
  return false;
}

template < class TState >
typename CSolverLBFGS< TState >::T
CSolverLBFGS< TState >::evaluate(  const T *x,
                                   T *g,
                                   const int n,
                                   const T step
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

  return ComputedEnergy.dEnergy;
}

template < class TState >
int CSolverLBFGS< TState >::progress( const T *x,
                                      const T *g,
                                      const T fx,
                                      const T xnorm,
                                      const T gnorm,
                                      const T step,
                                      int n,
                                      int k,
                                      int ls
                                      )
{
  std::cout << "Iter:" << k << " ";
  std::cout << "E = " << fx << " ";
  std::cout << "  xnorm = " << xnorm << ", gnorm = " << gnorm << ", step = " <<  step << std::endl;
  return 0;
}

template < class TState >
double CSolverLBFGS< TState >::_evaluate( void *instance,
                                          const double *x,
                                          double *g,
                                          const int n,
                                          const double step
                                          )
{
  typedef CSolverLBFGS< TState > objective_function;
  return reinterpret_cast<objective_function*>(instance)->evaluate(x, g, n, step);
}

template < class TState >
float CSolverLBFGS< TState >::_evaluate( void *instance,
                                        const float *x,
                                        float *g,
                                        const int n,
                                        const float step
                                        )
{
  throw std::runtime_error( "CLBFGS not implemented for float type. Use double.");
}

template < class TState >
int CSolverLBFGS< TState >::_progress( void *instance,
                                      const double *x,
                                      const double *g,
                                      const double fx,
                                      const double xnorm,
                                      const double gnorm,
                                      const double step,
                                      int n,
                                      int k,
                                      int ls
                                      )
{
  typedef CSolverLBFGS< TState > objective_function;
  return reinterpret_cast<objective_function*>(instance)->progress(x, g, fx, xnorm, gnorm, step, n, k, ls);
}

template < class TState >
int CSolverLBFGS< TState >::_progress( void *instance,
                                      const float *x,
                                      const float *g,
                                      const float fx,
                                      const float xnorm,
                                      const float gnorm,
                                      const float step,
                                      int n,
                                      int k,
                                      int ls
                                      )
{
  throw std::runtime_error( "CLBFGS not implemented for float type. Use double.");
}


#endif


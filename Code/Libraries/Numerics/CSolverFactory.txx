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

#ifndef C_SOLVER_FACTORY_TXX
#define C_SOLVER_FACTORY_TXX

template < class TState >
CSolverFactory< TState >::CSolverFactory()
{
}

template < class TState >
CSolverFactory< TState >::~CSolverFactory()
{
}

template < class TState >
typename CSolverFactory< TState >::SolverType*
CSolverFactory< TState >::CreateNewSolver( NumericSolverType solver )
{
  SolverType* ptrSolver = NULL;
  switch ( solver )
  {
  case LineSearchConstrained:
    ptrSolver = new SolverLineSearchConstrainedType;
    break;
  case IpOpt:
    ptrSolver = new SolverIpOptType;
    break;
  case LBFGS:
    ptrSolver = new SolverLBFGSType;
    break;
  default:
    std::cout << "Unknown solver type = " << solver << "; defaulting to LineSearchUnconstrained." << std::endl;
  case LineSearchUnconstrained:
    ptrSolver = new SolverLineSearchUnconstrainedType;
    break;
  }

  return ptrSolver;
}

template < class TState >
typename CSolverFactory< TState >::SolverType*
CSolverFactory< TState >::CreateNewSolver( std::string sSolver )
{
  NumericSolverType solverType = GetSolverTypeFromString( sSolver );
  return CreateNewSolver( solverType );
}

template < class TState >
typename CSolverFactory< TState >::NumericSolverType
CSolverFactory< TState >::GetSolverTypeFromString( std::string sSolver )
{
  std::string sSolverLowerCase = sSolver;
  // convert to all lower case
  std::transform( sSolverLowerCase.begin(), sSolverLowerCase.end(), sSolverLowerCase.begin(), ::tolower );

  if ( sSolverLowerCase == "linesearchunconstrained" )
  {
    return LineSearchUnconstrained;
  }
  else if ( sSolverLowerCase == "linesearchconstrained" )
  {
    return LineSearchConstrained;
  }
  else if ( sSolverLowerCase == "ipopt" )
  {
    return IpOpt;
  }
  else if ( sSolverLowerCase == "lbfgs" )
  {
    return LBFGS;
  }
  else
  {
    std::cout << "Unknown solver type " << sSolver << "; defaulting to LineSearchUnconstrained." << std::endl;
    return LineSearchUnconstrained;
  }
}

#endif


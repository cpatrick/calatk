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

#ifndef C_QUERY_ENVIRONMENT_VARIABLES_TXX
#define C_QUERY_ENVIRONMENT_VARIABLES_TXX

#include "CQueryEnvironmentVariables.h"
#include "ApplicationUtils.h"

namespace CALATK
{

//
// constructor
//
CQueryEnvironmentVariables::CQueryEnvironmentVariables()
  : DefaultLogLevel( LOG_LEVEL_NORMAL )
{
}

//
// destructor
//
CQueryEnvironmentVariables::~CQueryEnvironmentVariables()
{
}

//
// Gets value of specified type from string
//
template < class T >
T CQueryEnvironmentVariables::GetFromString( std::string str, T defaultValue )
{
  if ( str.empty() )
    {
    return defaultValue;
    }
  else
    {
    std::stringstream ss;
    ss << str;
    T val;
    ss >> val;
    return val;
    }
}

//
// Gets path names from a colon delimited string
//
std::vector< std::string > CQueryEnvironmentVariables::GetPathFromString( std::string str )
{
  std::vector< std::string > res;
  if ( str.empty() )
    {
    return res;
    }
  else
    {
    // parse it
    typedef std::delimiter_istream_iterator< std::string, ':' > IteratorType;

    std::istringstream iss( str );
    std::copy( IteratorType( iss ),
               IteratorType(),
               std::back_inserter< std::vector< std::string > >( res ) );
    }

  // add a slash at the end if there is none
  std::vector< std::string >::iterator iter;
  for ( iter = res.begin(); iter != res.end(); ++iter )
  {
    if ( !ApplicationUtils::endsWith( *iter, '/') )
    {
      *iter += "/";
    }
  }

  return res;
}

//
// Gets an environment variables and returns it as a string
//
std::string CQueryEnvironmentVariables::GetEnv( const std::string & query )
{
  const char * queryResult = std::getenv( query.c_str() );
  if ( queryResult == NULL )
    {
    return "";
    }
  else
    {
    return queryResult;
    }
}

//
// Gets the log level for the output
//
int CQueryEnvironmentVariables::GetLogLevel()
{
  return GetFromString<int>( GetEnv( "CALATK.logLevel" ), DefaultLogLevel );
}

//
// Gets the data path (where data will be searched for)
//
std::vector< std::string > CQueryEnvironmentVariables::GetDataPath()
{
  std::vector< std::string > dataPath;
  dataPath = GetPathFromString( GetEnv( "CALATK.dataPath" ) );

  return dataPath;
}

//
// Gets the json path (where json configuration files will be searched for)
//
std::vector< std::string > CQueryEnvironmentVariables::GetJSONPath()
{
  std::vector< std::string > jsonPath;
  jsonPath = GetPathFromString( GetEnv( "CALATK.jsonPath" ) );

  return jsonPath;
}

} // end namespace CALATK

#endif

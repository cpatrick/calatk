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

#ifndef C_QUERY_ENVIRONMENT_VARIABLES_H
#define C_QUERY_ENVIRONMENT_VARIABLES_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include "LogUtils.h"

// first a custom iterator which allows to split a string based on a given delimiter

namespace std
{

template <class T, char T_delim = ':' >
class delimiter_istream_iterator: public iterator< input_iterator_tag, T>
{
    istream * _input;
    char _delim;
    string _value;
public:
    delimiter_istream_iterator( char delim = T_delim ): _input( 0 ), _delim( delim ) {}
    delimiter_istream_iterator( istream & in, char delim = T_delim ): _input( &in ), _delim( delim ) { ++*this; }

    const T operator *() const {
        istringstream ss( _value );
        T value;
        getline( ss, value );
        return value;
    }

    istream & operator ++() {
        if( !( getline( *_input, _value, _delim ) ) )
        {
            _input = 0;
        }
        return *_input;
    }

    bool operator !=( const delimiter_istream_iterator & rhs ) const {
        return _input != rhs._input;
    }
};

} // end namespace STD

namespace CALATK
{

class CQueryEnvironmentVariables
{
public:
  CQueryEnvironmentVariables();
  ~CQueryEnvironmentVariables();

  /**
   * @brief Returns log level which controls the verbosity of the output of CALATK functions which support it.
   *
   * @return int
   */
  int GetLogLevel();

  /**
   * @brief Returns the data path encoded in a colon delimited string
   *
   * @return std::vector<std::string>
   */
  std::vector< std::string > GetDataPath();

  /**
   * @brief Returns the json path encoded in a colon delimited string
   *
   * @return std::vector<std::string>
   */
  std::vector< std::string > GetJSONPath();

protected:

  /**
   * @brief Queries an environment variable
   *
   * @param query - name of the environment variable
   * @return std::string
   */
  std::string GetEnv( const std::string & query );

  /**
   * @brief Converts the content of a string to a different data type.
   *  Returns a default value if the string is empty.
   *
   * @param str - string to be converted
   * @param defaultValue - default value in case of empty string
   * @return T
   */
  template < class T > T GetFromString( std::string str, T defaultValue );

  /**
   * @brief Splits a string into pieces (colon delimited) to extract CALATK path information.
   *
   * @param str - string to be split
   * @return std::vector<std::string>
   */
  std::vector< std::string > GetPathFromString( std::string str );

private:
  const int DefaultLogLevel; //< default log level for the output
};

} // end namespace CALATK

#endif // C_QUERY_ENVIRONMENT_VARIABLES_H

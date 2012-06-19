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
#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <streambuf>
#include <ostream>
#include <iomanip>
#include <vector>

namespace CALATK
{

// Manipulators allowing the user to specify logging levels
struct LogLevelManipulator {
  
  unsigned value;
  LogLevelManipulator( unsigned int v) : value(v) { }
};

// This is just a convenience function for creating level manipulators
LogLevelManipulator log_level( unsigned int n);

// Define some level manipulators
const LogLevelManipulator
  log_level_most_verbose = log_level(4),
  log_level_very_verbose = log_level(3),
  log_level_verbose = log_level(2),
  log_level_normal = log_level(1),
  log_level_always = log_level(0);


// A class with which users can register logging streams
struct LogRegister {

  struct LevelRangeType
  {
    unsigned int levelFrom;
    unsigned int levelTo;
    std::ostream* stream;
  };
  
  typedef std::vector< LevelRangeType > log_vector_t;
  log_vector_t log_vector;
  
  void register_stream( std::ostream& stream, unsigned int levelFrom, unsigned int levelTo, unsigned int streamId );
  
  void write( const std::string& s, unsigned int currentLevel, unsigned int maxDesiredLevel ) const;
  
  static LogRegister& instance();
  
private:
  LogRegister();
};


// Forward declaration
class LogStream;

// Custom streambuf
struct LogBuffer : public std::streambuf {

  LogBuffer(LogStream* s);
  ~LogBuffer();

  void SetMaxDesiredLogLevel( unsigned int ll )
  {
    maxDesiredLogLevel = ll;
  }

private:

// The buffer holds a pointer to the stream, as it needs to query
  
// the stream to obtain the current logging level
  LogStream* stream_ptr;
  std::string buffer;

  int_type overflow(int_type i);
  void flush();

  unsigned int maxDesiredLogLevel;
};


// The actual stream class (the `message dispatcher')
class LogStream : public std::ostream {
  
  LogBuffer buffer;

public:

  void SetMaxDesiredLogLevel( unsigned int maxDesiredLogLevel )
  {
    buffer.SetMaxDesiredLogLevel( maxDesiredLogLevel );
  }
  
// Index into the ios state array
  static const int level_index;
  
  LogStream();
  unsigned int get_level();
  
};


// The function which modifies the stream's logging level according to
// a manipulator object
std::ostream& operator<<(std::ostream& out, LogLevelManipulator l);

} // end namespace

#endif

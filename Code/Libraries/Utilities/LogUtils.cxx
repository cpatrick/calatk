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

#include "LogUtils.h"

//--------------------
// Implementation
//------------------

namespace CALATK
{

void LogRegister::register_stream( std::ostream& stream, unsigned int minLevel, unsigned int maxLevel, unsigned int streamId )
{ 
  LevelRangeType range;
  range.levelFrom = minLevel;
  range.levelTo = maxLevel;
  range.stream = &stream;

  // if the stream is reset it is simply overwritten

  if ( log_vector.size() >= streamId + 1 )
    { // overwrite it
      log_vector[ streamId ] = range;
    }
  else
    { // resize and enter
      LevelRangeType defaultRange;
      defaultRange.levelFrom = 0;
      defaultRange.levelTo = 0;
      defaultRange.stream = NULL;
      log_vector.resize( streamId + 1, defaultRange );
      log_vector[ streamId ] = range;
    }
}

void LogRegister::write( const std::string& s, unsigned int currentLevel, unsigned int maxDesiredLevel ) const
{

  if ( currentLevel <= maxDesiredLevel ) // otherwise we don't even need to bother
    {
    for(log_vector_t::const_iterator i = log_vector.begin(); i != log_vector.end(); ++i)
      {
      // check if level is within the range of the current stream. If so, output
      if ( ( i->levelFrom <= currentLevel ) && ( currentLevel <= i->levelTo ) ) // is within range
        {
          *(i->stream) << s;
          //flush it immediately
          (*(i->stream)).flush();
        }
      }
    }
}

LogRegister& LogRegister::instance()
{ static LogRegister lr; return lr; }

LogRegister::LogRegister()
{ }


LogBuffer::LogBuffer(LogStream* s)
  : stream_ptr(s), maxDesiredLogLevel( 1 )
{ }


LogBuffer::~LogBuffer()
{ flush(); }


LogBuffer::int_type LogBuffer::overflow( int_type i) {

if(!traits_type::eq_int_type(i, traits_type::eof())) {

char_type c = traits_type::to_char_type(i);
buffer.push_back(c);
if( c == '\n') flush();
}

return traits_type::not_eof(i);
}


void LogBuffer::flush() {

  LogRegister::instance().write( buffer, stream_ptr->get_level(), maxDesiredLogLevel );
  buffer.clear();
}


// Stream class

LogStream::LogStream()
  : std::ostream(&buffer), buffer(this)
{ 
}


unsigned int LogStream::get_level()
{ return iword(level_index); }

const int LogStream::level_index = std::ios::xalloc();

// Manipulators

LogLevelManipulator log_level( unsigned int n)
{ return LogLevelManipulator(n); }


std::ostream& operator<<(std::ostream& out, LogLevelManipulator l) {
  
// First flush the stream
  out << std::flush;
  
// Now set the level
  out.iword(LogStream::level_index) = l.value;
  return out;
}

}

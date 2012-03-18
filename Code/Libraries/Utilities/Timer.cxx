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

#include "Timer.h"

Timer
::Timer()
{
  reset();
}

void
Timer
::start()
{
  if (!_isRunning)
  {
    _ftime(_lastStartTime);
    _isRunning  = true;
  }  
}

void
Timer
::stop()
{
  if (_isRunning)
  {
    timeb stopTime;
    _ftime(stopTime);
    
    _totalStartToStopSeconds += 
  static_cast<unsigned long>(stopTime.time - _lastStartTime.time);

    _additionalTotalStartToStopMilliseconds += 
  static_cast<long>(stopTime.millitm) - 
  static_cast<long>(_lastStartTime.millitm);

    _isRunning = false;
  }
}

void
Timer
::reset()
{
  _totalStartToStopSeconds = 0;
  _additionalTotalStartToStopMilliseconds = 0;
  _isRunning = false;
}

void
Timer
::restart()
{
  reset();
  start();
}

unsigned long
Timer
::getDays() const
{
  return this->getSeconds()/(60*60*24);
}

unsigned long
Timer
::getHours() const
{
  return this->getSeconds()/(60*60);
}

unsigned long
Timer
::getMinutes() const
{
  return this->getSeconds()/60;
}

unsigned long
Timer
::getSeconds() const
{
  if (_isRunning)
  {
    timeb now; 
    _ftime(now);
    return _totalStartToStopSeconds + 
  static_cast<unsigned long>(now.time - _lastStartTime.time);
  }
  else
  {
    return _totalStartToStopSeconds;
  }
}

std::string 
Timer
::getTime() const
{
  unsigned long sec;
  unsigned long msec;
  
  if (_isRunning)
  {
    timeb now;
    _ftime(now);
    sec = 
    _totalStartToStopSeconds + 
  static_cast<unsigned long>(now.time - _lastStartTime.time);
    msec =
    _additionalTotalStartToStopMilliseconds
  + static_cast<long>(now.millitm)
  - static_cast<long>(_lastStartTime.millitm);     
  }
  else
  {
    sec = _totalStartToStopSeconds; 
    msec = _additionalTotalStartToStopMilliseconds;
  }

  unsigned long hours = sec / (60 * 60);
  sec -= hours * 60 * 60;
  
  unsigned long minutes = sec / 60;
  sec -= minutes * 60;

  std::ostringstream oss;
  oss << hours << ":" 
    << std::setw(2) << std::setfill('0') << minutes << ":"
    << std::setw(2) << std::setfill('0') << sec << "." 
    << std::setw(3) << std::setfill('0') << msec;

  return oss.str();
}

unsigned long
Timer
::getMilliseconds() const
{
  if (_isRunning)
  {
    timeb now;
    _ftime(now);
    unsigned long seconds = _totalStartToStopSeconds + 
  static_cast<unsigned long>(now.time - _lastStartTime.time);
    
    return seconds * 1000 + _additionalTotalStartToStopMilliseconds
  + static_cast<long>(now.millitm)
  - static_cast<long>(_lastStartTime.millitm);     
  }
  else
  {
    return _totalStartToStopSeconds * 1000 
  + _additionalTotalStartToStopMilliseconds;
  }
}

void
Timer
::_ftime( timeb& theTime ) const
{
  struct timeval timeVal;
  struct timezone timeZone;
  gettimeofday( &timeVal, &timeZone );
  theTime.time = timeVal.tv_sec;
  theTime.millitm = timeVal.tv_usec / 1000;
  theTime.timezone = timeZone.tz_minuteswest;
  theTime.dstflag = timeZone.tz_dsttime;
}


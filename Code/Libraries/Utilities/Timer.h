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

#ifndef Timer_h
#define Timer_h

#include <iostream>
#include <sys/timeb.h>
#include <string>
#include <sstream>

#ifdef DARWIN
#include <sys/time.h>
#endif

#include <iomanip>

/**
 * simple timing class that acts like a stop watch
 *
 * @author bcd 2003
 */

class Timer
{
public:
  Timer();

  /** start timer if not already running */
  void start();

  /** stop timer if running */
  void stop();

  /** reset all values to 0, stop timer if running */
  void reset();

  /** sugar for common command: reset and then start */
  void restart();

  /** get number of days timer has run */
  unsigned long getDays() const;

  /** get number of hours timer has run */
  unsigned long getHours() const;

  /** get number of minutes timer has run */
  unsigned long getMinutes() const;

  /** get number of seconds timer has run */
  unsigned long getSeconds() const;

  /** get number of milliseconds timer has run */
  unsigned long getMilliseconds() const;

  /** get time as a string hours:minutes:seconds.milliseconds */
  std::string getTime() const;

private:
  bool       _isRunning;
  timeb      _lastStartTime;

  unsigned long  _totalStartToStopSeconds;
  long       _additionalTotalStartToStopMilliseconds;

  void _ftime( timeb& theTime ) const;
};

#endif

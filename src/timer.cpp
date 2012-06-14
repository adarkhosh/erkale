/*
 *                This source code is part of
 * 
 *                     E  R  K  A  L  E
 *                             -
 *                       DFT from Hel
 *
 * Written by Jussi Lehtola, 2010-2011
 * Copyright (c) 2010-2011, Jussi Lehtola
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */



#include <cmath>
#include <cstdio>
#include <string>
#include <sstream>

#include "timer.h"

Timer::Timer() {
  // Get time.
  time(&start);
  gettimeofday(&tstart,NULL);

  elapsd=0.0;
}

Timer::~Timer() {
}

void Timer::stop() {
  struct timeval tstop;

  gettimeofday(&tstop,NULL);

  elapsd+=(tstop.tv_sec-tstart.tv_sec)+(tstop.tv_usec-tstart.tv_usec)/1000000.0;
}

void Timer::cont() {
  time(&start);
  gettimeofday(&tstart,NULL);
}

void Timer::set() {
  time(&start);
  gettimeofday(&tstart,NULL);
  elapsd=0.0;
}

void Timer::print() const {
  printf("Time elapsed is %s.\n",elapsed().c_str());
}

std::string Timer::current_time() const {
  char out[256];

  // Get time
  time_t t;
  time(&t);

  // Convert it into struct tm
  struct tm tm;
  gmtime_r(&t,&tm);

  const char * days[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  const char * months[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

  // Print time
  sprintf(out,"%s %02i %s %4i %02i:%02i:%02i",days[tm.tm_wday],tm.tm_mday,months[tm.tm_mon],1900+tm.tm_year,tm.tm_hour,tm.tm_min,tm.tm_sec);
  return std::string(out);
}

void Timer::print_time() const {
  printf("Current time is %s.\n",current_time().c_str());
}

double Timer::get() const {
  struct timeval tstop;

  tstop.tv_sec=0;
  tstop.tv_usec=0;
  gettimeofday(&tstop,NULL);

  return elapsd+(tstop.tv_sec-tstart.tv_sec)+(tstop.tv_usec-tstart.tv_usec)/1000000.0;
}

std::string Timer::elapsed() const {
  std::ostringstream ret;

  struct timeval tstop;

  gettimeofday(&tstop,NULL);

  time_t isecs=tstop.tv_sec-tstart.tv_sec;

  // Minute is 60 sec
  time_t min=60;
  // Hour is 60 minutes
  time_t hour=60*min;
  // Day is 24 hours
  time_t day=24*hour;

  // Compute number of days
  time_t days=(time_t) trunc(isecs/day);
  if(days) {
    isecs-=days*day;

    ret << days << " d";
  }
  
  // Compute number of hours
  time_t hours=(time_t) trunc(isecs/hour);
  if(hours) {
    isecs-=hours*hour;

    // Check that there is a space at the end
    std::string tmp=ret.str();
    if(tmp.size() && tmp[tmp.size()-1]!=' ')
      ret << " ";
    
    ret << hours << " h";
  }

  // Compute number of minutes
  time_t mins=(time_t) trunc(isecs/min);
  if(mins) {
    isecs-=mins*min;

    // Check that there is a space at the end
    std::string tmp=ret.str();
    if(tmp.size() && tmp[tmp.size()-1]!=' ')
      ret << " ";
    
    ret << mins << " min";
  }

  // Compute number of seconds
  double secs=isecs+(tstop.tv_usec-tstart.tv_usec)/1000000.0;
  
  // Check that there is a space at the end
  std::string tmp=ret.str();
  if(tmp.size() && tmp[tmp.size()-1]!=' ')
    ret << " ";

  char hlp[80];
  sprintf(hlp,"%.2f s",secs);
  ret << hlp;
  
  return ret.str();
}

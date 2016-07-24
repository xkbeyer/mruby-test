// time_meas.h :
// Simple warpper class to measure time.
// Written by Klaus Beyer, 2015
//
// Examples:
// TimeMeaseremtn tm;
// ... a lot of code 
// tm.stop();
// std::cout << "Time elapsed so far (ms) " << tm.diffTime<std::chrono::milliseconds>() << std::endl;
//
#pragma once

#include <chrono>

class TimeMeasurement
{
public:
   TimeMeasurement() { start(); }
   template<typename T>
   long long diffTime()
   {
      return std::chrono::duration_cast<T>(endTime - startTime).count();
   }
   void stop()
   {
      endTime = std::chrono::high_resolution_clock::now();
   }
   void start()
   {
      startTime = std::chrono::high_resolution_clock::now();
      endTime = startTime;
   }
private:
   std::chrono::time_point< std::chrono::high_resolution_clock> startTime;
   std::chrono::time_point< std::chrono::high_resolution_clock> endTime;
};

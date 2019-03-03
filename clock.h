#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <Arduino.h>
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
//#include <String.h>

class ClockClass {
private:
  uint32_t sleepTimeMs; 

public:
	ClockClass(uint32_t sleepTimeMs);
  uint32_t tick();
  uint32_t tickMs(void);
	void setClock(uint32_t year, uint32_t month, uint32_t day, uint32_t hours, uint32_t minutes, uint32_t seconds);
	String toString(void);
  char * timeToCStr(void);
};

#endif _CLOCK_H_

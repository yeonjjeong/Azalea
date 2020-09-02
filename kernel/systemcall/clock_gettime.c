#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "macros.h"
#include "timer.h"

#define CLOCK_REALTIME           0
#define CLOCK_MONOTONIC          1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID  3
#define CLOCK_MONOTONIC_RAW      4
#define CLOCK_REALTIME_COARSE    5
#define CLOCK_MONOTONIC_COARSE   6
#define CLOCK_BOOTTIME           7
#define CLOCK_REALTIME_ALARM     8
#define CLOCK_BOOTTIME_ALARM     9
#define CLOCK_SGI_CYCLE         10
#define CLOCK_TAI               11

static inline int realtime_gettime (struct timespec *tp) 
{ 
  struct timeval tv; 
  int retval = sys_gettimeofday (&tv, NULL); 

  if (retval == 0) 
    /* Convert into `timespec'.  */ 
    TIMEVAL_TO_TIMESPEC (&tv, tp); 
  return retval; 
} 

int sys_clock_gettime (clockid_t clock_id, struct timespec *tp) 
{ 
  int retval = -1; 

  if(unlikely(!tp)) {
    lk_print("clock_gettime: tp is null\n");
    return -EINVAL;
  }

  switch (clock_id) 
  { 
    case CLOCK_REALTIME: 
      { 
        struct timeval tv; 
        retval = sys_gettimeofday (&tv, NULL); 
        if (retval == 0) 
          TIMEVAL_TO_TIMESPEC (&tv, tp); 
      } 
      break; 

    default: 
      retval = -EINVAL; 
      break; 
  } 
  return retval; 
} 

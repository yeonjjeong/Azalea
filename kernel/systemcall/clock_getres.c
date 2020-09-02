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


#define NSEC_PER_SEC		1000000000
#define USEC_PER_SEC		1000000
#define MSEC_PER_SEC		1000
#define DSEC_PER_SEC		10
#define NSEC_PER_DSEC		100000000
#define USEC_PER_DSEC		100000
#define MSEC_PER_DSEC		100
#define NSEC_PER_MSEC		1000000
#define USEC_PER_MSEC		1000
#define NSEC_PER_USEC		1000
#define USEC_PER_TICK        (10000)

#define TICK_PER_DSEC         (USEC_PER_DSEC / USEC_PER_TICK)            /* Truncates! */
#define TICK_PER_SEC          (USEC_PER_SEC  / USEC_PER_TICK)            /* Truncates! */
#define TICK_PER_MSEC         (USEC_PER_MSEC / USEC_PER_TICK)            /* Truncates! */
#define MSEC_PER_TICK         (USEC_PER_TICK / USEC_PER_MSEC)            /* Truncates! */
#define NSEC_PER_TICK         (USEC_PER_TICK * NSEC_PER_USEC)            /* Exact */

#define SYSTEM_CLK_TCK        100


static inline int realtime_getres (struct timespec *res) 
{ 
  /* This implementation assumes that the realtime clock has a 
     resolution higher than 1 second.  This is the case for any 
     reasonable implementation.  */ 
  res->tv_sec = 0; 
  //res->tv_nsec = sys_get_ticks(); 
  res->tv_nsec = NSEC_PER_TICK; 
  //res->tv_nsec = 1000000000 / SYSTEM_CLK_TCK;; 

  return 0; 
} 


/* Get resolution of clock.  */ 
int sys_clock_getres (clockid_t clock_id, struct timespec *res) 
{ 
  int retval = -1; 

  if(unlikely(!res)) {
    lk_print("clock_getres: res is null\n");
    return -EINVAL;
  }

  switch (clock_id) 
  { 
    case CLOCK_REALTIME: 
      retval = realtime_getres (res); 
      break; 

    default: 
      retval = -EINVAL; 
      break; 
  } 

  return retval; 
}

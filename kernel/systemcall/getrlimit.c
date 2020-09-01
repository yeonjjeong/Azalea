#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "macros.h"
#include "memory_config.h"

/* Resources identifiers */
#define RLIMIT_CPU 		0
#define RLIMIT_FSIZE		1
#define RLIMIT_DATA		2
#define RLIMIT_STACK		3
#define RLIMIT_CORE		4
#define RLIMIT_RSS		5
#define RLIMIT_NPROC		6
#define RLIMIT_NOFILE		7
#define RLIMIT_MEMLOCK		8
#define RLIMIT_AS		9
#define RLIMIT_LOCKS		10
#define RLIMIT_SIGPENDING	11
#define RLIMIT_MSGQUEUE		12
#define RLIMIT_NICE		13
#define RLIMIT_RTPRIO		14
#define RLIMIT_NLIMITS		15


typedef long long int rlim_t;

struct rlimit {
	rlim_t rlim_cur;
	rlim_t rlim_max;
};

/**
 * @brief get resource limits
 * @param resource - resource
 * @param rlim - pointer to the rlimit structure
 * @return success - (0), fail - (-1)
 */
long sys_getrlimit(unsigned int resource, struct rlimit *rlim) {
  long ret = 0;

  if(unlikely(!rlim)) {
    lk_print("getrlimit: rlim is null\n");
    return -EINVAL;
  }

  switch(resource) {
    case RLIMIT_STACK:
      rlim->rlim_cur = CONFIG_STACK_SIZE;
      rlim->rlim_max = CONFIG_STACK_SIZE;
      break;

    case RLIMIT_NPROC:
      rlim->rlim_cur = CONFIG_NUM_THREAD;
      rlim->rlim_max = CONFIG_NUM_THREAD;
      break;

    case RLIMIT_NOFILE:
      rlim->rlim_cur = 0x100000; /* linux limit */
      rlim->rlim_max = 0x100000; /* linux limit */
      break;

    case RLIMIT_AS:
    case RLIMIT_DATA:
      rlim->rlim_cur = HEAP_START + HEAP_SIZE;
      rlim->rlim_max = HEAP_START + HEAP_SIZE;
      break;

    case RLIMIT_NICE:
      rlim->rlim_cur = 0;
      rlim->rlim_max = 0;
      break;

    case RLIMIT_CORE:
      rlim->rlim_cur = 0; /* no core dump creation in hermitux */
      rlim->rlim_max = 0;
      break;

    case RLIMIT_FSIZE:
      rlim->rlim_cur = -1; /* same as default in linux */
      rlim->rlim_max = -1;
      break;

    default:
      lk_print("getrlimit: unsupported operation %d\n", resource);
      ret = -ENOSYS;
  }

  return ret;
}

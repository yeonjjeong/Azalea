#include "systemcall.h"
#include "console.h"
#include "errno.h"

/**
 * @brief set resource limits
 * @param resource - resource
 * @param rlim - pointer to the rlimit structure
 * @return success - (0), fail - (-1)
 */
int sys_setrlimit(int resource, const struct rlimit *rlim) {
  lk_print("setrlimit: syscall not supported\n");
  return -ENOSYS;
}

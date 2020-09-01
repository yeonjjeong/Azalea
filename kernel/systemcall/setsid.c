#include <sys/lock.h>

#include "systemcall.h"
#include "thread.h"

/**
 * @brief creates a session and sets the process group ID
 * @param none
 * @return success - ( the (new) session ID), fail - ((pid_t) -1)
 */
int sys_setsid(void) {
  return get_current()->id;
}

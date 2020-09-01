#include <sys/lock.h>

#include "systemcall.h"
#include "thread.h"

/**
 * @brief set pointer to thread ID
 * @param tidptr -  a pointer to thread ID
 * @return (PID)
 */
long sys_set_tid_address(int *tidptr) {
  TCB* tcb;

  tcb = get_current();
  tcb->clear_child_tid = tidptr;

  return tcb->id;
}

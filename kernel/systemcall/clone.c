#include <sys/lock.h>

#include "console.h"
#include "thread.h"

/* From Linux sources: */
#define CLONE_VM		0x00000100
#define CLONE_CHILD_CLEARTID	0x00200000
#define CLONE_CHILD_SETTID	0x01000000

/** 
 * @brief clone systemcall
 * @param clone_flags - clone flags
 * @param stack -  the location of the stack used by the child process
 * @param ptid - address to store child id
 * @param ctid - address to clear child id
 * @param tls - address to thread-local storage 
 * @param ep - clone entrypoint
 * @return Success (thread id), Fail: (-1)
 */
int sys_clone(unsigned long clone_flags, void *stack, int *ptid, int *ctid, void *tls, void *ep)
{
  tid_t tid = -1;;
  void *arg = NULL;

#if DEBUG
  cs_printf("sys_clone(): clone_flags: %q, stack:%q, ptid: %q, ctid: %q, tls: %q, ep:%q \n", (QWORD) clone_flags, (QWORD)stack, (QWORD) ptid, (QWORD) ctid, (QWORD) tls, (QWORD)ep);
#endif

  //  set tid address 
  void *set_child_tid = (clone_flags & CLONE_CHILD_SETTID) ? ctid : NULL;
  void *clear_child_tid = (clone_flags & CLONE_CHILD_CLEARTID) ? ctid : NULL;

  // arg: parameters want to pass to clone thread
  arg = (void *) *(unsigned long *)stack;

  tid = clone_thread((QWORD) ep, (QWORD) arg, -1, set_child_tid, clear_child_tid, tls);

  if(tid) {
    if(ptid) {
     *(unsigned int *)ptid = tid;
    }

    if(clone_flags & CLONE_CHILD_SETTID) {
      if (ctid) {
        *(int *)ctid = tid;
      }
    }
    return tid;
  }
  else 
  {
    return (-1);
  }
}

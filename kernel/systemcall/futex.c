#include <sys/lock.h>

#include "atomic32.h"
#include "console.h"
#include "console_function.h"
#include "futex.h"
#include "memory.h"
#include "sync.h"
#include "thread.h"

//#define DEBUG
#define L_CACHE_LINE_SIZE	64

static futex_q* head = NULL;
static spinlock_irqsave_t futex_lock __attribute__((aligned(L_CACHE_LINE_SIZE)));
int id_to_wake[MAX_TASKS] = {0, };

/**
 * @brief futex wait
 * @param uaddr - futex address
 * @param val - value
 * @return success (0), fail(-1)
 */
inline static int futex_wait(int* futex, int val) 
{
  int i = 0;
  futex_q* walk = NULL;
  int f_val = atomic_int32_read(((atomic_int32_t *) futex));

  if (f_val != val) {
    spinlock_irqsave_unlock(&futex_lock);
    return -EAGAIN;
  }

  //task_t* curr_task = per_core(current_task);
  TCB *curr_task = get_current();

  // if the futex is in the list, then add this task as a waiter to the list
  walk = head;
  while (walk) {
    if (walk->futex == futex) {
      walk->waiters[curr_task->id] = 1;
      break;
    }
    walk = walk->next;
  }

  // walk will be null if it wasn't in the list.
  if (!walk) {
    //walk = (futex_q*)kmalloc(sizeof(*walk));
    walk = (futex_q*) az_alloc(sizeof(futex_q));
    walk->futex = futex;
    for(i=0; i<MAX_TASKS; i++)
      walk->waiters[i] = 0;

    walk->waiters[curr_task->id] = 1;

    // push front
    if (head) {
      head->prev = walk;
      walk->next = head;
      walk->prev = NULL;
      head = walk;
    } else {
        head = walk;
        walk->next = NULL;
        walk->prev = NULL;
    }
  }

  // Now that the futex and waiters are in the list, we can block
  spinlock_irqsave_unlock(&futex_lock);

  //block_current_task();
  get_current()->remaining_time_slice = 0;
  schedule(THREAD_INTENTION_BLOCKED);

  return 0;
}


/**
 * @brief futex wake
 * @param uaddr - futex address
 * @param val - maximum of waiters to wake up
 * @return woken count
 */
inline static int futex_wake(int* futex) 
{
  int i = 0;
  futex_q* walk = head;
  int id_to_wake_count = 0;

#ifdef DEBUG
	//cs_printf(futex: wake: futex: %q\n", (unsigned long) futex);
#endif

  // try to find the futex
  while (walk) {
    if (walk->futex == futex) {
      for(i=0; i<MAX_TASKS; i++) {
        if(walk->waiters[i]) {
          id_to_wake[id_to_wake_count++] = i;
          walk->waiters[i] = 0;
          //break;
        }
      }
      break;
    }

    walk = walk->next;
  }

  if (!id_to_wake_count) {
    // remove the futex and free if there was one
    if (walk) {
      if (walk == head) {
        head = walk->next;
        az_free(walk);
      } else {
        walk->prev->next = walk->next;
        if (walk->next) {
          walk->next->prev = walk->prev;
        }
        az_free(walk);
      }
    }
    // no waiters woken up
    return 0;
  }

  // waking up all waiter for now
  for(i = 0; i < id_to_wake_count; i++) {
    thread_wake_up(id_to_wake[i]);
  }

  return id_to_wake_count;
}

/**
 * @brief futex system call
 * @param uaddr - futex address
 * @param op - operations
 * @param val - value whose meaning and purpose depends on futex op
 * @param timeout - timeout for the operation
 * @param uaddr2 - second futex address
 * @param val3 - value whose meaning and purpose depends on futex op
 * @return success - depends on the operation, fail - (-1)
 */
int sys_futex(int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3) 
{
  int cmd = op & FUTEX_CMD_MASK;
  int ret;

  if(unlikely(!uaddr)) {
    lk_print("futex: uaddr is null\n");
    return -EINVAL;
  }

#ifdef DEBUG
  cs_printf("sys_futex(): uaddr: %q, op: %q, val: %d, timespec: %q, uaddr2: %q, val3: %d\n", (unsigned long) uaddr,  op, val, timeout, uaddr2, val3);
#endif

  if(op & FUTEX_PRIVATE_FLAG) {
#ifdef DEBUG
    cs_puts("sys_futex(): FUTEX_PRIVATE_FLAG\n");
#endif
    return -ENOSYS;
  }

  switch(cmd) {
    case FUTEX_WAIT:
      spinlock_irqsave_lock(&futex_lock);
      ret = futex_wait(uaddr, val);
      /* wait takes care of unlock */
      return ret;

    case FUTEX_WAKE:
      if(val != 1) {
        lk_print("futex: cannot wake more than 1 thread\n");
        return -ENOSYS;
      }

      spinlock_irqsave_lock(&futex_lock);
      ret = futex_wake(uaddr);
      spinlock_irqsave_unlock(&futex_lock);
      return ret;

    default:
      lk_print("Unsupported futex operation\n");
      return -ENOSYS;
  }

  return -ENOSYS;
}


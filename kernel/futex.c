#include <sys/lock.h>
#include "atomic32.h"
#include "sync.h"
#include "thread.h"
#include "memory.h"
#include "console.h"
#include "console_function.h"

#include "futex.h"

#define DEBUG

static futex_q* head = NULL;

static spinlock_irqsave_t futex_lock = SPINLOCK_IRQSAVE_INIT;


inline static int futex_wait(int* futex, int val) 
{
int i = 0;

	int f_val = atomic_int32_read(((atomic_int32_t *)futex));
	if (f_val != val) {
		spinlock_irqsave_unlock(&futex_lock);
		return -EAGAIN;
	}

	//task_t* curr_task = per_core(current_task);
	TCB *curr_task = get_current();

	// if the futex is in the list, then add this task as a waiter to the list
	futex_q* walk = head;
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
		//walk = (futex_q*)az_alloc(sizeof(*walk));
		walk = (futex_q*)az_alloc(sizeof(futex_q));
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

#ifdef DEBUG
	cs_printf("ftuex: wait: id_to_wait: %d futex: %q\n", curr_task->id, (unsigned long) futex);
#endif
	//block_current_task();
	//reschedule();
	get_current()->remaining_time_slice = 0;
	schedule(THREAD_INTENTION_BLOCKED);

	return 0;
}

inline static int futex_wake(int* futex) 
{
int i = 0;

	// Try to find the futex
	futex_q* walk = head;
	int id_to_wake = 0;

#ifdef DEBUG
	//cs_printf("ftuex: wake: futex: %q\n", (unsigned long) futex);
#endif

	while (walk) {
		if (walk->futex == futex) {
			for(i=0; i<MAX_TASKS; i++) {
				if(walk->waiters[i]) {
					id_to_wake = i;
					walk->waiters[i] = 0;
					break;
				}
			}
			break;
		}

		walk = walk->next;
	}

	if (!id_to_wake) {
		// remove the futex and free if there was one
#ifdef DEBUG
		cs_printf("ftuex: wake: id to wake: %d not found : futex: %q\n", id_to_wake, futex);
#endif
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

	// Only waking up one waiter for now
	//wakeup_task(id_to_wake);
#ifdef DEBUG
	cs_printf("ftuex: wake: id to wake: %d futex: %q\n", id_to_wake, futex);
#endif
	thread_wake_up(id_to_wake);
	return 1;
}

int sys_futex(int *uaddr, int op, int val, const struct timespec *timeout, int *uaddr2, int val3) 
{
int cmd = op & FUTEX_CMD_MASK;
int ret;

	if(unlikely(!uaddr)) {
		//LOG_ERROR("futex: uaddr is null\n");
		lk_print("futex: uaddr is null\n");
		return -EINVAL;
	}

#ifdef DEBUG
	//cs_puts("futex: called\n");
	//cs_printf("futex: called uaddr: %q val: %d uaddr2: %q\n", (unsigned long) uaddr,  val, uaddr2);
#endif

#if 1
	if(op & FUTEX_PRIVATE_FLAG) {
#ifdef DEBUG
		cs_puts("futex: FUTEX_PRIVATE_FLAG\n");
#endif
		return -ENOSYS;
	}
#endif

	switch(cmd) {
		case FUTEX_WAIT:
#ifdef DEBUG
			//cs_printf("futex: wait: uaddr: %q, op: %dd\n", (unsigned long) uaddr, op);
#endif
			spinlock_irqsave_lock(&futex_lock);
			ret = futex_wait(uaddr, val);
			/* wait takes care of unlock */
			return ret;

		case FUTEX_WAKE:
#ifdef DEBUG
			//cs_printf("futex: wake: uaddr: %q val: %d uaddr2: %q\n", (unsigned long) uaddr, val, uaddr2);
#endif
			if(val != 1) {
				//LOG_ERROR("futex: cannot wake more than 1 thread\n");
				lk_print("futex: cannot wake more than 1 thread\n");
				return -ENOSYS;
			}

//uaddr = 0xFFFF80033FC62F60;
			spinlock_irqsave_lock(&futex_lock);
			ret = futex_wake(uaddr);
			spinlock_irqsave_unlock(&futex_lock);
			return ret;

		default:
			//LOG_ERROR("Unsupported futex operation\n");
#ifdef DEBUG
			cs_puts("futex: default: Unsupported futex operation\n");
#endif
			lk_print("Unsupported futex operation\n");
			return -ENOSYS;
	}
	return -ENOSYS;
}


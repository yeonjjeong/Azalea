#include <sys/lock.h>

#include "systemcall.h"
#include "thread.h"

/**
 * @brief get parent identity
 * @param - none
 * @return (parent id)
 */
tid_t sys_getppid(void)
{
	return get_current()->parent;
}


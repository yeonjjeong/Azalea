#include <sys/lock.h>

#include "systemcall.h"
#include "thread.h"

/**
 * @brief get thread identity
 * @param - none
 * @return (thread id)
 */
int sys_gettid(void)
{
	return get_current()->id;
}


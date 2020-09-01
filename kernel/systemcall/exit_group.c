#include <sys/lock.h>

#include "systemcall.h"
#include "thread.h"

/** @brief To be called by the systemcall to exit tasks */
void sys_exit_group(int arg)
{
	do_exit(arg);
}

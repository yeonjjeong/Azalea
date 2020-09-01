#include <sys/types.h>

#include "systemcall.h"
#include "errno.h"

/**
 * @brief determine whether pages are resident in memory
 * @param start - start address
 * @param len - size length
 * @param vec - a vector that indicates whether pages of the calling process's virtual memory
 *              are resident in core (RAM)
 * @return success - (0), fail - (-1)
 */
/* TODO */
int sys_mincore(unsigned long start, size_t len, unsigned char *vec) {
	return -ENOSYS;
}

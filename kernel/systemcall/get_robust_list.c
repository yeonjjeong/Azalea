#include <sys/types.h>

#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "macros.h"

/**
 * @brief get list of robust futexes
 * @param pid
 * @param head_ptr - pointer to the head of the list of robust futexes
 * @param len_ptr - sizeof(*head)
 * @return success - (0), fail - (error code)
 */
long sys_get_robust_list(int pid, void *head_ptr, size_t *len_ptr) {

	if(unlikely(!head_ptr || !len_ptr)) {
		lk_print("get_robust_list: some parameter(s) is (are) null\n");
		return -EINVAL;
	}

	lk_print("get_robust_list: unsuported syscall\n");

	/* TODO */
	return -ENOSYS;
}

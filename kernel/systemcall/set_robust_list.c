#include "systemcall.h"

#include "console.h"

char *ptr = NULL;

/**
 * @brief set list of robust futexes
 * @param head - pointer to the head of the list of robust futexes
 * @param len - sizeof(*head)
 * @return success - (0), fail - (error code)
 */
/* Fake implementation */
long sys_set_robust_list(void *head, size_t len) {
	/* TODO */
	lk_print("set_robust_list: syscall not supported, faking success\n");
	ptr = head;
	return 0;
}

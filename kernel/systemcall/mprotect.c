#include <sys/types.h>

#include "systemcall.h"
#include "console.h"

/*
 * @brief set protection on a region of memory
 * @param addr - the address to change protection
 * @param len - length
 * @param prot - protection
 * @return success - (0), fail - (-1)
 */
long sys_mprotect(size_t addr, size_t len, unsigned long prot) {

	lk_print("mprotect: unsupported syscall, faking success\n");

	/* FIXME */
	return 0;
}

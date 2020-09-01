#include <sys/types.h>

#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "utility.h"

#define MAP_FIXED 0x10

/*
 * @brief remap a virtual memory address
 * @param addr - old address 
 * @param old_len - old length
 * @param new_len - new length
 * @param flags - flags
 * @param new_addr - new address 
 * @return success - (a pointer to the new virtual memory area), fail - ((void *) -1)
 */
size_t sys_mremap(unsigned long addr, unsigned long old_len, unsigned long new_len,
		unsigned long flags, unsigned long new_addr) 
{
  uint64_t ret;

  if(flags & MAP_FIXED) {
    lk_print("mremap: no support for MAP_FIXED\n");
    return -ENOSYS;
  }

  ret = sys_mmap((unsigned long)NULL, new_len, 0x0, flags, 0x0, 0x0);

  lk_memcpy((void *)ret, (void *)addr, old_len);

  sys_munmap(addr, old_len);

  return ret;
}

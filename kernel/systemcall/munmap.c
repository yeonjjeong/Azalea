#include <sys/types.h>

#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "macros.h"
#include "memory.h"

/*
 * @brief unmap files or devices into memory
 * @param viraddr - the address to unmap 
 * @param len - length
 * @return success - (0), fail - (-1)
 */
int sys_munmap(size_t viraddr, size_t len) {
  int ret;

  if (BUILTIN_EXPECT(!viraddr, 0))
    return -EINVAL;

  if (BUILTIN_EXPECT(!len, 0))
    return -EINVAL;

  /* Free address space */
  ret = az_free((void *) viraddr);

  if(ret < 0) {
    lk_print("munmap: cannot free\n");
    return ret;
  }

  return 0;
}

#include <sys/types.h>

#include "systemcall.h"
#include "offload_fio.h"
#include "console.h"
#include "errno.h"
#include "macros.h"
#include "memory.h"

#define MAP_FIXED	0x10
#define MAP_PRIVATE	0x02

#define PROT_NONE	0
#define PROT_READ	1
#define PROT_WRITE	2
#define PROT_EXEC	4

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

#define	sys_read	sys_off_read
#define	sys_lseek	sys_off_lseek

int map_file(int fd, void *addr, size_t offset, size_t len);

/* 
 * @brief map files or devices into memory
 * @param addr - the address at which to create the mapping
 * @param len - length
 * @param prot - the desired memory protection of the mapping
 * @param flags - flags
 * @param fd - file descriptor
 * @param off - offset
 * @return success - (a pointer to the mapped area), fail ((void *) -1) 
 */
size_t sys_mmap(unsigned long addr, unsigned long len, unsigned long prot,
		unsigned long flags, unsigned long fd, unsigned long off) 
{
  size_t viraddr = NULL;

  if(!(flags & MAP_PRIVATE)) {
    lk_print("mmap: non-private mappings are not supported\n");
    return -ENOSYS;
  }

  if (BUILTIN_EXPECT(!len, 0))
    return -EINVAL;

  viraddr = (unsigned long) az_alloc(len);

  /* Emulate a private file mapping */
  if(fd && (int)fd != -1)
    if(map_file(fd, (void *)viraddr, off, len))
      return -EFAULT;

  return (size_t)viraddr;
}

/* 
 * @brief Read into address addr the file fd starting from offset in the file, for len bytes
 * param fd - file descriptor
 * param addr - address to map
 * param offset - offset
 * param len - length
 * return success 0, fail -1
 */
int map_file(int fd, void *addr, size_t offset, size_t len) {
  int bytes_read = -1;
  size_t old_offset = -1;

  /* save old offset */
  old_offset = sys_lseek(fd, 0x0, SEEK_CUR);
  if(old_offset == -1) {
    lk_print("mmap: cannot get offset of file (fd %d)\n", fd);
    return (-1);
  }

  /* Set the asked offset */
  if(sys_lseek(fd, offset, SEEK_SET) != offset) {
    lk_print("mmap: cannot lseek in file (fd %d)\n", fd);
    return (-1);
  }

  /* Read the file in memory */
  bytes_read = sys_read(fd, addr, len);
  if(bytes_read < 0) {
    /* It is actually OK to read less bytes that requested because one may
     * want to mmap a file within a area that is bigger than the file
     * itself */
    lk_print("mmap: cannot read file (read returns %d)\n", bytes_read);
    return (-1);
  }

  /* restore old offset */
  sys_lseek(fd, old_offset, SEEK_SET);

  return (0);
}


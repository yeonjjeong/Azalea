#include "systemcall.h"
#include "console.h"
#include "errno.h"
#include "macros.h"
#include "utility.h"

struct utsname {
  char sysname[65];
  char nodename[65];
  char release[65];
  char version[65];
  char machine[65];
  char domainname[65];
};

/**
 * @brief get name and information about current kernel
 * @param buf -  a pointer to buffer
 * @return success - (0), fail - (-1)
 */
int sys_uname(struct utsname *buf) {
  if(unlikely(!buf)) {
    lk_print("uname: buf is null\n");
    return -EINVAL;
  }

  lk_memset(buf , 0x0, sizeof(struct utsname));
  lk_memcpy(buf->sysname, "Azalea unikernel", lk_strlen("Azalea unikernel"));
  lk_memcpy(buf->nodename, "Azalea", lk_strlen("Azalea"));
  lk_memcpy(buf->release, "4.9.0-4-amd64", lk_strlen("4.9.0-4-amd64"));    /* Faking Linux here for compat with glibc */
  lk_memcpy(buf->version, "0.1-june-2020", lk_strlen("0.1-june-2020"));
  lk_memcpy(buf->machine, "x86_64", lk_strlen("x86_64"));
  *buf->domainname = '\0';

  return 0;
}

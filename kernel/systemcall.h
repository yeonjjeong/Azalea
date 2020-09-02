#ifndef __SYSTEMCALL_H__
#define __SYSTEMCALL_H__

#include <sys/types.h>
#include "az_types.h"
#include "assemblyutility.h"

static inline void set_kernel_gs_base(QWORD kernel_pointer)
{
  QWORD rdx, rax;

  // IA32_KERNELGSBASE 
  rdx = ((QWORD) kernel_pointer) >> 32;
  rax = (kernel_pointer & 0xFFFFFFFF);

  write_msr(0xC0000102, rdx, rax);
}

void systemcall_entrypoint(void);
QWORD process_systemcall(QWORD param1, QWORD param2, QWORD param3,
                         QWORD param4, QWORD param5, QWORD param6,
                         QWORD no);
void systemcall_init(void);


/* Pierre */
struct utsname;
struct stat;
struct iovec;
struct timespec;
struct timeval;
struct sigaction;
struct sockaddr;
struct rlimit;
struct sysinfo;
struct linux_dirent64;
struct linux_dirent;
typedef struct fd_set fd_set;
typedef unsigned short umode_t;
//typedef uint32_t socklen_t;

int sys_arch_prctl(int option, unsigned long *arg2, unsigned long *addr);
int sys_clock_getres (clockid_t clock_id, struct timespec *res);
int sys_clock_gettime (clockid_t clock_id, struct timespec *tp);
int sys_clone(unsigned long clone_flags, void *stack, int *ptid, int *ctid, void *tls, void *ep);
void sys_exit_group(int arg);
int sys_getegid(void);
int sys_geteuid(void);
int sys_getgid(void);
int sys_getppid(void);
long sys_getrlimit(unsigned int resource, struct rlimit *rlim);
long sys_get_robust_list(int pid, void *head_ptr, size_t *len_ptr);
int sys_gettid(void);
int sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg);
int sys_madvise(unsigned long start, size_t len_in, int behavior);
int sys_mincore(unsigned long start, size_t len, unsigned char *vec);
size_t sys_mmap(unsigned long addr, unsigned long len, unsigned long prot, unsigned long flags, unsigned long fd, unsigned long off);
long sys_mprotect(size_t addr, size_t len, unsigned long prot);
size_t sys_mremap(unsigned long addr, unsigned long old_len, unsigned long new_len,
                  unsigned long flags, unsigned long new_addr);
int sys_munmap(size_t viraddr, size_t len);
int sys_rt_sigaction(int signum, struct sigaction *act, struct sigaction *oldact);
int sys_setrlimit(int resource, const struct rlimit *rlim);
long sys_set_robust_list(void *head, size_t len);
int sys_setsid(void);
long sys_set_tid_address(int *tidptr);
int sys_uname(struct utsname *buf);

#endif                          // __SYSTEMCALL_H__

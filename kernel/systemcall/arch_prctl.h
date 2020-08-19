#ifndef __ARCH_PRCTL_H__
#define __ARCH_PRCTL_H__

int sys_arch_prctl(int option, unsigned long *arg2, unsigned long *addr);
unsigned long getfs();

#endif

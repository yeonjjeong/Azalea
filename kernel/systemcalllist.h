#ifndef __SYSTEMCALLLIST_H__
#define __SYSTEMCALLLIST_H__

#include "syscall.h.in"



#define SYSCALL_sys_mmap        __NR_mmap
#define SYSCALL_sys_munmap      __NR_munmap


#define SYSCALL_sys_getpid              __NR_getpid
#define SYSCALL_sys_fork                __NR_fork
#define SYSCALL_sys_wait                __NR_wait4
#define SYSCALL_sys_execve              __NR_execve
#define SYSCALL_sys_getprio             __NR_getpriority
#define SYSCALL_sys_setprio             __NR_setpriority
#define SYSCALL_sys_exit                __NR_exit
#define SYSCALL_sys_read                __NR_read
#define SYSCALL_sys_readv               __NR_readv
#define SYSCALL_sys_write               __NR_write
#define SYSCALL_sys_writev              __NR_writev
#define SYSCALL_sys_open                __NR_open
#define SYSCALL_sys_close               __NR_close
#define SYSCALL_sys_clone               __NR_clone
#define SYSCALL_sys_lseek               __NR_lseek
#define SYSCALL_sys_creat               __NR_creat
#define SYSCALL_sys_unlink              __NR_unlink
#define SYSCALL_sys_stat                __NR_stat
#define SYSCALL_sys_fstat               __NR_fstat
#define SYSCALL_sys_brk                 __NR_brk
#define SYSCALL_sys_chdir               __NR_chdir
#define SYSCALL_sys_link                __NR_link
#define SYSCALL_sys_shmget              __NR_shmget
#define SYSCALL_sys_shmat               __NR_shmat
#define SYSCALL_sys_shmdt               __NR_shmdt
#define SYSCALL_sys_shmctl              __NR_shmctl
#define SYSCALL_sys_futex               __NR_futex
#define SYSCALL_sys_getdents            __NR_getdents
#define SYSCALL_sys_getdents64          __NR_getdents64

#define SYSCALL_create_thread      853

#define SYSCALL_get_vcon_addr      500
#define SYSCALL_get_cpu_num        501

#define SYSCALL_delay              503
#define SYSCALL_mdelay             504


#define SYSCALL_sys_sbrk                509 
#define SYSCALL_sys_msleep              512
#define SYSCALL_sys_sem_init            513
#define SYSCALL_sys_sem_destroy         514
#define SYSCALL_sys_sem_wait            515
#define SYSCALL_sys_sem_post            516
#define SYSCALL_sys_sem_timedwait       517
#define SYSCALL_sys_sem_cancelablewait  518
#define SYSCALL_sys_get_ticks           521
#define SYSCALL_sys_rcce_init           522
#define SYSCALL_sys_rcce_malloc         523
#define SYSCALL_sys_rcce_fini           524
#define SYSCALL_sys_yield               525
#define SYSCALL_sys_kill                526
#define SYSCALL_sys_signal              527
//#define SYSCALL_sys_gettimeofday        528
#define SYSCALL_sys_gettimeofday        __NR_gettimeofday


#define SYSCALL_do_exit                 550
#define SYSCALL_block_current_task      551
#define SYSCALL_reschedule              552
#define SYSCALL_wakeup_task             553
#define SYSCALL_numtest                 554
#define SYSCALL_sys_alloc               555
#define SYSCALL_sys_free                556

// Network offloading related systemcalls
#define SYSCALL_sys_gethostname         601
#define SYSCALL_sys_gethostbyname       602
#define SYSCALL_sys_getsockname         603
#define SYSCALL_sys_socket              604
#define SYSCALL_sys_bind                605
#define SYSCALL_sys_listen              606
#define SYSCALL_sys_connect             607
#define SYSCALL_sys_accept              608

#define SYSCALL_get_start_tsc           901
#define SYSCALL_get_freq                902
//#define SYSCALL_sys3_getcwd             903
#define SYSCALL_sys3_getcwd             79
#define SYSCALL_sys3_system             904
#define SYSCALL_sys3_opendir            905
#define SYSCALL_sys3_closedir           906
#define SYSCALL_sys3_readdir            907
#define SYSCALL_sys3_rewinddir          908

#define SYSCALL_print_log               854
#define SYSCALL_get_syscall_handler     855
#endif // __SYSTEMCALLLIST_H__

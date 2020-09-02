#include <sys/lock.h>
#include "systemcall.h"
#include "console.h"
#include "debug.h"
#include "descriptor.h"
#include "interrupthandler.h"
#include "map.h"
#include "memory_config.h"
#include "multiprocessor.h"
#include "page.h"
#include "systemcalllist.h"
#include "thread.h"
#include "az_types.h"
#include "utility.h"
#include "semaphore.h"
#include "delay.h"
#include "memory.h"
#include "sync.h"
#include "shellstorage.h"
#include "timer.h"
#include "offload_fio.h"
#include "offload_network.h"
#include "console_function.h"
#include "shm/shm.h"

#include "futex.h"

static spinlock_t a_lock;
/**
 * initialize system call
 */
void systemcall_init(void)
{
  QWORD rdx = 0;
  QWORD rax = 0;

  // IA32_STR MSR(0xC0000081)
  // Upper 32bit SYSRET CS/SS[63:48], SYSCALL CS/SS[47:32]
  rdx = ((GDT_KERNEL_CODE_SEGMENT | SELECTOR_RPL_0) << 16) | GDT_KERNEL_CODE_SEGMENT;
  rax = 0;
  write_msr(0xC0000081, rdx, rax);

  // IA32_LSTAR MSR (0xC0000082), systemcall_entrypoint
  rdx = ((QWORD) systemcall_entrypoint) >> 32;
  write_msr(0xC0000082, rdx, (QWORD) systemcall_entrypoint);

  // IA32_FMASK MSR (0xC0000084), 0x00 (nothing to change)
  write_msr(0xC0000084, 0, 0x200);

  read_msr(0xC0000080, &rdx, &rax);
  write_msr(0xC0000080, rdx, (rax | 0x1));

  spinlock_init(&a_lock); 
}

extern BYTE uthread[4096 * 10];

//#define DDBUG
#define YOFFSET 25

static int yyloc = 2;

QWORD process_systemcall(QWORD param1, QWORD param2, QWORD param3,
                         QWORD param4, QWORD param5, QWORD param6,
                         QWORD no)
{
  QWORD ret_code = 0;
#ifdef DDBUG
  tid_t tid = sys_getpid();
#endif
//  TCB *current = NULL;
//  int cid = get_apic_id();

  //lk_print_xy(0, yyloc++%YOFFSET, "system call ** %d", no);
  switch (no) {
  case SYSCALL_create_thread:
    ret_code = create_thread((QWORD) param1, (QWORD) param2, (int) param3);
    break;
  case SYSCALL_get_vcon_addr:
    ret_code = get_vcon_addr();
    break;
  case SYSCALL_get_cpu_num:
    ret_code = get_cpu_num();
    break;
  case SYSCALL_delay:
    ret_code = az_delay((QWORD) param1);
    break;
  case SYSCALL_mdelay:
    ret_code = az_mdelay((QWORD) param1);
    break;
  case SYSCALL_sys_getpid:
    ret_code = sys_getpid();
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "getpid(%d)(%q), ret_code: %q **", tid, yyloc, ret_code);
#endif
    break;
  case SYSCALL_sys_fork:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "fork system call **");
#endif
    break;
  case SYSCALL_sys_wait:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "wait system call **");
#endif
    break;
  case SYSCALL_sys_execve:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "execve system call **");
#endif
    break;
  case SYSCALL_sys_getprio:
    ret_code = sys_getprio((tid_t *) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "getprio system call, p1: %q, ret: %q **", param1, ret_code);
#endif
    break;
  case SYSCALL_sys_setprio:
    ret_code = sys_setprio((tid_t *) param1, (int) param2);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "setprio system call, p1: %q, p2: %q, ret: %q **", param1, param2, ret_code);
#endif
    break;
  case SYSCALL_sys_exit:
    sys_exit((int) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "exit system call, p1: %q, ret: %q **", param1, ret_code);
#endif
    break;
  case SYSCALL_sys_read:
    ret_code = sys_off_read((int) param1, (void *) param2, (size_t) param3);
#ifdef DDBUG
   lk_print_xy(0, yyloc++%YOFFSET, "read system call **");
#endif
    break;
  case SYSCALL_sys_write:
#if 1
    ret_code = sys_off_write((int) param1, (void *) param2, (size_t) param3);
#else
    if ( param1 == 1 || param1 == 2 ) { //stdout, stderr
      ret_code = cs_write((int) param1, (void *) param2, (size_t) param3);
    }
    else
    {
      ret_code = sys_off_write((int) param1, (void *) param2, (size_t) param3);
    }
#endif

#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "write system call **");
#endif
    break;
  case SYSCALL_sys_sbrk:
    ret_code = sys_sbrk((ssize_t) param1);
#ifdef DDBUG
//    lk_print_xy(0, yyloc%YOFFSET, "sbrk (%d)(%q), p1: %q, ret: %q **", tid, yyloc, param1, ret_code);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_open:
    ret_code = sys_off_open((const char *) param1, (int) param2, (mode_t) param3);
#ifdef DDBUG
    if (param3 == NULL)
      lk_print_xy(0, yyloc%YOFFSET, "open (%d)(%q) **, p1: %q, p2: %q         **", tid, yyloc, param1, param2);
    else
      lk_print_xy(0, yyloc%YOFFSET, "open (%d)(%q) **, p1: %q, p2: %q, p3: %q         **", tid, yyloc, param1, param2, param3);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_creat:
    ret_code = sys_off_creat((const char *) param1, (mode_t) param2);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "creat system call **");
#endif
    break;
  case SYSCALL_sys_close:
    ret_code = sys_off_close((int) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "close system call **");
#endif
    break;
  case SYSCALL_sys_msleep:
    sys_msleep((unsigned int) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "msleep system call, p1: %q **", param1);
#endif
    break;
  case SYSCALL_sys_sem_init:
    ret_code = sys_sem_init((sem_t **) param1, (unsigned int) param2);
#ifdef DDBUG
{
    QWORD **tmp = (sem_t **) param1;
    lk_print_xy(0, yyloc%YOFFSET, "sem_init (%d)(%q), p1: %q, p2: %q, ret: %q **", tid, yyloc, *tmp, param2, ret_code);
    yyloc++;
}
#endif
    break;
  case SYSCALL_sys_sem_destroy:
    ret_code = sys_sem_destroy((sem_t*) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "sem_destroy (%d)(%q), p1: %q, ret: %q **", tid, yyloc, param1, ret_code);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_sem_wait:
    ret_code = sys_sem_wait((sem_t*) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "sem_wait (%d)(%q), p1: %q, ret: %q **", tid, yyloc, param1, ret_code);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_sem_post:
    ret_code = sys_sem_post((sem_t*) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "sem_post (%d)(%q), p1: %q, ret: %q **", tid, yyloc, param1, ret_code);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_sem_timedwait:
    ret_code = sys_sem_timedwait((sem_t *) param1, (unsigned int) param2);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "sem_timedwait (%d)(%q), p1: %q, p2: %q **", tid, yyloc, param1, param2);
    yyloc++;
#endif
    break;
  case SYSCALL_sys_sem_cancelablewait:
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "sem_cancelablewait (%d)(%q), p1: %q, p2: %q **", tid, yyloc, param1, param2);
    yyloc++;
#endif
    ret_code = sys_sem_cancelablewait((sem_t *) param1, (unsigned int) param2);
    break;
  case SYSCALL_sys_clone:
    ret_code = sys_clone((unsigned long) param1, (void *)param2, (int *)param3, (int *)param4, (void *)param5, (void *)param6);
    if (ret_code < 0)
      debug_halt((char *) __func__, no);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "clone(%d)(%q), p1: %q, p2: %q, ret: %q **", tid, yyloc, param1, param2, ret_code);
#endif
    yyloc++;
    break;
  case SYSCALL_sys_lseek:
    ret_code = sys_off_lseek((int) param1, (off_t) param2, (int) param3);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "lseek system call **");
#endif
    break;
  case SYSCALL_sys_get_ticks:
    ret_code = sys_get_ticks();
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "get_ticks system call, ret: %q **", ret_code);
#endif
    break;
  case SYSCALL_sys_rcce_init:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "rcce_init system call **");
#endif
    break;
  case SYSCALL_sys_rcce_malloc:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "rcce_malloc system call **");
#endif
    break;
  case SYSCALL_sys_rcce_fini:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "rcce_fini system call **");
#endif
    break;
  case SYSCALL_sys_yield:
    sys_yield();
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "yield system call **");
#endif
    break;
  case SYSCALL_sys_kill:
    ret_code = sys_kill((tid_t) param1, (int) param2);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "kill system call, p1: %q, p2: %q, ret: %q **", param1, param2, ret_code);
#endif
    break;
  case SYSCALL_sys_signal:
    ret_code = sys_signal((signal_handler_t) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "signal system call, p1: %q **", param1);
#endif
    break;
  case SYSCALL_do_exit:
    do_exit((int) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc%YOFFSET, "do_exit(%d)(%q), p1: %q **", tid, yyloc, param1);
    yyloc++;
#endif
    break;
  case SYSCALL_block_current_task:
    block_current_task();
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "block_current_task system call **");
#endif
    break;
  case SYSCALL_reschedule:
    reschedule();
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "reschedule system call **");
#endif
    break;
  case SYSCALL_wakeup_task:
    ret_code = wakeup_task((tid_t) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "wakeup system call, p1: %q, ret: %q **", param1, ret_code);
#endif
    break;
  case SYSCALL_numtest:
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "numtest system call **");
#endif
    break;
  case SYSCALL_print_log:
    shell_enqueue((char*)param1) ;
    ret_code = 0 ;
    break ;
  case SYSCALL_sys_alloc:
    ret_code = (QWORD) az_alloc((QWORD) param1);
    break;
  case SYSCALL_sys_free:
    ret_code = az_free((void *) param1);
    break;
  case SYSCALL_get_start_tsc:
    ret_code = get_start_tsc();
    break;
  case SYSCALL_get_freq:
    ret_code = get_freq();
    break;
  case SYSCALL_sys_gettimeofday:
    ret_code = sys_gettimeofday((struct timeval *)param1, (void *)param2);
    break;
  case SYSCALL_sys_link:
    ret_code = sys_off_link((char *) param1, (char *) param2);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "link system call **");
#endif
    break;
  case SYSCALL_sys_unlink:
    ret_code = sys_off_unlink((char *) param1);
#ifdef DDBUG
    lk_print_xy(0, yyloc++%YOFFSET, "unlink system call **");
#endif
    break;
  case SYSCALL_sys_stat:
    ret_code = sys_off_stat((char *) param1, (struct stat *) param2);
    break;
  case SYSCALL_sys_brk:
    ret_code = sys_brk((ssize_t) param1);
    break;
  // Network related systemcalls
  case SYSCALL_sys_gethostname:
    ret_code = sys_off_gethostname((char *)param1, (size_t)param2);
    break;
  case SYSCALL_sys_gethostbyname:
    ret_code = (QWORD) sys_off_gethostbyname((char *)param1);
    break;
  case SYSCALL_sys_getsockname:
    ret_code = sys_off_getsockname((int)param1, (struct sockaddr *)param2, (socklen_t *)param3);
    break;
  case SYSCALL_sys_socket:
    ret_code = sys_off_socket((int)param1, (int)param2, (int)param3);
    break;
  case SYSCALL_sys_bind:
    ret_code = sys_off_bind((int)param1, (const struct sockaddr *)param2, (socklen_t)param3);
    break;
  case SYSCALL_sys_listen:
    ret_code = sys_off_listen((int)param1, (int)param2);
    break;
  case SYSCALL_sys_connect:
    ret_code = sys_off_connect((int)param1, (struct sockaddr *)param2, (socklen_t)param3);
    break;
  case SYSCALL_sys_accept:
    ret_code = sys_off_accept((int)param1, (struct sockaddr *)param2, (socklen_t *)param3);
    break;
  case SYSCALL_sys3_getcwd:
    ret_code = (QWORD) sys3_off_getcwd((char *)param1, (size_t)param2);
    break;
  case SYSCALL_sys3_system:
    ret_code = (QWORD) sys3_off_system((char *)param1);
    break;
  case SYSCALL_sys_chdir:
    ret_code = (QWORD) sys_off_chdir((char *)param1);
    break;
  case SYSCALL_sys3_opendir:
    ret_code = (QWORD) sys3_off_opendir((const char *)param1);
    break;
  case SYSCALL_sys3_closedir:
    ret_code = sys3_off_closedir((DIR *)param1);
    break;
  case SYSCALL_sys3_readdir:
    ret_code = (QWORD) sys3_off_readdir((DIR *)param1);
    break;
  case SYSCALL_sys3_rewinddir:
    sys3_off_rewinddir((DIR *)param1);
    break;
  case SYSCALL_sys_shmget:
    ret_code = (QWORD) shmget((key_t) param1, (size_t) param2, (int) param3);
    break;
  case SYSCALL_sys_shmat:
    ret_code = (QWORD) shmat((int) param1, (const void *) param2, (int) param3);
    break;
  case SYSCALL_sys_shmdt:
    ret_code = (QWORD) shmdt((const void *) param1);
    break;
  case SYSCALL_sys_shmctl:
    ret_code = (QWORD) shmctl((int) param1, (int) param2, (struct shmid_ds *) param3);
    break;

// musl
//#define DEBUG
  case __NR_arch_prctl:
    ret_code =  sys_arch_prctl((int) param1, (unsigned long *) param2, NULL);
#ifdef DEBUG
    cs_printf("arch_prctl(): code:%q, *addr:%q, tid:%d\n", param1, *(unsigned long *)param2, get_current()->id);
#endif
    break ;
  case __NR_clock_getres:
    ret_code = sys_clock_getres((clockid_t) param1, (struct timespec *) param2);
    break ;
  case __NR_clock_gettime:
    ret_code = sys_clock_gettime((clockid_t) param1, (struct timespec *) param2);
    break ;
  case __NR_exit_group:
    sys_exit_group((int) param1);
#ifdef DEBUG
    cs_printf("exit_group(%d)\n", param1);
#endif
    ret_code = 0 ;
    break ;
  case __NR_futex:
    ret_code = sys_futex((int *) param1, (int) param2, (int) param3, (const struct timespec *) param4, (int *)param5, (int) param6);
    break ;
  case __NR_getegid:
    ret_code = (QWORD) sys_getegid();
    break ;
  case __NR_geteuid:
    ret_code = (QWORD) sys_geteuid();
    break ;
  case __NR_getgid:
    ret_code = (QWORD) sys_getgid();
    break ;
  case __NR_getppid:
    ret_code = (QWORD) sys_getppid();
    break ;
  case __NR_getrlimit:
    ret_code = (QWORD)  sys_getrlimit((unsigned int) param1, (struct rlimit *) param2);
    break ;
  case __NR_get_robust_list:
    ret_code = (QWORD) sys_get_robust_list((int) param1, (void *) param2, (size_t *) param3);
    break ;
  case __NR_gettid:
    ret_code = (QWORD) sys_gettid();
    break ;
  case __NR_ioctl:
    ret_code = (QWORD) sys_ioctl((unsigned int) param1, (unsigned int) param2, (unsigned long) param3);
    break ;
  case __NR_madvise:
    ret_code = (QWORD) sys_madvise((unsigned long) param1, (size_t) param2, (int) param3);
    break ;
  case __NR_mincore:
    ret_code = (QWORD) sys_mincore((unsigned long) param1, (size_t) param2, (unsigned char *) param3);
    break ;
  case __NR_mmap:
    ret_code = (QWORD) sys_mmap((unsigned long) param1, (unsigned long) param2, (unsigned long) param3, (unsigned long) param4, (unsigned long) param5, (unsigned long) param6);
    break ;
  case __NR_mprotect:
    ret_code = (QWORD) sys_mprotect((size_t) param1, (size_t) param2, (unsigned long) param3);
    break ;
  case __NR_mremap:
    ret_code = (QWORD) sys_mremap((unsigned long) param1, (unsigned long) param2, (unsigned long) param3, (unsigned long) param4, (unsigned long) param5);
    break ;
  case __NR_munmap:
    ret_code = (QWORD) sys_munmap((size_t) param1, (size_t) param2);
    break ;
  case __NR_rt_sigaction:
    ret_code = (QWORD) sys_rt_sigaction((int) param1, (struct sigaction *) param2, (struct sigaction *) param3);
    break ;
  case __NR_setrlimit:
    ret_code = (QWORD) sys_setrlimit((int) param1, (const struct rlimit *) param2);
    break ;
  case __NR_set_robust_list:
    ret_code = (QWORD) sys_set_robust_list((void *) param1, (size_t) param2);
    break ;
  case __NR_setsid:
    ret_code = (QWORD) sys_setsid();
    break ;
  case __NR_set_tid_address:
    ret_code = (QWORD) sys_set_tid_address((int *) param1);
#ifdef DEBUG
    cs_printf("set_tid_address(): clear_child_tid:%q, tid:%d\n", param1, ret_code);
#endif
    break ;
  case __NR_uname:
    ret_code = (QWORD) sys_uname((struct utsname *) param1);
    break ;

  case __NR_readv:
    ret_code = sys_off_readv((int) param1, (void *) param2, (size_t) param3);
    break ;
  case __NR_writev:
    if ((int) param1 == 1 || (int) param1 == 2) { //stdout, stderr
      ret_code = cs_writev((int) param1, (void *) param2, (size_t) param3);
    }
    else
    {
      ret_code = sys_off_writev((int) param1, (void *) param2, (size_t) param3);
    }
    break ;
  case __NR_fstat:
    ret_code = sys_off_fstat((int) param1, (struct stat *) param2);
    break;
  case __NR_times:
    ret_code = cs_times((struct tms *)param1);
    break ;
  case __NR_fcntl:
    ret_code = 0;
    break ;
  case __NR_rt_sigprocmask:
    ret_code = 0;
    break ;
  case __NR_nanosleep:
    __asm volatile ("pause" ::: "memory");
    ret_code = 0;
    break ;
  case __NR_getdents:
    ret_code = (int) sys_getdents((unsigned int) param1, (struct dirent *) param2, (unsigned int) param3);
    break ;
  case __NR_getdents64:
    ret_code = (int) sys_getdents((unsigned int) param1, (struct dirent *) param2, (unsigned int) param3);
    break ;
  case __NR_sched_setscheduler:
    ret_code = 0;
    break ;

  case SYSCALL_get_syscall_handler:
    ret_code = (QWORD) process_systemcall ;
    break ;

  default:
    cs_printf("Invalid system calls: %d\n", no);
    printk("Invalid system calls");
    debug_halt((char *) __func__, no);
    break;
  }

  return ret_code;
}

#include <sys/lock.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>

#include "errno.h"
#include "console.h"
#include "thread.h"

#define ARCH_SET_GS		0x1001
#define ARCH_SET_FS		0x1002
#define ARCH_GET_FS		0x1003
#define ARCH_GET_GS		0x1004

#define ARCH_GET_CPUID		0x1011
#define ARCH_SET_CPUID		0x1012

#define ARCH_MAP_VDSO_X32	0x2001
#define ARCH_MAP_VDSO_32	0x2002
#define ARCH_MAP_VDSO_64	0x2003

// 64bit FS base
#define MSR_FS_BASE                             0xc0000100
// 64bit GS base
#define MSR_GS_BASE                             0xc0000101

/** @brief Read MSR
 *
 * The asm instruction rdmsr which stands for "Read from model specific register"
 * is used here.
 *
 * @param msr The parameter which rdmsr assumes in ECX
 * @return The value rdmsr put into EDX:EAX
 */
inline static uint64_t rdmsr(uint32_t msr) {
  uint32_t low, high;

  asm volatile ("rdmsr" : "=a" (low), "=d" (high) : "c" (msr));

  return ((uint64_t)high << 32) | low;
}

/** @brief Write a value to a  Machine-Specific Registers (MSR)
 *
 * The asm instruction wrmsr which stands for "Write to model specific register"
 * is used here.
 *
 * @param msr The MSR identifier
 * @param value Value, which will be store in the MSR
 */
inline static void wrmsr(uint32_t msr, uint64_t value)
{
  uint32_t low =  (uint32_t) value;
  uint32_t high = (uint32_t) (value >> 32);

  asm volatile("wrmsr" :: "c"(msr), "a"(low), "d"(high) : "memory");
}

static void default_writefs(size_t fs)
{
  wrmsr(MSR_FS_BASE, fs);
}

static size_t default_readfs(void)
{
  return rdmsr(MSR_FS_BASE);
}

static void default_writegs(size_t gs)
{
  wrmsr(MSR_GS_BASE, gs);
}

static size_t default_readgs(void)
{
  return rdmsr(MSR_GS_BASE);
}

typedef size_t (*func_read_fsgs)(void);
typedef void (*func_write_fsgs)(size_t);

func_read_fsgs readfs = default_readfs;
func_read_fsgs readgs = default_readgs;
func_write_fsgs writefs = default_writefs;
func_write_fsgs writegs = default_writegs;

/**
 * @brief set architecture-specific thread state
 * @param option - selects a subfunction
 * @param addr - addr is interpreted as either an unsigned long for the  "set"  operations,
 *               or  as  an unsigned long *, for the "get" operations
 * @return success (0), fail(-1)
 */

/**
 * arch_prctl is used for pthread to implement pthread_self() in musl/arch/x86_64/pthread_arch.h
 * the modified pthread_arch.h is as follow.
 *
 * static inline struct pthread *__pthread_self()
 * {
 *   unsigned long fs;
 *   unsigned long *addr;
 *
 *   addr = &fs;
 *   __syscall(SYS_arch_prctl, ARCH_GET_FS, addr);
 *   return((struct pthread *) *addr);
 * }
 */


int sys_arch_prctl(int option, unsigned long *addr) {

  switch(option) {
    case ARCH_SET_GS:
      writegs((uint64_t)addr);
      return 0;

  case ARCH_SET_FS:
     get_current()->tls = addr;
     writefs((uint64_t)addr);
   return 0;

  case ARCH_GET_GS: {
    unsigned long gs_val = readgs();
    *addr = gs_val;
    return 0;
  }

  case ARCH_GET_FS: {
    if(get_current()->tls) {
      *addr = (unsigned long) get_current()->tls;
       writefs((uint64_t)addr);
    }
    else {
      unsigned long fs_val = readfs();
      *addr = fs_val;
    }
    return 0;
  }

  case ARCH_GET_CPUID:
    lk_print("arch_prctl option GET_CPUID not implemented\n");
    return -ENOSYS;

  case ARCH_SET_CPUID:
    lk_print("arch_prctl option SET_CPUID not implemented\n");
    return -ENOSYS;

  case ARCH_MAP_VDSO_X32:
    lk_print("arch_prctl option MAP_VDSO_X32 not implemented\n");
    return -ENOSYS;

  case ARCH_MAP_VDSO_32:
    lk_print("arch_prctl option MAP_VDSO_32 not implemented\n");
    return -ENOSYS;

  case ARCH_MAP_VDSO_64:
    lk_print("arch_prctl option MAP_VDSO_64 not implemented\n");
    return -ENOSYS;

  default:
    lk_print("arch_prctl: unknown option 0x%x\n", option);
    return -EINVAL;
  }

  return -ENOSYS;
}


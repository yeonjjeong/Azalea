/*
 * Just an empty dummy handler to be POSIX compliant.
 */
#ifndef __SYS_UIO_H__
#define __SYS_UIO_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(iovec)
struct iovec {
  void  *iov_base;
  size_t iov_len;
};
#endif


#ifdef __cplusplus
}
#endif

#endif

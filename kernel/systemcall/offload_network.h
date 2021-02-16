#ifndef __OFFLOAD_NETWORK_H__
#define __OFFLOAD_NETWORK_H__

#include <sys/types.h>
#include <sys/socket.h>

// Functions
int sys_off_gethostname(char *name, size_t len);
struct hostent *sys_off_gethostbyname(char *name);
int sys_off_getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int sys_off_socket(int domain, int type, int protocol);
int sys_off_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int sys_off_listen(int sockfd, int backlog);
int sys_off_connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
int sys_off_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t sys_off_recv(int sockfd, void *buf, size_t len, int flags);
ssize_t sys_off_send(int sockfd, const void *buf, size_t len, int flags);

#endif /*__OFFLOAD_NETWORK_H__*/

#include "futex.h"

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<sys/syscall.h>
//#include<linux/futex.h>
//#include<linux/unistd.h>
#include<unistd.h>
//#include "syscallsmac.h"

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1
 
//_syscall6(int,futex,void *,addr1,int,op,int,val1,struct timespec*,timeout,void*,addr2,int,val3)
static int
       futex(int *uaddr, int futex_op, int val,
             const struct timespec *timeout, int *uaddr2, int val3)
       {
           return syscall(SYS_futex, uaddr, futex_op, val,
                          timeout, uaddr, val3);
       }

 
int m1=1;

 
void *threadfn1(void *p)
{
    puts("thread1 start\n");
    futex(&m1,FUTEX_WAIT,m1,NULL,NULL,0);
    puts("thread1 after lock\n");
    //sleep(2);
 
    return NULL;
}
 
void *threadfn2(void *p)
{
    //sleep(2);
    puts("thread2 before lock\n");
    futex(&m1,FUTEX_WAKE,1,NULL,NULL,0);
    return NULL;
}
 
int main()
{
    pthread_t t1,t2;
    puts("start futex test\n");
    //pthread_create(&t1,NULL,threadfn1,NULL);
    //pthread_create(&t2,NULL,threadfn2,NULL);
    create_thread((QWORD)threadfn1, 0, -1);

    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");

    create_thread((QWORD)threadfn2, 0, -1);
    sleep(15);
    puts("end test");
    return 0;
}

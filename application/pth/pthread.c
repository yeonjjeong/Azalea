#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
#define N_THREAD	10000

void t_function(void *data)
{
    int num = *((int *)data);
    int result = 0;
    printf("[app]num %d\n", num);
    
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");
    __asm volatile ("pause" ::: "memory");

    result = num * num;
    printf(" [app[%d]]pthread[%d] has run. result = %d.\n", num, num, result);
}

pthread_t p_thread[N_THREAD];
int a[N_THREAD];

int main()
{
    int thr_id;
    int result;
    int i;
 
    printf(" [app]pthread create start \n");

    for(i = 0; i < N_THREAD; i++) {
      a[i] = i;
      thr_id = pthread_create(&p_thread[i], NULL, (void *)t_function, (void *)&a[i]);

      printf(" [app[%d]]pthread created\n", i);

      if (thr_id < 0)
      {
          perror("thread create error : ");
          exit(0);
      }
    }

    printf(" [app]thread join start\n");
    for(i = 0; i < N_THREAD; i++) {
      pthread_join(p_thread[i], (void *)&result);
      printf(" [app[%d]]thread join : %d\n", i, result);
    }
    printf("\n [app]thread join finished\n");
 
    printf("\n [app]main() exit done\n");
 
    return 0;
}


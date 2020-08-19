#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
 
#define N_THREAD	2	

void *t_function(void *data)
{
    int num = *((int *)data);
    //printf("[app]num %d\n", num);
    sleep(5);
    num *= num;
    printf(" [app[%d]]pthread[%d] has run.\n", num, num);
    return (void *)(num);  // warning 발생
}
#if 1 
int main()
{
    pthread_t p_thread[N_THREAD];
    int thr_id[N_THREAD];
    int result[N_THREAD];
    int a[N_THREAD];
    int i;
 
    printf(" [app]pthread create start \n");
    for(i = 0; i < N_THREAD; i++) {
      a[i] = i;
      thr_id[i] = pthread_create(&p_thread[i], NULL, t_function, (void *)&a[i]);

      printf(" [app[%d]]pthread created\n", i);
      //thr_id = pthread_create(&p_thread2, NULL, t_function, (void *)&a);
      if (thr_id[i] < 0)
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
 
    printf(" ==> [app]main() exit done\n");
 
    return 0;
}

#else
int main()
{
    pthread_t p_thread;
    pthread_t p_thread2;
    int thr_id;
    int result;
    int a = 200;
 
    printf("[app]main start \n");
    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    printf("[app]pthread created\n");
    //thr_id = pthread_create(&p_thread2, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    printf("[app]thread join : pthread id: %lx\n", p_thread);
    pthread_join(p_thread, (void *)&result);
    //pthread_join(p_thread2, (void *)&result);
    printf("[app]thread join : %d\n", result);
 
    printf("[app]main() exit\n");
 
    return 0;
}
#endif

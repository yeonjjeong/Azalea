#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/mman.h>
#include <sys/wait.h>


int fn(void *arg)
{
   int i = 0;
   int n = atoi((char *)arg);

   printf("\nINFO: This code is running under child process.\n");

   for ( i = 1 ; i <= 10 ; i++ )
      printf("%d * %d = %d\n", n, i, (n*i));

   printf("\n");

   return 0;
}

void main()
{
   char str[10]; 
   void *pchild_stack = malloc(1024 * 1024);
   int pid;

   strcpy(str, "2");
   printf("Hello, World! str addr: %lx, str value :%s, stack addr:%lx\n", str, str, pchild_stack);

   if ( pchild_stack == NULL ) {
      printf("ERROR: Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
   }

   pid = clone(fn, pchild_stack + (1024 * 1024), SIGCHLD, str);

   if ( pid < 0 ) {
        printf("ERROR: Unable to create the child process.\n");
        exit(EXIT_FAILURE);
   }

   wait(NULL);

   free(pchild_stack);

   printf("INFO: Child process terminated.\n");
}

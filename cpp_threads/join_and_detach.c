/***************** The different of join and detach **********
* $ gcc -pedantic -Wall -o join_and_detach join_and_detach.c -pthread -lm
* Background:
* A thread has two status: joinable and detachable. When a thread is 
* created, one of its attributes defines whether it is joinable or 
* detached. Only threads that are created as joinable can be joined. 
* If a thread is created as detached, it can never be joined. When 
* thread::join() is called, the calling thread will block until 
* the thread of execution has completed. That is to say, the calling 
* thread will wait until the called thread terminates. 
* 
* In order to destroy a C++ thread object and free memory, either join() needs to be 
* called (and completed) or detach() must be called. When 
* thread::detach() is called, the called thread is "detached" from 
* the main thread and no longer represented by the "main" thread. 
* If the program needs to know when that thread of execution has 
* completed, some other mechanism needs to be used. When the detached
* thread completes, it destroies itself.
*
* Recommendations:
* 1. If a thread requires joining, consider explicitly creating it as
* joinable. This provides portability as not all implementations 
* create threads as joinable by default.
* 2. If you know in advance that a thread will never need to join with
* another thread, consider creating it in a detached state. Some system
* resources may be able to be freed.
* 
* DESCRIPTION:
*   This example demonstrates how to "wait" for thread completions by using
*   the Pthread join routine.  Threads are explicitly created in a joinable
*   state for portability reasons. Use of the pthread_exit status argument is 
*   also shown. Compare to detached.c
* AUTHOR: 8/98 Blaise Barney
* LAST REVISED:  01/30/09
*
* An output may like:
Main: creating thread 0
Main: creating thread 1
Main: creating thread 2
Main: creating thread 3
Thread 2 starting...
Thread 3 starting...
Thread 1 starting...
Thread 0 starting...
Thread 0 done. Result = -3.153838e+06
Thread 3 done. Result = -3.153838e+06
Main: completed join with thread 0 having a status of 0
Thread 1 done. Result = -3.153838e+06
Main: completed join with thread 1 having a status of 1
Thread 2 done. Result = -3.153838e+06
Main: completed join with thread 2 having a status of 2
Main: completed join with thread 3 having a status of 3
Main: program completed. Exiting.
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS 4

void *BusyWork(void *t)
{
   int i;
   long tid;
   double result=0.0;
   tid = (long)t;
   printf("Thread %ld starting...\n",tid);
   for (i=0; i<1000000; i++)
   {
      result = result + sin(i) * tan(i);
   }
   printf("Thread %ld done. Result = %e\n",tid, result);
   pthread_exit((void*) t);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NUM_THREADS];
   pthread_attr_t attr;
   int rc;
   long t;
   void *status;

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   // Since some implementations of pthreads may not create threads
   // in a joinable state, the threads here are explicitly created
   // in a joinable state so that they can be joined later
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0; t<NUM_THREADS; t++) {
      printf("Main: creating thread %ld\n", t);
      rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t); 
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
         }
      }

   /* Free attribute and wait for the other threads */
   pthread_attr_destroy(&attr);
   for(t=0; t<NUM_THREADS; t++) {
      rc = pthread_join(thread[t], &status);
      if (rc) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
         }
      printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
      }
 
printf("Main: program completed. Exiting.\n");
pthread_exit(NULL);
}


/******************************************************************************
* $ gcc -ptread convar.c
* ./a.out

* A result:
* Starting watch_count(): thread 1
* watch_count(): thread 1 Count= 0. Going into wait...
* inc_count(): thread 2, count = 1, unlocking mutex
* inc_count(): thread 3, count = 2, unlocking mutex
* inc_count(): thread 2, count = 3, unlocking mutex
* inc_count(): thread 3, count = 4, unlocking mutex
* inc_count(): thread 2, count = 5, unlocking mutex
* inc_count(): thread 3, count = 6, unlocking mutex
* inc_count(): thread 2, count = 7, unlocking mutex
* inc_count(): thread 3, count = 8, unlocking mutex
* inc_count(): thread 2, count = 9, unlocking mutex
* inc_count(): thread 3, count = 10, unlocking mutex
* inc_count(): thread 2, count = 11, unlocking mutex
* inc_count(): thread 3, count = 12  Threshold reached. Just sent signal.
* inc_count(): thread 3, count = 12, unlocking mutex
* watch_count(): thread 1 Condition signal received. Count= 12
* watch_count(): thread 1 Updating the value of count...
* watch_count(): thread 1 count now = 137.
* watch_count(): thread 1 Unlocking mutex.
* inc_count(): thread 3, count = 138, unlocking mutex
* inc_count(): thread 2, count = 139, unlocking mutex
* inc_count(): thread 2, count = 140, unlocking mutex
* inc_count(): thread 3, count = 141, unlocking mutex
* inc_count(): thread 2, count = 142, unlocking mutex
* inc_count(): thread 3, count = 143, unlocking mutex
* inc_count(): thread 3, count = 144, unlocking mutex
* inc_count(): thread 2, count = 145, unlocking mutex
* main(): Waited and joined with 3 threads. Final value of count = 145. Done.
*
* BACKGROUND:
* pthread_cond_wait() blocks the calling thread until the specified condition 
*   is signalled. This routine should be called while mutex is locked, and it 
*   will automatically release the mutex while it waits. After signal is 
*   received and thread is awakened, mutex will be automatically locked for 
*   use by the thread. The programmer is then responsible for unlocking 
*   mutex when the thread is finished with it.
* pthread_cond_signal() routine is used to signal (or wake up) another 
*   thread which is waiting on the condition variable. It should be called 
*   after mutex is locked, and must unlock mutex in order for 
*   pthread_cond_wait() routine to complete.
* It is a logical error to call pthread_cond_signal() before 
*   calling pthread_cond_wait().
*
* DESCRIPTION:
*   Example code for using Pthreads condition variables.  The main thread
*   creates three threads.  Two of those threads increment a "count" variable,
*   while the third thread watches the value of "count".  When "count" 
*   reaches a predefined limit, the waiting thread is signaled by one of the
*   incrementing threads. The waiting thread "awakens" and then modifies
*   count. The program continues until the incrementing threads reach
*   TCOUNT. The main program prints the final value of count.
* 
* Here is a boss/worker model, the boss is Watch_count(), and the worker is
* inc_count(). The workers are just woking, and when variable count reach to
* 12, an signal will be sent to boss to update count ( 12 + 125 = 137). When
* the boss works done, two works begin to work again.
*
* SOURCE: Adapted from example code in "Pthreads Programming", B. Nichols
*  et al. O'Reilly and Associates. 
* UPDATE : Kangjie Ling, Gaosuncn
* LAST REVISED: 03/27/19 
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) 
{
  int i;
  long my_id = (long)t;

  for (i=0; i < TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    count++;

    /* 
    Check the value of count and signal waiting thread when condition is
    reached.  Note that this occurs while mutex is locked. 
    */
    if (count == COUNT_LIMIT) {
      printf("inc_count(): thread %ld, count = %d  Threshold reached. ",
             my_id, count);
      pthread_cond_signal(&count_threshold_cv);
      printf("Just sent signal.\n");
      }
    printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", 
       my_id, count);
    pthread_mutex_unlock(&count_mutex);

    /* Do some work so threads can alternate on mutex lock */
    sleep(1);
    }
  pthread_exit(NULL);
}

void *watch_count(void *t) 
{
  long my_id = (long)t;

  printf("Starting watch_count(): thread %ld\n", my_id);

  /*
  Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
  will automatically and atomically unlock mutex while it waits. 
  Also, note that if COUNT_LIMIT is reached before this routine is run by
  the waiting thread, the loop will be skipped to prevent pthread_cond_wait
  from never returning.
  */
  pthread_mutex_lock(&count_mutex);
  while (count < COUNT_LIMIT) {
    printf("watch_count(): thread %ld Count= %d. Going into wait...\n", my_id,count);
    pthread_cond_wait(&count_threshold_cv, &count_mutex);
    printf("watch_count(): thread %ld Condition signal received. Count= %d\n", my_id,count);
    }
  printf("watch_count(): thread %ld Updating the value of count...\n", my_id);
  count += 125;
  printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
  printf("watch_count(): thread %ld Unlocking mutex.\n", my_id);
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  int i, rc; 
  long t1=1, t2=2, t3=3;
  pthread_t threads[3];
  pthread_attr_t attr;

  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

  /* For portability, explicitly create threads in a joinable state */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&threads[0], &attr, watch_count, (void *)t1);
  pthread_create(&threads[1], &attr, inc_count, (void *)t2);
  pthread_create(&threads[2], &attr, inc_count, (void *)t3);

  /* Wait for all threads to complete */
  for (i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n", 
          NUM_THREADS, count);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit (NULL);

}


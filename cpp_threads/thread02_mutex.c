/* $ gcc -pthread thread_mutex.c
 * 
 * 1. Mutexes can be applied only to threads in a single process, and do not 
 *    work between processes as do semaphores.
 * 2. When a mutex lock is attempted against a mutex which is held by another
 *    thread, the thread is blocked until the mutex is unlocked. When a thread 
 *    terminates, the mutex does not unless explicitly unlocked. Nothing happens 
 *    by default.
 * 3. For Mutexes, who locks the resource, who is responsible for it.
 * pthread_mutex_lock() - acquire a lock on the specified mutex variable. If 
 *    the mutex is already locked by another thread, this call will block the 
 *    calling thread until the mutex is unlocked.
 * pthread_mutex_unlock() - unlock a mutex variable. An error is returned if 
 *    mutex is already unlocked or owned by another thread.
 * pthread_mutex_trylock() - attempt to lock a mutex or will return error code 
 *    if busy. Useful for preventing deadlock conditions.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

void *functionC();
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

int main()
{
   int rc1, rc2, rc3;
   pthread_t thread1, thread2, thread3;

   /* Create independent threads each of which will execute functionC */

   // if thread created success, will return 0
   //pthread_create() - create a new thread
   if( (rc1=pthread_create( &thread1, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }

   if( (rc2=pthread_create( &thread2, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   if( (rc3=pthread_create( &thread3, NULL, &functionC, NULL)) )
   {
      printf("Thread creation failed: %d\n", rc3);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   //pthread_join() - wait for termination of another thread
   //A join is performed when one wants to wait for a thread to finish
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 
   pthread_join( thread3, NULL); 

   exit(EXIT_SUCCESS);
}

void *functionC()
{
    //pthread_self() - return identifier of current thread
    printf("Thread number %ld\n", pthread_self());
    pthread_mutex_lock( &mutex1 );
    counter++;
    printf("Counter value: %d\n",counter);
    //printf("before calling pthread_create getpid: %d getpthread_self: %lu tid:%lu\n",getpid(), pthread_self(), syscall(SYS_gettid));
    pthread_mutex_unlock( &mutex1 );
}

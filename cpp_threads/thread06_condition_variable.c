/* $ gcc -pthread condition_variables.c
 * 
 * 1. A condition variable is a variable of type pthread_cond_t and is used
 * with the appropriate functions for waiting and later. The condition variable
 * mechanism allows threads to suspend execution and relinquish the processor
 * until some condition is true. A condition variable must always be associated 
 * with a mutex to avoid a race condition created by one thread preparing to wait
 * and another thread which may signal the condition before the first thread 
 * actually waits on it resulting in a deadlock. The thread will be perpetually 
 * waiting for a signal that is never sent. Any mutex can be used, there is 
 * no explicit link between the mutex and the condition variable.
 * 2. The rule for condition variable: A wait without a condition can miss a 
 * wakeup or wake up simply to find that there is no work to do.
 * 3. functions:
 * Creating/Destroying:
 *     pthread_cond_init
 *     pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
 *     pthread_cond_destroy
 * Waiting on condition:
 *     pthread_cond_wait - unlocks the mutex and waits for the condition variable cond to be signaled.
 *     pthread_cond_timedwait - place limit on how long it will block.
 * Waking thread based on condition:
 *     pthread_cond_signal - restarts one of the threads that are waiting on the condition variable cond.
 *     pthread_cond_broadcast - wake up all threads blocked by the specified condition variable.
 *
 * Result maybe:
 * Counter value functionCount1: 1, Thread number 140652676863744
 * Counter value functionCount1: 2, Thread number 140652676863744
 * Counter value functionCount1: 3, Thread number 140652676863744
 * Counter value functionCount2: 4, Thread number 140652668471040
 * Counter value functionCount2: 5, Thread number 140652668471040
 * Counter value functionCount2: 6, Thread number 140652668471040
 * Counter value functionCount2: 7, Thread number 140652668471040
 * Counter value functionCount1: 8, Thread number 140652676863744
 * Counter value functionCount1: 9, Thread number 140652676863744
 * Counter value functionCount1: 10, Thread number 140652676863744
 * Final count: 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
int  count = 0;
#define COUNT_DONE  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6

int main()
{
   pthread_t thread1, thread2;

   pthread_create( &thread1, NULL, &functionCount1, NULL);
   pthread_create( &thread2, NULL, &functionCount2, NULL);

   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL);

   printf("Final count: %d\n",count);

   exit(EXIT_SUCCESS);
}

// Write numbers 1-3 and 8-10 as permitted by functionCount2()

void *functionCount1()
{
   for(;;)
   {
      // Lock mutex and then wait for signal to relase mutex
      //printf("Thread number %ld\n", pthread_self());
      pthread_mutex_lock( &count_mutex );

      // Wait while functionCount2() operates on count
      // mutex unlocked if condition varialbe in functionCount2() signaled.
      pthread_cond_wait( &condition_var, &count_mutex );
      count++;
      printf("Counter value functionCount1: %d, Thread number %ld\n",count,pthread_self());

      pthread_mutex_unlock( &count_mutex );

      if(count >= COUNT_DONE) return(NULL);
    }
}

// Write numbers 4-7

void *functionCount2()
{
    for(;;)
    {
       //printf("Thread number %ld\n", pthread_self());
       pthread_mutex_lock( &count_mutex );

       if( count < COUNT_HALT1 || count > COUNT_HALT2 )
       {
          // Condition of if statement has been met. 
          // Signal to free waiting thread by freeing the mutex.
          // Note: functionCount1() is now permitted to modify "count".
          pthread_cond_signal( &condition_var );
       }
       else
       {
          count++;
          printf("Counter value functionCount2: %d, Thread number %ld\n",count,pthread_self());
       }

       pthread_mutex_unlock( &count_mutex );

       if(count >= COUNT_DONE) return(NULL);
    }

}
//Note that functionCount1() was halted while count was between the values COUNT_HALT1 and COUNT_HALT2. The only thing that has been ensures is that functionCount2 will increment the count between the values COUNT_HALT1 and COUNT_HALT2. Everything else is random.

//The logic conditions (the "if" and "while" statements) must be chosen to insure that the "signal" is executed if the "wait" is ever processed. Poor software logic can also lead to a deadlock condition.

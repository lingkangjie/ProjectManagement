/******************************************************************************
* $ gcc -pthread mutex.c
* $ ./a.out
* Output:
* Thread 0 did 0 to 100000:  mysum=100000.000000 global sum=100000.000000
* Thread 2 did 200000 to 300000:  mysum=100000.000000 global sum=200000.000000
* Thread 1 did 100000 to 200000:  mysum=100000.000000 global sum=300000.000000
* Thread 3 did 300000 to 400000:  mysum=100000.000000 global sum=400000.000000
* Sum =  400000.000000
*
* BACKGROUND:
*    Mutex variables are one of the primary means of implementing thread 
*    synchronization and for protecting shared data when multiple writes occur.
*    The basic concept of a mutex as used in Pthreads is that only one thread can
*    lock (or own) a mutex variable at any given time. No other thread can own
*    that mutex until the owning thread unlocks that mutex.
*    A typical sequence in the use of a mutex is as follows:
*    - Create ans initialize a mutex variable
*    - Several threads attempt to lock the mutex
*    - Only one succeeds and that thread owns the mutex
*    - The owner thread performs some set of actions
*    - The owner unlocks the mutex
*    - Another thread acquires the mutex and repeates the process
*    - Finally the mutex is destroyed
* 
*    The pthread_mutex_lock() routine is used by a thread to acquire a lock on 
*    the specified mutex variable. If the mutex is already locked by another 
*    thread, this call will block the calling thread until the mutex is unlocked.
*    
*    pthread_mutex_trylock() will attempt to lock a mutex. However, if the 
*    mutex is already locked, the routine will return immediately with a "busy" 
*    error code. This routine may be useful in preventing deadlock conditions, 
*    as in a priority-inversion situation.
*    
*    pthread_mutex_unlock() will unlock a mutex if called by the owning thread. 
*    Calling this routine is required after a thread has completed its use of 
*    protected data if other threads are to acquire the mutex for their work 
*    with the protected data. An error will be returned if:
*    - If the mutex was already unlocked
*    - If the mutex is owned by another thread

* DESCRIPTION:
*    This is a simple serial program which computes the dot product of two 
*    vectors. The main data is made available to all threads through a 
*    globally accessible structure. Each thread works on a different part
*    of the data. The main thread waits for all the threads to complete their
*    computations, and then it prints the resulting sum.
* SOURCE: Vijay Sonnad, IBM
* UPDATE : Kangjie Ling, Gaosuncn
* LAST REVISED: 03/27/19 
******************************************************************************/
/*****************************************************************************
* FILE: dotprod_mutex.c
* DESCRIPTION:
*   This example program illustrates the use of mutex variables 
*   in a threads program. This version was obtained by modifying the
*   serial version of the program (dotprod_serial.c) which performs a 
*   dot product. The main data is made available to all threads through 
*   a globally accessible  structure. Each thread works on a different 
*   part of the data. The main thread waits for all the threads to complete 
*   their computations, and then it prints the resulting sum.
* SOURCE: Vijay Sonnad, IBM
* LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*   
The following structure contains the necessary information  
to allow the function "dotprod" to access its input data and 
place its output into the structure.  This structure is 
unchanged from the sequential version.
*/

typedef struct 
 {
   double      *a;
   double      *b;
   double     sum; 
   int     veclen; 
 } DOTDATA;

/* Define globally accessible variables and a mutex */

#define NUMTHRDS 4
#define VECLEN 100000
   DOTDATA dotstr; 
   pthread_t callThd[NUMTHRDS];
   pthread_mutex_t mutexsum;

/*
The function dotprod is activated when the thread is created.
As before, all input to this routine is obtained from a structure 
of type DOTDATA and all output from this function is written into
this structure. The benefit of this approach is apparent for the 
multi-threaded program: when a thread is created we pass a single
argument to the activated function - typically this argument
is a thread number. All  the other information required by the 
function is accessed from the globally accessible structure. 
*/

void *dotprod(void *arg)
{

/* Define and use local variables for convenience */

   int i, start, end, len ;
   long offset;
   double mysum, *x, *y;
   offset = (long)arg;
     
   len = dotstr.veclen;
   start = offset*len;
   end   = start + len;
   x = dotstr.a;
   y = dotstr.b;

/*
Perform the dot product and assign result
to the appropriate variable in the structure. 
*/
   mysum = 0;
   for (i=start; i<end ; i++) 
    {
      mysum += (x[i] * y[i]);
    }

/*
Lock a mutex prior to updating the value in the shared
structure, and unlock it upon updating.
*/
   pthread_mutex_lock (&mutexsum);
   dotstr.sum += mysum;
   printf("Thread %ld did %d to %d:  mysum=%f global sum=%f\n",offset,start,end,mysum,dotstr.sum);
   pthread_mutex_unlock (&mutexsum);

   pthread_exit((void*) 0);
}

/* 
The main program creates threads which do all the work and then 
print out result upon completion. Before creating the threads,
The input data is created. Since all threads update a shared structure, we
need a mutex for mutual exclusion. The main thread needs to wait for
all threads to complete, it waits for each one of the threads. We specify
a thread attribute value that allow the main thread to join with the
threads it creates. Note also that we free up handles  when they are
no longer needed.
*/

int main (int argc, char *argv[])
{
  long i;
  double *a, *b;
  void *status;
  pthread_attr_t attr;

  /* Assign storage and initialize values */

  a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
  b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
    
  for (i=0; i<VECLEN*NUMTHRDS; i++) {
    a[i]=1;
    b[i]=a[i];
    }

  dotstr.veclen = VECLEN; 
  dotstr.a = a; 
  dotstr.b = b; 
  dotstr.sum=0;

  pthread_mutex_init(&mutexsum, NULL);
           
  /* Create threads to perform the dotproduct  */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for(i=0;i<NUMTHRDS;i++)
    {
    /* Each thread works on a different set of data.
     * The offset is specified by 'i'. The size of
     * the data for each thread is indicated by VECLEN.
     */
     pthread_create(&callThd[i], &attr, dotprod, (void *)i); 
     }

  pthread_attr_destroy(&attr);
  /* Wait on the other threads */

  for(i=0;i<NUMTHRDS;i++) {
    pthread_join(callThd[i], &status);
    }
  /* After joining, print out the results and cleanup */

  printf ("Sum =  %f \n", dotstr.sum);
  free (a);
  free (b);
  pthread_mutex_destroy(&mutexsum);
  pthread_exit(NULL);
}   


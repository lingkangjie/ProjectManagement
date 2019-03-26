/******************************************************************************
* compile: $ gcc -Wall -pthread thread_creation_and_termination.c
* compile for debugging: $ gcc -Wall -pthread -ggdb thread_creation_and_termination.c
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
* DISCRIPTION:This simple example code creates 5 threads with the pthread_create() 
* routine. Each thread prints a "Hello World!" message, and then terminates 
* with a call to pthread_exit(). 
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

struct thread_data{
    int thread_id;
    char* message;
};
struct thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data*) threadarg;
    long tid;
    char* hello_msg;
    tid = (long)my_data->thread_id;
    hello_msg = my_data->message;
    printf("Hello World! It's me, thread #%ld!, message %s\n", tid,hello_msg);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    char* m[] = {"ABCDE","aa","bb","ccc","ddd"};
    for(t=0;t<NUM_THREADS;t++){
        printf("In main: creating thread %ld\n", t);
        thread_data_array[t].thread_id = t;
        thread_data_array[t].message = m[t];
        // all arguments must be passed by reference and cast to (void *)
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *) &thread_data_array[t]);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
     }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

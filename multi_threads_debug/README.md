## Debug tips and tools
1. install [gdb-dashboard](https://github.com/cyrus-and/gdb-dashboard)
2. using Tmux and gdb-dashboard to debug. Open a panel, `$ tty` to get panel address, 
then issue the command `dashboard source -output panel-address[e.g. /dev/pts/9]` to
redirect its `source` output to the newly created terminal.

## non-stop mode vs all-stop mode(default)
Non-stop mode means that when one thread in a program stops, other threads will continue running. In non-stop mode, gdb executes some commands asynchronously. When given an asynchronous, or background, command, gdb prompts you for the next command while the original command continues executing in the background. gdb will notify you when the background command finishes.  
In all-stop mode gdb executes all commands synchronously, meaning that each command must complete before you can enter the next command. All-stop mode means that when one thread in a program stops, all other threads will stop as well.  

## Debug flows
Turn on non-stop mode:  
```
(gdb) set pagination off
(gdb) set target-async on
(gdb) set non-stop on
```
basic commands:  
`(gdb) info threads` - Displays a list of threads
`(gdb) thread X` - Sets thread with ID of X to the selected thread
`(gdb) thread apply X Y` - Applies the command Y to a list of threads X. For example "thread apply 3 4 step"
`(gdb) thread apply all Y` - Applies the command Y to all active threads. For example "thread apply all step"

Core ideas:  
In `example_1`, the main thread will create 5 threads. When the child thread is created, the main thread lets
it goes.
```
... ...
20    for(t=0; t<NUM_THREADS; t++){
21       printf("In main: creating thread %ld\n", t);
22       rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
23       if (rc){
24          printf("ERROR; return code from pthread_create() is %d\n", rc);
25          exit(-1);
26       }
... ...
```
One episode may like:  
```
info threads
  Id   Target Id         Frame
* 1    Thread 0x7ffff7fc7740 (LWP 40265) "example_1" main (argc=1, argv=0x7fffffffe418) at example_1.c:23
  3    Thread 0x7ffff6dab700 (LWP 43550) "example_1" PrintHello (threadid=0x1) at example_1.c:10
  4    Thread 0x7ffff65aa700 (LWP 44246) "example_1" PrintHello (threadid=0x2) at example_1.c:10
  5    Thread 0x7ffff5da9700 (LWP 44347) "example_1" PrintHello (threadid=0x3) at example_1.c:10
  6    Thread 0x7ffff55a8700 (LWP 44546) "example_1" PrintHello (threadid=0x4) at example_1.c:10
```
Then for each thread one is interested in, can switch to the  particular thread `(gdb) thread thread-id`, 
to debug as usual.  
e.g `(gdb) thread apply 3 c`
Output:
```
─── Output/messages ───────────────────────────────────────────────────
Hello World! It's me, thread #1!
[Thread 0x7ffff6dab700 (LWP 43550) exited]
No unwaited-for children left.
─── Assembly ──────────────────────────────────────────────────────────
0x00005555555547ee main+94  mov    $0x0,%esi
0x00005555555547f3 main+99  callq  0x555555554600 <pthread_create@plt>
0x00005555555547f8 main+104 mov    %eax,-0x3c(%rbp)
0x00005555555547fb main+107 cmpl   $0x0,-0x3c(%rbp)
0x00005555555547ff main+111 je     0x555555554821 <main+145>
0x0000555555554801 main+113 mov    -0x3c(%rbp),%eax
0x0000555555554804 main+116 mov    %eax,%esi
─── Expressions ───────────────────────────────────────────────────────
─── History ───────────────────────────────────────────────────────────
─── Memory ────────────────────────────────────────────────────────────
─── Stack ─────────────────────────────────────────────────────────────
[0] from 0x00005555555547fb in main+107 at example_1.c:23
arg argc = 1
arg argv = 0x7fffffffe418
─── Threads ───────────────────────────────────────────────────────────
[6] id 44546 name example_1 from 0x0000555555554766 in PrintHello+12 at example_1.c:10
[5] id 44347 name example_1 from 0x0000555555554766 in PrintHello+12 at example_1.c:10
[4] id 44246 name example_1 from 0x0000555555554766 in PrintHello+12 at example_1.c:10
[1] id 40265 name example_1 from 0x00005555555547fb in main+107 at example_1.c:23
```


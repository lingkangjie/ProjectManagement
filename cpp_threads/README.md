## Thread attributes:
- Detached or joinable state
- Scheduling inheritance
- Scheduling policy
- Scheduling parameters
- Scheduling contention scope
- Stack size
- Stack address
- Stack guard (overflow) size

## Thread terminated scenes:
- The thread returns normally from its starting routine. Its work is done.
- The thread makes a call to the pthread_exit subroutine - whether its work is done or not.
- The thread is canceled by another thread via the pthread_cancel routine.
- The entire process is terminated due to making a call to either the exec() or exit()
- If main() finishes first, without calling pthread_exit explicitly itself

## Examples
Each example is self-contained, details and explanations please reference each file respectively.

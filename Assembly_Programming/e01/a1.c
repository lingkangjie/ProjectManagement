/*
 * $ gcc -Wall a1.c
 * 
 * Core Ideas:
 * 1. syntax for '__asm__': 
 *     __asm__ ( "assembly code here"
               : output operands                  // optional 
               : input operands                   // optional
               : list of clobbered registers      // optional 
               );
        If there are no output operands but there are input operands, 
        we must place two consecutive colons surrounding the place 
        where the output operands would go. The clobbered register 
        means GCC won't use this register to store any other value 
        except the value in current '__asm__' scope.
 * 2. Some implement Details:
    2.1. '"=a" (add)'  means variable add (which is in memory) referred to by register 'eax',
          and "=" means write-only mode. Shortcut for registers ('r' means any registers): 

        +---+--------------------+
        | r |    Register(s)     |
        +---+--------------------+
        | a |   %eax, %ax, %al   |
        | b |   %ebx, %bx, %bl   |
        | c |   %ecx, %cx, %cl   |
        | d |   %edx, %dx, %dl   |
        | S |   %esi, %si        |
        | D |   %edi, %di        |
        +---+--------------------+

    2.2. the different of '%%' and '%': '%%register', '%immediate number'
    2.3 If our assembly statement must execute where we put it, 
    (i.e. must not be moved out of a loop as an optimization), use
     "__volatile__" keyword.
);
 */

#include <stdio.h>

int main() {

    int arg1=8, arg2=4, add, sub, mul, quo, rem ;

    //printf( "Enter two integer numbers : " );
    //scanf( "%d%d", &arg1, &arg2 );

    /* Perform Addition, Subtraction, Multiplication & Division */
    __asm__ ( "addl %%ebx, %%eax;" : "=a" (add) : "a" (arg1) , "b" (arg2) );
    __asm__ ( "subl %%ebx, %%eax;" : "=a" (sub) : "a" (arg1) , "b" (arg2) );
    __asm__ ( "imull %%ebx, %%eax;" : "=a" (mul) : "a" (arg1) , "b" (arg2) );

    __asm__ __volatile__ ( "movl $0x0, %%edx;"
              "movl %2, %%eax;" // %2, the third register, here is 'g' register (arg1)
              "movl %3, %%ebx;" // %3, the fourth register, here is 'g' register (arg2)
               "idivl %%ebx;" : "=a" (quo), "=d" (rem) : "g" (arg1), "g" (arg2) );

    printf( "%d + %d = %d\n", arg1, arg2, add );
    printf( "%d - %d = %d\n", arg1, arg2, sub );
    printf( "%d * %d = %d\n", arg1, arg2, mul );
    printf( "%d / %d = %d\n", arg1, arg2, quo );
    printf( "%d %% %d = %d\n", arg1, arg2, rem );

    return 0 ;
}

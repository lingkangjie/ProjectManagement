// An example program to perform simple floating point arithmetic:
#include <stdio.h>

int main() {

    float arg1, arg2, add, sub, mul, div ;

    printf( "Enter two numbers : " );
    scanf( "%f%f", &arg1, &arg2 );

    /* Perform floating point Addition, Subtraction, Multiplication & Division */
    // for GCC compiler, you must choost use "m" register instead of "g" (genenral
    // register) for performing floating point calculation.
    __asm__ ( "fld %1;"
              "fld %2;"
              "fadd;"
              "fstp %0;" : "=m" (add) : "m" (arg1), "m" (arg2) ) ;

    __asm__ ( "fld %2;"
              "fld %1;"
              "fsub;"
              "fstp %0;" : "=m" (sub) : "m" (arg1), "m" (arg2) ) ;

    __asm__ ( "fld %1;"
              "fld %2;"
              "fmul;"
              "fstp %0;" : "=m" (mul) : "m" (arg1), "m" (arg2) ) ;

    __asm__ ( "fld %2;"
              "fld %1;"
              "fdiv;"
              "fstp %0;" : "=m" (div) : "m" (arg1), "m" (arg2) ) ;

    printf( "%f + %f = %f\n", arg1, arg2, add );
    printf( "%f - %f = %f\n", arg1, arg2, sub );
    printf( "%f * %f = %f\n", arg1, arg2, mul );
    printf( "%f / %f = %f\n", arg1, arg2, div );

    return 0 ;
}

## Assembly Programming
Windows platform: [book: Modeern X86 Assembly Language Programming](https://doc.lagout.org/programmation/asm/Modern%20X86%20Assembly%20Language%20Programming_%2032-bit%2C%2064-bit%2C%20SSE%2C%20and%20AVX%20%5BKusswurm%202014-11-25%5D.pdf)
Linux platform: [x86-64 Assembly Language Programming with Ubuntu](http://www.egr.unlv.edu/~ed/assembly64.pdf)
## Instructions
Using `lscpu` to check cpu information. Install `NASM` assembler:  
```
$ sudo apt-get install nasm
```
Compile:  
```
nasm -f elf64 program.asm && gcc program.o && ./a.out
```


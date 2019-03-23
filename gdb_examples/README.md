# gdb commands

## compile source files
1.# gdb commands

## compile source files
1. Compile it with and without debug symbols

```console
$ gcc Add_Sub.c -o Add_Sub_NO_DEBUG
```

2. Compile it with and with debug symbols

3. run the program
```console
$ gcc Add_Sub.c -ggdb -o Add_Sub_DEBUG
```
## gdb debug

- Run the program output with DEBUG symbols inside GDB
```console
$ gdb ./Add_Sub_DEBUG
```
- Run the program output without DEBUG symbols inside GDB
```console
$ gdb ./Add_Sub_NO_DEBUG
```
- another way to run program
```console
$ gdb
$(gdb) run executable_file
```
4. gdb commands
- List source code: list
'list 1': means list 1-th to 10-th lines. If the previous command is 'list nums', the next command is '<Enter>' will list next 10 lines.

- List all the functions in the binary. For debug and no-debug version, the functions will be different.
```console
$ (gdb) info functions
```

- Identify the source file of the binary
```console
$ info sources
```
- List variables, will only list global and static variables.
```console
$ info variables
```

- List local variables:
```console
$ info scope scope-names
```
e.g., list local variables in main function: 
```console
$ info scope main
```
If we want to check there are how many and which scope we interested in, using <TAB> to list: $ info scope <TAB>

- rip debug symbols off a binary
Method 1: The debug symbols have just been copied, they have not been stripped from the binary. As in some binaries, the debug symbols are bigger than the binary executable themself, it is convenient for us to strip debug symbols from binary.
```console
objcopy --only-keep-debug Add_Sub_DEBUG DEBUG_symbols
```

Method 2: enter the gdb deubg environment, using
(gdb) maint print symbols DEBUG_symbols_2
(gdb) shell ls
Add_Sub.c  Add_Sub_DEBUG  Add_Sub_NO_DEBUG  DEBUG_symbols  DEBUG_symbols_2
(gdb)

- Stripping symbols off a binary
$ strip --strip-debug binary_file
$ (gdb) info functions
Still some variables can be seen.
In order to further remove them use --strip-unneeded option
$ strip --strip-debug --strip-unneeded Add_Sub_DEBUG_stripped

- Inspecting Symbols witm NM
$ nm ./executable_file

output e.g.: 
......
0804a034 B IamAGlobalVariable
......

- Grep for function names
$ nm ./executable_file | grep function_name
$ nm ./executable_file | grep 'B'  

- In sorted order
$ nm -n ./executable_file

- List external symbols
$ nm -g ./executable_file

- Display size
$ nm -S ./executable_file

- Trace system calls
Using strace to traces all system calls a program does, don't even need source code.
$ strace ./executable_file argv1(for executable_file)  argv2(for executable_file) ...
e.g $ strace ./Add_Sub_change 20 30

- check registers, break points, menmory
```console
$ (gdb) info registers
$ (gdb) break function_name
$ (gdb) info breakpoints
$ (gdb) disable/enable/delete break_points_num
$ (gdb) help x
$ (gdb) print variables
$ (gdb) x/ls argv[0]
$ (gdb) disassemble main ( Dump of assembler code for function main)
$ (gdb) x/i 0x80484f5
$ (gdb) x/10i 0x80484f5
(gdb) x/10xw $esp
0xbffff650:     0xb7fc13c4      0xb7fff000      0x0804851b      0xb7fc1000
0xbffff660:     0x08048510      0x00000000      0x00000000      0xb7e2fa83
0xbffff670:     0x00000002      0xbffff704
(gdb) x/20xw $esp
0xbffff650:     0xb7fc13c4      0xb7fff000      0x0804851b      0xb7fc1000
0xbffff660:     0x08048510      0x00000000      0x00000000      0xb7e2fa83
0xbffff670:     0x00000002      0xbffff704      0xbffff710      0xb7feccea
0xbffff680:     0x00000002      0xbffff704      0xbffff6a4      0x0804a01c
0xbffff690:     0x0804823c      0xb7fc1000      0x00000000      0x00000000
(gdb) x/1xw argv
0xbffff704:     0xbffff83f
(gdb) break *0x08048505
(gdb) continue
(gdb) stepi
(gdb) step
(gdb) help step
Step program until it reaches a different source line.
Usage: step [N]
Argument N means step N times (or till program stops for another reason).
(gdb) help stepi
Step one instruction exactly.
Usage: stepi [N]
Argument N means step N times (or till program stops for another reason).
```
- Modifying registers and memory
```console
$ gdb ./executable_file
$ (gdb) run param1 parma2 ... e.g. run AAAA 20 30
(gdb) print argv[1]
$1 = 0xbffff870 "AAAA"
(gdb) print argv[2]
$2 = 0xbffff875 "20"
(gdb) print argv[3]
$3 = 0xbffff878 "30"
(gdb) x/x argv[1]
0xbffff870:     0x41414141
(gdb) x/s argv[1]
0xbffff870:     "AAAA"
(gdb) x/s argv[2]
0xbffff875:     "20"
(gdb) x/s argv[3]
0xbffff878:     "30"
(gdb) x/5c argv[1]
0xbffff870:     65 'A'  65 'A'  65 'A'  65 'A'  0 '\000'
(gdb) set {char} 0xbffff870 = 'B'
(gdb) x/5c argv[1]
0xbffff870:     66 'B'  65 'A'  65 'A'  65 'A'  0 '\000'
(gdb) set {char} 0xbffff870 = 67
(gdb) x/5c argv[1]
0xbffff870:     67 'C'  65 'A'  65 'A'  65 'A'  0 '\000'
(gdb) set {int} 0xbffff870 = 67
(gdb) x/5c argv[1]
0xbffff870:     67 'C'  0 '\000'        0 '\000'        0 '\000'        0 '\000'
(gdb) set {int} 0xbffff870 = 0
(gdb) x/5c argv[1]
0xbffff870:     0 '\000'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
(gdb) set {int} 0xbffff870 = 1
(gdb) x/5c argv[1]
0xbffff870:     1 '\001'        0 '\000'        0 '\000'        0 '\000'        0 '\000'
(gdb) set {char} 0xbffff870 = 'B'
(gdb) set {char} (0xbffff870 + 1) = 'B'
(gdb) set {char} (0xbffff870 + 2) = 'B'
(gdb) set {char} (0xbffff870 + 3) = 'B'
(gdb) x/5c argv[1]
0xbffff870:     66 'B'  66 'B'  66 'B'  66 'B'  0 '\000'
(gdb) c
Continuing.

(gdb) set $eax = 10
(gdb) info registers
(gdb) set $eip = 0x80484fd
```

- Calling function in gdb
```console
$ gdb ./executable_file
(gdb) break main
(gdb) run arg1 arg2 ... # e.g run AAA 10 20
(gdb) n
(gdb) c
(gdb) print argv[1] # will output 'AAA'
(gdb) set $demo = "BBBB" # set a variables
(gdb) set argv[1] = $demo # set argv[1] to a user defined variable
(gdb) set $dyn = (char \*) malloc(10) # malloc 10 bytes
(gdb) print $dyn
(gdb) x/10xb $syn # print 10 bytes in binary format of $dyn
(gdb) call strcpy($dyn,argv[1])
(gdb) x/10xb $dyn
(gdb) x/10xc $dyn # now variable $dyn will be "BBBB"
(gdb) info functions  
All defined functions:
......
File Program_noexecfun_fix.c:
int AddNumbers(int, int);
void FunctionShouldNotExecure(void);
......

(gdb) call AddNumbers(10,20) # user-defined function

```


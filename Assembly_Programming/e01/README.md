# inline assembly in C++
## AT&T Assembly Syntax
Since GCC compiler uses AT&T assembly syntex, this is a little bit different
from regular Intel Format:  
* AT&T syntax uses the opposite order for source and destination operands,
          source followed by destination.
* Register operands are preceded by the "%" character, including sections.
* Immediate operands are preceded by the "$" character.
* The size of memory operands are specified using the last character of
   the opcode. These are "b" (8-bit), "w" (16-bit), and "l" (32-bit).
Examples:
```
     (AT&T syntax)                   (INTEL syntax)

    movw %bx, %ax                     (mov ax, bx)
    xorl %eax, %eax                   (xor eax, eax)
    movw $1, %ax                      (mov ax,1)
    movb X, %ah                       (mov ah, byte ptr X)
    movw X, %ax                       (mov ax, word ptr X)
    movl X, %eax                      (mov eax, X)
    movl 4(%ebp), %eax                (mov eax, [ebp+4])
    addl (%eax,%eax,4), %ecx          (add ecx, [eax + eax*4])
    movb $4, %fs:(%eax)               (mov fs:eax, 4)
    movl _array(,%eax,4), %eax        (mov eax, [4*eax + array])
    movw _array(%ebx,%eax,4), %cx     (mov cx, [ebx + 4*eax + array])
```

# Profile for ELF file
ELF: Executable Linkable Format
PE: Protable Executable
COFF: Common File Format
DLL: Dynamic Linking Library
SLL: Static Linking Library
The file structrue is defined in `/usr/include/elf.h`.  
.strtab: String Table
.shstrtab: Section Header String Table
```
lkj@lkj:~/tmp/ob$ readelf -h SimpleSection.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          1104 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         13
  Section header string table index: 12

00000000: 7f45 4c46 02(ELF64)01(little endian) 0100 0000 0000 0000 0000  .ELF............
00000010: 0100(e_type) 3e00(e_machine) 0100 0000(->e_version) 0000 0000 0000 0000(->e_entry)  ..>.............
00000020: 0000 0000 0000 0000(->e_phoff) 5004 0000 0000 0000(->e_shoff)  ........P.......
00000030: 0000 0000(->e_flags) 4000(->e_ehsize) 0000 0000 4000(->e_shentsize) 0d00 0c00  ....@.....@.....
... ...
```
ps: in e_shoff, 00 00 00 00 00 00 04 50(HEX) = 1104 (DEC), 'Start of section headers)  
ps: in e_ehsize, 00 04(HEX) = 64(DEC), 'Size of this header'   
ps: in e_shentsize, 00 04(HEX) = 64(DEC), 'Size of section headers'  
ps: in e_shnum, 00 0d(HEX) = 13(DEC), 'Number of section headers'  
ps: in e_shstrndx, 00 0c(HEX) = 12(DEC), 'Section header string table index'  
```
typedef struct
{
  unsigned char e_ident[16]; /* Magic number and other info */
  Elf64_Half    e_type;         /* 2 bytes, Object file type */
  Elf64_Half    e_machine;      /* 2 bytes, Architecture */
  Elf64_Word    e_version;      /* 4 bytes, Object file version */
  Elf64_Addr    e_entry;        /* 8 bytes, Entry point virtual address */
  Elf64_Off e_phoff;        /* 8 bytes, Program header table file offset */
  Elf64_Off e_shoff;        /* 8 bytes, Section header table file offset */
  Elf64_Word    e_flags;        /* 4 bytes, Processor-specific flags */
  Elf64_Half    e_ehsize;       /* ELF header size in bytes */
  Elf64_Half    e_phentsize;        /* Program header table entry size */
  Elf64_Half    e_phnum;        /* Program header table entry count */
  Elf64_Half    e_shentsize;        /* Section header table entry size */
  Elf64_Half    e_shnum;        /* Section header table entry count */
  Elf64_Half    e_shstrndx;     /* Section header string table index */
} Elf64_Ehdr;

typedef struct
{
  Elf64_Word    sh_name;        /* Section name (string tbl index) */
  Elf64_Word    sh_type;        /* Section type */
  Elf64_Xword   sh_flags;       /* Section flags */
  Elf64_Addr    sh_addr;        /* Section virtual addr at execution */
  Elf64_Off sh_offset;      /* Section file offset */
  Elf64_Xword   sh_size;        /* Section size in bytes */
  Elf64_Word    sh_link;        /* Link to another section */
  Elf64_Word    sh_info;        /* Additional section information */
  Elf64_Xword   sh_addralign;       /* Section alignment */
  Elf64_Xword   sh_entsize;     /* Entry size if section holds table */
} Elf64_Shdr;
Elf64_Shar is section header, total has 4+4 +8 +8 +8 +8 +4+4 +8+8=64

```

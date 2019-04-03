## discovering the host processor instruction set
`config.h` file will be generated from `config.h.in`file, and is
included in `processor-info.cpp` file.  
config.h.in:  
```
#pragma once

#define NUMBER_OF_LOGICAL_CORES   @_NUMBER_OF_LOGICAL_CORES@
#define NUMBER_OF_PHYSICAL_CORES  @_NUMBER_OF_PHYSICAL_CORES@
#define TOTAL_VIRTUAL_MEMORY      @_TOTAL_VIRTUAL_MEMORY@
... ...
```
config.h:  
```
#pragma once

#define NUMBER_OF_LOGICAL_CORES   2
#define NUMBER_OF_PHYSICAL_CORES  2
#define TOTAL_VIRTUAL_MEMORY      2047
```

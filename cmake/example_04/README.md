## detecting CPU architecture
Output:  
```
lkj@lkj:~/ProjectManagement/cmake/example_04$ mkdir build
lkj@lkj:~/ProjectManagement/cmake/example_04$ cd build/
lkj@lkj:~/ProjectManagement/cmake/example_04/build$ cmake ..
-- The CXX compiler identification is GNU 5.4.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Target is 64 bits
-- x86_64 architecture detected
-- Configuring done
-- Generating done
-- Build files have been written to: /home/lkj/ProjectManagement/cmake/example_04/build
lkj@lkj:~/ProjectManagement/cmake/example_04/build$ cmake --build .
Scanning dependencies of target arch-dependent
[ 50%] Building CXX object CMakeFiles/arch-dependent.dir/arch-dependent.cpp.o
[100%] Linking CXX executable arch-dependent
[100%] Built target arch-dependent
lkj@lkj:~/ProjectManagement/cmake/example_04/build$ ./arch-dependent
x86_64 architecture. Compiled on a 64 bit host processor.

```

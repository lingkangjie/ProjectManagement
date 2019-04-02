## Output compiler information
```
message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
  message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
  message(STATUS "Is the C++ from GNU? ${CMAKE_COMPILER_IS_GNUCXX}")
  message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

message(STATUS "Is the C compiler loaded? ${CMAKE_C_COMPILER_LOADED}")
if(CMAKE_C_COMPILER_LOADED)
  message(STATUS "The C compiler ID is: ${CMAKE_C_COMPILER_ID}")
  message(STATUS "Is the C from GNU? ${CMAKE_COMPILER_IS_GNUCC}")
  message(STATUS "The C compiler version is: ${CMAKE_C_COMPILER_VERSION}")
endif()
```

## Example of using `option` and 'cmake_dependent_option'
```
$ cmake -D USE_LIBRARY=OFF -D MAKE_STATIC_LIBRARY=ON ..
```
This will not build a library, since `USE_LIBRARY` is still set to OFF.

If `USE_LIBRARY` is set to `ON`:  
```
lkj@lkj:~/ProjectManagement/cmake/example_03/build$ cmake -D USE_LIBRARY=ON ..
-- The CXX compiler identification is GNU 5.4.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Is the C++ compiler loaded? 1
-- The C++ compiler ID is: GNU
-- Is the C++ from GNU? 1
-- The C++ compiler version is: 5.4.0
-- Is the C compiler loaded?
-- Compile sources into a library? ON
-- Compile sources into a STATIC library? OFF
-- Compile sources into a SHARED library? ON
-- Configuring done
-- Generating done
-- Build files have been written to: /home/lkj/ProjectManagement/cmake/example_03/build
```

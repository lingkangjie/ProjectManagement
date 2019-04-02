## command for build
```
$ cd /ROOT ## cd to root path of the project
$ mkdir -p build ## -p parent
$ cd build
$ cmake ..
$ cmake --build . 
```
Use `--VERBOSE=1` to see building details, especially in complex building process  
```
$ cmake --build . --VERBOSE=1
```

## the files Cmake generates
1. *Makefile*: The set of instructions that `make` will run to build the project.
2. *CMakeFiles*: directory which contains temporary files, used by CMake for
detecting the operating system, compiler, and so on.
3. *cmake_install.cmake*: a Cmake script handling install rules.
4. *CMakeCache.txt*: used by Cmake when re-running the configuration.

## build options
```
$ cmake --build . --target help 
```
will give you more options for building

## switching to another generators
```
$ cmake --help
```
will give you an aspect of generators Cmake supports:
```
The following generators are available on this platform:
  Unix Makefiles               = Generates standard UNIX makefiles.
  Ninja                        = Generates build.ninja files.
  Watcom WMake                 = Generates Watcom WMake makefiles.
... ...
```
Using `-G` option to switch a generator, e.g. 
```
$ cmake -G Ninja ..
```

# CMake tutorials
## Example 1
Adding a generated file and generator from CMake.  
Files structures:
```
├── build1.cmake
├── CMakeLists.txt
├── directions.txt
├── License.txt
├── MathFunctions
│   ├── CMakeLists.txt
│   ├── MakeTable.cxx
│   ├── MathFunctions.h
│   └── mysqrt.cxx
├── README.md
├── TutorialConfig.h.in
└── tutorial.cxx
```
After builded, a file called `Table.h` will be produced by CMake from 
`MakeTable.cxx`. Then, `mysqrt.cxx` can use this file.  
From: [CMake tutorials from official](https://cmake.org/cmake-tutorial/)


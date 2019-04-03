# find Python

# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(recipe-01 LANGUAGES NONE)

# detect python

# CMake module for finding python, named Find<PythonInterp>.cmake
# REQUIRED: CMmake will abort configuration if no suitable executable
# for the Python interpreter is found in the usual lookup locations
find_package(PythonInterp REQUIRED)

# Execute a tiny Python script

# PYTHON_EXECUTABLE: the path to the executable for the Python interpreter
execute_process(
  COMMAND
    ${PYTHON_EXECUTABLE} "-c" "print('Hello, world!')"
  RESULT_VARIABLE _status
  OUTPUT_VARIABLE _hello_world
  ERROR_QUIET
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# output:
# -- RESULT_VARIABLE is: 0
# -- OUTPUT_VARIABLE is: Hello, world!
message(STATUS "RESULT_VARIABLE is: ${_status}")
message(STATUS "OUTPUT_VARIABLE is: ${_hello_world}")

# compare the "manual" messages with the following handy helper
include(CMakePrintHelpers)

# output: 
# -- _status="0" ; _hello_world="Hello, world!"
cmake_print_variables(_status _hello_world)

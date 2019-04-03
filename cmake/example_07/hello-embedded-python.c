/*
 * This code samples will initialize an instance of Python interpreter
 * within the program and print the date using the 'time' Python module
 *
 */
#include <Python.h>

int main(int argc, char *argv[]) {
  Py_SetProgramName(argv[0]); /* optional but recommended */
  Py_Initialize();
  PyRun_SimpleString("from time import time,ctime\n"
                     "print('Today is', ctime(time()))\n");
  Py_Finalize();
  return 0;
}

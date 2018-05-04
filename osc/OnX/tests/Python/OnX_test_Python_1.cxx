
#include <Python.h>

//////////////////////////////////////////////////////////////////////////////
int main (
 int
,char**
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : OnX_test_Python_1 : 005\n");

  Py_Initialize();
  PyEval_InitThreads();

  PyRun_SimpleString((char*)"import OnX_exa_loop");
  //PyRun_SimpleString((char*)"OnX_exa_loop.start()");
  PyRun_SimpleString((char*)"OnX_exa_loop.start_timer()");

  //PyGILState_STATE state = PyGILState_Ensure();
  Py_BEGIN_ALLOW_THREADS

  while(true) {}

  Py_END_ALLOW_THREADS
  //PyGILState_Release(state);

  //while(true) {
  //  PyRun_SimpleString((char*)""); //Else the thread never execute !
  //}

  return 0;
}

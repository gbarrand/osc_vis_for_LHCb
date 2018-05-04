
#include <string> // KCC : else Python.h does not compile !

//#define WITH_THREAD
#define USE_DL_IMPORT

#include <Python.h>

#include <Lib/smanip.h>

#include <stdio.h>
#define READBUFSIZE 32768  //16384  //65536;
//////////////////////////////////////////////////////////////////////////////
int main (
 int
,char**
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // O'Reilly p 578 :
  Py_Initialize();

  /*
  PyRun_SimpleString((char*)"import string");
  PyRun_SimpleString((char*)"print string.uppercase");
  PyRun_SimpleString((char*)"x = string.uppercase");
  PyRun_SimpleString((char*)"print string.lower(x)");

  PyRun_SimpleString((char*)"import OnX_Hello");

  PyRun_SimpleString((char*)"import OnX");
  PyRun_SimpleString((char*)"from OnX import *");
  PyRun_SimpleString((char*)"OnX_SWIG_Hello()");
  PyRun_SimpleString((char*)"import math");
  PyRun_SimpleString((char*)"from math import cos");
  PyRun_SimpleString((char*)"print cos(0)");
  */

  PyObject* fNewStdout = PyFile_FromString("OnX_out", "wb+");
  if(!fNewStdout) return 0;
  PyFile_SetBufSize(fNewStdout,100);

  FILE* fFile = PyFile_AsFile(fNewStdout);
  if(!fFile) return 0;

  printf("debug : 000 fNewStdout %ld %ld\n",fFile,fNewStdout);

  //PySys_SetObject("stdout",fNewStdout);

  printf("debug : 002\n");
  //PyObject* outf = PySys_GetObject("stdout");
  PyObject* outf = fNewStdout;
  printf("debug : 003 %ld\n",outf);
  _PyObject_Dump(outf); 
  //PyObject_Print(outf,stdout,0); 

  if(!outf) return 0;

  printf("debug : 004\n");
  Lib::smanip smanip;
  for(int i=0;i<100;i++) {
    std::string s;
    smanip.printf(s,32,"line %d\n",i);
    PyFile_WriteString(s.c_str(),outf);
  }
  printf("debug : 004+\n");

  while(1) {
    PyObject* line = PyFile_GetLine(outf,1);
    if(line) printf("\"%s\"\n",PyString_AsString(line));
    else break;
    //_PyObject_Dump(line); 
  }
  /*
  */

  /*
  //{if (PyFile_Check(outf)) {
    printf("debug : 005\n");
    FILE *fp = PyFile_AsFile(outf);
    printf("debug : 006-- %ld\n",fp);
    Py_BEGIN_ALLOW_THREADS
    flockfile(fp);
    //::fputs("test2 \n",fp);
    ::fprintf(fp,"test2 \n");
    funlockfile(fp);
    Py_END_ALLOW_THREADS
    printf("debug : 006+\n");
    //}}
  */

  Py_Finalize();

  return 0;
}


/* trash 

  {printf("debug : +--000\n");
  PyObject* outf = PySys_GetObject("stdout");
  printf("debug : --001\n");
  if (outf == NULL) {
    return 0;
  }
  printf("debug : --002\n");
  PyFile_WriteString("test \n", outf);
  printf("debug : --003\n");}
  
  FILE* fFile = ::fopen("OnX_Python.out","wb+");
  if(!fFile) return 0;
  PyObject* fNewStdout = PyFile_FromFile(fFile, "OnX_out", "w", NULL);
  if(!fNewStdout) return 0;
  Py_INCREF(fNewStdout);

  //PyObject* fNewStdout = PyFile_FromString("OnX_out", "wb+");

 {PyThreadState* tstate = PyThreadState_Get();
  PyObject* sd = tstate->interp->sysdict;
  printf("debug : 001 %d\n",PyDict_SetItemString(sd, "stdout",fNewStdout));}

  PyObject* outf = 0;
 {PyThreadState *tstate = PyThreadState_Get();
  PyObject *sd = tstate->interp->sysdict;
  if (!sd) return 0;
  outf = PyDict_GetItemString(sd, "stdout");}

  FILE* file = ::fopen("OnX_out","rb+");
  if(!file) return 0;

  printf("debug : 007+\n");
#define READBUFSIZE 32768  //16384  //65536;
  char buf[READBUFSIZE];
  int len;
  ::fflush(file);
  ::fseek(file,0L,SEEK_SET);
  do {
    len = ::fread(buf,1,READBUFSIZE-1,file);
    printf("debug : 100 %d\n",len);
    if(len<=0) break;
    buf[len] = '\0';
    printf("debug : buf \"%s\"\n",buf);
  } while (len > 0);
  
  printf("debug : 008\n");
  PyRun_SimpleString("print 'Hello'");
  printf("debug : 009\n");

  if(file) ::fclose(file);
*/



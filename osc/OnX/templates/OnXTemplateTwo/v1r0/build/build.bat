@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --incs`) DO SET onx_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --py_incs`) DO SET onx_py_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --libs`) DO SET onx_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --py_libs`) DO SET onx_py_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I. /I.. 
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Hello.obj /Tp..\source\Hello.cxx

cl.exe /c %cppflags% %onx_incs% %onx_py_incs% /Fo%bin%PythonSWIG.obj /Tp..\source\Python\PythonSWIG.cxx

REM # Build the archive lib :
lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateTwo_SWIG_Python.arc %bin%Hello.obj %bin%PythonSWIG.obj

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateTwo_SWIG_Python.arc
%bin%win32def.exe OnXTemplateTwo_SWIG_Python < %bin%tmp > %bin%OnXTemplateTwo_SWIG_Python.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateTwo_SWIG_Python.def /out:%bin%OnXTemplateTwo_SWIG_Python.lib
IF EXIST %bin%OnXTemplateTwo_SWIG_Python.dll DEL %bin%OnXTemplateTwo_SWIG_Python.dll
link.exe /nologo /dll /out:%bin%OnXTemplateTwo_SWIG_Python.dll %bin%OnXTemplateTwo_SWIG_Python.exp %bin%OnXTemplateTwo_SWIG_Python.arc %onx_py_libs% %onx_libs%

move %bin%OnXTemplateTwo_SWIG_Python.dll %bin%OnXTemplateTwo_SWIG_Python.pyd

del %bin%tmp
del %bin%Hello.obj
del %bin%PythonSWIG.obj

del %bin%win32def.obj
del %bin%OnXTemplateTwo_SWIG_Python.arc
del %bin%OnXTemplateTwo_SWIG_Python.def
del %bin%OnXTemplateTwo_SWIG_Python.exp

ENDLOCAL
@ECHO ON

@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --incs`) DO SET onx_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --libs`) DO SET onx_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I. /I..
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Callbacks.obj /Tp..\source\Callbacks.cxx

REM # Build the archive lib :
lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateFive.arc %bin%Callbacks.obj

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateFive.arc
%bin%win32def.exe OnXTemplateFive < %bin%tmp > %bin%OnXTemplateFive.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateFive.def /out:%bin%OnXTemplateFive.lib
IF EXIST %bin%OnXTemplateFive.dll DEL %bin%OnXTemplateFive.dll
link.exe /nologo /dll /out:%bin%OnXTemplateFive.dll %bin%OnXTemplateFive.exp %bin%OnXTemplateFive.arc %onx_libs%

del %bin%tmp
del %bin%Callbacks.obj

del %bin%win32def.obj
del %bin%OnXTemplateFive.arc
del %bin%OnXTemplateFive.def
del %bin%OnXTemplateFive.exp


ENDLOCAL
@ECHO ON

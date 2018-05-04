@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --incs`) DO SET onx_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --libs`) DO SET onx_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I .
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Callbacks.obj /Tp..\source\Callbacks.cxx

REM # Build the archive lib :
lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateOne.arc %bin%Callbacks.obj

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateOne.arc
%bin%win32def.exe OnXTemplateOne < %bin%tmp > %bin%OnXTemplateOne.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateOne.def /out:%bin%OnXTemplateOne.lib
IF EXIST %bin%OnXTemplateOne.dll DEL %bin%OnXTemplateOne.dll
link.exe /nologo /dll /out:%bin%OnXTemplateOne.dll %bin%OnXTemplateOne.exp %bin%OnXTemplateOne.arc %onx_libs%

del %bin%tmp
del %bin%Callbacks.obj
del %bin%win32def.obj
del %bin%OnXTemplateOne.arc
del %bin%OnXTemplateOne.def
del %bin%OnXTemplateOne.exp

ENDLOCAL
@ECHO ON

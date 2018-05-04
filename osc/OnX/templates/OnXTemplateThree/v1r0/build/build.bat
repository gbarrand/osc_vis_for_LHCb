@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --gl_incs`) DO SET onx_gl_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --gl_libs`) DO SET onx_gl_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I .
cl.exe /c %cppflags% %onx_gl_incs% /Fo%bin%Callbacks.obj /Tp..\source\Callbacks.cxx

REM # Build the archive lib :
lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateThree.arc %bin%Callbacks.obj

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateThree.arc
%bin%win32def.exe OnXTemplateThree < %bin%tmp > %bin%OnXTemplateThree.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateThree.def /out:%bin%OnXTemplateThree.lib
IF EXIST %bin%OnXTemplateThree.dll DEL %bin%OnXTemplateThree.dll
link.exe /nologo /dll /out:%bin%OnXTemplateThree.dll %bin%OnXTemplateThree.exp %bin%OnXTemplateThree.arc %onx_gl_libs%

del %bin%tmp
del %bin%Callbacks.obj

del %bin%win32def.obj
del %bin%OnXTemplateThree.arc
del %bin%OnXTemplateThree.def
del %bin%OnXTemplateThree.exp


ENDLOCAL
@ECHO ON

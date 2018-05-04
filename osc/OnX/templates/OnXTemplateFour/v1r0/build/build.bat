@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_cppflags`) DO SET onx_iv_cppflags=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_libs`) DO SET onx_iv_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I .
cl.exe /c %cppflags% %onx_iv_cppflags% /Fo%bin%Callbacks.obj /Tp..\source\Callbacks.cxx

REM # Build the archive lib :
lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateFour.arc %bin%Callbacks.obj

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateFour.arc
%bin%win32def.exe OnXTemplateFour < %bin%tmp > %bin%OnXTemplateFour.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateFour.def /out:%bin%OnXTemplateFour.lib
IF EXIST %bin%OnXTemplateFour.dll DEL %bin%OnXTemplateFour.dll
link.exe /nologo /dll /out:%bin%OnXTemplateFour.dll %bin%OnXTemplateFour.exp %bin%OnXTemplateFour.arc %onx_iv_libs%

del %bin%tmp
del %bin%Callbacks.obj

del %bin%win32def.obj
del %bin%OnXTemplateFour.arc
del %bin%OnXTemplateFour.def
del %bin%OnXTemplateFour.exp


ENDLOCAL
@ECHO ON

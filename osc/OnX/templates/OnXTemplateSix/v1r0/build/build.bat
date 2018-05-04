@ECHO OFF
SETLOCAL

SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`osc-config --incs`) DO SET onx_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --libs`) DO SET onx_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_cppflags`) DO SET onx_iv_cppflags=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_libs`) DO SET onx_iv_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --core_libs`) DO SET onx_core_libs=%%s

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I. /I..

REM #//////////////////////////////////////////////////
REM #// library UserData :
REM #//////////////////////////////////////////////////
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Track.obj /Tp..\source\User\Track.cxx
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Event.obj /Tp..\source\User\Event.cxx
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Processor.obj /Tp..\source\User\Processor.cxx
cl.exe /c %cppflags% %onx_incs% /Fo%bin%PrintProcessor.obj /Tp..\source\User\PrintProcessor.cxx
cl.exe /c %cppflags% %onx_incs% /Fo%bin%Framework.obj /Tp..\source\User\Framework.cxx

SET UserData_objs=%bin%\Track.obj %bin%\Event.obj %bin%\Processor.obj %bin%\PrintProcessor.obj %bin%\Framework.obj

REM #//////////////////////////////////////////////////
REM #// library OnXTemplateSixCore :
REM #//////////////////////////////////////////////////
cl.exe /c %cppflags% %onx_incs% /Fo%bin%FrameworkManager.obj /Tp..\source\FrameworkManager.cxx
cl.exe /c %cppflags% %onx_incs% %onx_iv_cppflags% /Fo%bin%Track_Accessor.obj /Tp..\source\Track_Accessor.cxx

SET OnXTemplateSixCore_objs=%bin%\FrameworkManager.obj %bin%\Track_Accessor.obj

REM #//////////////////////////////////////////////////
REM #// dll OnXTemplateSix :
REM #//////////////////////////////////////////////////
cl.exe /c %cppflags% %onx_incs% %onx_iv_cppflags% /Fo%bin%Callbacks.obj /Tp..\source\Callbacks.cxx

REM # Build the archive lib :
SET objs=%bin%\Callbacks.obj %OnXTemplateSixCore_objs% %UserData_objs%

SET libs=
SET libs=%libs% %onx_iv_libs%
SET libs=%libs% %onx_libs%

lib.exe /nologo /machine:ix86 /out:%bin%OnXTemplateSix.arc %objs%

REM # Build the win32def.exe utility program :
cl.exe %cppflags% /Fe%bin%win32def.exe /Tpwin32def.c

REM # Building the .def file :
dumpbin /out:%bin%tmp /symbols %bin%OnXTemplateSix.arc
%bin%win32def.exe OnXTemplateSix < %bin%tmp > %bin%OnXTemplateSix.def

REM # Build the DLL :
lib.exe /nologo /machine:ix86 /def:%bin%OnXTemplateSix.def /out:%bin%OnXTemplateSix.lib
IF EXIST %bin%OnXTemplateSix.dll DEL %bin%OnXTemplateSix.dll
link.exe /nologo /dll /out:%bin%OnXTemplateSix.dll %bin%OnXTemplateSix.exp %bin%OnXTemplateSix.arc %libs%

REM #//////////////////////////////////////////////////
REM #// Application in which the user framework
REM #// is the master of the main loop.
REM #//////////////////////////////////////////////////

REM # application OnXTemplateSix_vis :
cl.exe /c %cppflags% %onx_incs% %onx_iv_cppflags% /Fo%bin%OnXTemplateSix_vis.obj /Tp..\applications\OnXTemplateSix_vis.cxx
cl.exe /c %cppflags% %onx_incs% %onx_iv_cppflags% /Fo%bin%VisProcessor.obj /Tp..\source\VisProcessor.cxx

SET objs=%bin%\OnXTemplateSix_vis.obj %OnXTemplateSixCore_objs% %bin%\VisProcessor.obj %UserData_objs%
SET libs=%onx_iv_libs%
SET libs=%libs% %onx_core_libs%
SET libs=%libs% %onx_libs%

IF EXIST %bin%OnXTemplateSix_vis.exe DEL %bin%OnXTemplateSix_vis.exe
link.exe /nologo /out:%bin%OnXTemplateSix_vis.exe %objs% %libs%

del %bin%tmp
del %objs%

del %bin%win32def.obj
del %bin%OnXTemplateSix.arc
del %bin%OnXTemplateSix.def
del %bin%OnXTemplateSix.exp

ENDLOCAL
@ECHO ON

@ECHO OFF
SETLOCAL

IF DEFINED ROOTSYS GOTO build_end_if_0
ECHO Environment variable ROOTSYS not defined.
ECHO You have to "call thisroot.bat" CERN-ROOT.
GOTO build_return
:build_end_if_0

IF DEFINED AIDA_HOME_DIR GOTO build_end_if_1
ECHO Environment variable AIDA_HOME_DIR not defined.
ECHO You have to "call aida-setup.bat" your AIDA implementation.
GOTO build_return
:build_end_if_1

REM //////////////////////////////////////////////////////////////////
REM /// options //////////////////////////////////////////////////////
REM //////////////////////////////////////////////////////////////////
SET bin=.\

FOR /F "usebackq delims=;" %%s IN (`aida-config --incs`) DO SET AIDA_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`aida-config --libs`) DO SET AIDA_libs=%%s

REM # no root-config.exe around (with 5.27.02),
REM # then we do "by hand" from ROOTSYS :
SET ROOT_cflags=/I%ROOTSYS%\include
SET ROOT_libs=/LIBPATH:%ROOTSYS%\lib libCore.lib libCint.lib libRIO.lib libNet.lib libHist.lib libGraf.lib libGraf3d.lib libGpad.lib libTree.lib libRint.lib libPostscript.lib libMatrix.lib libPhysics.lib libMathCore.lib libThread.lib libfreetype.lib
SET ROOT_bin_dir=%ROOTSYS%\bin

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
REM # /W3 warmings
SET cppflags=/nologo /DWIN32 /MD /GX /GR /O2 /W3 /I .

SET ROOT_rootcint=%ROOT_bin_dir%\rootcint

REM //////////////////////////////////////////////////////////////////
REM /// Generating A2R CINT wrapping : ///////////////////////////////
REM //////////////////////////////////////////////////////////////////

REM # Have to convert "/I" to "-I" in AIDA_incs :
CL.exe %cppflags% /Fe%bin%obuild_subs.exe obuild_subs.cpp
FOR /F "usebackq delims=;" %%s IN (`.\obuild_subs.exe %AIDA_incs% /I -I`) DO SET AIDA_incs_unix=%%s
REM ECHO %AIDA_incs_unix%

REM SET AIDA_incs_unix=-IC:\usr\local\osc_batch\16.11\Resources\AIDA\src\cpp
%ROOT_rootcint% -f G__A2R.cpp -c %AIDA_incs_unix% A2R_Def.h A2R_LinkDef.h 

REM # there is a hell of warnings :
REM # G__A2R.cpp(9740) : warning C4800: 'long': valeur forcee a la valeur...
REM # we set /wd4800 to remove them.

CL.EXE /c /wd4800 %cppflags% %ROOT_cflags% %AIDA_incs% /Fo%bin%G__A2R.obj /Tp.\G__A2R.cpp

CL.EXE /c %cppflags% %ROOT_cflags% %AIDA_incs% /Fo%bin%A2R_Imp.obj /Tp.\A2R_Imp.cpp

REM //////////////////////////////////////////////////////////////////
REM /// Building the A2R lib : ///////////////////////////////////////
REM //////////////////////////////////////////////////////////////////

REM # Build the archive lib :
LIB.EXE /nologo /machine:ix86 /out:%bin%A2R.arc %bin%G__A2R.obj %bin%A2R_Imp.obj

REM //////////////////////////////////////////////////////////////////
REM /// Building the A2R dll : ///////////////////////////////////////
REM //////////////////////////////////////////////////////////////////

LINK.EXE /nologo /dll /out:%bin%A2R.dll %bin%G__A2R.obj %bin%A2R_Imp.obj %ROOT_libs% %AIDA_libs%

DEL %bin%G__A2R.obj
DEL G__A2R.cpp
DEL G__A2R.h
DEL obuild_subs.exe
DEL obuild_subs.obj

:build_return

ENDLOCAL
@ECHO ON


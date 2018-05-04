@ECHO OFF
SETLOCAL

IF DEFINED ROOTSYS GOTO build_end_if_0
ECHO Environment variable ROOTSYS not defined.
ECHO You have to "call thisroot.bat" CERN-ROOT.
GOTO build_return
:build_end_if_0

IF DEFINED AIDA_HOME_DIR GOTO build_end_if_1
ECHO Environment variable AIDA_HOME_DIR not defined.
ECHO You have to "call setup" your AIDA implementation.
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
REM /// compiling and linking : //////////////////////////////////////
REM //////////////////////////////////////////////////////////////////

CL.EXE /c %cppflags% /wd4800 %ROOT_cflags% %AIDA_incs% /Fo%bin%A2R_read_main.obj /Tp.\A2R_read_main.cpp

LINK.EXE /nologo /OPT:NOREF /out:%bin%A2R_read.exe %bin%A2R_read_main.obj %bin%A2R.arc %ROOT_libs% %AIDA_libs%

DEL %bin%A2R_read_main.obj
DEL %bin%A2R_read.exp
DEL %bin%A2R_read.lib

:build_return

ENDLOCAL
@ECHO ON


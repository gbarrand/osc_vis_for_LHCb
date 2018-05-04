@ECHO off
SETLOCAL

SET script_app=oroot

SET script_drive=%~d0
SET script_path=%~p0

REM # ////////////////////////////////////////////////////////////////
REM # /// Check VisualC++ ////////////////////////////////////////////
REM # ////////////////////////////////////////////////////////////////
SET script_vsvars="C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools"\vsvars32.bat
IF EXIST %script_vsvars% GOTO else_if_vsvars
  ECHO %script_vsvars% not found.
  ECHO Can't set VisualC++ environment.
  GOTO script_return
  GOTO end_if_vsvars
:else_if_vsvars
  CALL %script_vsvars%
  @ECHO off 
:end_if_vsvars

REM # Check that VisualC++ environment is available :
CL.exe 1> NUL 2> NUL
IF NOT ERRORLEVEL 1 GOTO end_if_CL
  ECHO CL.exe program not found.
  ECHO You have to modify desktop/oroot.bat
  ECHO to setup VisualC++ environment.
  GOTO script_return
:end_if_CL

REM # ////////////////////////////////////////////////////////////////
REM # /// Check HOME /////////////////////////////////////////////////
REM # ////////////////////////////////////////////////////////////////

REM #
REM # As the program may produce output files, we start from a 
REM #    %script_app%
REM # directory created in the home directory.
REM #

IF DEFINED HOMEDRIVE GOTO end_if_HOMEDRIVE
  ECHO Environment variable HOMEDRIVE not defined.
  GOTO script_return
:end_if_HOMEDRIVE

IF DEFINED HOMEPATH GOTO end_if_HOMEPATH
  ECHO Environment variable HOMEPATH not defined.
  GOTO script_return
:end_if_HOMEPATH

SET script_home=%HOMEDRIVE%%HOMEPATH%

IF EXIST %script_home% GOTO end_if_home
  ECHO Directory %script_home% does not exist.
  GOTO script_return
:end_if_home

FOR /F "usebackq delims=;" %%d IN (`CD`) DO SET pwd_path=%%d

CD /D %script_home%

IF NOT EXIST OpenScientist MKDIR OpenScientist
CD OpenScientist
IF NOT EXIST log MKDIR log
CD log

REM # Do not rm, since another run may use it.
REM IF EXIST %script_app% RMDIR /S /Q %script_app%

IF NOT EXIST %script_app% MKDIR %script_app%
CD %script_app%

REM # ////////////////////////////////////////////////////////////////
REM # /// run ////////////////////////////////////////////////////////
REM # ////////////////////////////////////////////////////////////////

CALL %script_drive%%script_path%\..\setup.bat
@ECHO off 

%script_app%.exe

CD /D %pwd_path%

:script_return
ENDLOCAL
@ECHO on

@ECHO OFF
SETLOCAL

REM # We use wget.exe, unzip.exe (taken from CYGWIN). 
REM # We have put a copy of these (as long as the needed cyg*.dll) 
REM # under C:\usr\bin. Thes programs can be downloaded from :
REM #   http://OpenScientist.lal.in2p3.fr/download/foreign/CYGWIN
REM # Done in our login.
REM # SET PATH=%PATH%;C:\usr\bin

SET osc_wget_run_wget_exe=wget.exe
SET osc_wget_run_unzip_exe=unzip.exe

REM # //////////////////////////////////////////////////////////////////
REM # // args //////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

SET osc_wget_run_kit=
SET osc_wget_run_version=
SET osc_wget_run_platform=
SET osc_wget_run_application=
SET osc_wget_run_clean=yes

:next_arg
IF "%1%" == "" GOTO end_args

IF NOT "%1%" == "-h" GOTO end_if_h
  ECHO Arguments :
  ECHO  -kit : [osc_batch,osc_vis,...].
  ECHO  -version : version to run.
  ECHO
  ECHO Options :
  ECHO  -no_clean : do not remove the build area when finished.
  ECHO
  ECHO Examples :
  ECHO   osc_wget_run -kit osc_batch -version v16r7 -application AIDA_example_Store
  GOTO osc_wget_run_return
:end_if_h

IF NOT "%1%" == "-kit" GOTO end_if_kit
  SHIFT
  SET osc_wget_run_kit=%1%
  SHIFT
  GOTO next_arg
:end_if_kit

IF NOT "%1%" == "-version" GOTO end_if_version
  SHIFT
  SET osc_wget_run_version=%1%
  SHIFT
  GOTO next_arg
:end_if_version

IF NOT "%1%" == "-no_clean" GOTO end_if_no_clean
  SET osc_wget_run_clean=no
  SHIFT
  GOTO next_arg
:end_if_no_clean

IF NOT "%1%" == "-platform" GOTO end_if_platform
  SHIFT
  SET osc_wget_run_platform=%1%
  SHIFT
  GOTO next_arg
:end_if_platform

IF NOT "%1%" == "-application" GOTO end_if_application
  SHIFT
  SET osc_wget_run_application=%1%
  SHIFT
  GOTO next_arg
:end_if_application

:end_args

REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

IF NOT "%osc_wget_run_kit%" == "" GOTO end_if_check_kit
  ECHO No target given
  GOTO osc_wget_run_return
:end_if_check_kit

IF NOT "%osc_wget_run_version%" == "" GOTO end_if_check_version
  ECHO No version given
  GOTO osc_wget_run_return
:end_if_check_version

IF NOT "%osc_wget_run_application%" == "" GOTO end_if_check_application
  ECHO No application given
  GOTO osc_wget_run_return
:end_if_check_application

IF NOT "%osc_wget_run_platform%" == "" GOTO end_if_check_platform
  SET osc_wget_run_platform=Windows_NT-ix86-cl_1310
:end_if_check_platform

SET osc_wget_run_kit_file=%osc_wget_run_kit%-%osc_wget_run_version%-%osc_wget_run_platform%.zip
SET osc_wget_download_area=http://OpenScientist.lal.in2p3.fr/download/tmpup

%osc_wget_run_wget_exe% -q %osc_wget_download_area%/%osc_wget_run_kit_file%
IF ERRORLEVEL 1 ECHO Unable to get %osc_wget_run_kit_file% && GOTO osc_wget_run_return

IF NOT EXIST %osc_wget_run_kit_file% ECHO Unable to get %osc_wget_run_kit_file% && GOTO osc_wget_run_return

%osc_wget_run_unzip_exe% -q %osc_wget_run_kit_file%

IF "%osc_wget_run_clean%" == "yes" DEL %osc_wget_run_kit_file%

CD %osc_wget_run_kit%\%osc_wget_run_version%
CALL setup.bat
@ECHO OFF
CD ..\..

SET osc_wget_run_app_file=%osc_wget_run_kit%\%osc_wget_run_version%\bin\%osc_wget_run_application%.exe

IF NOT EXIST %osc_wget_run_app_file% ECHO Application %osc_wget_run_app_file% not found. && GOTO osc_wget_run_return

REM # FIXME : app args
%osc_wget_run_app_file%

IF "%osc_wget_run_clean%" == "yes" DEL %osc_wget_run_kit_file%
IF "%osc_wget_run_clean%" == "yes" RMDIR /S /Q %osc_wget_run_kit%

:osc_wget_run_return
ENDLOCAL
@ECHO ON

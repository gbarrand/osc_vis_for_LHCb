@ECHO OFF
SETLOCAL

REM # We use wget.exe, unzip.exe (taken from CYGWIN). 
REM # We have put a copy of these (as long as the needed cyg*.dll) 
REM # under C:\usr\bin. Thes programs can be downloaded from :
REM #   http://OpenScientist.lal.in2p3.fr/download/foreign/CYGWIN
REM # Done in our login.
REM # SET PATH=%PATH%;C:\usr\bin

SET osc_wget_install_wget_exe=wget.exe
SET osc_wget_install_unzip_exe=unzip.exe

REM # //////////////////////////////////////////////////////////////////
REM # // args //////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

SET osc_wget_install_kit=
SET osc_wget_install_version=
SET osc_wget_install_platform=
SET osc_wget_install_check=no

:next_arg
IF "%1%" == "" GOTO end_args

IF NOT "%1%" == "-h" GOTO end_if_h
  ECHO Arguments :
  ECHO  -kit : [osc_batch,osc_vis,...].
  ECHO  -version : version to run.
  ECHO
  ECHO Options :
  ECHO  -check : execute check_install after installation.
  ECHO
  ECHO Examples :
  ECHO   osc_wget_install -kit osc_batch -version 16.10
  GOTO osc_wget_install_return
:end_if_h

IF NOT "%1%" == "-kit" GOTO end_if_kit
  SHIFT
  SET osc_wget_install_kit=%1%
  SHIFT
  GOTO next_arg
:end_if_kit

IF NOT "%1%" == "-version" GOTO end_if_version
  SHIFT
  SET osc_wget_install_version=%1%
  SHIFT
  GOTO next_arg
:end_if_version

IF NOT "%1%" == "-platform" GOTO end_if_platform
  SHIFT
  SET osc_wget_install_platform=%1%
  SHIFT
  GOTO next_arg
:end_if_platform

IF NOT "%1%" == "-check" GOTO end_if_check
  SET osc_wget_install_check=yes
  SHIFT
  GOTO next_arg
:end_if_check

:end_args

REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

IF NOT "%osc_wget_install_kit%" == "" GOTO end_if_check_kit
  ECHO No target given
  GOTO osc_wget_install_return
:end_if_check_kit

IF NOT "%osc_wget_install_version%" == "" GOTO end_if_check_version
  ECHO No version given
  GOTO osc_wget_install_return
:end_if_check_version

IF NOT "%osc_wget_install_platform%" == "" GOTO end_if_check_platform
  SET osc_wget_install_platform=Windows_NT-ix86-cl_1310
:end_if_check_platform

SET osc_wget_install_kit_file=%osc_wget_install_kit%-%osc_wget_install_version%-%osc_wget_install_platform%.zip
SET osc_wget_download_area=http://OpenScientist.lal.in2p3.fr/download/tmpup

%osc_wget_install_wget_exe% -q %osc_wget_download_area%/%osc_wget_install_kit_file%
IF ERRORLEVEL 1 ECHO Unable to get %osc_wget_install_kit_file% && GOTO osc_wget_install_return

IF NOT EXIST %osc_wget_install_kit_file% ECHO Unable to get %osc_wget_install_kit_file% && GOTO osc_wget_install_return

%osc_wget_install_unzip_exe% -q %osc_wget_install_kit_file%

DEL %osc_wget_install_kit_file%

IF NOT %osc_wget_install_check% == yes GOTO end_if_check_check
  CD %osc_wget_install_kit%\%osc_wget_install_version%
  CALL setup.bat
  @ECHO OFF
  CALL CHECK_INSTALL
  @ECHO OFF
:end_if_check_check

:osc_wget_install_return
ENDLOCAL
@ECHO ON

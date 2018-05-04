@ECHO OFF
SETLOCAL

REM # VisualC++ :
REM # Done in our login.
REM # CALL "C:\Program Files\Microsoft Visual Studio .NET 2003\Common7\Tools"\vsvars32.bat

REM # We use wget.exe, unzip.exe (taken from CYGWIN). 
REM # We have put a copy of these (as long as the needed cyg*.dll) 
REM # under C:\usr\bin. Thes programs can be downloaded from :
REM #   http://OpenScientist.lal.in2p3.fr/download/foreign/CYGWIN
REM # Done in our login.
REM # SET PATH=%PATH%;C:\usr\bin

SET osc_wget_build_wget_exe=wget.exe
SET osc_wget_build_unzip_exe=unzip.exe

REM # //////////////////////////////////////////////////////////////////
REM # // args //////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

SET osc_wget_build_kit=
SET osc_wget_build_version=
SET osc_wget_build_clean=yes

:next_arg
IF "%1%" == "" GOTO end_args

IF NOT "%1%" == "-h" GOTO end_if_h
  ECHO Arguments :
  ECHO  -kit : [osc_batch,osc_vis,...].
  ECHO  -version : version to build.
  ECHO  -no_clean : do not remove the build area when finished.
  ECHO
  ECHO Examples :
  ECHO   osc_wget_build -kit osc_vis -version v16r3
  GOTO osc_wget_build_return
:end_if_h

IF NOT "%1%" == "-kit" GOTO end_if_kit
  SHIFT
  SET osc_wget_build_kit=%1%
  SHIFT
  GOTO next_arg
:end_if_kit

IF NOT "%1%" == "-version" GOTO end_if_version
  SHIFT
  SET osc_wget_build_version=%1%
  SHIFT
  GOTO next_arg
:end_if_version

IF NOT "%1%" == "-no_clean" GOTO end_if_no_clean
  SET osc_wget_build_clean=no
  SHIFT
  GOTO next_arg
:end_if_no_clean

:end_args

REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////
REM # //////////////////////////////////////////////////////////////////

IF NOT "%osc_wget_build_kit%" == "" GOTO end_if_check_kit
  ECHO No target given
  GOTO osc_wget_build_return
:end_if_check_kit

IF NOT "%osc_wget_build_version%" == "" GOTO end_if_check_version
  ECHO No version given
  GOTO osc_wget_build_return
:end_if_check_version

SET build_Zebra=no
SET build_Python=no
SET build_Qt=no
SET build_gtk=no
SET build_Geant4=no
SET build_Geant3=no
IF NOT %osc_wget_build_kit% == osc_batch GOTO osc_wget_build_end_if_osc_batch
  SET build_Zebra=yes
:osc_wget_build_end_if_osc_batch

IF NOT %osc_wget_build_kit% == osc_iv GOTO osc_wget_build_end_if_osc_iv
  SET build_Python=yes
  SET build_Qt=yes
REM  SET build_gtk=yes
:osc_wget_build_end_if_osc_iv

IF NOT %osc_wget_build_kit% == osc_vis GOTO osc_wget_build_end_if_osc_vis
  SET build_Zebra=yes
  SET build_Python=yes
  SET build_Qt=yes
REM  SET build_gtk=yes
  SET build_Geant4=yes
  SET build_Geant3=yes
:osc_wget_build_end_if_osc_vis

IF NOT %osc_wget_build_kit% == osc_g4 GOTO osc_wget_build_end_if_osc_g4
  SET build_Zebra=yes
  SET build_Python=yes
  SET build_Qt=yes
REM  SET build_gtk=yes
  SET build_Geant4=yes
  SET build_Geant3=yes
:osc_wget_build_end_if_osc_g4

FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET osc_wget_build_dir_pwd=%%d

SET osc_wget_build_build_dir=OpenScientist\build_%osc_wget_build_kit%_%osc_wget_build_version%
IF EXIST %osc_wget_build_build_dir% RMDIR /S /Q %osc_wget_build_build_dir%
IF NOT EXIST %osc_wget_build_build_dir% MKDIR %osc_wget_build_build_dir%
CD %osc_wget_build_build_dir%

SET osc_wget_build_kit_file=%osc_wget_build_kit%_source_%osc_wget_build_version%.zip
SET osc_wget_build_download_area=http://OpenScientist.lal.in2p3.fr/download/tmpup

%osc_wget_build_wget_exe% -q %osc_wget_build_download_area%/%osc_wget_build_kit_file%
IF ERRORLEVEL 1 ECHO Unable to get %osc_wget_build_kit_file% && GOTO osc_wget_build_return

IF NOT EXIST %osc_wget_build_kit_file% ECHO Unable to get %osc_wget_build_kit_file% && GOTO osc_wget_build_return

%osc_wget_build_unzip_exe% -q %osc_wget_build_kit_file%
CD OpenScientist\%osc_wget_build_kit%\obuild
CALL setup.bat
@ECHO OFF
CALL DOS\build.bat
@ECHO OFF
IF NOT %build_Zebra% == yes GOTO osc_wget_build_end_if_build_Zebra
  REM # Set DF98 FORTRAN :
  SET PATH=%PATH%;C:\Program Files\Microsoft Visual Studio\DF98\BIN
  SET LIB=%LIB%;C:\Program Files\Microsoft Visual Studio\DF98\LIB
  SET INCLUDE=%INCLUDE%;C:\Program Files\Microsoft Visual Studio\DF98\INCLUDE
  ECHO ////////////////////
  ECHO build group Zebra...
  ECHO ////////////////////
  CALL DOS\build.bat -group Zebra
  @ECHO OFF
:osc_wget_build_end_if_build_Zebra

IF NOT %build_Python% == yes GOTO osc_wget_build_end_if_build_Python
  ECHO ////////////////////
  ECHO build group Python...
  ECHO ////////////////////
  CALL DOS\build -group Python
  @ECHO OFF
:osc_wget_build_end_if_build_Python

IF NOT %build_Qt% == yes GOTO osc_wget_build_end_if_build_Qt
  ECHO ////////////////////
  ECHO build group Qt...
  ECHO ////////////////////
  CALL DOS\build -group Qt
  @ECHO OFF
:osc_wget_build_end_if_build_Qt

IF NOT %build_gtk% == yes GOTO osc_wget_build_end_if_build_gtk
  ECHO ////////////////////
  ECHO build group gtk...
  ECHO ////////////////////
  CALL DOS\build -group gtk
  @ECHO OFF
:osc_wget_build_end_if_build_gtk

IF NOT %build_Geant4% == yes GOTO osc_wget_build_end_if_build_Geant4
  ECHO ////////////////////
  ECHO build group Geant4...
  ECHO ////////////////////
  CALL DOS\build -group Geant4
  @ECHO OFF
:osc_wget_build_end_if_build_Geant4

IF NOT %build_Geant3% == yes GOTO osc_wget_build_end_if_build_Geant3
  ECHO ////////////////////
  ECHO build group Geant3...
  ECHO ////////////////////
  CALL DOS\build -group Geant3
  @ECHO OFF
:osc_wget_build_end_if_build_Geant3

REM # zip.exe must be up and running.
ECHO ////////////////////
ECHO build release...
ECHO ////////////////////
CALL DOS\build_release
@ECHO OFF

IF NOT EXIST ..\bin_obuild ECHO ../bin_obuild directory not found. && GOTO osc_wget_build_return

CD ..\bin_obuild

FOR %%d IN (*.zip) DO SET zip_file=%%d

IF "%zip_file%" == "" ECHO zip file not found. && GOTO osc_wget_build_return

FOR %%d IN (%zip_file%) DO SET sz_local=%%~zd
REM ECHO %sz_local%

REM # SSH :
SET PATH=%PATH%;C:\Program Files\SSH Communications Security\SSH Secure Shell

SET remote=barrand@lx3.lal.in2p3.fr
scp2 %zip_file% %remote%:tmpup/%zip_file%

REM SET command=ssh2 %remote% "ls -al tmpup/%zip_file% | awk '{print $5;}'"
REM FOR /F "usebackq" %%d IN (`%command%`) DO SET sz_remote=%%d
REM ECHO %sz_remote%
REM IF "%sz_local%" NEQ "%sz_remote%" ECHO uploaded zip file mismatch. && GOTO osc_wget_build_return

IF "%osc_wget_build_clean%" NEQ "yes" GOTO osc_wget_build_return

CD %osc_wget_build_dir_pwd%
RMDIR /S /Q %osc_wget_build_build_dir%

:osc_wget_build_return
ENDLOCAL
@ECHO ON

@ECHO OFF

REM #
REM # File produced by the obuild tool version 1.10.0
REM # for the package OnX with version v17r12p2.
REM #

REM #////////////////////////////////////////////////
REM # Usage :
REM #   DOS> setup
REM # or :
REM #   DOS> setup.bat
REM # or :
REM #   DOS> call setup.bat
REM #////////////////////////////////////////////////

SET obuild_OnX_path=%~dp0
REM # Remove ending \obuild\
SET obuild_OnX_path=%obuild_OnX_path:~0,-8%

IF "%OBUILD_DIR_BIN%" == "" (
  SET obuild_dir_bin=bin_obuild
) ELSE (
  SET obuild_dir_bin=%OBUILD_DIR_BIN%
)

IF "%obuild_dir_bin%" == "" (
  ECHO obuild_dir_bin variable not defined.
  GOTO obuild_return
)

IF NOT EXIST "%obuild_OnX_path%\%obuild_dir_bin%" MKDIR "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST "%obuild_OnX_path%\%obuild_dir_bin%\obuild_platform.exe" (
  cl.exe /nologo /DWIN32 /MD /O2 /W3  /GX /GR /Fe"%obuild_OnX_path%\%obuild_dir_bin%\obuild_platform.exe" "%obuild_OnX_path%\obuild\cpp\obuild_platform.cpp" 1> cl_exe_error
  IF ERRORLEVEL 1 TYPE cl_exe_error
  IF NOT ERRORLEVEL 1 DEL cl_exe_error 
  IF EXIST obuild_platform.obj DEL obuild_platform.obj
)

FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET obuild_pwd_OnX_0=%%d
cd "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST .\obuild_platform.exe  ECHO Program obuild_platform.exe not found.  && GOTO obuild_return
FOR /F "usebackq" %%j IN (`.\obuild_platform.exe  "Windows_NT"`) DO SET obuild_platform_result=%%j
cd "%obuild_pwd_OnX_0%"
SET obuild_pwd_OnX_0=
IF NOT %obuild_platform_result% == yes GOTO end_if_platform_1
SET Qt_home=C:\usr\local\Qt\4.4.2
:end_if_platform_1
SET obuild_platform_result=
FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET obuild_pwd_OnX_0=%%d
cd "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST .\obuild_platform.exe  ECHO Program obuild_platform.exe not found.  && GOTO obuild_return
FOR /F "usebackq" %%j IN (`.\obuild_platform.exe  "Windows_NT"`) DO SET obuild_platform_result=%%j
cd "%obuild_pwd_OnX_0%"
SET obuild_pwd_OnX_0=
IF NOT %obuild_platform_result% == yes GOTO end_if_platform_3
SET Qt_moc=%Qt_home%\bin\moc
:end_if_platform_3
SET obuild_platform_result=
FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET obuild_pwd_OnX_0=%%d
cd "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST .\obuild_platform.exe  ECHO Program obuild_platform.exe not found.  && GOTO obuild_return
FOR /F "usebackq" %%j IN (`.\obuild_platform.exe  "Windows_NT"`) DO SET obuild_platform_result=%%j
cd "%obuild_pwd_OnX_0%"
SET obuild_pwd_OnX_0=
IF NOT %obuild_platform_result% == yes GOTO end_if_platform_5
SET SDL_home=C:\usr\local\SDL\1.2.11
:end_if_platform_5
SET obuild_platform_result=
FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET obuild_pwd_OnX_0=%%d
cd "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST .\obuild_platform.exe  ECHO Program obuild_platform.exe not found.  && GOTO obuild_return
FOR /F "usebackq" %%j IN (`.\obuild_platform.exe  "Windows_NT"`) DO SET obuild_platform_result=%%j
cd "%obuild_pwd_OnX_0%"
SET obuild_pwd_OnX_0=
IF NOT %obuild_platform_result% == yes GOTO end_if_platform_7
SET Python_home=C:\usr\local\Python\2.5
:end_if_platform_7
SET obuild_platform_result=
FOR /F "usebackq delims=;" %%d IN (`cd`) DO SET obuild_pwd_OnX_0=%%d
cd "%obuild_OnX_path%\%obuild_dir_bin%"
IF NOT EXIST .\obuild_platform.exe  ECHO Program obuild_platform.exe not found.  && GOTO obuild_return
FOR /F "usebackq" %%j IN (`.\obuild_platform.exe  "Windows_NT"`) DO SET obuild_platform_result=%%j
cd "%obuild_pwd_OnX_0%"
SET obuild_pwd_OnX_0=
IF NOT %obuild_platform_result% == yes GOTO end_if_platform_9
SET Python_version=25
:end_if_platform_9
SET obuild_platform_result=
SET obuild_Lib_path="..\..\Lib"
SET obuild_HEPVis_path="..\..\HEPVis"
SET obuild_KUIP_path="..\..\KUIP"

REM # Execute setup of used packages :
IF NOT EXIST "%obuild_Lib_path%\obuild\setup.bat"  ECHO setup.bat not found for package Lib.  && GOTO obuild_return
CALL "%obuild_Lib_path%\obuild\setup.bat"
@ECHO OFF

IF NOT EXIST "%obuild_HEPVis_path%\obuild\setup.bat"  ECHO setup.bat not found for package HEPVis.  && GOTO obuild_return
CALL "%obuild_HEPVis_path%\obuild\setup.bat"
@ECHO OFF

IF NOT EXIST "%obuild_KUIP_path%\obuild\setup.bat"  ECHO setup.bat not found for package KUIP.  && GOTO obuild_return
CALL "%obuild_KUIP_path%\obuild\setup.bat"
@ECHO OFF

REM # Set environment variables of this package :

IF DEFINED PATH GOTO else_if_path_OnX_10
  SET PATH=%obuild_OnX_path%\%obuild_dir_bin%
  GOTO end_if_path_OnX_10
:else_if_path_OnX_10
  SET PATH=%PATH%;%obuild_OnX_path%\%obuild_dir_bin%
:end_if_path_OnX_10

:obuild_return

SET obuild_OnX_path=

REM # FIXME SET obuild_dir_bin=

@ECHO ON

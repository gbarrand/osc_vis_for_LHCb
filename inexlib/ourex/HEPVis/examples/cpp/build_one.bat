@ECHO off
SETLOCAL

SET EXA=%1%

REM ECHO %EXA%

IF "%EXA%" == "" (
  ECHO Give the name of a program to build.
  GOTO obuild_return
)

CL.exe 1> NUL 2> NUL
IF ERRORLEVEL 1 (
  ECHO CL.exe program not found. You have to setup VisualC++ environment.
  GOTO obuild_return
)

FOR /F "usebackq delims=;" %%s IN (`osc-coin-win-config --libs`) DO SET osc_coin_win_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-coin-win-config --cppflags`) DO SET osc_coin_win_cppflags=%%s
FOR /F "usebackq delims=;" %%s IN (`hepvis-config --libs`) DO SET hepvis_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`hepvis-config --cppflags`) DO SET hepvis_cppflags=%%s

SET flags=/nologo /DWIN32 /MD /O2 /W3  /GX /GR 

SET flags=%flags% /DHEPVisWIn

IF EXIST %EXA%.obj DEL %EXA%.obj
CL.exe %flags% /c %osc_coin_win_cppflags% %hepvis_cppflags% /I.\lib /Fo%EXA%.obj /Tp%EXA%.cpp 1> cl_exe_error
IF ERRORLEVEL 1 TYPE cl_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL cl_exe_error 

LINK.exe /nologo /OPT:NOREF /out:iv_example_%EXA%.exe %EXA%.obj %osc_coin_win_libs% %hepvis_libs% 1> link_exe_error
IF ERRORLEVEL 1 TYPE link_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL link_exe_error 

IF EXIST %EXA%.obj DEL %EXA%.obj

:obuild_return
ENDLOCAL
@ECHO on

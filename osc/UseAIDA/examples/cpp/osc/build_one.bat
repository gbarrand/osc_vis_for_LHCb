@ECHO off
SETLOCAL

SET EXA=%1%

REM ECHO %EXA%

IF "%EXA%" == "" (
  ECHO Give the name of a program to build.
  ECHO For example :
  ECHO   build_one rmason
  GOTO obuild_return
)

CL.exe 1> NUL 2> NUL
IF ERRORLEVEL 1 (
  ECHO CL.exe program not found. You have to setup VisualC++ environment.
  GOTO obuild_return
)

FOR /F "usebackq delims=;" %%s IN (`aida-config --incs`) DO SET aida_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`aida-config --libs`) DO SET aida_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`amanip-config --incs`) DO SET amanip_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`inlib-config --incs`) DO SET inlib_incs=%%s

SET flags=/nologo /DWIN32 /MD /O2 /W3  /GX /GR 

IF EXIST %EXA%.obj DEL %EXA%.obj
CL.exe %flags% /c %aida_incs% %amanip_incs% %inlib_incs% /I.\lib /Fo%EXA%.obj /Tp%EXA%.cpp 1> cl_exe_error
IF ERRORLEVEL 1 TYPE cl_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL cl_exe_error 

LINK.exe /nologo /OPT:NOREF /out:AIDA_example_%EXA%.exe %EXA%.obj %aida_libs% 1> link_exe_error
IF ERRORLEVEL 1 TYPE link_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL link_exe_error 

IF EXIST %EXA%.obj DEL %EXA%.obj

:obuild_return
ENDLOCAL
@ECHO on

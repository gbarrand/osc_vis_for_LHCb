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

REM FOR /F "usebackq delims=;" %%s IN (`slash-config --incs`) DO SET slash_incs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --core_libs`) DO SET osc_core_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_cppflags`) DO SET osc_iv_cppflags=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --iv_libs`) DO SET osc_iv_libs=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --gl_cppflags`) DO SET osc_gl_cppflags=%%s
FOR /F "usebackq delims=;" %%s IN (`osc-config --gl_libs`) DO SET osc_gl_libs=%%s

SET flags=/nologo /DWIN32 /MD /O2 /W3  /GX /GR 

IF EXIST %EXA%.obj DEL %EXA%.obj
CL.exe %flags% /c %osc_iv_cppflags% %osc_gl_cppflags% /I.\lib /Fo%EXA%.obj /Tp%EXA%.cpp 1> cl_exe_error
IF ERRORLEVEL 1 TYPE cl_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL cl_exe_error 

LINK.exe /nologo /OPT:NOREF /out:osc_example_%EXA%.exe %EXA%.obj %osc_core_libs% %osc_iv_libs% %osc_gl_libs% 1> link_exe_error
IF ERRORLEVEL 1 TYPE link_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL link_exe_error 

IF EXIST %EXA%.obj DEL %EXA%.obj

:obuild_return
ENDLOCAL
@ECHO on

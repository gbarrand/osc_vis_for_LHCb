@ECHO off
SETLOCAL

REM #//////////////////////////////////////////////////////////
REM #// Used by Lib to produce dll for C function.           //
REM #//////////////////////////////////////////////////////////

REM # Check that VisualC++ environment is available :
CL.exe 1> NUL 2> NUL
IF ERRORLEVEL 1 (
  ECHO CL.exe program not found. You have to setup VisualC++ environment.
  GOTO obuild_return
)

REM # To find include files :
IF NOT DEFINED INLIBROOT (
  ECHO Environment variable INLIBROOT not defined.
  GOTO obuild_return
)
IF NOT DEFINED EXLIBROOT (
  ECHO Environment variable EXLIBROOT not defined.
  GOTO obuild_return
)
IF NOT DEFINED SLASHROOT (
  ECHO Environment variable SLASHROOT not defined.
  GOTO obuild_return
)
IF NOT DEFINED LIBROOT (
  ECHO Environment variable LIBROOT not defined.
  GOTO obuild_return
)
IF NOT DEFINED BATCHLABROOT (
  ECHO Environment variable BATCHLABROOT not defined.
  GOTO obuild_return
)

REM # To find libraries :
IF NOT DEFINED LIBDIRLIB (
  ECHO Environment variable LIBDIRLIB not defined.
  GOTO obuild_return
)
IF NOT DEFINED BATCHLABDIRLIB (
  ECHO Environment variable BATCHLABDIRLIB not defined.
  GOTO obuild_return
)

SET bin=
SET FILE_NAME=%1%
SET SUFFIX=%2%
SET WRAP_NAME=%3%

REM ECHO %FILE_NAME%
IF NOT EXIST %FILE_NAME%.%SUFFIX% (
  ECHO File %FILE_NAME%.%SUFFIX% not found.
  GOTO obuild_return
)
IF NOT EXIST %WRAP_NAME%.cpp (
  ECHO File %WRAP_NAME%.cpp not found.
  GOTO obuild_return
)

IF EXIST %bin%%FILE_NAME%.obj DEL %bin%%FILE_NAME%.obj
IF EXIST %bin%%WRAP_NAME%.obj DEL %bin%%WRAP_NAME%.obj

IF EXIST %bin%%FILE_NAME%.lib DEL %bin%%FILE_NAME%.lib
IF EXIST %bin%%FILE_NAME%.dll DEL %bin%%FILE_NAME%.dll
IF EXIST %bin%%FILE_NAME%.exp DEL %bin%%FILE_NAME%.exp

SET flags= /nologo /DWIN32 /MD /O2 /W3  /GX /GR 

CL.exe %flags% /c /DWIN32 /I"%INLIBROOT%" /I"%EXLIBROOT%" /I"%SLASHROOT%" /I"%LIBROOT%\include" /I"%BATCHLABROOT%" /I"%BATCHLABROOT%\BatchLab\HBOOK" /Fo%bin%%FILE_NAME%.obj /Tp%FILE_NAME%.%SUFFIX% 1> cl_exe_error
IF ERRORLEVEL 1 TYPE cl_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL cl_exe_error 

CL.exe %flags% /c /DWIN32 /I"%SLASHROOT%" /Fo%bin%%WRAP_NAME%.obj /Tp%WRAP_NAME%.cpp 1> cl_exe_error
IF ERRORLEVEL 1 TYPE cl_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL cl_exe_error 

LINK.exe /nologo /dll /out:%bin%%FILE_NAME%.dll %bin%%WRAP_NAME%.obj %bin%%FILE_NAME%.obj "%BATCHLABDIRLIB%\BatchLabHBOOK.lib" "%LIBDIRLIB%\LibUtils.lib" 1> link_exe_error
IF ERRORLEVEL 1 TYPE link_exe_error && GOTO obuild_return
IF NOT ERRORLEVEL 1 DEL link_exe_error 

IF EXIST %bin%%FILE_NAME%.obj DEL %bin%%FILE_NAME%.obj
IF EXIST %bin%%WRAP_NAME%.obj DEL %bin%%WRAP_NAME%.obj

IF EXIST %bin%%FILE_NAME%.exp DEL %bin%%FILE_NAME%.exp

:obuild_return

ENDLOCAL
@ECHO on

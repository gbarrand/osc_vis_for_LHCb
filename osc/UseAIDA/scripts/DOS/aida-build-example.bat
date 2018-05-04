@ECHO OFF
SETLOCAL

REM # Usage :
REM #   DOS> aida-build-example <file1>.cpp <file2>.cpp
REM #   DOS> aida_example.exe
REM # for example :
REM #   DOS> <edit some Histogram.cpp main program>
REM #   DOS> aida-build-example Histogram.cpp Random.cpp
REM #   DOS> aida_example.exe

FOR /F "usebackq delims=;" %%j IN (`aida-config --incs`) DO SET obuild_incs=%%j

FOR /F "usebackq delims=;" %%j IN (`aida-config --libs`) DO SET obuild_libs=%%j

SET obuild_args=%*

REM ECHO %obuild_args%

REM # /GR to enable RTTI and dynamic cast.
REM # /GX to enable exception handling (for STL).
REM # /O2 optimize
SET cppflags=/nologo /GX /GR /O2 /I .

cl.exe /nologo /Foaida_example.obj /c %cppflags% %obuild_incs% %obuild_args%
link.exe /nologo /out:aida_example.exe aida_example.obj %obuild_libs%

:obuild_return
ENDLOCAL
@ECHO ON

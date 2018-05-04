@ECHO OFF

set dir_exas="%ONXROOT%\examples\OnX"
dir /B %dir_exas%\*.onx > tmp1
REM type tmp1

REM FOR /F %%f IN ( tmp1 ) DO echo %dir_exas%\%%f

FOR /F %%f IN ( tmp1 ) DO echo %dir_exas%\%%f && onx -stdout %dir_exas%\%%f

@ECHO ON




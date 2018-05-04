@ECHO off
SETLOCAL

SET OSC_ARGS=-balance

FOR %%f IN (.\*.py) DO python %%f

ENDLOCAL
@ECHO on

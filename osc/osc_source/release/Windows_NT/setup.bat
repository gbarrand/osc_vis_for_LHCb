@ECHO off 

REM # Usage :
REM #     DOS> call <path>\setup.bat

SET OSC_home=%~d0%~p0

SET PATH=%PATH%;%OSC_home%\bin

SET OSC_HOME_DIR=%OSC_home%

REM # AIDA :
SET AIDA_HOME_DIR=%OSC_home%

REM # else :
SET COIN_DONT_INFORM_INDIRECT_RENDERING=1

SET OBUILD_PATH=%OSC_home%\Resources

@ECHO on

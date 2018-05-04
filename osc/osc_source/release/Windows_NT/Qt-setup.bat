@ECHO off 

REM # Usage :
REM #     DOS> call <osc_vis_path>\Qt-setup.bat

SET Qt_home=C:\usr\local\Qt\4.4.2

REM SET Qt_drive=%~d0
REM SET Qt_home=%Qt_drive%\cern.ch\sw\lcg\external\qt\4.4.2\win32_vc71

SET PATH=%PATH%;%Qt_home%\bin

@ECHO on

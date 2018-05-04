@ECHO off 

REM # Usage :
REM #     DOS> call <osc_vis_path>\Python-setup.bat

IF DEFINED PYTHONPATH (
  SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\bin
) ELSE (
  SET PYTHONPATH=%OSC_HOME_DIR%\bin
)

SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\Resources\OnXLab\scripts\Python
SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\Resources\OnX\scripts\Python
SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\Resources\HEPVis\scripts\Python
SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\Resources\CoinPython\scripts
SET PYTHONPATH=%PYTHONPATH%;%OSC_HOME_DIR%\Resources\inlib\py

@ECHO on

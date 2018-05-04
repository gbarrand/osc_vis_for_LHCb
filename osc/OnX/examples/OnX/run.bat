@ECHO off
SETLOCAL

REM FOR %%f IN (.\*.onx) DO osc-gui-viewer %%f

"%OSC_HOME_DIR%"\bin\osc-gui-viewer hello_world.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer box.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer calc.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer command_line.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer tab_stack.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer tree.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer progress.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer list.onx

"%OSC_HOME_DIR%"\bin\osc-gui-viewer opengl_area.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer so_examiner_viewer.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer page_viewer.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer plotter.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer shapes.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer iv_viewer.onx
"%OSC_HOME_DIR%"\bin\osc-gui-viewer create_package.onx

ENDLOCAL
@ECHO on

@ECHO off
SETLOCAL

CALL .\build_one create_ui
@ECHO OFF

CALL .\build_one detector_visualize
@ECHO OFF

CALL .\build_one opengl_cube
@ECHO OFF

CALL .\build_one opengl_contour
@ECHO OFF

CALL .\build_one plotter_example
@ECHO OFF

ENDLOCAL
@ECHO on

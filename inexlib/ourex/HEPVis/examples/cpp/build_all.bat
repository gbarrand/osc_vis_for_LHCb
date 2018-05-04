@ECHO off
SETLOCAL

CALL .\build_one win_so_cube
@ECHO OFF

CALL .\build_one hepvis_polyhedron
@ECHO OFF

CALL .\build_one hepvis_detector_tree_kit
@ECHO OFF

CALL .\build_one hepvis_hershey
@ECHO OFF

CALL .\build_one hepvis_ttf
@ECHO OFF

CALL .\build_one hepvis_plotters
@ECHO OFF

ENDLOCAL
@ECHO on

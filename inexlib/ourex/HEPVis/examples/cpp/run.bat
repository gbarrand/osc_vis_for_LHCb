@ECHO off
SETLOCAL

set TTFPATH=%OSC_HOME_DIR%\Resources\HEPVis\fonts

.\iv_example_win_so_cube

.\iv_example_hepvis_polyhedron
.\iv_example_hepvis_detector_tree_kit
.\iv_example_hepvis_hershey
.\iv_example_hepvis_ttf
.\iv_example_hepvis_plotters

ENDLOCAL
@ECHO on

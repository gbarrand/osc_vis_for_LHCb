@ECHO off
SETLOCAL

REM //////////////////////////////
REM //////////////////////////////
REM //////////////////////////////
CALL .\build_one analysis
@ECHO OFF

CALL .\build_one animated_fit
@ECHO OFF

CALL .\build_one animated_fit_2
@ECHO OFF

CALL .\build_one ascii_tuple
@ECHO OFF

CALL .\build_one banner
@ECHO OFF

CALL .\build_one cloud_2D
@ECHO OFF

CALL .\build_one cloud_fill
@ECHO OFF

CALL .\build_one cloud_slice
@ECHO OFF

CALL .\build_one complex_and_func
@ECHO OFF

CALL .\build_one data_point_set
@ECHO OFF

CALL .\build_one fitting
@ECHO OFF

CALL .\build_one func_2D
@ECHO OFF

CALL .\build_one func_2D_curve
@ECHO OFF

CALL .\build_one func_cut
@ECHO OFF

CALL .\build_one func_to_cloud
@ECHO OFF

CALL .\build_one func_write
@ECHO OFF

CALL .\build_one hippo
@ECHO OFF

CALL .\build_one hippo_write
@ECHO OFF

CALL .\build_one hopera
@ECHO OFF

CALL .\build_one jec
@ECHO OFF

CALL .\build_one lego
@ECHO OFF

CALL .\build_one mag
@ECHO OFF

CALL .\build_one mandel
@ECHO OFF

CALL .\build_one on_the_fly_func
@ECHO OFF

CALL .\build_one profile
@ECHO OFF

CALL .\build_one reopen
@ECHO OFF

CALL .\build_one rio
@ECHO OFF

CALL .\build_one rmason
@ECHO OFF

CALL .\build_one root_th
@ECHO OFF

CALL .\build_one store
@ECHO OFF

CALL .\build_one xml
@ECHO OFF

CALL .\build_one shahrokh_a_write
@ECHO OFF

CALL .\build_one shahrokh_read
@ECHO OFF

CALL .\build_one thread_and_auto_refresh
@ECHO OFF

CALL .\build_one pure_thread
@ECHO OFF

CALL .\build_one A2R
@ECHO OFF

REM //////////////////////////////
REM //////////////////////////////
REM //////////////////////////////
CALL .\build_load load_aida
@ECHO OFF

CALL .\build_load load_plotter
@ECHO OFF

REM //////////////////////////////
REM //////////////////////////////
REM //////////////////////////////
CALL .\build_slash slash_cyclic
@ECHO OFF

CALL .\build_slash slash_thread_cyclic
@ECHO OFF

CALL .\build_slash slash_shape_collect_tuple
@ECHO OFF

CALL .\build_slash slash_shape_hits_collect
@ECHO OFF

REM //////////////////////////////
REM //////////////////////////////
REM //////////////////////////////
FOR /F "usebackq delims=;" %%s IN (`aida-config --implementation`) DO SET aida_imp=%%s

IF "%aida_imp%" NEQ "OnXLab" GOTO end_if_iv

CALL .\build_iv slash_iv_regions
@ECHO OFF

CALL .\build_iv slash_iv_plot_and_det
@ECHO OFF

CALL .\build_iv slash_iv_plot_and_images
@ECHO OFF

:end_if_iv

REM //////////////////////////////
REM //////////////////////////////
REM //////////////////////////////

ENDLOCAL
@ECHO on

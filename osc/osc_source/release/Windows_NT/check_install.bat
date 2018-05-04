@ECHO off 
SETLOCAL

IF DEFINED OSC_HOME_DIR GOTO end_if_osc_home_dir
  ECHO You have to execute the setup.bat of OpenScientist.
  GOTO check_install_return
:end_if_osc_home_dir

REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM # try to guess the kit from OSC_HOME_DIR
SET osc_batch_kit=no
SET osc_iv_kit=no
SET osc_vis_kit=no

SET xx=%OSC_HOME_DIR%
:do_osc_home_dir_words
IF "%xx%" == "" GOTO do_osc_home_dir_done
FOR /F "tokens=1* delims=\" %%a IN ("%xx%") DO SET yy=%%a
FOR /F "tokens=1* delims=\" %%a IN ("%xx%") DO SET xx=%%b

IF %yy% NEQ osc_batch GOTO end_if_kind_osc_batch
  SET osc_batch_kit=yes
:end_if_kind_osc_batch

IF %yy% NEQ osc_iv GOTO end_if_kind_osc_iv
  SET osc_iv_kit=yes
:end_if_kind_osc_iv

IF %yy% NEQ osc_vis GOTO end_if_kind_osc_vis
  SET osc_vis_kit=yes
:end_if_kind_osc_vis

GOTO do_osc_home_dir_words

:do_osc_home_dir_done

REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM #/////////////////////////////////////

IF EXIST osc_tmp RMDIR /S /Q osc_tmp

IF NOT EXIST osc_tmp MKDIR osc_tmp
CD osc_tmp

SET OSC_ARGS=-balance
SET OSC_ARGS=-balance -debug_check_class

SET do_AIDA_Store=no
SET do_AIDA_slac=no
SET do_AIDA_anaphe=no
SET do_AIDA_osc=no

SET do_osc_plot=no
SET do_OnX_cpp=no
SET do_osc_gui_viewer=no
SET do_osc_viewer=no
SET do_osc_new_app=no
SET do_iv=no
SET do_opaw=no
SET do_oroot=no
SET do_osc_new_app=no


SET do_g3=no
SET do_OnX_py=no
SET do_AIDA_py=no

SET do_specific=no

:next_arg
IF "%1%" == "" GOTO end_args

IF NOT "%1%" == "-h" GOTO end_if_h
  ECHO Usage :
  ECHO   check_install [option] [what]
  ECHO what :
  ECHO   all
  ECHO   py
  ECHO   osc-plot
  ECHO   osc-gui-viewer
  ECHO   osc-viewer
  ECHO   osc-new-app
  ECHO   opaw
  ECHO   oroot
  ECHO   AIDA_slac
  ECHO   AIDA_anaphe
  ECHO   AIDA_osc
  ECHO   OnX_cpp
  ECHO   OnX_py
  ECHO   AIDA_py
  GOTO check_install_return
:end_if_h

IF NOT "%1%" == "all" GOTO end_if_all
  SET do_specific=yes
  SET do_AIDA_Store=yes
  SET do_osc_plot=yes
  SET do_AIDA_slac=yes
  SET do_AIDA_anaphe=yes
  SET do_AIDA_osc=yes
  SET do_OnX_cpp=yes
  SET do_osc_gui_viewer=yes
  SET do_osc_viewer=yes
  SET do_osc_new_app=yes
  SET do_iv=yes
  SET do_opaw=yes
  SET do_oroot=yes
  SHIFT
  GOTO next_arg
:end_if_all

IF NOT "%1%" == "py" GOTO end_if_py
  SET do_specific=yes
  SET do_OnX_py=no
  SET do_AIDA_py=no
  SHIFT
  GOTO next_arg
:end_if_py

IF NOT "%1%" == "osc-plot" GOTO end_if_osc_plot
  SET do_AIDA_Store=yes
  SET do_osc_plot=yes
  SHIFT
  GOTO next_arg
:end_if_osc_plot

IF NOT "%1%" == "osc-gui-viewer" GOTO end_if_osc_gui_viewer
  SET do_specific=yes
  SET do_osc_gui_viewer=yes
  SHIFT
  GOTO next_arg
:end_if_osc_gui_viewer

IF NOT "%1%" == "osc-viewer" GOTO end_if_osc_viewer
  SET do_specific=yes
  SET do_osc_viewer=yes
  SHIFT
  GOTO next_arg
:end_if_osc_viewer

IF NOT "%1%" == "osc-new-app" GOTO end_if_osc_new_app
  SET do_specific=yes
  SET do_osc_new_app=yes
  SHIFT
  GOTO next_arg
:end_if_osc_new_app

IF NOT "%1%" == "iv" GOTO end_if_iv
  SET do_specific=yes
  SET do_iv=yes
  SHIFT
  GOTO next_arg
:end_if_iv

IF NOT "%1%" == "opaw" GOTO end_if_opaw
  SET do_specific=yes
  SET do_opaw=yes
  SHIFT
  GOTO next_arg
:end_if_opaw

IF NOT "%1%" == "oroot" GOTO end_if_oroot
  SET do_specific=yes
  SET do_oroot=yes
  SHIFT
  GOTO next_arg
:end_if_oroot

IF NOT "%1%" == "AIDA_slac" GOTO end_if_AIDA_slac
  SET do_specific=yes
  SET do_AIDA_slac=yes
  SHIFT
  GOTO next_arg
:end_if_AIDA_slac

IF NOT "%1%" == "AIDA_anaphe" GOTO end_if_AIDA_anaphe
  SET do_specific=yes
  SET do_AIDA_anaphe=yes
  SHIFT
  GOTO next_arg
:end_if_AIDA_anaphe

IF NOT "%1%" == "AIDA_osc" GOTO end_if_AIDA_osc
  SET do_specific=yes
  SET do_AIDA_osc=yes
  SHIFT
  GOTO next_arg
:end_if_AIDA_osc

IF NOT "%1%" == "OnX_cpp" GOTO end_if_OnX_cpp
  SET do_specific=yes
  SET do_OnX_cpp=yes
  SHIFT
  GOTO next_arg
:end_if_OnX_cpp

IF NOT "%1%" == "OnX_py" GOTO end_if_OnX_py
  SET do_specific=yes
  SET do_OnX_py=yes
  SHIFT
  GOTO next_arg
:end_if_OnX_py

IF NOT "%1%" == "AIDA_py" GOTO end_if_AIDA_py
  SET do_specific=yes
  SET do_AIDA_py=yes
  SHIFT
  GOTO next_arg
:end_if_AIDA_py

IF NOT "%1%" == "g3" GOTO end_if_g3
  SET do_specific=yes
  SET do_g3=yes
  SHIFT
  GOTO next_arg
:end_if_g3

:end_args

REM #////////////////////////////////////////////////
REM #/// End arguments.
REM #////////////////////////////////////////////////

REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM #/////////////////////////////////////

REM # in case of a not specific check, we raise 
REM # the check flags accordint the kind of kit.

IF "%do_specific%" NEQ "no" GOTO end_if_do_specific

IF "%osc_batch_kit%" NEQ "yes" GOTO end_if_osc_batch_kit
  ECHO Check an osc_batch kit...

REM # SET do_AIDA_Store=yes
  SET do_AIDA_slac=yes
  SET do_AIDA_anaphe=yes
  SET do_AIDA_osc=yes

REM # do_AIDA_py=yes

:end_if_osc_batch_kit

IF "%osc_iv_kit%" NEQ "yes" GOTO end_if_osc_iv_kit
  ECHO Check an osc_iv kit...
  SET do_iv=yes
:end_if_osc_iv_kit

IF "%osc_vis_kit%" NEQ "yes" GOTO end_if_osc_vis_kit
  ECHO Check an osc_vis kit...

  SET do_AIDA_Store=yes
  SET do_AIDA_slac=yes
  SET do_AIDA_anaphe=yes
  SET do_AIDA_osc=yes

  SET do_iv=yes

  SET do_osc_plot=yes
  SET do_OnX_cpp=yes
  SET do_osc_gui_viewer=yes
  SET do_osc_viewer=yes
  SET do_osc_new_app=yes
  SET do_osc_server=yes
  SET do_opaw=yes
  SET do_oroot=yes
  SET do_darwin_app=yes

REM #  SET do_AIDA_py=yes
REM #  SET do_OnX_py=yes
REM #  SET do_g4=yes
REM #  SET do_g3=yes

:end_if_osc_vis_kit

:end_if_do_specific

REM #/////////////////////////////////////
REM #/// osc_batch kit : /////////////////
REM #/////////////////////////////////////

IF "%do_AIDA_Store%" NEQ "yes" GOTO end_do_AIDA_Store

REM # compile and run AIDA_example_Store :

MKDIR slac
CD slac
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\AIDA\examples\cpp\slac .
CALL .\build_one Store
@ECHO off 

.\AIDA_example_Store
IF NOT EXIST AIDA_example_Store.aida ECHO AIDA_example_Store.aida not produced.
MOVE AIDA_example_Store.aida ..\.

IF EXIST "%OSC_HOME_DIR%"\bin\BatchLabZebra.dll GOTO if_exist_store_hbook
  ECHO Zebra storage plugin not found.
  GOTO end_if_store_hbook
:if_exist_store_hbook
  .\AIDA_example_Store hbook
  IF NOT EXIST aida_example_store.hbook ECHO aida_example_store.hbook not produced.
  MOVE AIDA_example_store.hbook ..\.
:end_if_store_hbook

.\AIDA_example_Store root
IF NOT EXIST AIDA_example_Store.root ECHO AIDA_example_Store.root not produced.
MOVE AIDA_example_Store.root ..\.

.\AIDA_example_Store hdf5
IF NOT EXIST aida_example_store.hdf5 ECHO aida_example_store.hdf5 not produced.
MOVE AIDA_example_store.hdf5 ..\.

CD ..
RMDIR /S /Q slac

:end_do_AIDA_Store

REM #/////////////////////////////////////
REM #/// AIDA/examples/cpp/slac //////////
REM #/////////////////////////////////////
IF "%do_AIDA_slac%" NEQ "yes" GOTO end_do_AIDA_slac
REM if [ ${do_AIDA_slac} = yes ] ; then
ECHO AIDA slac...
MKDIR slac
CD slac
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\AIDA\examples\cpp\slac .
CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 
CD ..
:end_do_AIDA_slac

REM #/////////////////////////////////////
REM #/// AIDA/examples/cpp/anaphe ////////
REM #/////////////////////////////////////
IF "%do_AIDA_anaphe%" NEQ "yes" GOTO end_do_AIDA_anaphe
ECHO AIDA anaphe...
MKDIR anaphe
CD anaphe
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\AIDA\examples\cpp\anaphe .
CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 
CD ..
:end_do_AIDA_anaphe

REM #/////////////////////////////////////
REM #/// AIDA/examples/cpp/osc ///////////
REM #/////////////////////////////////////
IF "%do_AIDA_osc%" NEQ "yes" GOTO end_do_AIDA_osc
ECHO AIDA osc...
MKDIR osc
CD osc
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\AIDA\examples\cpp\osc .
CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 
CD ..
:end_do_AIDA_osc

REM #/////////////////////////////////////
REM #/// osc_iv kit : ////////////////////
REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM #/// iv /.////////////////////////////
REM #/////////////////////////////////////

IF "%do_iv%" NEQ "yes" GOTO end_do_iv

IF NOT EXIST "%OSC_HOME_DIR%"\bin\hepvis-config.exe GOTO end_do_iv

MKDIR iv_exas
CD iv_exas
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\HEPVis\examples\cpp .

CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 

CD ..


:end_do_iv

REM #/////////////////////////////////////
REM #/// osc_vis kit : ///////////////////
REM #/////////////////////////////////////
REM #/////////////////////////////////////
REM #/// OnX/examples/cpp ////////////////
REM #/////////////////////////////////////
IF "%do_OnX_cpp%" NEQ "yes" GOTO end_do_OnX_cpp
IF NOT EXIST "%OSC_HOME_DIR%"\bin\osc-config.exe GOTO end_do_OnX_cpp

ECHO OnX/examples/cpp...
MKDIR cpp
CD cpp
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\OnX\examples\cpp .
CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 
CD ..
:end_do_OnX_cpp

REM #/////////////////////////////////////
REM #/// osc-plot ////////////////////////
REM #/////////////////////////////////////

IF "%do_osc_plot%" NEQ "yes" GOTO end_do_osc_plot
IF NOT EXIST "%OSC_HOME_DIR%"\bin\osc-plot.exe GOTO end_do_osc_plot

"%OSC_HOME_DIR%"\bin\osc-plot

"%OSC_HOME_DIR%"\bin\osc-plot .\AIDA_example_Store.aida
"%OSC_HOME_DIR%"\bin\osc-plot .\AIDA_example_Store.root
"%OSC_HOME_DIR%"\bin\osc-plot .\AIDA_example_store.hdf5

IF EXIST .\AIDA_example_store.hbook "%OSC_HOME_DIR%"\bin\osc-plot .\AIDA_example_store.hbook

:end_do_osc_plot

REM #/////////////////////////////////////
REM #/// opaw ////////////////////////////
REM #/////////////////////////////////////

IF "%do_opaw%" NEQ "yes" GOTO end_do_opaw
IF NOT EXIST "%OSC_HOME_DIR%"\bin\opaw.exe GOTO end_do_opaw

MKDIR opaw
CD opaw
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\OpenPAW\examples .

cd osc
CALL .\run
@ECHO off 
cd ..

cd users
CALL .\run
@ECHO off 
cd ..

cd PAW_C
CALL .\run
@ECHO off 
cd ..

CD ..

:end_do_opaw

REM #/////////////////////////////////////
REM #/// oroot ///////////////////////////
REM #/////////////////////////////////////

IF "%do_oroot%" NEQ "yes" GOTO end_do_oroot
IF NOT EXIST "%OSC_HOME_DIR%"\bin\oroot.exe GOTO end_do_oroot

MKDIR oroot
CD oroot
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\oroot\examples .

oroot
oroot plotHisto.C
DEL oroot_plotHisto.cpp
DEL oroot_plotHisto.dll
CALL .\build_all
@ECHO off 
CALL .\run
@ECHO off 

CD ..

:end_do_oroot

REM #/////////////////////////////////////
REM #/// osc-gui-viewer //////////////////
REM #/////////////////////////////////////

IF "%do_osc_gui_viewer%" NEQ "yes" GOTO end_do_osc_gui_viewer
IF NOT EXIST "%OSC_HOME_DIR%"\bin\osc-gui-viewer.exe GOTO end_do_osc_gui_viewer

MKDIR OnX
CD OnX
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\OnX\examples\OnX .
CALL .\run
@ECHO off 
CD ..

:end_do_osc_gui_viewer

REM #/////////////////////////////////////
REM #/// osc-viewer //////////////////////
REM #/////////////////////////////////////

IF "%do_osc_viewer%" NEQ "yes" GOTO end_do_osc_viewer
IF NOT EXIST "%OSC_HOME_DIR%"\bin\osc-viewer.exe GOTO end_do_osc_viewer

COPY "%OSC_HOME_DIR%"\Resources\OnX\examples\OnX\bird.iv .
"%OSC_HOME_DIR%"\bin\osc-viewer
"%OSC_HOME_DIR%"\bin\osc-viewer "%OSC_HOME_DIR%"\Resources\OnX\examples\OnX\LHCb_magnet.hiv
"%OSC_HOME_DIR%"\bin\osc-viewer "%OSC_HOME_DIR%"\Resources\OnX\examples\OnX\piano2.wrl

:end_do_osc_viewer

REM #/////////////////////////////////////
REM #/// osc-new-app /////////////////////
REM #/////////////////////////////////////

IF "%do_osc_new_app%" NEQ "yes" GOTO end_do_osc_new_app
IF NOT EXIST "%OSC_HOME_DIR%"\bin\osc-new-app.exe GOTO end_do_osc_new_app

"%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateOne"
CD my_app\v1r0\build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
CD ..\..\..
RMDIR /S /Q my_app

"%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateThree"
CD my_app\v1r0\build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
CD ..\..\..
RMDIR /S /Q my_app

"%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateFour"
CD my_app\v1r0\build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
CD ..\..\..
RMDIR /S /Q my_app

"%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateFive"
CD my_app\v1r0\build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
CD ..\..\..
RMDIR /S /Q my_app

"%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateSix"
CD my_app\v1r0\build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
CD ..\..\..
RMDIR /S /Q my_app

:end_do_osc_new_app

REM #/////////////////////////////////////
REM #/// OnX/examples/Python /////////////
REM #/////////////////////////////////////
IF "%do_OnX_py%" NEQ "yes" GOTO end_do_OnX_py

MKDIR OnX_Python
CD OnX_Python
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\OnX\examples\Python .
CALL .\run
@ECHO off 
CD ..
RMDIR /S /Q OnX_Python

REM do_OnX_py
REM "%OSC_HOME_DIR%"\bin\osc-new-app  -name=my_app -template="%OSC_HOME_DIR%\Resources\OnX\templates\OnXTemplateTwo"
REM CD my_app\v1r0\build
REM CALL .\build
REM @ECHO off 
REM "%OSC_HOME_DIR%"\bin\osc-gui-viewer ../scripts/OnX/main.onx
REM CD ..\..\..
REM RMDIR /S /Q my_app

:end_do_OnX_py

REM #/////////////////////////////////////
REM #/// AIDA/examples/Python ////////////
REM #/////////////////////////////////////
IF "%do_AIDA_py%" NEQ "yes" GOTO end_do_AIDA_py

MKDIR AIDA_Python
CD AIDA_Python
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\AIDA\examples\Python .
CALL .\run
@ECHO off 
CD ..
RMDIR /S /Q AIDA_Python

:end_do_AIDA_py

REM #/////////////////////////////////////
REM #/// g3 //////////////////////////////
REM #/////////////////////////////////////

IF "%do_g3%" NEQ "yes" GOTO end_do_g3

MKDIR VOLU
CD VOLU
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\G3Lab\applications\VOLU .
"%OSC_HOME_DIR%"\bin\osc-gui-viewer main.onx
CD ..
RMDIR /S /Q VOLU

MKDIR G3Simple
CD G3Simple
XCOPY /E /Q "%OSC_HOME_DIR%"\Resources\G3Lab\examples\G3Simple .
CD build
CALL .\build
@ECHO off 
"%OSC_HOME_DIR%"\bin\osc-gui-viewer ..\scripts\OnX\main.onx
CD ..\..
RMDIR /S /Q G3Simple

:end_do_g3

REM #/////////////////////////////////////
REM #/////////////////////////////////////

CD ..
RMDIR /S /Q osc_tmp

:check_install_return
ENDLOCAL
@ECHO on

@ECHO off
SETLOCAL

SET OSC_ARGS=-balance

REM FOR %%f IN (.\*.py) DO python %%f

python LHCb.py
python cube.py
python cylinder.py
python detector.py
python find_widgets.py
python gui.py
python hello.py
python images.py
python menu.py
python osc_iv.py
python polyhedron.py
python shapes.py
python so_examiner_viewer.py
python str.py
python transparency.py
python tree.py
python typemaps.py

REM FIXME python -i prompt.py
REM FIXME python -i prompt_thread.py
REM FIXME python -i loop.py

ENDLOCAL
@ECHO on

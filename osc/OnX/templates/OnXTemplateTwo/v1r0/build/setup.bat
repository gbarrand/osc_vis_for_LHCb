@ECHO OFF

IF DEFINED PYTHONPATH GOTO else_path
  SET PYTHONPATH=.;..\scripts\Python
  GOTO end_path
:else_path
  SET PYTHONPATH=%PYTHONPATH%;.;..\scripts\Python
:end_path

@ECHO ON

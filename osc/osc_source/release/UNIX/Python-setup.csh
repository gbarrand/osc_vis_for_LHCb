
#
#  Usage :
#    csh> source <osc_vis_path>/Python-setup.csh
#

#set verbose

if ( ! $?OSC_HOME_DIR ) then
  echo "Environment variable OSC_HOME_DIR not defined."
  echo "You have first to source the OpenScientist setup."
  exit
endif

# WARNING :
#   have paths for scripts first
#   because of Linux OnX.py/OnX.so clash.

set py_path="${OSC_HOME_DIR}/Resources/OnXLab/scripts/Python"
set py_path="${py_path}:${OSC_HOME_DIR}/Resources/OnX/scripts/Python"
set py_path="${py_path}:${OSC_HOME_DIR}/Resources/HEPVis/scripts/Python"
set py_path="${py_path}:${OSC_HOME_DIR}/Resources/CoinPython/scripts"
set py_path="${py_path}:${OSC_HOME_DIR}/Resources/exlib/exlib/py"
set py_path="${py_path}:${OSC_HOME_DIR}/Resources/CoinXt/py"

set py_path="${py_path}:${OSC_HOME_DIR}/bin"

set py_curr=`printenv PYTHONPATH`
if ( "${py_curr}" == "" ) then
  setenv PYTHONPATH "${py_path}"
else
  if ( `echo "${py_curr}" | grep "${py_path}" ` == "" ) then
    setenv PYTHONPATH "${PYTHONPATH}:${py_path}"
  endif
endif
unset py_curr
unset py_path


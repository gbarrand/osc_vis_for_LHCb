
#
#  Usage :
#    sh> . <osc_vis_path>/Python-setup.sh
#

#set -x

if [ "${OSC_HOME_DIR}" = "" ] ; then
  echo "Environment variable OSC_HOME_DIR not defined."
  echo "You have first to source the OpenScientist setup."
  return
fi

# WARNING :
#   have paths for scripts first
#   because of Linux OnX.py/OnX.so clash.

py_path="${OSC_HOME_DIR}/Resources/OnXLab/scripts/Python"
py_path="${py_path}:${OSC_HOME_DIR}/Resources/OnX/scripts/Python"
py_path="${py_path}:${OSC_HOME_DIR}/Resources/HEPVis/scripts/Python"
py_path="${py_path}:${OSC_HOME_DIR}/Resources/CoinPython/scripts"
py_path="${py_path}:${OSC_HOME_DIR}/Resources/exlib/exlib/py"

py_path="${py_path}:${OSC_HOME_DIR}/bin"

if [ "${PYTHONPATH}" = "" ] ; then
  PYTHONPATH="${py_path}"
  export PYTHONPATH
else 
  PYTHONPATH="${PYTHONPATH}:${py_path}"
  export PYTHONPATH
fi

unset py_path


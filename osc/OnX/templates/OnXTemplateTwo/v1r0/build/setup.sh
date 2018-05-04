
if [ -z "${PYTHONPATH}" ] ; then
  PYTHONPATH=".:../scripts/Python"
  export PYTHONPATH
else
  PYTHONPATH="${PYTHONPATH}:.:../scripts/Python"
  export PYTHONPATH
fi


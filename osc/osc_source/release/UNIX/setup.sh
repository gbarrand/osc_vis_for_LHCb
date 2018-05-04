#
#  Usage :
#    sh> . <osc_kit path>/setup.sh
#

#set -x

OSC_home="@@absolute_dir@@"

if [ "${OSC_home}" = '@@absolute_dir@@' ] ; then
  echo '  The setup scripts had not been installed.'
  echo ''
  echo ' In the file :'
  echo '    <kit>/<vers>/setup.sh'
  echo ' someone has to replace '
  echo '    @@absolute_dir@@'
  echo ' by the absolute path of the package.'
  echo ''
  echo '  If having the write access, this replacement can be done'
  echo ' on the setup scripts by using the install script :'
  echo '    cd <install_path>/<kit>/<vers>'
  echo '    ./install'
  echo ''
  return
fi

# The "." is for the OpenPAW on-the-fly compilation and loading.
if [ `uname` = "Linux" ]; then
  lib_path="${OSC_home}/lib:${OSC_home}/bin:."
  if [ -z "${LD_LIBRARY_PATH}" ] ; then
    LD_LIBRARY_PATH="${lib_path}"
    export LD_LIBRARY_PATH
  else
    osc_status=1;(echo $LD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && osc_status=0
    if [ $osc_status != 0 ] ; then
      LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${lib_path}"
      export LD_LIBRARY_PATH
    fi
    unset osc_status
  fi
  unset lib_path
fi

if [ `uname` = "Darwin" ]; then
  lib_path="${OSC_home}/lib:${OSC_home}/bin:."
  if [ -z "${DYLD_LIBRARY_PATH}" ] ; then
    DYLD_LIBRARY_PATH="${lib_path}"
    export DYLD_LIBRARY_PATH
  else
    osc_status=1;(echo $DYLD_LIBRARY_PATH | grep "${lib_path}" > /dev/null  ) && osc_status=0
    if [ $osc_status != 0 ] ; then
      DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${lib_path}"
      export DYLD_LIBRARY_PATH
    fi
    unset osc_status
  fi
  unset lib_path
fi

bin_path="${OSC_home}/bin"
if [ -z "${PATH}" ] ; then
  PATH="${bin_path}"
  export PATH
else
  osc_status=1;(echo $PATH | grep "${bin_path}" > /dev/null  ) && osc_status=0
  if [ $osc_status != 0 ] ; then
    PATH="${PATH}:${bin_path}"
    export PATH
  fi
  unset osc_status
fi
unset bin_path

OSC_HOME_DIR="${OSC_home}";export OSC_HOME_DIR

# AIDA :
AIDA_HOME_DIR="${OSC_home}";export AIDA_HOME_DIR

# else :
COIN_DONT_INFORM_INDIRECT_RENDERING="1"
export COIN_DONT_INFORM_INDIRECT_RENDERING

OBUILD_PATH="${OSC_home}/Resources";export OBUILD_PATH

unset OSC_home

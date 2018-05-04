#
#  Usage :
#    CYGWIN-sh> . <osc_kit path>/aida-setup.sh
#

if [ "`uname | grep CYGWIN`" = "" ] ; then
  echo "WARNING : you are using a script for CYGWIN whilst not under CYGWIN."
fi

#set -x

OSC_home="@@absolute_dir@@"

if [ "${OSC_home}" = '@@absolute_dir@@' ] ; then
  echo '  The aida-setup scripts had not been installed.'
  echo ''
  echo ' In the file :'
  echo '    <kit>/<vers>/aida-setup.sh'
  echo ' someone has to replace '
  echo '    @@absolute_dir@@'
  echo ' by the absolute path of the package.'
  echo ''
  echo '  If having the write access, this replacement can be done'
  echo ' on the aida-setup scripts by using the install script :'
  echo '    cd <install_path>/<kit>/<vers>'
  echo '    ./install'
  echo ''
  return
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

# convert path to a Windows path :
OSC_HOME_DIR="`cygpath -w ${OSC_home}`";export OSC_HOME_DIR

# AIDA :
AIDA_HOME_DIR="${OSC_HOME_DIR}";export AIDA_HOME_DIR

# else :
COIN_DONT_INFORM_INDIRECT_RENDERING="1"
export COIN_DONT_INFORM_INDIRECT_RENDERING

OBUILD_PATH="${OSC_HOME_DIR}\Resources";export OBUILD_PATH

unset OSC_home

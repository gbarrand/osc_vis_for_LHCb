
#
#  Usage :
#    sh> . <osc_vis_path>/Qt-setup.sh
#

#set -x

Qt_home="none"
if [ "`uname -n | grep lxplus`" != "" ] ; then
  if [ `uname -m` = "x86_64" ] ; then
    Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.4.2/slc4_amd64_gcc34"
  else
    Qt_home="/afs/cern.ch/sw/lcg/external/qt/4.4.2/slc4_ia32_gcc34"
  fi
else
  if [ `uname` = Darwin ] ; then
    Qt_home=/Library/Frameworks
  fi
fi

if [ "${Qt_home}" != "none" ]; then

if [ `uname` = "Linux" ]; then
  lib_path="${Qt_home}/lib"
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

  lib_path="${Qt_home}/Qt3Support.framework:${Qt_home}/QtCore.framework:${Qt_home}/QtGui.framework:${Qt_home}/QtOpenGL.framework:${Qt_home}/QtNetWork.framework:${Qt_home}/QtAssistant.framework:${Qt_home}/QtDBus.framework:${Qt_home}/QtDesigner.framework:${Qt_home}/QtDesignerComponents.framework:${Qt_home}/QtHelp.framework:${Qt_home}/QtScript.framework:${Qt_home}/QtScriptTools.framework:${Qt_home}/QtSql.framework:${Qt_home}/QtSvg.framework:${Qt_home}/QtTest.framework:${Qt_home}/QtWebKit.framework:${Qt_home}/QtXml.framework:${Qt_home}/QtXmlPatterns.framework"

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

bin_path="${Qt_home}/bin"
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

fi

unset Qt_home


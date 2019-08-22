
setenv BUILD_CONFIG LCG_96_gcc8

# gcc :
source /cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/8.2.0-3fa06/x86_64-centos7/setup.csh

# Python :
set python_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/Python/2.7.16-e553a/x86_64-centos7-gcc8-opt
setenv LD_LIBRARY_PATH "${python_home}/lib:${LD_LIBRARY_PATH}"
setenv PATH "${python_home}/bin:${PATH}"

# Qt :
set qt_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.12.1-d6e2e/x86_64-centos7-gcc8-opt
setenv LD_LIBRARY_PATH "${qt_home}/lib:${LD_LIBRARY_PATH}"

setenv QT_PLUGIN_PATH "${qt_home}/plugins"
setenv QT_GRAPHICSSYSTEM_CHECKED 1
setenv QT_GRAPHICSSYSTEM native

setenv QTDIR "${qt_home}"
setenv QTINC "${qt_home}/include"
setenv QTLIB "${qt_home}/lib"

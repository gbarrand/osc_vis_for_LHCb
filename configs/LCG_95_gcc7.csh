
setenv BUILD_CONFIG LCG_95_gcc7

# gcc :
source /cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/7.3.0-cb1ee/x86_64-centos7/setup.csh

# Python :
set python_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/Python/2.7.15-c333c/x86_64-centos7-gcc7-opt
setenv LD_LIBRARY_PATH "${python_home}/lib:${LD_LIBRARY_PATH}"
setenv PATH "${python_home}/bin:${PATH}"

# Qt :
set qt_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.11.1-88bfc/x86_64-centos7-gcc7-opt
setenv LD_LIBRARY_PATH "${qt_home}/lib:${LD_LIBRARY_PATH}"
setenv QT_PLUGIN_PATH "${qt_home}/plugins"
setenv QT_GRAPHICSSYSTEM_CHECKED 1
setenv QT_GRAPHICSSYSTEM native
setenv QTDIR "${qt_home}"
setenv QTINC "${qt_home}/include"
setenv QTLIB "${qt_home}/lib"

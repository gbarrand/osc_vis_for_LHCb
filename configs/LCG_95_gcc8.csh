setenv BUILD_CONFIG LCG_95_gcc8

# gcc :
source /cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/8.2.0-3fa06/x86_64-centos7/setup.csh

# Python :
set python_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/Python/2.7.15-c333c/x86_64-centos7-gcc8-opt
setenv LD_LIBRARY_PATH "${python_home}/lib:${LD_LIBRARY_PATH}"
setenv PATH "${python_home}/bin:${PATH}"

# Qt :
set qt_home=/cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.11.1-88bfc/x86_64-centos7-gcc8-opt
setenv LD_LIBRARY_PATH "${qt_home}/lib:${LD_LIBRARY_PATH}"
setenv LD_LIBRARY_PATH "${qt_home}/plugins/platforms:${LD_LIBRARY_PATH}"

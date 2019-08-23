
setenv BUILD_CONFIG LCG_95_gcc7

# gcc :
source /cvmfs/lhcb.cern.ch/lib/lcg/releases/gcc/7.3.0-cb1ee/x86_64-centos7/setup.csh

# Qt and Python for building are defined for this BUILD_CONFIG in :
#   inexlib/exlib/mgr/use_Python
#   inexlib/exlib/mgr/use_Qt
# Qt and Python for running are defined in the final packing in :
#  <osc_path>/<version>/[Qt,py]-setup.[csh.sh]

# They should point to :
#   /cvmfs/lhcb.cern.ch/lib/lcg/releases/qt5/5.11.1-88bfc/x86_64-centos7-gcc7-opt
#   /cvmfs/lhcb.cern.ch/lib/lcg/releases/Python/2.7.15-c333c/x86_64-centos7-gcc7-opt


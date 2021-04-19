export BUILD_CONFIG=LCG97apython3_gcc9

# gcc :
.  /cvmfs/sft.cern.ch/lcg/views/LCG_97apython3/x86_64-centos7-gcc9-opt/setup.sh

# Qt and Python for building are defined for this BUILD_CONFIG in :
#   inexlib/exlib/mgr/use_Python
#   inexlib/exlib/mgr/use_Qt
# Qt and Python for running are defined in the final packing in :
#  <osc_path>/<version>/[Qt,py]-setup.[csh.sh]

# They should point to :
#  /cvmfs/sft.cern.ch/lcg/releases/qt5/5.12.4-d91b2/x86_64-centos7-gcc9-opt
#  /cvmfs/sft.cern.ch/lcg/releases/Python/3.7.6-b96a9/x86_64-centos7-gcc9-opt

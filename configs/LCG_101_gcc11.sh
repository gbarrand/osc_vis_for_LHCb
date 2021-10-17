
export BUILD_CONFIG=LCG_101_gcc11

#.  /cvmfs/sft.cern.ch/lcg/views/LCG_97apython3/x86_64-centos7-gcc9-opt/setup.sh

# gcc :
#. /cvmfs/sft.cern.ch/lcg/releases/gcc/10.1.0-6f386/x86_64-centos7/setup.sh
. /cvmfs/sft.cern.ch/lcg/views/LCG_101/x86_64-centos7-gcc11-opt/setup.sh

# gtk :
export PKG_CONFIG_PATH="${PKG_CONFIG_PATH}:/usr/lib64/pkgconfig"

# Qt and Python for building are defined for this BUILD_CONFIG in :
#   inexlib/exlib/mgr/use_Python
#   inexlib/exlib/mgr/use_Qt
# Qt and Python for running are defined in the final packing in :
#  <osc_path>/<version>/[Qt,py]-setup.[csh.sh]

# They should point to :
#  /cvmfs/sft.cern.ch/lcg/releases/qt5/5.12.4-d91b2/x86_64-centos7-gcc9-opt
#  /cvmfs/sft.cern.ch/lcg/releases/Python/3.7.6-b96a9/x86_64-centos7-gcc9-opt

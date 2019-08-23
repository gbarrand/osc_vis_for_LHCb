
#
#  Usage :
#    csh> source <osc_vis_path>/Qt-setup.csh
#


set gcc_home=none

if ( -f "${gcc_home}/setup.csh" ) then

source ${gcc_home}/setup.csh

else
  echo "${gcc_home}/setup.csh not found."
endif

unset gcc_home

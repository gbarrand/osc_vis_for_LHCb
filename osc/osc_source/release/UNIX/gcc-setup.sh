
#
#  Usage :
#    sh> . <osc_vis_path>/gcc-setup.sh
#

gcc_home=none

if [ -f "${gcc_home}/setup.sh" ] ; then

. ${gcc_home}/setup.sh
    
else
  echo "${gcc_home}/setup.sh not found."
fi

unset gcc_home


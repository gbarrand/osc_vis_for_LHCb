#
#  Usage :
#    CYGWIN-csh> source <osc_kit path>/aida-setup.csh
#

if ( "`uname | grep CYGWIN`" == "" ) then
  echo "WARNING : you are using a script for CYGWIN whilst not under CYGWIN."
endif

#set verbose

set OSC_home="@@absolute_dir@@"

if ( "${OSC_home}" == '@@absolute_dir@@' ) then
  echo '  The aida-setup scripts had not been installed.'
  echo ''
  echo ' In the file :'
  echo '    <kit>/<vers>/obuild/aida-setup.csh'
  echo ' someone has to replace '
  echo '    @@absolute_dir@@'
  echo ' by the absolute path of the package.'
  echo ''
  echo '  If having the write access, this replacement can be done'
  echo ' on the aida-setup scripts by using the install script :'
  echo '    cd <install_path>/<kit>/<vers>'
  echo '    ./install'
  echo ''
  exit
endif

set bin_path="${OSC_home}/bin"
set bin_curr=`printenv PATH`
if ( "${bin_curr}" == "" ) then
 setenv PATH "${bin_path}"
else
 if ( `echo "${bin_curr}" | grep "${bin_path}" ` == "" ) then
   setenv PATH "${PATH}:${bin_path}"
 endif
endif
unset bin_curr
unset bin_path

# convert path to a Windows path :
setenv OSC_HOME_DIR "`cygpath -w ${OSC_home}`"

# AIDA :
setenv AIDA_HOME_DIR "${OSC_HOME_DIR}"

# else :
setenv COIN_DONT_INFORM_INDIRECT_RENDERING "1"

setenv OBUILD_PATH "${OSC_HOME_DIR}\Resources"

unset OSC_home

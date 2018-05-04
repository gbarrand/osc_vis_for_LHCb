#
#  Usage :
#    csh> source aida-setup.csh
#

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

# The "." is for the OpenPAW on-the-fly compilation and loading.
if ( `uname` == "Linux" ) then
  set lib_path="${OSC_home}/lib:${OSC_home}/bin:."
  set lib_curr=`printenv LD_LIBRARY_PATH`
  if ( "${lib_curr}" == "" ) then
    setenv LD_LIBRARY_PATH "${lib_path}"
  else
    if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
      setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${lib_path}"
    endif
  endif
  unset lib_curr
  unset lib_path
endif

if ( `uname` == "Darwin" ) then
  set lib_path="${OSC_home}/lib:${OSC_home}/bin:."
  set lib_curr=`printenv DYLD_LIBRARY_PATH`
  if ( "${lib_curr}" == "" ) then
    setenv DYLD_LIBRARY_PATH "${lib_path}"
  else
    if ( `echo "${lib_curr}" | grep "${lib_path}" ` == "" ) then
      setenv DYLD_LIBRARY_PATH "${DYLD_LIBRARY_PATH}:${lib_path}"
    endif
  endif
  unset lib_curr
  unset lib_path
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

setenv OSC_HOME_DIR "${OSC_home}"

# AIDA :
setenv AIDA_HOME_DIR "${OSC_home}"

# else :
setenv COIN_DONT_INFORM_INDIRECT_RENDERING "1"

setenv OBUILD_PATH "${OSC_home}/Resources"

unset OSC_home

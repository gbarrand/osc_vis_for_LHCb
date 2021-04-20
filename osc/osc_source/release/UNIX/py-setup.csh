
#
#  Usage :
#    csh> source <osc_vis_path>/py-setup.csh
#

#set verbose
set Python_home=none
set Python_exe=py_exe

if ( -d "${Python_home}" ) then

if ( `uname` == "Linux" ) then
  set lib_path="${Python_home}/lib"
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

set bin_path="${Python_home}/bin"
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

else
  echo "directory ${Python_home} not found."
endif

unset Python_home

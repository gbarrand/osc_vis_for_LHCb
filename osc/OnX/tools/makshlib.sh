#!/bin/sh -f
#set -x
############################################
if test $# -ge 1 ; then
libname=$1
lklibs=$2
bindir=`pwd`
liba=$bindir/$libname.a
############################################
##### build libs ###########################
############################################
if test -f $liba ; then

if test `uname` = "Linux" ; then
#  Run time environment variable LD_LIBRARY_PATH 
# equal to a colon separated list of directories.
#  If you are superuser, use the tool ldconfig to 
# declare the created shared lib to the system and then 
# avoid the use of LD_LIBRARY_PATH.
#set -x
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.so
/bin/rm -f $libso
# get .o files
/bin/rm -R -f tmp
mkdir tmp
cd tmp
ar x $liba
cd ..
objs=`find tmp -name "*.o" -print`
g++ -shared -o $libso $objs $lklibs
/bin/rm -R -f tmp
cd $cpath
echo "$libso built."
fi

if test `uname` = "Darwin" ; then
#set -x
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.dylib
/bin/rm -f $libso
# get .o files
/bin/rm -R -f tmp
mkdir tmp
cd tmp
ar x $liba
cd ..
objs=`find tmp -name "*.o" -print`
c++ -dynamiclib -twolevel_namespace -undefined dynamic_lookup -dynamic -single_module -o $libso $objs $lklibs
/bin/rm -R -f tmp
cd $cpath
echo "$libso built."
fi

if test `uname` = "OSF1" ; then
#  Run time environment variable _RLD_LIST 
# equal to a colon separated list of so libs.
#  -lcxxstd needed due to the fact that /usr/shlib/libstdcxx.so do not exist
# and then, when running things with shlibs, some entry point (std type_info)
# are going to be undefined.
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.so
/bin/rm -f $libso
/bin/rm -f so_locations
ld -expect_unresolved '*' -shared -o $libso -all $liba -rpath $rpath -lcxxstd
cd $cpath
echo "$libso built."
fi

if test `uname` = "IRIX" ; then
#  Run time environment variable _RLD_LIST  
# equal to a colon separated list of so libs.
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.so
/bin/rm -f $libso
/bin/rm -f so_locations
ld -shared -o $libso -all $liba
cd $cpath
echo "$libso built."
fi

if test `uname` = "IRIX64" ; then
# Same as upper.
# equal to a colon separated list of so libs.
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.so
/bin/rm -f $libso
/bin/rm -f so_locations
#ld -64 -shared -o $libso -all $liba
cd $cpath
echo "$libso built."
fi

if test `uname` = "HP-UX" ; then
#  Binaries must be loaded with the option +s.
#  Run time environment variable SHLIB_PATH 
# equal to a colon separated list of directories.
#set -x
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.sl
/bin/rm -f $libso
# get .o files
if test ! -d tmp ; then mkdir tmp ; fi
cd tmp;/bin/rm -f *.o;ar x ../$liba;cd ..
objs=`ls tmp/*.o`
sys=`uname -r`
name=`echo $sys | awk '{name=substr($0,1,1);print name;}'`
if test $name = "A" ; then
ld -b -a shared -o $libso $objs
else
ld -b +h $libso -a shared -o $libso $objs
fi
/bin/rm -f tmp/*.o
/bin/rmdir -f tmp
cd $cpath
echo "$libso built."
fi

if test `uname` = "SunOS" ; then
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
libso=$rpath/$libname.so
/bin/rm -f $libso
# get .o files
if test ! -d tmp ; then mkdir tmp ; fi
cd tmp;/bin/rm -f *.o;ar x $liba;cd ..
objs=`ls tmp/*.o`
ld -G -o $libso $objs
/bin/rm -f tmp/*.o
/bin/rmdir tmp
cd $cpath
echo "$libso built."
fi

if test `uname` = "AIX" ; then
# Take care, on AIX the sh suffix is also .a !!!
#set -x
cpath=`pwd`
cd $bindir
rpath=`pwd`
rpath=`echo $rpath | sed -e 's%/tmp_mnt%%'`
# get .o files
if test ! -d tmp ; then mkdir tmp ; fi
cd tmp;/bin/rm -f *.o;ar x ../$liba;cd ..
objs=`ls tmp/*.o`
libso=$rpath/$libname.so
/bin/rm -f $libso
/usr/lpp/xlC/bin/makeC++SharedLib -p 0 -o $libso $objs
/bin/mv $liba $bindir/$libname.arc
/bin/mv $libso $liba
/bin/rm -f tmp/*.o
/bin/rmdir tmp
cd $cpath
echo "$libso built."
fi

fi
############################################
else
echo 'Give name of binaray directory and library'
fi

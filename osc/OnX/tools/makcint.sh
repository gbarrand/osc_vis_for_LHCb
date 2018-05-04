#!/bin/sh -f
#set -x
#
# From CINT/source/init.c :
# -p : to use a preprocessor to handle macros.
#      The preprocessor is defined in $CINTROOT/usr/MAKEINFO
# -z<NAME> : set project name.
# -n<NAME> : customize G__cpplink file name.
# -N<NAME> : customize DLL identification name.
# -A : C++ mode lock.
# -w<NUMBER>: set Windows-NT DLL mode.
# -c-1 : create global variable & function information from C++ header file.
#
# Things to wrap are put in source/CINT_i.h :
#
# CINTSYSDIR must be set so that $CINTROOT/usr/include/*h are found :

if test -z $CINTROOT ;
then
  echo "setup not executed."
else
  cint_exe=$CINTROOT/$CINTCONFIG/CINT_sh.exe
fi

#cint_exe=/usr/local/ROOT/4.00.08/bin/cint

# -I. is to hook some dummy string include file.

#incs="-I. -I../include -I$LIBROOT/include $ONX_GLFLAGS -I$COINGLROOT/include -I$HEPVISROOT/include"

incs="-I. -I../include -I$LIBROOT/include $ONX_GLFLAGS"
#echo $incs

${cint_exe} -p -w0 -zOnXCint -nCintWrap.ic -NOnXCintWrap -D__MAKECINT__  $incs -c-1 -A ../source/CINT/CINT_i.h

#awk -f cint.awk CintWrap.ic > ../source/CintWrap.ic
mv CintWrap.ic ../source/CINT/CintWrap.ic

/bin/rm -f CintWrap.ic
mv CintWrap.h  ../source/CINT/.

touch  ../source/CINT/CintWrap.cxx

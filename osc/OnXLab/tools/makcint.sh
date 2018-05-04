#!/bin/sh -f
#set -x

# From CINT/source/init.c :
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

# -I. is to hook some dummy string, vector include files.

incs="-I. -I../include -I$AIDAROOT/src/cpp -I$LIBROOT/include"
#echo $incs

${cint_exe} -p -w0 -zOnXCint -nCintWrap.ic -NOnXLabCintWrap -D__MAKECINT__  $incs -c-1 -A ../source/CINT/CINT_i.h

awk -f cint.awk CintWrap.ic > ../source/CINT/CintWrap.ic

/bin/rm -f CintWrap.ic
mv CintWrap.h  ../source/CINT/.

touch  ../source/CINT/CintWrap.cxx

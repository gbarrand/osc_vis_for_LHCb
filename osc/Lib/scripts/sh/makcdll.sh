#!/bin/sh -f
#set -x

bin=
FILE_NAME=$1
SUFFIX=$2
WRAP_NAME=$3

#echo ${FILE_NAME}
#echo ${WRAP_NAME}

if [ "${SLASHROOT}" = "" ] ; then
  echo "Lib : makcdll.sh : environment variable SLASHROOT not defined."
  exit
fi

/bin/rm -f ${bin}${FILE_NAME}.o
/bin/rm -f ${bin}${WRAP_NAME}.o

cxx_compiler=""
if test `uname` = "Linux" ; then
  cxx_compiler="g++ -fPIC"
elif test `uname` = "Darwin" ; then
  cxx_compiler=c++
elif [ "`uname | grep CYGWIN`" != "" ] ; then
  cxx_compiler=g++
elif test `uname` = "OSF1" ; then
  cxx_compiler=cxx
fi

if [ "${cxx_compiler}" = "" ] ; then
  echo "Lib : makcdll.sh : c++ compiler not found."
  exit
fi

libs=""

${cxx_compiler} -c -o ${bin}${FILE_NAME}.o ${FILE_NAME}.${SUFFIX}
if [ $? != 0 ] ; then
  echo "Lib : makcdll.sh : compilation of ${FILE_NAME}.${SUFFIX} failed."
  exit
fi

${cxx_compiler} -c -I"${SLASHROOT}" -o ${bin}${WRAP_NAME}.o ${WRAP_NAME}.cpp
if [ $? != 0 ] ; then
  echo "Lib : makcdll.sh : compilation of ${WRAP_NAME}.cpp failed."
  exit
fi

if test `uname` = "Linux" ; then
/bin/rm -f ${bin}lib${FILE_NAME}.so
g++ -shared -o ${bin}lib${FILE_NAME}.so ${bin}${WRAP_NAME}.o ${bin}${FILE_NAME}.o ${libs}
fi

if test `uname` = "Darwin" ; then
/bin/rm -f ${bin}${FILE_NAME}.bundle
c++ -bundle -twolevel_namespace -o ${bin}${FILE_NAME}.bundle ${bin}${WRAP_NAME}.o ${bin}${FILE_NAME}.o ${libs}
fi

if [ "`uname | grep CYGWIN`" != "" ] ; then
/bin/rm -f ${bin}${FILE_NAME}.dll
g++ -shared -o ${bin}${FILE_NAME}.dll ${bin}${WRAP_NAME}.o ${bin}${FILE_NAME}.o ${libs}
fi

if test `uname` = "OSF1" ; then
/bin/rm -f ${bin}lib${FILE_NAME}.so
cxx -shared -o ${bin}lib${FILE_NAME}.so ${bin}${WRAP_NAME}.o ${bin}${FILE_NAME}.o ${libs}
fi

/bin/rm -f ${bin}${FILE_NAME}.o
/bin/rm -f ${bin}${WRAP_NAME}.o

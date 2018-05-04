#!/bin/sh -f

cp ../../include/OnX/Xt/OpenGLArea.h .
cp ../../source/Xt/OpenGLAreaP.h .
cp ../../source/Xt/OpenGLArea.c .

sed -e 's#<OnX/Xt/OpenGLArea.h>#"OpenGLArea.h"#g' OpenGLAreaP.h > tmp;mv tmp OpenGLAreaP.h

if [ `uname` == Darwin ] ; then

# It aassume that OpenMotif is installed through Fink.

g++ -I. -I/sw/include -I/usr/X11R6/include OnX_test_Xt_OpenGLArea.cxx  OpenGLArea.c -L/sw/lib -lXm -L/usr/X11R6/lib -lGL -lXt -lX11

#g++ -I/sw/include -I/usr/X11R6/include OnX_test_Xm.cxx  -L/sw/lib -lXm -L/usr/X11R6/lib -lXt -lX11

fi

if [ `uname` == Linux ] ; then

g++ -I. -I/usr/X11R6/include OnX_test_Xt_OpenGLArea.cxx  OpenGLArea.c -L/usr/X11R6/lib -lGL -lXm -lXt -lX11

#g++ -I/usr/X11R6/include OnX_test_Xm.cxx  -L/usr/X11R6/lib -lXm -lXt -lX11

fi

#/bin/rm OpenGLArea.h
#/bin/rm OpenGLAreaP.h
#/bin/rm OpenGLArea.c

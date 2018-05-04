@echo off
rem ##############################################
rem #
set swig=E:\lal\SWIG\jswig1.2a1\NT\swig.exe
rem #
rem # tcl :
copy ..\source\SWIG.i .
%swig% -tcl8 -c++ -I../source SWIG.i
copy SWIG_wrap.c ..\source\TclWrap.ic
del SWIG.i
rem # del SWIG_wrap.doc
del SWIG_wrap.c
rem #
rem # java :
rem # copy ..\source\SWIG.i .
rem # %swig% -java -c++ -I../source SWIG.i
rem # copy SWIG_wrap.c ..\include\OnXLab\Java.ic
rem # copy OnXLab.java ..\source\OnXLab.java
rem # del SWIG.i
rem # del SWIG_wrap.doc
rem # del SWIG_wrap.c
rem # del OnXLab.java
rem #
rem #

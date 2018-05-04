rem 
rem #makecint -mk Makefile -o Test -H Test.h -C++ Test.cxx
rem 
%CINTHOME%\%CINTCONFIG%\cint -w0 -zLabCINT -nCINT.ic -NLabCINT -D__MAKECINT__  -I..\include -c-1 -A ../include/Lab/CINT_i.h
copy CINT.ic ..\include\Lab\CINT.ic
copy CINT.h ..\include\Lab\CINT.h
rem #
del CINT.ic
del CINT.h
rem #

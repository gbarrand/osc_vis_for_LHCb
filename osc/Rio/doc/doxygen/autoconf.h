/**

@page autoconf

@subsection autoconf_unix Installing with autoconf on UNIX

@verbatim
     UNIX> unzip <package>-<version>.zip
    (UNIX> rm <package>-<version>.zip)
     UNIX> cd <package>/<version>/configure
     UNIX> autoconf
       to produce the configure shell script from configure.in.

     UNIX> ./configure [--prefix=<install dir>]
       to produce the Makefile from the Makefile.in.
       The distrib comes with a configure script in case autoconf
      is not available or does not work properly.
       Someone can use the option --prefix so that "gmake install" 
      will install in the <install dir> directory. By default the 
      <install dir> is "/usr/local".

     UNIX> gmake
       to reconstruct libRio.so, libRio.a, Rio_test.exe
      When doing gmake, the binaries are put in the ../<config dir>  
      directory which is build with : `uname`-<compiler>. For example :
       Linux-c++
       Linux-KCC
       OSF1-cxx
       SunOS-CC

     UNIX> gmake install
       to install in the <install dir> directory.
      By default things are installed in /usr/local.

@endverbatim

  Testing the installation :

@verbatim
     sh> LD_LIBRARY_PATH=<install dir>/lib; export $LD_LIBRARY_PATH
   or :
    csh> setenv LD_LIBRARY_PATH <install dir>/lib
     so that libRio.so shared lib is found at run time.

   UNIX> ../<config dir>/Rio_test.exe -create
     to create the hello.root file.
   UNIX> ../<config dir>/Rio_test.exe -read
     to read the hello.root file.
     The option "-verbose" can be added to have some verbosity.
@endverbatim

*/

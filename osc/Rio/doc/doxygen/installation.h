/**

@page installation

 To install Rio your need :
 - an ANSI C++ compiler (supporting namespace, STL).
 - UNIX or Windows.
 - On UNIX, CMT or autoconf and gmake ; on Windows, CMT and/or VisualC++.

@section installation_cmt Installing with CMT

 CMT can be found at http://www.lal.in2p3.fr/SI/CMT/CMT.htm.

 Each OpenScientist package contains a cmt directory that holds a 
 "requirements" file describing what to reconstruct for the package 
 and the relationships to other packages.

@b Installation @b UNIX

@verbatim
     UNIX> unzip <package>-<version>.zip
    (UNIX> rm <package>-<version>.zip)
     UNIX> cd <package>/<version>/cmt
      csh> source <path>/CMT/<version>/mgr/setup.csh
    (  sh> . <path>/CMT/<version>/mgr/setup.sh )
    (UNIX> cmt show uses    # To see needed packages )
     UNIX> edit the requirements file if necessary.
     UNIX> cmt config       # To produce makefiles, setup files,etc...
      csh> source setup.csh # To set the <package> environment. 
    (  sh> . setup.sh )
     UNIX> (g)make          # To build according the default CMT "tag"
    (UNIX> (g)make tag=Linux_KCC #  To build with another tag, for example Linux_KCC)
@endverbatim

 The reconstruction must build in the "tag" directory 
 the libraries and some applications, tests programs.
 If the OS supports them, shared libs are also produced.

  Run with :

@verbatim
     UNIX> ../<tag>/<application>.exe
@endverbatim

@b Installation @b Windows

  Use some interactive facility : WinZip, 
   PowerArchiver (http://www.powerarchiver.com), 
 PowerZip) to decompress and extract the files.

  Being on Windows, get a Windows (DOS) "command prompt"
 by double clicking in :
@verbatim
     "Program manager/Main/Command prompt".
@endverbatim
  Then type :
@verbatim
     DOS> cd <package>\<version>\cmt
     DOS> call <path>\CMT\<version>\mgr\setup.bat
    (DOS> cmt show uses      # To see needed packages )
     DOS> edit the requirement file if necessary
     DOS> cmt config         # To produce makefiles, setup files,etc...
     DOS> call setup.bat     # To set environment.
     DOS> nmake /f NMake     # To build with the default CMT "tag"
@endverbatim
  The reconstruction must build in the parallel "tag" directory 
 the libraries and some applications.

  Run with :
@verbatim
     DOS> ..\<tag>\<application>.exe
 or simply (if "path" is correctly incremented by setup script) : 
     DOS> <application>
@endverbatim

@section installation_autoconf_unix Installing with autoconf on UNIX

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

@section installation_visual_net Installation with .Net (then on Windows)

  Use some interactive facility : WinZip, 
   PowerArchiver (http://www.powerarchiver.com), 
 PowerZip) to decompress and extract the files.

  From the Microsoft Developer Studio .Net interactive application, open
 the Rio/mdsnet/Rio.sln workspace with "File/Open project".

  Reconstruct first the Rio dll by selecting 
 the Rio project in the "solutions" tree and issuing 
 a "Build/build Rio".

  Reconstruct the Rio_test.exe by selecting 
 the Rio_test project in ther "solution" tree and issuing a 
 "Build/build Rio_test.exe".

@b Testing

  Open a DOS prompt. Then :
@verbatim
   DOS> cd <path>\Rio\<version>\[Win32, Win32Debug] 
   DOS> Rio_test.exe -create
    to create the hello.root file.
   DOS> Rio_test.exe -read
    to read the hello.root file.
@endverbatim

@b Notes
  The Rio.sln file came from a conversion of the 
 Rio/&lt;version&gt;/mds/Rio.dsw file by .Net. 

  The Rio.dll is built by using the "old" .def technique.
 A classical archive, Rio.arc, is first produced. Then the 
 owindef.exe program (coming with the distrib) is used to 
 analyse it in order to produce a Rio.def used by link.exe
 in order to produce the Rio.dll.

  The Rio.dsp file had been modified by hand in order to 
 do that (see "# Begin Custom Build" section in Rio.dsp
 in case of problem). 

  (I know Bill, we must never do that ; 
 but having the "declspec" everywhere in the code is really 
 a pain).

@section installation_visual_6 Installation with VisualC++-6 (then on Windows)

  Use some interactive facility : WinZip, 
   PowerArchiver (http://www.powerarchiver.com), 
 PowerZip) to decompress and extract the files.

  From the Microsoft Developer Studio interactive application, open
 the Rio/mds/Rio.dsw workspace with "File/Open workspace".

  Reconstruct first the Rio dll by selecting 
 "Project/Select active project/Rio" and issuing a "Build/build Rio.arc".

  Reconstruct the Rio_test.exe by selecting 
 "Project/Select active project/Rio_test" and issuing a 
 "Build/build Rio_test.exe".

@b Testing

  Open a DOS prompt. Then :
@verbatim
   DOS> cd <path>\Rio\<version>\[Win32, Win32Debug] 
   DOS> Rio_test.exe -create
    to create the hello.root file.
   DOS> Rio_test.exe -read
    to read the hello.root file.
@endverbatim

@b Notes
  The Rio.dll is built by using the "old" .def technique.
 A classical archive, Rio.arc, is first produced. Then the 
 owindef.exe program (coming with the distrib) is used to 
 analyse it in order to produce a Rio.def used by link.exe
 in order to produce the Rio.dll.

  The Rio.dsp file had been modified by hand in order to 
 do that (see "# Begin Custom Build" section in Rio.dsp
 in case of problem). 

  (I know Bill, we must never do that ; 
 but having the "declspec" everywhere in the code is really 
 a pain).

*/

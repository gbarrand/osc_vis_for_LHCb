/**

@page cmt

@subsection cmt_tool Installing with the CMT tool

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

*/

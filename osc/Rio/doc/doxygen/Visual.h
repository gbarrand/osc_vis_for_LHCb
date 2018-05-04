/**

@page visual

@subsection visual_installation Installation with VisualC++ (then on Windows)

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

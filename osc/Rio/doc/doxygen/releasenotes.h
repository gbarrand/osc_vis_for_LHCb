/**

@page releasenotes

@section releasenotes_v5r0 v5r0 (Nov 2003) 
  Add Array, ArrayInteger, ArrayFloat, ArrayDouble to ease streamer writing.
 Add the Graf directory with the AttLine, AttMarker, AttFill classes to ease
 the writing of the TTree streamers. Here we burden the bad design of ROOT in
 which people had put an inheritance of graphic things on the file manager
 classes (TTree). How to make understand these people that an IO package
 is not a graphic package ? Have a checkSum method on the Rio::IClass.
 Handle the check sum in the various dictionaries in order to be compatible
 with ROOT when mimicking the streaming of some ROOT classes (like the 
 TH* in the OpenScientist/Lab package).

@section releasenotes_v4r0 v4r0 (July 2003) 
  Buffer navigation protection. In particular the read/write functions
 in Rio/Core/tobuf.h had been protected against overflow navigation. The same
 for read/write methods in Rio/Core/Buffer. They return a false bool in case
 of failure. All streamers had been protected in the same way.
 Someone can now protect from basic IOs up to high level read/write of objects
 in a simple way by using the bool return status of methods. No "carpet hiding"
 exceptions had been used.

@section releasenotes_v3r0 v3r0 (September 2002) 
  REWRITING of the ROOT IO logic. Real birth of Rio as a standalone package.

*/

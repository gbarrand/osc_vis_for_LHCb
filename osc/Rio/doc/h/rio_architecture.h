/**

@page rio_architecture Architecture, Driving rules

  No "g" logic (that is to say no global pointers). In ROOT, the fact 
 that classes may see other classes through global pointers clearly 
 breaks the encapsulation. Any classes in ROOT can then establish 
 relationship to any other classes in an untrackable way. (We even 
 don't speak of relationship established by using the CINT interpreter
 (string relationship)). In Rio, the relationship are established 
 only by using inheritance, encapsulation and methods arguments.

  Do not provide, in Rio itself, an automatic dictionary production
 by using some interpreter. This will come in other packages (RioCint,
 RioJava, RioPython).

  Have Rio handles file IO only. Networking, etc... is another problem.
 Then, in Rio, the relationship to the operating system specific things 
 is minimal (it is concentrated in the Core/File.cxx file and concerns 
 mainly the C functions : open, close, read, write, lseek). Then 
 configuration and installation is straighforward.

  Use interfaces (that is to say pure abstract classes) to decouple things.
 In Rio, the interfaces are used for the moment to decouple the 
 dictionary from the rest.

  Use STL. In particular do not reinvent string, list, vector, etc...
 classes.

  Avoid pointers as much as possible, use references.

  Use the "Rio" namespace. Have Rio::Xxx instead of TXxx (or RXxx).

  Have I<name> to name an interface (IClass, IObject).

  Use basic data types (int, short, double, float). Do not reinvent
 all the data types (Int_t, etc...)

  Have no Rio::IObject::draw() method. An IO package is NOT a drawing 
 package. This seems obvious, except for two men on this planet...

  Try to be ANSI C++. Code had been tested with five ANSI C++ compiler
 (g++, VisualC++, DEC/cxx, Sun/CC, KCC).

@section rio_architecture_org Code organization

  Under the "Rio" package directory.
@verbatim
     Rio/<version>/source/zip     : code for compression.
                         /Core    : code for the IO system.
                         /Tree    : code to handle the "Tree" class.
                  /Rio/Interfaces : interfaces (IXxx.h files).
                      /zip        : headers for the compression.
                      /Code       : headers for the core.
                      /Tree       : headers for the Tree code.

     Rio/<version>/tests     : for test programs.

     Rio/<version>/cmt       : for the CMT tool package manager.
                  /configure : for the autoconf manager, installer.
                  /mds       : for Microsoft Visual Studio manager.
                  /mdsnet    : for Microsoft Visual Studio .Net manager.

     Rio/<version>/doc : documentation.
@endverbatim

*/


#include <Slash/Core/ISession.h>
#include <Slash/Core/IWriter.h>

// CINT :
#include <G__ci.h>
extern "C" {
extern void G__set_p2fsetup(void (*p2f)());
extern void G__cpp_setupOnXLabCintWrap();
}

extern "C" {
void OnXLabCintInitialize(Slash::Core::ISession&);
void OnXLabCintFinalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
void OnXLabCintInitialize(
 Slash::Core::ISession&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  G__set_p2fsetup(G__cpp_setupOnXLabCintWrap);
}
//////////////////////////////////////////////////////////////////////////////
void OnXLabCintFinalize(
 Slash::Core::ISession&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //  We have to rm G__cpp_setupOnXLabCintWrap from
  // the CINT setup list before closeing the DLL.
  //  Else CINT will have a reference on a function
  // no more in the data space.
  //  As there is no G__rm_p2fsetup function we reset
  // everything assuming that the whole application is exiting.

  //FIXME : Should have a G__rm_p2fsetup(G__cpp_setupOnXLabCintWrap);
  G__free_p2fsetup();

  //  We have to remove all reference to OnXLab things in CINT
  // before closing the DLL, but as we do not know how to 
  // do that properly we cleanup everything, assuming that
  // the application is exiting.
  G__scratch_all(); //FIXME
}





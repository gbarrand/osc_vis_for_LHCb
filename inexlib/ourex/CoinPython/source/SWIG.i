%module CoinPython

%pythoncode %{        
import Coin_SWIG_Python as _coin
%}

/* From Pivy-0.3.0/coin.i : */
%define RefCount(...)
  %typemap(newfree) __VA_ARGS__ * { $1->ref(); }
  %extend __VA_ARGS__ { ~__VA_ARGS__() { self->unref(); } }
  %ignore __VA_ARGS__::~__VA_ARGS__();
%enddef

// Problem with g++-3.4.6 :
%ignore SbString::vsprintf(const char*,va_list);

//G.Barrand : problem with SWIG-1.3.33 and Python-2.5 :
//            In generated code the const is not at the right place :
//              SbMat const &
%ignore SbMatrix::SbMatrix(const SbMat & matrix);
%ignore SbMatrix::setValue(const SbMat & m);
%ignore SbDPMatrix::setValue(const SbDPMat & m);

// Included from Pivy-0.3.0 :
%include pivy_common_typemaps.i

%include iv_all.i

/*
// Put helpers as extensions :

%define cast_SoNode(aClass)
  aClass* cast_##aClass () {
    SoType soType = SoType::fromName(#aClass);
    if(self->isOfType(soType)==FALSE) return 0;
    return (aClass*)self;
  }
%enddef

%extend SoNode {
  cast_SoNode(SoSeparator);
  cast_SoNode(SoCylinder);
  cast_SoNode(SoCube);
}
*/

// To fix the "recursive" exception message over nodekits :
%extend SoBase {
%pythoncode %{
    def __getattr__(self,name):
      try: return object.__getattr__(self, name)
      except AttributeError as e: raise e
%}
}

// From Pivy-0.3.0/coin.i :
/* removes all the properties for fields in classes derived from
   SoFieldContainer. this makes way for the dynamic access to fields
   as attributes.

   Note: this has to be the last code in the pivy file, therefore it
   is after all other SWIG declarations!
*/
%pythoncode %{        
for x in list(locals().values()):  #G.Barrand : 3.6 : have list()
  if isinstance(x, type) and issubclass(x, SoFieldContainer):
    for name, thing in list(x.__dict__.items()):  #G.Barrand : 3.6 : have list()
      if isinstance(thing, property):
        delattr(x, name)
%}

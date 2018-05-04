%module OnX

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%include std_string.i
%include std_vector.i

%template() std::vector<std::string>;
%template() std::vector<double>;
%template() std::vector<int>;

#ifdef SWIGJAVA
%include java_typemaps.i
%apply ARG_STD_VECTOR_STD_STRING {std::vector<std::string>}
%apply ARG_CONST_STD_VECTOR_STD_STRING {const std::vector<std::string>&}
%apply RET_STD_VECTOR_STD_STRING {std::vector<std::string>}
%apply RET_CONST_STD_VECTOR_STD_STRING {const std::vector<std::string>&}

%apply ARG_STD_VECTOR_DOUBLE {std::vector<double>}
%apply ARG_CONST_STD_VECTOR_DOUBLE {const std::vector<double>&}
%apply RET_STD_VECTOR_DOUBLE {std::vector<double>}
%apply RET_CONST_STD_VECTOR_DOUBLE {const std::vector<double>&}

%apply ARG_STD_VECTOR_INT {std::vector<int>}
%apply ARG_CONST_STD_VECTOR_INT {const std::vector<int>&}
%apply RET_STD_VECTOR_INT {std::vector<int>}
%apply RET_CONST_STD_VECTOR_INT {const std::vector<int>&}
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

%include Slash.i

//NOTE : Lib::smanip::p2sx is used in LHCb/Panoramix
//       it is seen as OnX.smanip.p2sx.
//       If deprecating the below Lib.i, someone
//       as to provide an equivalent.

%include Lib.i

%include OnX.i

#ifdef SWIGPYTHON

// Use the more compleete CoinPython and the HEPVis Python wrapping.

// From Pivy-0.3.0/coin.i :
%define RefCount(...)
  %typemap(newfree) __VA_ARGS__ * { $1->ref(); }
  %extend __VA_ARGS__ { ~__VA_ARGS__() { self->unref(); } }
  %ignore __VA_ARGS__::~__VA_ARGS__();
%enddef

// Problem with g++-3.4.6 :
%ignore SbString::vsprintf(const char*,va_list);

%include pivy_common_typemaps.i
// WARNING : %import and not %include to not rewrap Inventor and HEPVis !
%import iv_all.i
%import HEPVis_all.i

//FIXME : why do we need to have the below ?
#include <Inventor/fields/SoFields.h>
class OnX_wrap_fields_dummy {
public:
  OnX_wrap_fields_dummy(){}
  ~OnX_wrap_fields_dummy(){}
public:
  SoSFVec3f dummy_orientationX;
  SoSFBool dummy_SoSFBool;
  SoSFBox3s dummy_SoSFBox3s;
  SoSFColor dummy_SoSFColor;
  SoSFEnum dummy_SoSFEnum;
  SoSFBitMask dummy_SoSFBitMask;
  SoSFFloat dummy_SoSFFloat;
  SoSFImage dummy_SoSFImage;
  SoSFImage3 dummy_SoSFImage3;
  SoSFInt32 dummy_SoSFInt32;
  SoSFMatrix dummy_SoSFMatrix;
  SoSFName dummy_SoSFName;
  SoSFNode dummy_SoSFNode;
  SoSFPath dummy_SoSFPath;
  SoSFPlane dummy_SoSFPlane;
  SoSFRotation dummy_SoSFRotation;
  SoSFShort dummy_SoSFShort;
  SoSFString dummy_SoSFString;
  SoSFTime dummy_SoSFTime;
  SoSFTrigger dummy_SoSFTrigger;
  SoSFUInt32 dummy_SoSFUInt32;
  SoSFUShort dummy_SoSFUShort;
  SoSFVec2f dummy_SoSFVec2f;
  SoSFVec2s dummy_SoSFVec2s;
  SoSFVec3d dummy_SoSFVec3d;
  SoSFVec3f dummy_SoSFVec3f;
  SoSFVec3s dummy_SoSFVec3s;
  SoSFVec4f dummy_SoSFVec4f;
  SoSFEngine dummy_SoSFEngine;

  SoMFBool dummy_SoMFBool;
  SoMFColor dummy_SoMFColor;
  SoMFEngine dummy_SoMFEngine;
  SoMFEnum dummy_SoMFEnum;
  SoMFBitMask dummy_SoMFBitMask;
  SoMFFloat dummy_SoMFFloat;
  SoMFInt32 dummy_SoMFInt32;
  SoMFMatrix dummy_SoMFMatrix;
  SoMFName dummy_SoMFName;
  SoMFNode dummy_SoMFNode;
  SoMFPath dummy_SoMFPath;
  SoMFPlane dummy_SoMFPlane;
  SoMFRotation dummy_SoMFRotation;
  SoMFShort dummy_SoMFShort;
  SoMFString dummy_SoMFString;
  SoMFTime dummy_SoMFTime;
  SoMFUInt32 dummy_SoMFUInt32;
  SoMFUShort dummy_SoMFUShort;
  SoMFVec2f dummy_SoMFVec2f;
  SoMFVec3d dummy_SoMFVec3d;
  SoMFVec3f dummy_SoMFVec3f;
  SoMFVec4f dummy_SoMFVec4f;
};

// From Pivy-0.3.0/coin.i :
// removes all the properties for fields in classes derived from
// SoFieldContainer. this makes way for the dynamic access to fields
// as attributes.
//
//  Note: this has to be the last code in the pivy file, therefore it
// is after all other SWIG declarations!

// WARNING : if x instead of xxx, then clash with OnX.x('<module>')
%pythoncode %{        
import CoinPython as Inventor
for xxx in locals().values():
  if isinstance(xxx, type) and issubclass(xxx, Inventor.SoFieldContainer):
    for name, thing in xxx.__dict__.items():
      if isinstance(thing, property):
        delattr(xxx, name)
%}

%extend Slash::UI::IUI {
  Slash::UI::ISoViewer* findSoViewer(const std::string& where) {
    return Slash::find_SoViewer(*self,where);   
  }
  SoNode* findSoNode(const std::string& where,const std::string& interest) { 
    return Slash::find_SoNode(*self,where,interest);
  }
  SoPage* findSoPage(const std::string& where) { 
    return Slash::find_SoPage(*self,where);
  }
  SoRegion* findSoRegion(const std::string& where) { 
    return Slash::find_SoRegion(*self,where);
  }
  SoPage* currentPage() { //deprecated
    return Slash::find_SoPage(*self,Slash::s_current());
  }
  //void dumpNode(SoNode& aNode) {
  //  SbString ss;
  //  aNode.get(ss);
  //  self->echo(ss.getString());
  //}
}

%inline {
// FIXME : The SoNode returned by ISoViewer::sceneGraph()
//         can't be casted to a SoSeparator with the pivy cast :
//           soSep = Inventor.cast(soSode,'SoSeparator')
//         See OnX/examples/Python/OnX_exa_so_examiner_viewer.py
SoSeparator* cast_SoSeparator(SoNode* aNode) {
  if(aNode->isOfType(SoSeparator::getClassTypeId())==FALSE) return 0;
  return (SoSeparator*)aNode;
}
}

#else

// Java, Tcl :

// Reduced Inventor wrapping :
%include Inventor.i

// Reduced HEPVis wrapping :
%include HEPVis.i

%extend SoNode {
  cast_SoNode(SoDisplayRegion);
  cast_SoNode(SoImageRegion);
  cast_SoNode(SoTextRegion);
  cast_SoNode(SoPlotterRegion);
}

%extend Slash::UI::IUI {
  SoPage* currentPage() { 
    return ui_SoPage(*self);
  }
  void dumpNode(SoNode& aNode) {
    SbString ss;
    aNode.get(ss);
    self->echo(ss.getString());
  }
}

#endif

%include Test.i

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#ifdef SWIGJAVA
%pragma(java) jniclasscode=%{
  static {
    try {
      System.loadLibrary("OnX_SWIG_Java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Can't load OnX_SWIG_Java jni library.");
    }
  }
%}
#endif

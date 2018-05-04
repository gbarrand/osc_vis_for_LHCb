//
//WARNING : do not put the below in the OnX_SWIG.i file
//  in some %{... %}. Else it is going to be after Python.h
//  and then compilation problem may happen on some platform.
//  (For example clash with Coin-2.x code on a slc3_gcc323).
//

//#include <OnX/SWIG/SWIG.h>

// Slash :
#include <Slash/IAll.h>
#include <Slash/Tools/Style.h>

#include <inlib/cast>

// for the embedded remaining Lib :
#include <inlib/system>
#include <inlib/file>
#include <inlib/pointer>
#include <inlib/osc_env>
#include <inlib/scolor>
#include <cstdio> //sprintf
#include "../Core/inlib" //smanip_lines
#include "../SWIG/Lib.h"

// Inventor : 
#include <CoinPython/Inventor.h>

// HEPVis :
#include <HEPVis/Utils.h>
#include <HEPVis/Detector.h>
#include <HEPVis/Geometry.h>
#include <HEPVis/Plotter.h>
#include <HEPVis/GUI.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/misc/SoStyleCache.h>

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

#include <Slash/Tools/Inventor.h>

//FIXME : python does not permit to pass a user tag when 
//        setting PyOS_InputHook. Then we need the below
//        static.
static Slash::UI::IUI* sUI = 0;
typedef int(*PyOS_InputHook_t)(void);
static PyOS_InputHook_t save_PyOS_InputHook = 0;
static int OnX_PyOS_InputHook(void) {
  //printf("debug : OnX_PyOS_InputHook : %lu\n",sUI);
  if(sUI) sUI->synchronize();
  return 0;
}

#if defined(__linux)
// Clash between os_defines.h (coming from <string>) and pyconfig.h
#if defined(_POSIX_C_SOURCE)
#undef _POSIX_C_SOURCE
#endif
#if defined(_XOPEN_SOURCE)
#undef _XOPEN_SOURCE
#endif
#endif

#include <Python.h>

#ifdef WIN32
#include <windows.h>
#undef max
#undef ERROR
#undef DELETE
#endif

#include "OnX_SWIG_Python.ic"

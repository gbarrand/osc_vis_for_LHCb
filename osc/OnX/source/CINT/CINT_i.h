//
//  This file is used by OnX/omake/makcint[.sh, .bat]
// to wrap C++ compiled code.
//
//  Put here the things that must be wrapped :
// - include file of class header.
//
// Strip off some Mesa specific OpenGL functions :

#include <OnX/CINT/TestCint.h>

//  Warning, in a typedef for a function,
// CINT wants a space between the returned 
// type and the opening parenthesis :
//   typedef void (*x)();  //ok
//   typedef void(*x)();   //CINT is out !

#ifdef __CINT__
namespace std {
class string {
public:
  string();
  string(const char*);
  string(const string&);
  virtual ~string();
  const char* c_str() const;
  int length() const;
  void resize(int);
  string& operator=(const string&);
  string& operator=(const char*);
  string& operator+=(const string&);
  string& operator+=(const char*);
  string& append(const string&);
  string& append(const char*);
};
template <class T> class vector {
public:
  vector();
  virtual ~vector();
  void push_back(const T&);
};
template <class T> class complex {
public:
  complex();
  virtual ~complex();
};
template <class T> class list {
public:
  list();
  virtual ~list();
};
template <class A, classB> class pair {
public:
  pair();
  virtual ~pair();
};
}
// Get the OnX/tools/string dummy file :
#include <string>
#include <complex>
#include <list>
#include <vector>
#endif

// Lib :
#include <Slash/Core/IWriter.h>
#include <Slash/Core/ISession.h>
#include <Lib/smanip.h>
#include <Lib/System.h>
#include <Lib/Randoms.h>
#include <Lib/Functions.h>
#include <Lib/Expression.h>
#include <Lib/Processor.h>
#include <Lib/AccessorManager.h>
#include <Lib/Variable.h>
#include <Lib/Property.h>
#include <Lib/Out.h>

// OnX :
#include <OnX/Interfaces/IPage.h>
#include <Slash/UI/IUI.h>
#include <OnX/Interfaces/IRegion.h>
#include <OnX/Interfaces/IRepresentation.h>
#include <OnX/Interfaces/IRepresenter.h>

extern "C" {Slash::Core::ISession* OnXCintGetSession();}

#ifdef __CINT__
extern "C" {Slash::Core::ISession* GetSession();}
#else
#define GetSession OnXCintGetSession
#endif

/* debug : Inventor & HEPVis :
#ifdef __CINT__
// From OnX.i :
class SoType {
};
class SoNode {
protected:
  SoNode(void);        
  virtual ~SoNode();
public:
  void setName(const char*);
  void set(const char*);
  virtual SoType getTypeId(void) const = 0;
};
class SoSeparator : public SoNode {
protected:
  virtual ~SoSeparator();
public:
  SoSeparator();
  void addChild(SoNode*);
  virtual SoType getTypeId(void) const;
};
class SoArray : public SoNode {
protected:
  virtual ~SoArray();
public:
  SoArray();
  void addChild(SoNode*);
  virtual SoType getTypeId(void) const;
};
#include "../source/SWIG/iv.i"
#include "../source/SWIG/hepvis.i"
#else
#include <OnX/Inventor/iv.h>
#include <OnX/Inventor/HEPVisWrap.h>
#endif
debug */

// Remove the Mesa specific OpenGL functions :
#ifdef __CINT__

#pragma link off function glBlendEquationEXT ;
#pragma link off function glBlendColorEXT ;
#pragma link off function glPolygonOffsetEXT ;
#pragma link off function glVertexPointerEXT;
#pragma link off function glNormalPointerEXT;
#pragma link off function glColorPointerEXT;
#pragma link off function glIndexPointerEXT;
#pragma link off function glTexCoordPointerEXT;
#pragma link off function glEdgeFlagPointerEXT;
#pragma link off function glGetPointervEXT;
#pragma link off function glArrayElementEXT;
#pragma link off function glDrawArraysEXT;
#pragma link off function glGenTexturesEXT;
#pragma link off function glDeleteTexturesEXT;
#pragma link off function glBindTextureEXT;
#pragma link off function glPrioritizeTexturesEXT;
#pragma link off function glAreTexturesResidentEXT;
#pragma link off function glIsTextureEXT;
#pragma link off function glTexImage3DEXT;
#pragma link off function glTexSubImage3DEXT;
#pragma link off function glCopyTexSubImage3DEXT;
#pragma link off function glColorTableEXT;
#pragma link off function glColorSubTableEXT;
#pragma link off function glGetColorTableEXT;
#pragma link off function glGetColorTableParameterfvEXT;
#pragma link off function glGetColorTableParameterivEXT;
#pragma link off function glActiveTextureARB;
#pragma link off function glClientActiveTextureARB;
#pragma link off function glMultiTexCoord1dARB;
#pragma link off function glMultiTexCoord1dvARB;
#pragma link off function glMultiTexCoord1fARB;
#pragma link off function glMultiTexCoord1fvARB;
#pragma link off function glMultiTexCoord1iARB;
#pragma link off function glMultiTexCoord1ivARB;
#pragma link off function glMultiTexCoord1sARB;
#pragma link off function glMultiTexCoord1svARB;
#pragma link off function glMultiTexCoord2dARB;
#pragma link off function glMultiTexCoord2dvARB;
#pragma link off function glMultiTexCoord2fARB;
#pragma link off function glMultiTexCoord2fvARB;
#pragma link off function glMultiTexCoord2iARB;
#pragma link off function glMultiTexCoord2ivARB;
#pragma link off function glMultiTexCoord2sARB;
#pragma link off function glMultiTexCoord2svARB;
#pragma link off function glMultiTexCoord3dARB;
#pragma link off function glMultiTexCoord3dvARB;
#pragma link off function glMultiTexCoord3fARB;
#pragma link off function glMultiTexCoord3fvARB;
#pragma link off function glMultiTexCoord3iARB;
#pragma link off function glMultiTexCoord3ivARB;
#pragma link off function glMultiTexCoord3sARB;
#pragma link off function glMultiTexCoord3svARB;
#pragma link off function glMultiTexCoord4dARB;
#pragma link off function glMultiTexCoord4dvARB;
#pragma link off function glMultiTexCoord4fARB;
#pragma link off function glMultiTexCoord4fvARB;
#pragma link off function glMultiTexCoord4iARB;
#pragma link off function glMultiTexCoord4ivARB;
#pragma link off function glMultiTexCoord4sARB;
#pragma link off function glMultiTexCoord4svARB;
#pragma link off function glPointParameterfEXT;
#pragma link off function glPointParameterfvEXT;
#pragma link off function glBlendFuncSeparateINGR;
#pragma link off function glWindowPos2iMESA;
#pragma link off function glWindowPos2sMESA;
#pragma link off function glWindowPos2fMESA;
#pragma link off function glWindowPos2dMESA;
#pragma link off function glWindowPos2ivMESA;
#pragma link off function glWindowPos2svMESA;
#pragma link off function glWindowPos2fvMESA;
#pragma link off function glWindowPos2dvMESA;
#pragma link off function glWindowPos3iMESA;
#pragma link off function glWindowPos3sMESA;
#pragma link off function glWindowPos3fMESA;
#pragma link off function glWindowPos3dMESA;
#pragma link off function glWindowPos3ivMESA;
#pragma link off function glWindowPos3svMESA;
#pragma link off function glWindowPos3fvMESA;
#pragma link off function glWindowPos3dvMESA;
#pragma link off function glWindowPos4iMESA;
#pragma link off function glWindowPos4sMESA;
#pragma link off function glWindowPos4fMESA;
#pragma link off function glWindowPos4dMESA;
#pragma link off function glWindowPos4ivMESA;
#pragma link off function glWindowPos4svMESA;
#pragma link off function glWindowPos4fvMESA;
#pragma link off function glWindowPos4dvMESA;
#pragma link off function glResizeBuffersMESA;
#pragma link off function glDrawRangeElements;
#pragma link off function glTexImage3D;
#pragma link off function glTexSubImage3D;
#pragma link off function glCopyTexSubImage3D;
#pragma link off function glColorTable;
#pragma link off function glColorSubTable;
#pragma link off function glColorTableParameteriv;
#pragma link off function glColorTableParameterfv;
#pragma link off function glCopyColorSubTable;
#pragma link off function glCopyColorTable;
#pragma link off function glGetColorTable;
#pragma link off function glGetColorTableParameterfv;
#pragma link off function glGetColorTableParameteriv;
#pragma link off function glBlendEquation;
#pragma link off function glBlendColor;
#pragma link off function glHistogram;
#pragma link off function glResetHistogram;
#pragma link off function glGetHistogram;
#pragma link off function glGetHistogramParameterfv;
#pragma link off function glGetHistogramParameteriv;
#pragma link off function glMinmax;
#pragma link off function glResetMinmax;
#pragma link off function glGetMinmax;
#pragma link off function glGetMinmaxParameterfv;
#pragma link off function glGetMinmaxParameteriv;
#pragma link off function glConvolutionFilter1D;
#pragma link off function glConvolutionFilter2D;
#pragma link off function glConvolutionParameterf;
#pragma link off function glConvolutionParameterfv;
#pragma link off function glConvolutionParameteri;
#pragma link off function glConvolutionParameteriv;
#pragma link off function glCopyConvolutionFilter1D;
#pragma link off function glCopyConvolutionFilter2D;
#pragma link off function glGetConvolutionFilter;
#pragma link off function glGetConvolutionParameterfv;
#pragma link off function glGetConvolutionParameteriv;
#pragma link off function glSeparableFilter2D;
#pragma link off function glGetSeparableFilter;
#pragma link off function glLockArraysEXT;
#pragma link off function glUnlockArraysEXT;
#pragma link off function glHintPGI;
#pragma link off function gluPickMatrix;
#pragma link off function gluQuadricCallback;
#pragma link off function gluNurbsCallback;
#pragma link off function gluCheckExtension;

// There is a mess between Mesa-3.2, Mesa-3.4 
// about the below. Simply do not bind the Tess things.
#pragma link off function gluNewTess;
#pragma link off function gluDeleteTess;
#pragma link off function gluTessVertex;
#pragma link off function gluTessProperty;
#pragma link off function gluTessNormal;
#pragma link off function gluTessCallback;
#pragma link off function gluTessBeginPolygon;
#pragma link off function gluTessEndContour;
#pragma link off function gluTessBeginContour;
#pragma link off function gluTessEndPolygon;
#pragma link off function gluBeginPolygon;
#pragma link off function gluEndPolygon;
#pragma link off function gluGetTessProperty;
#pragma link off function gluNextContour;

// To handle namespace :
#pragma link C++ nestedclass;

#endif

#ifndef __CINT__
#if defined(WIN32) || defined(__KCC) || defined(__sun) || (defined(__GNUC__) && (__GNUC__ >= 3))
using namespace std;
#endif
#endif

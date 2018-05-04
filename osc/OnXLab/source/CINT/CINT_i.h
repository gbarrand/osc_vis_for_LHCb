//
//  This file is used by OnXLab/tools/makcint[.sh, .bat]
// to wrap C++ compiled code.
//
//  Put here the things that must be wrapped :
// - include file of class header.
//

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
}
// Get the OnX/tools/string dummy file :
#include <string>
#include <complex>
#include <list>
#include <vector>
#endif

// Lib :
#include <Lib/Function.h>
#include <Lib/Out.h>
#include <Lib/smanip.h>

// OnXLab :

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/IFunction.h>
#include <AIDA/IFunctionFactory.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITree.h>
#include <AIDA/IAxis.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IHistogram3D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>
#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>
#include <AIDA/IPlotterFactory.h>
#include <AIDA/IFitter.h>
#include <AIDA/IFitResult.h>
#include <AIDA/IFitFactory.h>

// CINT is not able to do dynamic casting.
#include <Slash/Core/IManager.h>
extern "C" {
AIDA::IAnalysisFactory* Slash::Core::IManager_to_IAnalysisFactory(Slash::Core::IManager*);
AIDA::ITree* Slash::Core::IManager_to_ITree(Slash::Core::IManager*);
AIDA::IHistogram1D* IManagedObject_to_IHistogram1D(AIDA::IManagedObject*);
AIDA::IHistogram2D* IManagedObject_to_IHistogram2D(AIDA::IManagedObject*);
}


#ifdef __CINT__
// To handle namespace :
#pragma link C++ nestedclass ;
#endif

#ifndef __CINT__
#if defined(WIN32) || defined(__KCC) || defined(__sun) || (defined(__GNUC__) && (__GNUC__ >= 3))
using namespace std;
#endif
#endif

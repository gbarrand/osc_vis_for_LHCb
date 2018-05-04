/********************************************************************
* CintWrap.h
********************************************************************/
#ifdef __CINT__
#error CintWrap.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#include "G__ci.h"
extern "C" {
extern void G__cpp_setup_tagtableOnXCintWrap();
extern void G__cpp_setup_inheritanceOnXCintWrap();
extern void G__cpp_setup_typetableOnXCintWrap();
extern void G__cpp_setup_memvarOnXCintWrap();
extern void G__cpp_setup_globalOnXCintWrap();
extern void G__cpp_setup_memfuncOnXCintWrap();
extern void G__cpp_setup_funcOnXCintWrap();
extern void G__set_cpp_environmentOnXCintWrap();
}


#include "../source/CINT/CINT_i.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__OnXCintWrapLN_Slash::Core::IWriter;
extern G__linked_taginfo G__OnXCintWrapLN_TestCint;
extern G__linked_taginfo G__OnXCintWrapLN_string;
extern G__linked_taginfo G__OnXCintWrapLN_Lib;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLsmanip;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLsmanipcLcLwhat;
extern G__linked_taginfo G__OnXCintWrapLN_bool;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlElonggR;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlEdoublegR;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlEstringgR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLSystem;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLRandomFlat;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLRandomGauss;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLRandomBreitWigner;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLVariable;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLVariablecLcLeType;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLExpression;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLExpressioncLcLeType;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLExpressioncLcLeFunction;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlELibcLcLVariablegR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLLexer;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLProcessor;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLGaussian;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLBreitWigner;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLExponential;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLPolynomial;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLArrayFunction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLArray2DFunction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLComplexArrayFunction;
extern G__linked_taginfo G__OnXCintWrapLN_complexlEdoublegR;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlEcomplexlEdoublegRsPgR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLScriptedFunction;
extern G__linked_taginfo G__OnXCintWrapLN_Slash::Core::IManager;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLProperty;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIIterator;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIAccessor;
extern G__linked_taginfo G__OnXCintWrapLN_vectorlELibcLcLPropertygR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLHandler;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIProcessor;
extern G__linked_taginfo G__OnXCintWrapLN_listlELibcLcLHandlergR;
extern G__linked_taginfo G__OnXCintWrapLN_listlELibcLcLIAccessormUgR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLAccessorManager;
extern G__linked_taginfo G__OnXCintWrapLN_listlELibcLcLIActionmUgR;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLCollectAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLFilterAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLDumpAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLNumberAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLGetValuesAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLDestroyAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLSetAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLCalculateAction;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLPropertycLcLTYPE;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLOut;
extern G__linked_taginfo G__OnXCintWrapLN_Slash::Core::ISession;
extern G__linked_taginfo G__OnXCintWrapLN_IRegion;
extern G__linked_taginfo G__OnXCintWrapLN_IPage;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIMessage;
extern G__linked_taginfo G__OnXCintWrapLN_LibcLcLIListener;
extern G__linked_taginfo G__OnXCintWrapLN_Slash::UI::ICyclic;
extern G__linked_taginfo G__OnXCintWrapLN_Slash::UI::ICallbackData;
extern G__linked_taginfo G__OnXCintWrapLN_Slash::UI::IUI;
extern G__linked_taginfo G__OnXCintWrapLN_IRepresentation;
extern G__linked_taginfo G__OnXCintWrapLN_IRepresenter;

/* STUB derived class for protected member access */
typedef vector<long> G__vectorlElonggR;
typedef vector<double> G__vectorlEdoublegR;
typedef vector<string> G__vectorlEstringgR;
typedef vector<Lib::Variable> G__vectorlELibcLcLVariablegR;
typedef complex<double> G__complexlEdoublegR;
typedef vector<complex<double> > G__vectorlEcomplexlEdoublegRsPgR;
typedef vector<Lib::Property> G__vectorlELibcLcLPropertygR;
typedef list<Lib::Handler> G__listlELibcLcLHandlergR;
typedef list<Slash::Data::IAccessor*> G__listlELibcLcLIAccessormUgR;
typedef list<Slash::Core::IAction*> G__listlELibcLcLIActionmUgR;

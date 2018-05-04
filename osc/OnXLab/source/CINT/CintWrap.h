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
extern void G__cpp_setup_tagtableOnXLabCintWrap();
extern void G__cpp_setup_inheritanceOnXLabCintWrap();
extern void G__cpp_setup_typetableOnXLabCintWrap();
extern void G__cpp_setup_memvarOnXLabCintWrap();
extern void G__cpp_setup_globalOnXLabCintWrap();
extern void G__cpp_setup_memfuncOnXLabCintWrap();
extern void G__cpp_setup_funcOnXLabCintWrap();
extern void G__set_cpp_environmentOnXLabCintWrap();
}


#include "../source/CINT/CINT_i.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__OnXLabCintWrapLN_string;
extern G__linked_taginfo G__OnXLabCintWrapLN_Slash::Core::IWriter;
extern G__linked_taginfo G__OnXLabCintWrapLN_Lib;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLVariable;
extern G__linked_taginfo G__OnXLabCintWrapLN_bool;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLVariablecLcLeType;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLExpression;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLExpressioncLcLeType;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLExpressioncLcLeFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlELibcLcLVariablegR;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLLexer;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlEstringgR;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLProcessor;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlEdoublegR;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLGaussian;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLBreitWigner;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLExponential;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLPolynomial;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLArrayFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLArray2DFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLComplexArrayFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_complexlEdoublegR;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlEcomplexlEdoublegRsPgR;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLScriptedFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLOut;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLsmanip;
extern G__linked_taginfo G__OnXLabCintWrapLN_LibcLcLsmanipcLcLwhat;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlElonggR;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDA;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIDataPointSetFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFitFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFunctionFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIHistogramFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIPlotterFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITree;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITreeFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITupleFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIAnalysisFactory;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIBaseHistogram;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLICloud1D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLICloud2D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLICloud3D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIHistogram1D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIHistogram2D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIHistogram3D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIProfile1D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIProfile2D;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIEvaluator;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFilter;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITuple;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlEAIDAcLcLITuplemUgR;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIAnnotation;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFunction;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFunctionCatalog;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIManagedObject;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIAxis;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIAxiscLcLdA;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIHistogram;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIProfile;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLICloud;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITupleEntry;
extern G__linked_taginfo G__OnXLabCintWrapLN_vectorlEfloatgR;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIPlotterRegion;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITitleStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIPlotter;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIDataPointSet;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIInfo;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIPlotterLayout;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIPlotterStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIAxisStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIDataStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFillStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLILineStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIMarkerStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLITextStyle;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFitData;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFitParameterSettings;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFitResult;
extern G__linked_taginfo G__OnXLabCintWrapLN_AIDAcLcLIFitter;
extern G__linked_taginfo G__OnXLabCintWrapLN_Slash::Core::IManager;

/* STUB derived class for protected member access */
typedef vector<Lib::Variable> G__vectorlELibcLcLVariablegR;
typedef vector<string> G__vectorlEstringgR;
typedef vector<double> G__vectorlEdoublegR;
typedef complex<double> G__complexlEdoublegR;
typedef vector<complex<double> > G__vectorlEcomplexlEdoublegRsPgR;
typedef vector<long> G__vectorlElonggR;
typedef vector<AIDA::ITuple*> G__vectorlEAIDAcLcLITuplemUgR;
typedef vector<float> G__vectorlEfloatgR;

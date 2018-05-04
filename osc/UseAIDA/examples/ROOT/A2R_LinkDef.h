#ifdef __CINT__

//FIXME : there are some !

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
// To handle namespace :
#pragma link C++ nestedclass;

//////////////////////////////////////////////////
/// AIDA : ///////////////////////////////////////
//////////////////////////////////////////////////
#pragma link C++ function AIDA_createAnalysisFactory;

#pragma link C++ class AIDA::IAnalysisFactory-;
#pragma link C++ class AIDA::IAnnotation-;
#pragma link C++ class AIDA::IAxis-;
#pragma link C++ class AIDA::IAxisStyle-;
#pragma link C++ class AIDA::IBaseHistogram-;
#pragma link C++ class AIDA::IBaseStyle-;
#pragma link C++ class AIDA::IBrushStyle-;
#pragma link C++ class AIDA::ICloud-;
#pragma link C++ class AIDA::ICloud1D-;
#pragma link C++ class AIDA::ICloud2D-;
#pragma link C++ class AIDA::ICloud3D-;
#pragma link C++ class AIDA::IDataPoint-;
#pragma link C++ class AIDA::IDataPointSet-;
#pragma link C++ class AIDA::IDataPointSetFactory-;
#pragma link C++ class AIDA::IDataStyle-;
#pragma link C++ class AIDA::IEvaluator-;
#pragma link C++ class AIDA::IFillStyle-;
#pragma link C++ class AIDA::IFilter-;
//FIXME #pragma link C++ class AIDA::IFitData-;
#pragma link C++ class AIDA::IFitFactory-;
#pragma link C++ class AIDA::IFitParameterSettings-;
#pragma link C++ class AIDA::IFitResult-;
#pragma link C++ class AIDA::IFitter-;
#pragma link C++ class AIDA::IFunction-;
#pragma link C++ class AIDA::IFunctionCatalog-;
#pragma link C++ class AIDA::IFunctionFactory-;
#pragma link C++ class AIDA::IHistogram-;
#pragma link C++ class AIDA::IHistogram1D-;
#pragma link C++ class AIDA::IHistogram2D-;
#pragma link C++ class AIDA::IHistogram3D-;
#pragma link C++ class AIDA::IHistogramFactory-;
#pragma link C++ class AIDA::IInfo-;
#pragma link C++ class AIDA::IInfoStyle-;
#pragma link C++ class AIDA::ILineStyle-;
#pragma link C++ class AIDA::IManagedObject-;
#pragma link C++ class AIDA::IMarkerStyle-;
#pragma link C++ class AIDA::IMeasurement-;
#pragma link C++ class AIDA::IModelFunction-;
#pragma link C++ class AIDA::IPlotter-;
#pragma link C++ class AIDA::IPlotterFactory-;
#pragma link C++ class AIDA::IPlotterLayout-;
#pragma link C++ class AIDA::IPlotterRegion-;
#pragma link C++ class AIDA::IPlotterStyle-;
#pragma link C++ class AIDA::IProfile-;
#pragma link C++ class AIDA::IProfile1D-;
#pragma link C++ class AIDA::IProfile2D-;
#pragma link C++ class AIDA::IRangeSet-;
#pragma link C++ class AIDA::ITextStyle-;
#pragma link C++ class AIDA::ITitleStyle-;
#pragma link C++ class AIDA::ITree-;
#pragma link C++ class AIDA::ITreeFactory-;
#pragma link C++ class AIDA::ITuple-;
//FIXME #pragma link C++ class AIDA::ITupleFactory-;
#pragma link C++ class AIDA::ITupleEntry-;

// If working with AIDA-3.2.1 :
#pragma link C++ function cast_ITuple;

//////////////////////////////////////////////////
/// A2R : ////////////////////////////////////////
//////////////////////////////////////////////////
#pragma link C++ class ATH1D+;
#pragma link C++ class ATH2D+;

#endif


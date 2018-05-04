//////////////////////////////////////////////////////////////////////////////
/// Pre SWIG /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// The main entry point. Done in OnXLab_SWIG.i.
%ignore AIDA_createAnalysisFactory;

%ignore AIDA::IBaseHistogram::annotation() const;
%ignore AIDA::IDataPointSet::annotation() const;
%ignore AIDA::IDataPointSet::point(int) const;
%ignore AIDA::IFunction::annotation() const;
%ignore AIDA::ITuple::annotation() const;
%ignore AIDA::ITuple::getTuple(int) const;
%ignore AIDA::ITuple::findTuple(int) const;
%ignore AIDA::IDataPoint::coordinate(int) const;
#ifdef SWIGJAVA
//%ignore AIDA::ITuple::fill(int,long);
%rename(fill_long) fill(int,long);

#endif

// method returning unmanaged objects :
%newobject AIDA::ITupleFactory::createEvaluator;
%newobject AIDA::ITupleFactory::createFilter;
%newobject AIDA::IFitter::fit;
%newobject AIDA::IFitFactory::createFitter;
%newobject AIDA::IPlotterFactory::create;
%newobject AIDA::IAnalysisFactory::createTreeFactory;
%newobject AIDA::IAnalysisFactory::createHistogramFactory;
%newobject AIDA::IAnalysisFactory::createTupleFactory;
%newobject AIDA::IAnalysisFactory::createFunctionFactory;
%newobject AIDA::IAnalysisFactory::createPlotterFactory;
%newobject AIDA::IAnalysisFactory::createDataPointSetFactory;
%newobject AIDA::IAnalysisFactory::createFitFactory;

//////////////////////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

%include <AIDA/IAnalysisFactory.h>
%include <AIDA/IAnnotation.h>
%include <AIDA/IAxis.h>
%include <AIDA/IBaseHistogram.h>
%include <AIDA/ICloud.h>
%include <AIDA/ICloud1D.h>
%include <AIDA/ICloud2D.h>
%include <AIDA/ICloud3D.h>
%include <AIDA/IConstants.h>
%include <AIDA/IDataPoint.h>
%include <AIDA/IDataPointSet.h>
%include <AIDA/IDataPointSetFactory.h>
%include <AIDA/IEvaluator.h>
%include <AIDA/IFilter.h>
%include <AIDA/IFitData.h>
%include <AIDA/IFitFactory.h>
%include <AIDA/IFitParameterSettings.h>
%include <AIDA/IFitResult.h>
%include <AIDA/IFitter.h>
%include <AIDA/IFunction.h>
%include <AIDA/IFunctionCatalog.h>
%include <AIDA/IFunctionFactory.h>
%include <AIDA/IHistogram.h>
%include <AIDA/IHistogram1D.h>
%include <AIDA/IHistogram2D.h>
%include <AIDA/IHistogram3D.h>
%include <AIDA/IHistogramFactory.h>
%include <AIDA/IInfo.h>
%include <AIDA/IManagedObject.h>
%include <AIDA/IMeasurement.h>
%include <AIDA/IModelFunction.h>
%include <AIDA/IPlotter.h>
%include <AIDA/IPlotterFactory.h>
%include <AIDA/IPlotterLayout.h>
%include <AIDA/IPlotterRegion.h>
%include <AIDA/IProfile.h>
%include <AIDA/IProfile1D.h>
%include <AIDA/IProfile2D.h>
%include <AIDA/IRangeSet.h>
%include <AIDA/ITree.h>
%include <AIDA/ITreeFactory.h>
%include <AIDA/ITupleEntry.h>
%include <AIDA/ITuple.h>
%include <AIDA/ITupleFactory.h>

// Styles :
%include <AIDA/IBaseStyle.h>
%include <AIDA/IBrushStyle.h>
%include <AIDA/IDataStyle.h>
%include <AIDA/IFillStyle.h>
%include <AIDA/IInfoStyle.h>
%include <AIDA/ILineStyle.h>
%include <AIDA/ITextStyle.h>
%include <AIDA/ITitleStyle.h>
%include <AIDA/IAxisStyle.h>
%include <AIDA/IMarkerStyle.h>
%include <AIDA/IPlotterStyle.h>

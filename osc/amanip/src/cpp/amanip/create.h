#ifndef amanip_create_h
#define amanip_create_h

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>

#include <AIDA/IHistogramFactory.h>
#include <AIDA/IFunctionFactory.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/IDataPointSetFactory.h>
#include <AIDA/IFitFactory.h>
#include <AIDA/IPlotterFactory.h>

#include <AIDA/IFunction.h>

namespace amanip {

inline AIDA::ITree* create_tree(
 AIDA::IAnalysisFactory& aAIDA
,const std::string& aStoreName
,const std::string& aStoreType
,bool aReadOnly
,bool aCreateNew
,const std::string& aOptions = ""
){
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) return 0;
  AIDA::ITree* tree = 
    treeFactory->create(aStoreName,aStoreType,aReadOnly,aCreateNew,aOptions);
  delete treeFactory;
  return tree;
}

inline AIDA::IHistogram1D* create_histogram_1D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IHistogram1D* histo = 
      factory->createHistogram1D(aPath,aTitle,aXnumber,aXmin,aXmax);
  delete factory;
  return histo;
}

inline AIDA::IHistogram2D* create_histogram_2D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,int aYnumber
,double aYmin
,double aYmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IHistogram2D* histo = factory->createHistogram2D(aPath,aTitle,
    aXnumber,aXmin,aXmax,aYnumber,aYmin,aYmax);
  delete factory;
  return histo;
}

inline AIDA::IHistogram3D* create_histogram_3D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,int aYnumber
,double aYmin
,double aYmax
,int aZnumber
,double aZmin
,double aZmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IHistogram3D* histo = factory->createHistogram3D(aPath,aTitle,
    aXnumber,aXmin,aXmax,aYnumber,aYmin,aYmax,aZnumber,aZmin,aZmax);
  delete factory;
  return histo;
}

inline AIDA::IProfile1D* create_profile_1D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IProfile1D* histo = 
      factory->createProfile1D(aPath,aTitle,aXnumber,aXmin,aXmax);
  delete factory;
  return histo;
}

inline AIDA::IProfile1D* create_profile_1D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,double aVmin
,double aVmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IProfile1D* histo = 
    factory->createProfile1D(aPath,aTitle,aXnumber,aXmin,aXmax,aVmin,aVmax);
  delete factory;
  return histo;
}

inline AIDA::IProfile2D* create_profile_2D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,int aYnumber
,double aYmin
,double aYmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IProfile2D* histo = 
    factory->createProfile2D(aPath,aTitle,
                             aXnumber,aXmin,aXmax,
                             aYnumber,aYmin,aYmax);
  delete factory;
  return histo;
}

inline AIDA::IProfile2D* create_profile_2D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,int aYnumber
,double aYmin
,double aYmax
,double aVmin
,double aVmax
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IProfile2D* histo = 
    factory->createProfile2D(aPath,aTitle,
                             aXnumber,aXmin,aXmax,
                             aYnumber,aYmin,aYmax,
                             aVmin,aVmax);
  delete factory;
  return histo;
}

inline AIDA::ICloud1D* create_cloud_1D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aLimit = -1
,const std::string& aOptions = ""
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::ICloud1D* cloud = factory->createCloud1D(aPath,aTitle,aLimit,aOptions);
  delete factory;
  return cloud;
}

inline AIDA::ICloud2D* create_cloud_2D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aLimit = -1
,const std::string& aOptions = ""
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::ICloud2D* cloud = factory->createCloud2D(aPath,aTitle,aLimit,aOptions);
  delete factory;
  return cloud;
}

inline AIDA::IDataPointSet* create_data_point_set(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,int aDimension
){
  AIDA::IDataPointSetFactory* factory = aAIDA.createDataPointSetFactory(aTree);
  if(!factory) return 0;
  AIDA::IDataPointSet* dps = factory->create(aPath,aTitle,aDimension);
  delete factory;
  return dps;
}

inline AIDA::IFitter* create_fitter(
 AIDA::IAnalysisFactory& aAIDA
,const std::string& aEngine = ""
){
  AIDA::IFitFactory* factory = aAIDA.createFitFactory(); 
  if(!factory) return 0;
  AIDA::IFitter* fitter = factory->createFitter("",aEngine,"");
  delete factory;
  return fitter;
}

inline AIDA::IFunction* create_function(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,const std::string& aModel
){
  AIDA::IFunctionFactory* factory = aAIDA.createFunctionFactory(aTree); 
  if(!factory) return 0;
  AIDA::IFunction* function = factory->createFunctionByName(aPath,aModel);
  delete factory;
  if(function) function->setTitle(aTitle);
  return function;
}

inline AIDA::IFunction* create_scripted_function(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,int aDimension
,const std::string& aScript
){
  AIDA::IFunctionFactory* factory = aAIDA.createFunctionFactory(aTree); 
  if(!factory) return 0;
  AIDA::IFunction* function = 
    factory->createFunctionFromScript(aPath,aDimension,aScript,"","","");
  delete factory;
  return function;
}

inline AIDA::ITuple* create_tuple(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,const std::string& aTitle
,const std::string& aColumnNames
,const std::string& aOptions = ""
){
  AIDA::ITupleFactory* factory = aAIDA.createTupleFactory(aTree);
  if(!factory) return 0;
  AIDA::ITuple* tuple = factory->create(aPath,aTitle,aColumnNames,aOptions);
  delete factory;
  return tuple;
}

inline AIDA::IEvaluator* create_evaluator(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aExpression
){
  AIDA::ITupleFactory* factory = aAIDA.createTupleFactory(aTree);
  if(!factory) return 0;
  AIDA::IEvaluator* evaluator = factory->createEvaluator(aExpression);
  delete factory;
  return evaluator;
}

inline AIDA::IFilter* create_filter(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aExpression
){
  AIDA::ITupleFactory* factory = aAIDA.createTupleFactory(aTree);
  if(!factory) return 0;
  AIDA::IFilter* filter = factory->createFilter(aExpression);
  delete factory;
  return filter;
}

inline AIDA::IPlotter* create_plotter(
 AIDA::IAnalysisFactory& aAIDA
,const std::string& aWhere = ""
){
  AIDA::IPlotterFactory* factory = aAIDA.createPlotterFactory(); 
  if(!factory) return 0;
  AIDA::IPlotter* plotter = factory->create(aWhere);
  delete factory;
  return plotter;
}

inline std::string path_not_managed() {return "/not_managed";}

}

#endif

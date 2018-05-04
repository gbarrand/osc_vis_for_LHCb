#ifndef amanip_copy_h
#define amanip_copy_h

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IHistogram3D.h>

#include "cast.h"

namespace amanip {

inline AIDA::IBaseHistogram* copy_base_histogram(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,AIDA::IBaseHistogram& aHisto
){
  if(AIDA::IHistogram1D* object = 
       amanip::cast<AIDA::IHistogram1D>(aHisto,"AIDA::IHistogram1D")) {
    AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
    if(!factory) return 0;
    AIDA::IHistogram1D* cobject = factory->createCopy(aPath,*object);
    delete factory;
    return cobject;
  } else if(AIDA::IHistogram2D* object = 
       amanip::cast<AIDA::IHistogram2D>(aHisto,"AIDA::IHistogram2D")) {
    AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
    if(!factory) return 0;
    AIDA::IHistogram2D* cobject = factory->createCopy(aPath,*object);
    delete factory;
    return cobject;
  } else if(AIDA::IHistogram3D* object = 
       amanip::cast<AIDA::IHistogram3D>(aHisto,"AIDA::IHistogram3D")) {
    AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
    if(!factory) return 0;
    AIDA::IHistogram3D* cobject = factory->createCopy(aPath,*object);
    delete factory;
    return cobject;
  } else {
    return 0;
  }
}

inline AIDA::IHistogram1D* copy_histogram_1D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,AIDA::IHistogram1D& aHisto
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IHistogram1D* histo = factory->createCopy(aPath,aHisto);
  delete factory;
  return histo;
}

inline AIDA::IHistogram2D* copy_histogram_2D(
 AIDA::IAnalysisFactory& aAIDA
,AIDA::ITree& aTree
,const std::string& aPath
,AIDA::IHistogram2D& aHisto
){
  AIDA::IHistogramFactory* factory = aAIDA.createHistogramFactory(aTree);
  if(!factory) return 0;
  AIDA::IHistogram2D* histo = factory->createCopy(aPath,aHisto);
  delete factory;
  return histo;
}

}

#endif

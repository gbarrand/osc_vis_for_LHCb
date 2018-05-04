#ifndef amanip_find_h
#define amanip_find_h

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/IBaseHistogram.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>
#include <AIDA/IFunction.h>
#include <AIDA/ITuple.h>

#include "cast.h"

namespace amanip {

inline AIDA::ITree* find_mem(AIDA::IAnalysisFactory& aAIDA) {
  AIDA::ITreeFactory* treeFactory = aAIDA.createTreeFactory();
  if(!treeFactory) return 0;
  AIDA::ITree* tree = treeFactory->create();
  delete treeFactory;
  return tree;
}

template <class aT> 
inline aT* find(AIDA::ITree& aTree,const std::string& aPath,const std::string& aCast,bool aWarn,std::ostream& a_out) {
  AIDA::IManagedObject* mo = aTree.find(aPath);
  if(!mo) {
    if(aWarn) {
      a_out << "amanip::find :"
            << " \"" << aPath << "\" not found"
            << " in tree \"" << aTree.storeName() << "\"."
            << std::endl;
    }
    return 0;
  }
  aT* obj = amanip::cast<aT>(*mo,aCast);
  if(!obj) {
    a_out << "amanip::find :"
          << " \"" << aPath << "\" cast to"
          << " \"" << aCast << "\" failed."
          << std::endl;
  }
  return obj;
}

inline AIDA::IBaseHistogram* find_base_histogram(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IBaseHistogram>(aTree,aPath,"AIDA::IBaseHistogram",aWarn,a_out);
}

inline AIDA::IHistogram1D* find_histogram_1D(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IHistogram1D>
           (aTree,aPath,"AIDA::IHistogram1D",aWarn,a_out);
}

inline AIDA::IHistogram2D* find_histogram_2D(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IHistogram2D>(aTree,aPath,"AIDA::IHistogram2D",aWarn,a_out);
}

inline AIDA::IProfile1D* find_profile_1D(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IProfile1D>
           (aTree,aPath,"AIDA::IProfile1D",aWarn,a_out);
}

inline AIDA::IProfile2D* find_profile_2D(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IProfile2D>
           (aTree,aPath,"AIDA::IProfile2D",aWarn,a_out);
}

inline AIDA::IFunction* find_function(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::IFunction>(aTree,aPath,"AIDA::IFunction",aWarn,a_out);
}

inline AIDA::ITuple* find_tuple(AIDA::ITree& aTree,const std::string& aPath,bool aWarn = false,std::ostream& a_out = std::cout) {
  return find<AIDA::ITuple>(aTree,aPath,"AIDA::ITuple",aWarn,a_out);
}

}

#endif

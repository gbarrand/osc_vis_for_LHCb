#ifndef BatchLab_FindAllocator_h
#define BatchLab_FindAllocator_h

#include <BatchLab/Core/HistogramAllocator.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/Manager.h>

#include <inlib/cast>

namespace BatchLab {

inline BatchLab::HistogramAllocator* histogramAllocator(Slash::Core::ISession& aSession) {
  // Look for an HistogramAllocator, pick the first found :
  std::vector<std::string> facs;
 {unsigned int number = aSession.numberOfManagers();
  for(unsigned int index=0;index<number;index++) {
    const Slash::Core::IManager* mgr = aSession.manager(index);
    if(INLIB_CONST_CAST(*mgr,BatchLab::HistogramAllocator)) {
      facs.push_back(mgr->name());
    }
  }}
  if(!facs.size()) {
    std::ostream& out = aSession.cout();
    out << "BatchLab::histogramAllocator :" 
        << " can't find a BatchLab::HistogramAllocator." 
        << std::endl;
    return 0;
  }
  BatchLab::HistogramAllocator* allocator = 
    Slash_findManager(aSession,facs[0],BatchLab::HistogramAllocator);
  if(!allocator) {
    std::ostream& out = aSession.cout();
    out << "BatchLab::histogramAllocator :" 
        << " can't find a BatchLab::HistogramAllocator (2)." 
        << std::endl;
    return 0;
  }
  if(aSession.verboseLevel()) {
    std::ostream& out = aSession.cout();
    out << "BatchLab::histogramAllocator :" 
        << " HistogramAllocator is " << inlib::sout(facs[0]) 
        << std::endl;
  }
  return allocator;
}

}

#endif

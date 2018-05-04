#ifndef BatchLab_Rio_T_Reader_h
#define BatchLab_Rio_T_Reader_h

// Rio :
#include <Rio/Interfaces/IObject.h>

// BatchLab :
#include <BatchLab/Rio/ReadContext.h>

namespace BatchLab {
namespace Rio {
  template <class BatchLab_Rio_T,class BatchLab_T>
  Slash::Store::IObject* reader(Rio::ReadContext& aContext,bool& aDelete){
    aDelete = true;
    BatchLab_Rio_T* robject =  ::Rio::cast<BatchLab_Rio_T>(aContext.object());
    if(!robject) return 0;
    BatchLab_T* bobject = robject->data();
    if(!bobject) return 0; //Reading could fail.
    bobject->setName(aContext.SID());
    return bobject;
  }
} //Rio
} //BatchLab


#endif


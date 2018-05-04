
#include <AIDA/AIDA.h>
#include <AIDA/ITupleEntry.h>

//FIXME : CINT can't dynamic_cast, then we use the below function
// if working with AIDA-3.2.1 to cast a ITupleEntry to ITuple.

AIDA::ITuple* cast_ITuple(AIDA::ITupleEntry* aEntry) {
  return dynamic_cast<AIDA::ITuple*>(aEntry);
}

#include "A2R.h"

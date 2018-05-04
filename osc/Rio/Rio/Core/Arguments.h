#ifndef Rio_Arguments_h
#define Rio_Arguments_h

#include <Rio/Interfaces/typedefs.h>

namespace Rio {

#define RIO_ARGS_DIRECTORY 0
#define RIO_ARGS_TREE 1
#define RIO_ARGS_BRANCH 2

inline void Args_init(Arguments& aArgs){ 
  aArgs[0] = 0;
  aArgs[1] = 0;
  aArgs[2] = 0;
}
inline void Args_insertDirectory(Arguments& aArgs,void* aValue){ 
  aArgs[RIO_ARGS_DIRECTORY] = aValue;
}
inline void Args_insertTree(Arguments& aArgs,void* aValue){ 
  aArgs[RIO_ARGS_TREE] = aValue;
}
inline void Args_insertBranch(Arguments& aArgs,void* aValue){ 
  aArgs[RIO_ARGS_BRANCH] = aValue;
}
inline void* Args_findDirectory(const Arguments& aArgs) { 
  return aArgs[RIO_ARGS_DIRECTORY];
}
inline void* Args_findTree(const Arguments& aArgs) { 
  return aArgs[RIO_ARGS_TREE];
}
inline void* Args_findBranch(const Arguments& aArgs) { 
  return aArgs[RIO_ARGS_BRANCH];
}
inline void Args_assign(Arguments& aTo,const Arguments& aFrom) { 
  aTo[0] = aFrom[0];
  aTo[1] = aFrom[1];
  aTo[2] = aFrom[2];
}


}


#endif

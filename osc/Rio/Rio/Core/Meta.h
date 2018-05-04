#ifndef Rio_Meta_h
#define Rio_Meta_h

#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>

//WARNING : in the below line a_Class does not contain the namespaces.
#define RIO_META_HEADER(a_Class) \
public:\
  static const ::Rio::IClass& is(const ::Rio::IDictionary&);\
public:\
  virtual const ::Rio::IClass& isA() const {return fClass;}\
private:\
  const ::Rio::IClass& fClass;

//WARNING : in the below line a_Class contains the namespaces.
#define RIO_META_SOURCE(a_Class) \
const ::Rio::IClass& a_Class::is(const ::Rio::IDictionary& aDict){	\
  ::Rio::IClass* cls = aDict.findClass(#a_Class);\
  if(!cls) {\
    /*::printf("%s::is : ERROR : can't find class %s\n",#a_Class,#a_Class);*/ \
    return aDict.noneClass();\
  }\
  return *cls;\
}

namespace Rio {

inline void stringCheckSum(unsigned int& aId,const std::string& aName) {
  unsigned int l = aName.size();
  for (unsigned int i=0; i<l; i++) aId = aId*3+aName[i];
}

inline void arrayCheckSum(unsigned int& aId,int aNumber,int* aDims) {
  for (int i=0; i<aNumber; i++) aId = aId*3+(unsigned int)aDims[i];
}

}

#endif

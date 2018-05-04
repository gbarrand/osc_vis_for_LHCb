// this :
#include <Rio/Tree/Leaf.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Tree/Branch.h>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#define RIO_LEAF_SOURCE(aLeaf,aType)\
RIO_META_SOURCE(Rio::aLeaf)\
void* Rio::aLeaf::cast(const IClass& aClass) const{\
  if(&aClass==&fClass) \
    return Rio_SCast(aLeaf);\
  else \
    return BaseLeaf::cast(aClass);\
}\
bool Rio::aLeaf::stream(IBuffer& aBuffer) {\
  if(aBuffer.isReading()) {\
    short v;\
    unsigned int s,c;\
    if(!aBuffer.readVersion(v,s,c)) return false;\
    if(!BaseLeaf::stream(aBuffer)) return false;\
    if(!aBuffer.read(fMinimum)) return false;\
    if(!aBuffer.read(fMaximum)) return false;\
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;\
  } else {\
    unsigned int c;\
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;\
    if(!BaseLeaf::stream(aBuffer)) return false;\
    if(!aBuffer.write(fMinimum)) return false;\
    if(!aBuffer.write(fMaximum)) return false;\
    if(!aBuffer.setByteCount(c,true)) return false;\
  }\
  return true;\
}\
void Rio::aLeaf::printValue(std::ostream& aOut,int aIndex) const { \
  aOut << fValue[aIndex];\
}\
Rio::aLeaf::aLeaf(const IDictionary& aDictionary,Branch& aBranch)\
:Leaf< aType >(aDictionary,aBranch)\
,fClass(aDictionary.opt_##aLeaf##_Class())\
{}\
Rio::aLeaf::aLeaf(const IDictionary& aDictionary,Branch& aBranch,const std::string& aName)\
:Leaf< aType >(aDictionary,aBranch,aName)\
,fClass(aDictionary.opt_##aLeaf##_Class())\
{}\
Rio::aLeaf::~aLeaf(){}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_LEAF_SOURCE(LeafFloat,float)
RIO_LEAF_SOURCE(LeafDouble,double)
RIO_LEAF_SOURCE(LeafShort,short)
RIO_LEAF_SOURCE(LeafInteger,int)
RIO_LEAF_SOURCE(LeafCharacter,char)




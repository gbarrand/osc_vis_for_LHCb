// this :
#include <Rio/Tree/LeafElement.h>

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

RIO_META_SOURCE(Rio::LeafElement)
//////////////////////////////////////////////////////////////////////////////
Rio::LeafElement::LeafElement(
 const IDictionary& aDictionary
,Branch& aBranch
)
:BaseLeaf(aDictionary,aBranch)
,fClass(aDictionary.opt_LeafElement_Class())
,fID(0)
,fType(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafElement::LeafElement(
 const IDictionary& aDictionary
,Branch& aBranch
,const std::string& aName
,int aID
,int aType
)
:BaseLeaf(aDictionary,aBranch,aName)
,fClass(aDictionary.opt_LeafElement_Class())
,fID(aID)
,fType(aType)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafElement::~LeafElement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
int Rio::LeafElement::number(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafElement::valuePointer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return (void*)branch().address();
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafElement::fillBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  branch().out() << "Rio::LeafElement::fillBasket : dummy." << std::endl;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafElement::readBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  branch().out() << "Rio::LeafElement::readBasket : dummy." << std::endl;
  return false;
}

void Rio::LeafElement::printValue(std::ostream& aOut,int) const {
/*
  Out out(aPrinter);
  //out << (unsigned long)valuePointer() << endl;
  IObject* obj = object();
  if(obj)
    out << obj << " " << obj->isA().name() << " " << obj->name() << endl;
  else 
    out << obj << endl;
*/
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafElement::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  branch().setAddress(aAddress);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafElement::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(LeafElement);
  else 
    return BaseLeaf::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafElement::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aBuffer.isReading()) {
    short v;
    unsigned int s,c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!BaseLeaf::stream(aBuffer)) return false;
    if(!aBuffer.read(fID)) return false;
    if(!aBuffer.read(fType)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!BaseLeaf::stream(aBuffer)) return false;
    if(!aBuffer.write(fID)) return false;
    if(!aBuffer.write(fType)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}

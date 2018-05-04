// this :
#include <Rio/Core/ObjArray.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

RIO_META_SOURCE(Rio::ObjArray)

//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::ObjArray(
 const IDictionary& aDictionary
)
:Object(aDictionary)
,fClass(aDictionary.opt_ObjArray_Class())
,fNoNull(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Args_init(fArgs);
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::ObjArray(
 const IDictionary& aDictionary
,int aSize
)
:Object(aDictionary)
,std::vector<IObject*>((std::vector<IObject*>::size_type)aSize,0)
,fClass(aDictionary.opt_ObjArray_Class())
,fNoNull(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Args_init(fArgs);
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::ObjArray(
 const ObjArray& aFrom
)
:Object(aFrom)
,std::vector<IObject*>(aFrom)
,fClass(aFrom.fClass)
,fNoNull(aFrom.fNoNull)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Args_init(fArgs);
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray::~ObjArray(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::ObjArray& Rio::ObjArray::operator=(
 const ObjArray& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Object::operator=(aFrom);
  std::vector<IObject*>::operator=(aFrom);
  //fClass
  fNoNull = aFrom.fNoNull;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::ObjArray::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(ObjArray);
  else 
    return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::ObjArray::setReadArguments(
 const Arguments& aArgs
,bool aNoNull
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Args_assign(fArgs,aArgs);
  fNoNull = aNoNull;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::ObjArray::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream all objects in the collection to or from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!Object::stream(aBuffer)) return false;
    std::string name;
    if(!aBuffer.read(name)) return false;
    int nobjects;
    if(!aBuffer.read(nobjects)) return false;
    int lowerBound;
    if(!aBuffer.read(lowerBound)) return false;
    clear();
    for (int i = 0; i < nobjects; i++) {
      IObject* obj;
      if(!aBuffer.readObject(fArgs,obj)) {
        aBuffer.out() << "Rio::ObjArray::stream : can't read object." 
                      << std::endl;
        return false;
      }
      if(fNoNull) {
        if(obj) push_back(obj);
      } else {
        push_back(obj);
      }
    }
    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!Object::stream(aBuffer)) return false;
    if(!aBuffer.write(std::string(""))) return false;
    int nobjects = size();
    if(!aBuffer.write(nobjects)) return false;
    //debug int lowerBound = 211;
    int lowerBound = 0;
    if(!aBuffer.write(lowerBound)) return false;
    
    std::vector<IObject*>::const_iterator it;
    for(it=begin();it!=end();++it) {
      if(!aBuffer.writeObject(*it)) return false;
    }
    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}

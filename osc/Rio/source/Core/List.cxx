// this :
#include <Rio/Core/List.h>

#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Arguments.h>

RIO_META_SOURCE(Rio::List)

//////////////////////////////////////////////////////////////////////////////
Rio::List::List(
 const IDictionary& aDictionary
)
:Object(aDictionary)
,fClass(aDictionary.opt_List_Class())
,fName("")
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::List::List(
 const List& aFrom
)
:Object(aFrom)
,std::list<IObject*>(aFrom)
,fClass(aFrom.fClass)
,fName(aFrom.fName)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::List::~List(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::List& Rio::List::operator=(
 const List& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Object::operator=(aFrom);
  std::list<IObject*>::operator=(aFrom);
  //fClass
  fName = aFrom.fName;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::List::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(List);
  else 
    return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::List::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  std::list<IObject*>::iterator it;
  for(it=begin();it!=end();it = erase(it)) delete (*it);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::List::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream all objects in the collection to or from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    reset();
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!Object::stream(aBuffer)) return false;
    if(!aBuffer.read(fName)) return false;

    int nobjects;
    if(!aBuffer.read(nobjects)) return false;

    for (int i = 0; i < nobjects; i++) {
      IObject* obj;
      Arguments args;
      Args_init(args);
      if(!aBuffer.readObject(args,obj)) {
        aBuffer.out() << "Rio::List::stream : can't read object." << std::endl;
        reset();
        return false;
      }
      unsigned char nch;
      if(!aBuffer.read(nch)) {
        reset();
        return false;
      }
      if (nch) {
        char readOption[256];
        if(!aBuffer.readFastArray(readOption,nch)) {
          reset();
          return false;
        }
        readOption[nch] = 0;
        push_back(obj);         //Add(obj,readOption);
      } else {
        push_back(obj);
      }
    }

    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) {
      reset();
      return false;
    }

  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!Object::stream(aBuffer)) return false;

    if(!aBuffer.write(fName)) return false;

    int nobjects = size();
    if(!aBuffer.write(nobjects)) return false;
    
    std::list<IObject*>::const_iterator it;
    for(it=begin();it!=end();++it) {
      if(!aBuffer.writeObject(*it)) return false;
      // Store object option :
      std::string dummy_string;
      unsigned char nch = dummy_string.size();
      if(!aBuffer.write(nch)) return false;
      if(!aBuffer.writeFastArray(dummy_string.c_str(),nch)) return false;
    }

    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}



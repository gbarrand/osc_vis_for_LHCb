// this :
#include <Rio/Core/Object.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IPrinter.h>
#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Core/Debug.h>

enum {
  //kIsOnHeap      = 0x01000000,    // object is on heap
  kNotDeleted    = 0x02000000    // object has not been deleted
  //kZombie        = 0x04000000,    // object ctor failed
  //kBitMask       = 0x00ffffff
};

RIO_META_SOURCE(Rio::Object)
//////////////////////////////////////////////////////////////////////////////
Rio::Object::Object(
 const IDictionary& aDictionary
)
//:fClass(Object::is(aDictionary))
:fClass(aDictionary.opt_Object_Class())
,fDictionary(aDictionary)
//,fBits(kNotDeleted)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Object");

  //FIXME if(TStorage::IsOnHeap(this))
  //FIXME   fBits |= kIsOnHeap;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Object::Object(
 const Object& aFrom
)
:IObject()
,fClass(aFrom.fClass)
,fDictionary(aFrom.fDictionary)
//,fBits(kNotDeleted)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Object");

  //FIXME if(TStorage::IsOnHeap(this))
  //FIXME   fBits |= kIsOnHeap;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Object::~Object(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fBits &= ~kNotDeleted;
  Debug::decrement("Rio::Object");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Object& Rio::Object::operator=(
 const Object& //aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fBits
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
const Rio::IDictionary& Rio::Object::dictionary(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDictionary;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Object::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return isA().inStoreName(); //FIXME : ?
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Object::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return isA().inStoreName(); //FIXME : ?
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::Object::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) return Rio_SCast(Object);
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Object::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (IsA()->CanIgnoreTObjectStreamer()) return;
  if (aBuffer.isReading()) {
    short v;
    if(!aBuffer.readVersion(v)) return false;
    unsigned int uniqueID;
    if(!aBuffer.read(uniqueID)) return false;
    unsigned int bits; //dummy. Not used.
    if(!aBuffer.read(bits)) return false;
    //FIXME fBits |= kIsOnHeap; //by definition de-serialized object is on heap
  } else {
    if(!aBuffer.writeVersion(fClass.version())) return false;
    if(!aBuffer.write((unsigned int)0)) return false;
    unsigned int bits = kNotDeleted;
    if(!aBuffer.write(bits)) return false;
  }
  return true;
}

void Rio::Object::out(std::ostream& aOut) const {
  aOut << "OBJ: " << isA().name() << "\t" << name() << "\t" << title() 
       << std::endl;
}

// this :
#include <Rio/Tree/LeafObject.h>

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/Arguments.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

RIO_META_SOURCE(Rio::LeafObject)
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::LeafObject(
 const IDictionary& aDictionary
,Branch& aBranch
)
:BaseLeaf(aDictionary,aBranch)
,fClass(aDictionary.opt_LeafObject_Class())
,fObjectClass(0)
,fObjAddress(0)
,fVirtual(true)
,fInternal(0) //Rio
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::LeafObject(
 const IDictionary& aDictionary
,Branch& aBranch
,const std::string& aName
,const IClass& aClass
)
:BaseLeaf(aDictionary,aBranch,aName)
,fClass(aDictionary.opt_LeafObject_Class())
,fObjectClass((IClass*)&aClass)
,fObjAddress(0)
,fVirtual(true)
,fInternal(0) //Rio
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  setTitle(aClass.inStoreName()); //FIXME : ?
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::~LeafObject(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
int Rio::LeafObject::number(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafObject::valuePointer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IClass* Rio::LeafObject::objectClass(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectClass;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::LeafObject::object(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fObjAddress) return 0;
  return *fObjAddress;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafObject::fillBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Pack leaf elements in Basket output buffer
  if (!fObjAddress) return true;
  IObject* obj  = object(); 
  if (obj) {
    if (fVirtual) {
      unsigned char n = obj->isA().inStoreName().size();
      if(!aBuffer.write(n)) return false;
      if(!aBuffer.writeFastArray(obj->isA().inStoreName().c_str(),n+1)) 
        return false;
    }
    if(!obj->stream(aBuffer)) return false;
  } else {
    std::ostream& out = branch().out();
    out << "Rio::LeafObject::fillBasket : why do we pass here ? " << std::endl;
    if (fObjectClass) {
      /*FIXME      if (fObjectClass->Property() & kIsAbstract) {
        obj = new Object;
        } else FIXME */ {
        Arguments args;
        Args_init(args);
        obj = fObjectClass->create(args);
      }
      if(!obj) {
        out << "Rio::LeafObject::fillBasket "
            << "can't create object from class :\"" 
            << fObjectClass->name() << "\"." << std::endl;
        return false;
      }
        //FIXME obj->SetBit(kInvalidObject);
        //FIXME obj->SetUniqueID(123456789);
      if(!obj->stream(aBuffer)) {
        delete obj;
        return false;
      }
      delete obj;
    } else {
      out << "Rio::LeafObject::fillBasket "
          << "Attempt to write a NULL object in leaf:\"" 
          << name() << "\"." << std::endl;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafObject::readBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Read leaf elements from Basket input buffer
  if (fVirtual) {
    unsigned char n;
    if(!aBuffer.read(n)) {
      branch().out() << "Rio::LeafObject::readBasket :" 
                     << " read(unsigned char) failed." 
                     << std::endl;
      return false;
    }
    char classname[128];
    if(!aBuffer.readFastArray(classname,n+1)) {
      branch().out() << "Rio::LeafObject::readBasket :" 
                     << " readFastArray failed." 
                     << std::endl;
      return false;
    }
    fObjectClass = dictionary().findInStoreClass(title());
  }
  if (fObjectClass) {
    IObject* object = 0;
    if (!fObjAddress) {
      //Out out(branch().printer());
      //out << "Rio::LeafObject::readBasket : " 
      //  << "debug : create object for class " << fObjectClass->name() 
      //  << endl;
      fObjAddress = &fInternal;
      Arguments args;
      Args_init(args);
      Args_insertDirectory(args,&(branch().tree().directory()));
      //WARNING : if the below created object is not mananged 
      // in the passed directory, it will not be managed at all. 
      // The user will have to delete it.
      *fObjAddress = fObjectClass->create(args);
    }
    object = *fObjAddress;
    if (branch().isAutoDelete()) {
      std::ostream& out = branch().out();
      out << "Rio::LeafObject::readBasket : debug : autodelete "  << std::endl;
      delete object;
      Arguments args;
      Args_init(args);
      Args_insertDirectory(args,&(branch().tree().directory()));
      object = fObjectClass->create(args);
    }
    if (!object) {
      branch().out() << "Rio::LeafObject::readBasket : " 
                     << "unable to get an object for class " 
                     << fObjectClass->name() << std::endl;
      return false;
    }
    if(!object->stream(aBuffer)) {
      branch().out() << "Rio::LeafObject::readBasket :" 
                     << " object stream failed." 
                     << " Object class was " << fObjectClass->name() << "."
                     << std::endl;
      delete object;
      return false;
    }
    // in case we had written a null pointer a Zombie object was created
    // we must delete it
    /*FIXME
    if (object->TestBit(kInvalidObject)) {
      if (object->GetUniqueID() == 123456789) {
        delete object;
        object = 0;
      }
    }
    */
    *fObjAddress = object;
  } else {
    std::ostream& out = branch().out();
    out << "Rio::LeafObject::readBasket : debug : no class !"  << std::endl;
    if(!branch().setAddress(0)) {
      branch().out() << "Rio::LeafObject::readBasket :" 
                     << " branch setAddress(0) failed." 
                     << std::endl;
      return false;
    }
  }
  return true;
}
/*
//////////////////////////////////////////////////////////////////////////////
TMethodCall* Rio::LeafObject::GetMethodCall(const char *name)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
// Returns pointer to method corresponding to name
//    name is a string with the general form  "method(list of params)"
//   If list of params is omitted, () is assumed;
//

   char* namecpy = new char[strlen(name)+1];
   strcpy(namecpy,name);
   char *params = strchr(namecpy,'(');
   if (params) { *params = 0; params++; }
   else params = ")";

   if (!fObjectClass) fObjectClass      = gROOT->GetClass(GetTitle());
   TMethodCall *m = new TMethodCall(fObjectClass, namecpy, params);
   delete [] namecpy;
   if (m->GetMethod()) return m;
   Error("GetMethodCall","Unknown method:%s",name);
   delete m;
   return 0;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::LeafObject::typeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   return fTitle;
}
*/

void Rio::LeafObject::printValue(std::ostream& aOut,int) const {
  //out << (unsigned long)valuePointer() << endl;
  IObject* obj = object();
  if(obj)
    aOut << obj << " " << obj->isA().name() << " " << obj->name() << std::endl;
  else 
    aOut << obj << std::endl;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafObject::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fObjAddress = (IObject**)aAddress;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafObject::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(LeafObject);
  else 
    return BaseLeaf::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafObject::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s,c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    /*
    if (R__v > 3 || R__v == 2) {
      TLeafObject::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
      fObjAddress = 0;
      fObjectClass  = gROOT->GetClass(fTitle.Data());
      if (!fObjectClass) Warning("Streamer","Cannot find class:%s",fTitle.Data());
      return;
    }
    */
    //====process old versions before automatic schema evolution
    if(!BaseLeaf::stream(aBuffer)) return false;
    fObjAddress = 0;
    fObjectClass = dictionary().findInStoreClass(fTitle);
    if(!aBuffer.read(fVirtual)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
    //====end of old versions
  } else {
    //FIXME TLeafObject::Class()->WriteBuffer(b,this);
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!BaseLeaf::stream(aBuffer)) return false;
    if(!aBuffer.write(fVirtual)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}

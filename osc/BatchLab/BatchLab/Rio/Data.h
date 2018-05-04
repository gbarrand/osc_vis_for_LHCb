#ifndef BatchLab_Rio_Data_h
#define BatchLab_Rio_Data_h

// Inheritance :
#include <Rio/Core/Named.h>

#include <Slash/Tools/Managers.h>
#include <Slash/Meta/IClass.h>
#include <Slash/Meta/IObject.h>

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/BaseClass.h>

#include <BatchLab/Rio/MemberWriter.h>
#include <BatchLab/Rio/MemberReader.h>

namespace BatchLab {
namespace Rio {

class Data : public ::Rio::Named {
public:
  virtual const ::Rio::IClass& isA() const { return fClass;}
public:
  virtual void* cast(const ::Rio::IClass& aClass) const {
    if(&aClass==&fClass) return Rio_SCast(BatchLab::Rio::Data);
    else return Named::cast(aClass);
  }
  virtual bool stream( ::Rio::IBuffer& aBuffer) {
    if (aBuffer.isReading()) {
      short v;
      if(!aBuffer.readVersion(v)) {
        delete fData;
        fData = 0;
        return false;
      }
      if(!Named::stream(aBuffer)) {
        delete fData;
        fData = 0;
        return false;
      }
      //FIXME : fData->setName(fName);
      MemberReader mReader(*fSession,aBuffer);
      if(!fData->read(mReader)) {
        delete fData;
        fData = 0;
        return false;
      }
   } else {
      if(!aBuffer.writeVersion(fClass.version())) return false;
      if(!Named::stream(aBuffer)) return false;
      MemberWriter mWriter(aBuffer);
      if(!fData->visit(mWriter)) return false;
    }
    return true;
  }
public:
  // Constructor when reading data :
  Data(const ::Rio::IDictionary& aDictionary,
       const ::Rio::IClass& aRioClass,       
       Slash::Store::IStorable* aData,
       Slash::Core::ISession* aSession)
  :Named(aDictionary,"","")
  ,fClass(aRioClass)
  ,fData(aData)
  ,fSession(aSession)
  {}
  // Constructor when writting data :
  Data(const ::Rio::IDictionary& aDictionary,
       const ::Rio::IClass& aRioClass,
       const std::string& aSID,
       const std::string& aTitle,
       const Slash::Store::IStorable& aData)
  :Named(aDictionary,aSID,aTitle)
  ,fClass(aRioClass)
  ,fData(0)
  ,fSession(0)
  {
    fData = const_cast<Slash::Store::IStorable*>(&aData);
  }
  virtual ~Data(){}
  Slash::Store::IStorable* data() const { return fData;}  
private:
  const ::Rio::IClass& fClass;
  Slash::Store::IStorable* fData;
  Slash::Core::ISession* fSession;
};

class Data_Class : public ::Rio::BaseClass {
public: 
  virtual ::Rio::IObject* create(const ::Rio::Arguments& aArgs) { 
    ::Rio::IDirectory* directory = 
      (::Rio::IDirectory*)::Rio::Args_findDirectory(aArgs);
    if(!directory) {
      fOut << "BatchLab::Rio::Data_Class::create :" 
           << " no directory given." 
           << std::endl;		 
      return 0;
    }
    std::string sclass = name();
    Slash::Meta::IDictionary* manager = Slash::dictionary(fSession);
    if(!manager) {
      fOut << "BatchLab::Rio::Data_Class::create :" 
           << " Lib::Dictionary manager not found." 
           << std::endl;		 
      return 0;
    }
    Slash::Meta::IClass* cls = manager->find(sclass);    
    if(!cls) {
      fOut << "BatchLab::Rio::Data_Class::create :" 
           << " no entry in Lib::Dictionary found for class " 
           << "\"" << sclass << "\"."
           << std::endl;		 
      return 0;
    }
    Slash::Meta::IObject* obj = cls->create();
    if(!obj) {
      fOut << "BatchLab::Rio::Data_Class::create :" 
           << " can't create an object for class \"" << sclass << "\"."
           << std::endl;		 
      return 0;
    }
    Slash::Store::IStorable* visited = INLIB_CAST(*obj,Slash::Store::IStorable);
    if(!visited) {
      fOut << "BatchLab::Rio::Data_Class::create :" 
           << " can't cast Slash::Meta::IObject to Slash::Store::IStorable"
           << " for class \"" << sclass << "\"."
           << std::endl;		 
      return 0;
    }
    const ::Rio::IDictionary& dico = directory->file().dictionary();
    ::Rio::IObject* robject = new Data(dico,*this,visited,&fSession);
    directory->appendObject(robject);
    return robject;
  }
  virtual unsigned int checkSum() const {return 0;}
public:
  Data_Class(const std::string& aDataClass,
             short aVersion,
             Slash::Core::ISession& aSession,
             std::ostream& aOut)
  : ::Rio::BaseClass(aDataClass,aDataClass,aVersion)
  ,fSession(aSession)
  ,fOut(aOut){}
  virtual ~Data_Class(){}
private:
  Slash::Core::ISession& fSession;
  std::ostream& fOut;
};

}}

#endif

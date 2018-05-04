#ifndef Slash_Tools_Dictionary_h
#define Slash_Tools_Dictionary_h

// Inheritance :
#include <Slash/Meta/IClass.h>
#include <Slash/Meta/IDictionary.h>
#include <Slash/Core/IManager.h>

#include <vector>
#include <ostream>

#include <inlib/cast>

namespace Slash {

  /*
class None_Class : public virtual Slash::Meta::IClass {
public://Slash::Meta::IClass
  virtual Slash::Meta::IObject* create() {return 0;}
  virtual std::string name() const { return fName;}
  virtual unsigned int version() const { return fVersion;}
public:
  //"Lib::None"
  None_Class(const std::string& aName):fName(aName),fVersion(1){}
  virtual ~None_Class(){}
private:
  std::string fName;
  unsigned int fVersion;
};
  */

class Class : public virtual Slash::Meta::IClass {
public: //Slash::Meta::IClass

  virtual std::string name() const {return fName;}
  virtual unsigned int version() const {return fVersion;}

  virtual Slash::Meta::IObject* create() {
    if(!fAllocator) return 0;
    return (Slash::Meta::IObject*)fAllocator(f_out);
  }

public:
  typedef Slash::Meta::IObject*(*Allocator)(std::ostream&);
public:
  inline Class(std::ostream& a_out,const std::string& aName,unsigned int aVersion,Allocator aAllocator = 0) 
  :f_out(a_out),fName(aName),fVersion(aVersion),fAllocator(aAllocator){
    //Lib::Debug::increment("Lib::Class");
  }
  virtual ~Class(){
    //Lib::Debug::decrement("Lib::Class");
  }
private:
  inline Class(const Class& a_from):f_out(a_from.f_out){}
  inline Class& operator=(const Class& aFrom) {return *this;}
private:
  std::ostream& f_out;
  std::string fName;
  unsigned int fVersion;
  Allocator fAllocator;
};

class Dictionary 
:public virtual Slash::Core::IManager
,public virtual Slash::Meta::IDictionary 
{
public: //Slash::Core::IManager

  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const { 
    INLIB_IF_CAST(Slash::Meta::IDictionary)
    else INLIB_IF_CAST(Slash::Dictionary)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //Slash::Meta::IDictionary

  virtual Slash::Meta::IClass* find(const std::string& aName) const {
    std::vector<Slash::Meta::IClass*>::const_iterator it;
    for(it=fClasses.begin();it!=fClasses.end();++it) {
      if((*it)->name()==aName) return *it;
    }
    f_out << "Slash::Dictionary::findClass :" 
          << " \"" << aName << "\" not found." 
          << std::endl;
    //return fNoneClass;
    return 0;
  }

  virtual bool add(Slash::Meta::IClass* aClass) {
    fClasses.push_back(aClass);
    return true;
  }

  virtual unsigned int numberOfClasses() const {return fClasses.size();}

  virtual const Slash::Meta::IClass* getClass(unsigned int aIndex) const{
    return fClasses[aIndex];
  }
public:
  Dictionary(std::ostream& a_out,const std::string& aName) 
  :fName(aName)
  ,f_out(a_out)
  //,fNoneClass(new None_Class())
  {
    //Lib::Debug::increment("Lib::Dictionary");
  }

  virtual ~Dictionary(){
    std::vector<Slash::Meta::IClass*>::iterator it;
    for(it=fClasses.begin();it!=fClasses.end();++it) delete (*it);
    fClasses.clear();
    //delete fNoneClass;
    //Lib::Debug::decrement("Lib::Dictionary");
  }
protected:
  inline Dictionary(const Dictionary& a_from)
  :f_out(a_from.f_out),fName(a_from.fName){}
private:
  inline Dictionary& operator=(const Dictionary& aFrom) {return *this;}
private:
  std::string fName;
  std::ostream& f_out;
  std::vector<Slash::Meta::IClass*> fClasses;
  //Slash::Meta::IClass* fNoneClass;
};

}

#endif

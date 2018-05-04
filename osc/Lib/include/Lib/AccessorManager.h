#ifndef Lib_AccessorManager_h
#define Lib_AccessorManager_h

//inheritance :
#include <Slash/Core/IManager.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Core/IAction.h>

#include <Slash/Core/IValue.h>
#include <Slash/Data/IDumpAction.h>
#include <Slash/Data/IHandlersIterator.h>
#include <Slash/Data/IProperty.h>

#include <Slash/Tools/Handler.h>
#include <Slash/Tools/names.h>

#include <inlib/cast>
#include <inlib/sprintf>
#include <inlib/pointer>

#include <Lib/Debug.h>

#include <list>
#include <algorithm>

namespace Lib {

class AccessorManager 
:public virtual Slash::Core::IManager
,public virtual Slash::Data::IProcessor {
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Data::IProcessor)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }

public: //IProcessor
  virtual std::ostream& out() {return f_out;}

  virtual void setVerboseLevel(int aVerboseLevel) {
    fVerboseLevel = aVerboseLevel;
  }
  virtual int verboseLevel() const {return fVerboseLevel;}

  virtual void addAccessor(Slash::Data::IAccessor* aType) {
    // Will be deleted by the AccessorManager destructor.
    if(!aType) return;
    fAccessors.push_back(aType);
  }

  virtual void removeAccessor(const std::string& aName) {
    Slash::Data::IAccessor* type = findAccessor(aName);
    if(!type) return;
    fAccessors.remove(type);
    delete type;
  }


  virtual Slash::Data::IAccessor* findAccessor(const std::string& aName){
    std::list<Slash::Data::IAccessor*>::iterator it;
    for(it=fAccessors.begin();it!=fAccessors.end();++it) {
      if(aName==(*it)->name()) return *it;
    }
    return 0;
  }

  virtual std::vector<std::string> accessorNames() const {
    std::vector<std::string> vs;
    std::list<Slash::Data::IAccessor*>::const_iterator it;
    for(it=fAccessors.begin();it!=fAccessors.end();++it) {
      vs.push_back((*it)->name());
    }
    return vs;
  }
  
  virtual std::vector<Slash::Data::IAccessor*> accessors() const {
    std::vector<Slash::Data::IAccessor*> v(fAccessors.size());
    std::list<Slash::Data::IAccessor*>::const_iterator it;
    unsigned int index = 0;
    for(it=fAccessors.begin();it!=fAccessors.end();++it,index++) {
      v[index] = *it;
    }
    return v;
  }

  virtual void addAction(Slash::Core::IAction* aAction){
    if(!aAction) return;
    fActions.push_back(aAction);
  }

  virtual void removeAction(const std::string& aName) {
    Slash::Core::IAction* action = findAction(aName);
    if(!action) return;
    fActions.remove(action);
    delete action;
  }
  
  virtual Slash::Core::IAction* findAction(const std::string& aName) const {
    std::list<Slash::Core::IAction*>::const_iterator it;
    for(it=fActions.begin();it!=fActions.end();++it) {
      if(aName==(*it)->name()) return *it;
    }
    return 0;
  }

  virtual std::string execute(const std::string& aName,
                              const std::vector<std::string>& aArgs){
    Slash::Core::IAction* action = findAction(aName);
    if(!action) {
      f_out << "Lib::AccessorManager::execute :" 
            << " Action " << inlib::sout(aName) << " not found." 
            << std::endl;
      return "";
    }
    return action->execute(aArgs);
  }

  virtual std::vector<std::string> actionNames() const {
    std::vector<std::string> vs;
    std::list<Slash::Core::IAction*>::const_iterator it;
    for(it=fActions.begin();it!=fActions.end();++it) {
      vs.push_back((*it)->name());
    }
    return vs;
  }

  virtual void addHandler(Slash::Data::IAccessor* aType,
                          Slash::Data::IAccessor::Data aData,void* aTag){
    fHandlers.push_back(Slash::Handler(aType,aData,aTag));
  }

  virtual unsigned int numberOfHandlers() const {return fHandlers.size();}

  virtual Slash::Data::IHandlersIterator* handlersIterator() const{
    return new HandlersIterator(fHandlers);
  }

  virtual Slash::Data::IHandlersIterator* outHandlersIterator() const {
    return new HandlersIterator(fOutHandlers);
  }

  virtual std::vector<Slash::Data::IAccessor*> handlersAccessors() const {
    std::vector<Slash::Data::IAccessor*> list;
    Slash::Data::IHandlersIterator* hit = handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      if(std::find(list.begin(),list.end(),h->type())==list.end())
        list.push_back(h->type());
    }
    delete hit;
    return list;
  }
  
  virtual void addOutHandler(Slash::Data::IAccessor* aType,
                             Slash::Data::IAccessor::Data aData,void* aTag){
    fOutHandlers.push_back(Slash::Handler(aType,aData,aTag));
  }


  virtual void eraseHandlers() {fHandlers.clear();}
  virtual void eraseOutHandlers() {fOutHandlers.clear();}

  virtual void swapHandlers() {
    // Copy fOutHandlers into fHandlers.
    fHandlers = fOutHandlers;
    fOutHandlers.clear();
  }

  virtual bool type_id_tag(const std::string& aString,
                           Slash::Data::IAccessor*& a_type,
                           void*& a_id,void*& a_tag){
    std::vector<std::string> fields;
    inlib::words(aString,"/",false,fields);
    if(fields.size()<=1) {
      //fAccessorManager.out().println
      //("Filter property \"%s\" :",fProperty.c_str());
      //fAccessorManager.out().println
      //(" \"%s\" : bad name format (should be class/id).",
      //words[index].c_str());
      a_type = 0;
      a_id = 0;
      a_tag = 0;
      return false;
    }
    
    std::string stype = fields[0];
    a_type = findAccessor(stype);
    if(!a_type) {
      f_out << "Lib::AccessorManager::::type_id_tag :"
            << " accessor " << inlib::sout(stype) << " not found." 
            << std::endl;
      a_type = 0;
      a_id = 0;
      a_tag = 0;
      return false;
    } 
    
    std::string sid = fields[1];
    if(!inlib::to_pointer(sid,a_id)) {
      f_out << "Lib::AccessorManager::::type_id_tag :"
            << " " << sid << " not an id." 
            << std::endl;
      a_type = 0;
      a_id = 0;
      a_tag = 0;
      return false;
    }
  
    a_tag = 0;
    if(fields.size()>=3) {
      std::string stag = fields[2];
      if(!inlib::to_pointer(stag,a_tag)) {
        f_out << "Lib::AccessorManager::::type_id_tag :"
              << " " << stag << " not a void*." 
              << std::endl;
        a_type = 0;
        a_id = 0;
        a_tag = 0;
        return false;
      }
    }
  
    return true;
  }

public:
  AccessorManager(std::ostream& a_out)
  :f_out(a_out),fName(Slash_AccessorManager),fVerboseLevel(0){
    Lib::Debug::increment("Lib::AccessorManager");
  }

  virtual ~AccessorManager(){
    eraseHandlers();
   {std::list<Slash::Data::IAccessor*>::iterator it;
    for(it=fAccessors.begin();it!=fAccessors.end();it=fAccessors.erase(it)) {
      if(fVerboseLevel) {
        f_out << "Lib::AccessorManager::~AccessorManager :"
              << " delete IAccessor " << inlib::sout((*it)->name()) << "." 
              << std::endl;
      }
      delete *it;
    }
   }
   {std::list<Slash::Core::IAction*>::iterator it;
    for(it=fActions.begin();it!=fActions.end();
        it=fActions.erase(it)) delete *it;}
    Lib::Debug::decrement("Lib::AccessorManager");
  }

public:
  void removeAccessor(Slash::Data::IAccessor* aType) {
    if(!aType) return;
    fAccessors.remove(aType);
  }
private:
  class HandlersIterator : public virtual Slash::Data::IHandlersIterator {
  public: //IHandlersIteractor
    virtual const Slash::Data::IHandler* handler() const {
      if(fIterator==fList.end()) return 0;
      return &(*fIterator);
    }
    virtual void next() { ++fIterator;}
  public:
    HandlersIterator(const std::list<Slash::Handler>& aList)
    :fList(aList),fIterator(aList.begin()) {
      Lib::Debug::increment("Lib::HandlersIterator");
    }
    virtual ~HandlersIterator() {
      Lib::Debug::decrement("Lib::HandlersIterator");
    }
  private:
    const std::list<Slash::Handler>& fList;
    std::list<Slash::Handler>::const_iterator fIterator;
  };
protected:
  std::list<Slash::Handler> fHandlers;
  std::list<Slash::Handler> fOutHandlers;
private:
  std::ostream& f_out;
  std::string fName;
  std::list<Slash::Data::IAccessor*> fAccessors;
  std::list<Slash::Core::IAction*> fActions;
  int fVerboseLevel;
};

}

#include <Lib/Processor.h>
#include <Slash/Data/IIterator.h>

#include <inlib/parse>

namespace Lib {

class CollectAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::CollectAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs) {
    //FIXME : throw
    if(aArgs.size()<2) return "";
    std::string what = aArgs[0];
    const std::string& cuts = aArgs[1];
  
    // Cuts
    // Get cut variables :
    Lib::Processor cut(cuts,fAccessorManager.out());
    std::vector<std::string> cvar;
    if(!cut.findVariables(cvar)) { //FIXME : throw
      std::ostream& out = fAccessorManager.out();
      out << "Lib::CollectAction::execute :" 
          << " in collect " << inlib::sout(what)
          << " can't find variables in cut " << inlib::sout(cuts)
          << std::endl;
      return "";
    }
    if(!cut.compile(cvar)) { //FIXME : throw
      std::ostream& out = fAccessorManager.out();
      out << "Lib::CollectAction::execute :" 
          << " in collect " << inlib::sout(what)
          << " compilation of cut " << inlib::sout(cuts) << " failed."
          << std::endl;
      return "";
    }
    unsigned int cvarn = cvar.size();
  
    if(what=="-u") { // First remove duplicates :  
  
      fAccessorManager.eraseOutHandlers();
      Slash::Data::IHandlersIterator* hit =
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        //FIXME : have a quick search :
        bool found = false;
        Slash::Data::IHandlersIterator* hit2 = 
          fAccessorManager.outHandlersIterator();
        for(;const Slash::Data::IHandler* h2 = hit2->handler();hit2->next()) {
          if(h2->isEqual(*h)) {
            found = true;
            break;
          }
        }
        delete hit2;
        if(!found) {
          fAccessorManager.addOutHandler(h->type(),h->object(),h->tag());
        }
      }
      fAccessorManager.swapHandlers();
      delete hit;
    }
  
    if((what=="-")||(what=="-u")) {
      
      fAccessorManager.eraseOutHandlers();
      Slash::Data::IHandlersIterator* hit = 
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        // Get variables values :
        std::vector<Lib::Value> cvals;
        bool status = true;
        for(unsigned int i=0;i<cvarn;i++) {
  	Slash::Core::IValue* value = 
            h->type()->findValue(h->object(),cvar[i],h->tag());
          if(!value) {
  	  std::ostream& out = fAccessorManager.out();
            out << "Lib::CollectAction::execute :" 
                << " Property not found : " << cvar[i] << std::endl;
            status = false;
            break;
          }
          //FIXME : work directly on the vals[]
          Lib::Value var;
          if(!var.copy(*value)) {
  	  std::ostream& out = fAccessorManager.out();
            out << "Lib::CollectAction::execute :" 
                << " convertion of IValue " << inlib::sout(value->stype()) 
                << " to a Lib::Value failed."
                << std::endl;
            status = false;
            delete value;
            break;
          }
          delete value;
          cvals.push_back(var);
        }
        if(!status) {
          fAccessorManager.eraseHandlers();
  	std::ostream& out = fAccessorManager.out();
          out << "Lib::CollectAction::execute :" 
              << " Can't init cut." << std::endl;
          fAccessorManager.eraseOutHandlers();
          break;
        }
        bool takeit;
        std::string serror;
        if(!cut.cut(cvals,takeit,serror)) {
          fAccessorManager.eraseHandlers();
  	std::ostream& out = fAccessorManager.out();
          out << "Lib::CollectAction::execute :" 
              << " cut failed."
              << std::endl;
          out << serror << std::endl;
          fAccessorManager.eraseOutHandlers();
          break;
        }
        if(takeit==false) continue;
  
        fAccessorManager.addOutHandler(h->type(),(void*)h->object(),h->tag());
      }
      fAccessorManager.swapHandlers();
      delete hit;
  
    } else {
      fAccessorManager.eraseHandlers();
  
      std::vector<std::string> args;
     {std::string name;
      if(!inlib::name_args(what,name,args)) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CollectAction::execute :" 
            << " bad syntax in " << inlib::sout(what) << "." << std::endl;
        return "";
      }
      what = name;}
  
      Slash::Data::IAccessor* type = fAccessorManager.findAccessor(what);
      if(!type) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CollectAction::execute :" 
            << " Unknown type " << inlib::sout(what) << "." << std::endl;
        return "";
      }
    
      if(fAccessorManager.verboseLevel()) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CollectAction::execute :" 
            << " type " << inlib::sout(what) << " found." << std::endl;
      }
    
      type->setIteratorArguments(args);
  
      Slash::Data::IIterator* iterator = type->iterator();
      if(!iterator) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CollectAction::execute :" 
            << " No iterator found for type " << inlib::sout(what) << "." 
            << std::endl;
        return "";
      }
    
      if(fAccessorManager.verboseLevel()) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CollectAction::execute :" 
            << " iterator found for type " << inlib::sout(what) << "." 
            << std::endl;
        out << "Lib::CollectAction::execute :"
            << " Cuts " << inlib::sout(cuts) << "." << std::endl;
      }
    
      for(Slash::Data::IAccessor::Data id = 0;
            (id = iterator->object());iterator->next()) {
        // Get variables values :
        std::vector<Lib::Value> cvals;
        bool status = true;
        for(unsigned int i=0;i<cvarn;i++) {
  	Slash::Core::IValue* value = 
            type->findValue(id,cvar[i],iterator->tag());
          if(!value) {
  	  std::ostream& out = fAccessorManager.out();
            out << "Lib::CollectAction::execute :" 
                << " Property not found : " << cvar[i] << std::endl;
            status = false;
            break;
          }
          //FIXME : work directly on the vals[]
          Lib::Value var;
          if(!var.copy(*value)) {
  	  std::ostream& out = fAccessorManager.out();
            out << "Lib::CollectAction::execute :" 
                << " convertion of IValue " << inlib::sout(value->stype()) 
                << " to a Lib::Variable failed."
                << std::endl;
            status = false;
            delete value;
            break;
          }
          delete value;
          cvals.push_back(var);
        }
        if(!status) {
          fAccessorManager.eraseHandlers();
  	std::ostream& out = fAccessorManager.out();
          out << "Lib::CollectAction::execute :" 
              << " Can't init cut." << std::endl;
          break;
        }
        bool takeit;
        std::string serror;
        if(!cut.cut(cvals,takeit,serror)) {
          fAccessorManager.eraseHandlers();
  	std::ostream& out = fAccessorManager.out();
          out << "Lib::CollectAction::execute :" 
              << " cut failed."
              << std::endl;
          out << serror << std::endl;
          break;
        }
        if(takeit==false) continue;
        fAccessorManager.addHandler(type,id,iterator->tag());
      }
      delete iterator;
    }
  
    if(fAccessorManager.verboseLevel()) {
      std::ostream& out = fAccessorManager.out();
      out << "Lib::CollectAction::execute :"
          << " collected " << fAccessorManager.numberOfHandlers()
          << std::endl;
    }
  
    return "";
  }

public:
  inline CollectAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_collect)
  {}

  virtual ~CollectAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class FilterAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::FilterAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    if(aArgs.size()<1) return "";
    const std::string& property = aArgs[0];
   
    std::string filter = (aArgs.size()>=2 ? aArgs[1] : std::string(""));
  
    fAccessorManager.eraseOutHandlers();
  
    Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      Slash::Core::IValue* value = 
        h->type()->findValue(h->object(),property,h->tag());
      if(!value) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::FilterAction::execute :"
            << " Property not found : " << property << std::endl;
        fAccessorManager.eraseOutHandlers();
        break;
      }
      std::string ss = value->toString();
      delete value;
      if(filter.size()) {
        if(!inlib::match(ss,filter)) continue;
      }
  
      std::vector<std::string> words;
      inlib::words(ss,"\n",false,words);
      unsigned int wordn = words.size();
      for(unsigned int index=0;index<wordn;index++) {
  
        Slash::Data::IAccessor* type = 0;
        void* id = 0;
        void* tag = 0;
        if(!fAccessorManager.type_id_tag(words[index],type,id,tag)) {
          fAccessorManager.eraseOutHandlers();
          break;
        }
        
        fAccessorManager.addOutHandler(type,id,tag);
  
      }
    }
    fAccessorManager.swapHandlers();
    delete hit;  
  
    return "";
  }

public:
  FilterAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_filter)
  {}

  virtual ~FilterAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class DumpAction 
: public virtual Slash::Core::IAction 
,public virtual Slash::Data::IDumpAction 
{
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::DumpAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    std::ostream& out = fAccessorManager.out();
    Lib::DumpAction::execute(aArgs,out);  
    return "";
  }

public: //IDumpAction
  virtual bool execute(const std::vector<std::string>& aArgs,
                       std::ostream& aOut){
    std::string format = aArgs.size()>=1 ? aArgs[0] : std::string("");
    if(format.empty()) format = "table";
  
    std::ostream& out = fAccessorManager.out(); //for errors.
  
    std::vector<Slash::Data::IAccessor*> types = 
      fAccessorManager.handlersAccessors();
    std::vector<Slash::Data::IAccessor*>::iterator it;
    for(it=types.begin();it!=types.end();++it) {
      Slash::Data::IAccessor* type = *it;
  
      unsigned int propn = type->numberOfProperties();
  
      // Count handlers of given type :
     {int count = 0;
     {Slash::Data::IHandlersIterator* hit = 
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        if( h->type() == type ) count++;
      }
      delete hit;}
  
      aOut << "Lib::DumpAction::execute :" 
           << " " << type->name() 
           << " informations (" << count << " objects) :" << std::endl;}
  
      if(format=="table") {
        // Dump header :
        for(unsigned int index=0;index<propn;index++) {
          aOut << " " << type->property(index)->header();
        }
        aOut << std::endl;
      }
      
     {Slash::Data::IHandlersIterator* hit = 
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        Slash::Data::IAccessor::Data id = h->object();
        if(h->type()==type) {
          if(format=="table") {
            bool stop = false;
            for(unsigned int propi=0;propi<propn;propi++) {
  	    Slash::Core::IValue* value = 
                type->findValue(id,type->property(propi)->name(),h->tag());
              if(!value) {
                out << "Lib::DumpAction::execute : "
                     << " Property not found : " 
                     << type->property(propi)->name()
                     << std::endl;
                stop = true;
                break;
              }
              std::string s;
              if(value->type()==Slash::Core::IValue::ARRAY_DOUBLE) {
                s = "vector";
              } else {
                s = value->toString();
              }
              delete value;
              type->property(propi)->dress(s);
              aOut << " " << s;
            }
            aOut << std::endl;
            if(stop) return false;
          } else {
            bool stop = false;
            for(unsigned int propi=0;propi<propn;propi++) {
  	    Slash::Core::IValue* value = 
                type->findValue(id,type->property(propi)->name(),h->tag());
              if(!value) {
                out << "Lib::DumpAction::execute : "
                    << " Property not found : " 
                    << type->property(propi)->name()
                    << std::endl;
                stop = true;
                break;
              }
              std::string s = value->toString();
              delete value;
              aOut << type->property(propi)->name() << " :" << s << std::endl;
            }
            aOut << std::endl;
            if(stop) return false;
          }
        }
      }
      delete hit;}
    }
  
    return true;
  }

public:
  inline DumpAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_dump)
  {}

  virtual ~DumpAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class NumberAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::NumberAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>&) {
    int number = fAccessorManager.numberOfHandlers();
    std::ostream& out = fAccessorManager.out();
    out << "Lib::NumberAction::execute :"
        << " " << number << " objects." << std::endl;
    std::string sreturn;
    inlib::sprintf(sreturn,32,"%d",number);
    return sreturn;
  }

public:
  inline NumberAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_number)
  {}
  virtual ~NumberAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class GetValuesAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::GetValuesAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    if(aArgs.size()<1) return "";
    const std::string& property = aArgs[0];

    std::vector<std::string> values;
  
    Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      Slash::Core::IValue* value = 
        h->type()->findValue(h->object(),property,h->tag());
      if(!value) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::GetValuesAction::execute :"
            << " Property not found : " << property << std::endl;
        values.clear();
        break;
      }
      values.push_back(value->toString());
      delete value;
    }
    delete hit;
    return inlib::tos<std::string>(values,"\n");
  }

public:
  inline GetValuesAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_values)
  {}
  virtual ~GetValuesAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class DestroyAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::DestroyAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>&){
    Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      h->type()->destroy(h->object(),h->tag());
    }
    delete hit;
    return "";
  }

public:
  inline DestroyAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_destroy)
  {}

  virtual ~DestroyAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class SetAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::SetAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs) {
    if(aArgs.size()<2) return "";
    const std::string& what = aArgs[0];
    const std::string& value = aArgs[1];
  
    Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      h->type()->set(h->object(),what,value,h->tag());
    }
    delete hit;
  
    return "";
  }

public:
  inline SetAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_set)
  {}

  virtual ~SetAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class CalculateAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::CalculateAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    if(aArgs.size()<2) return "";
  
    int dimension = aArgs.size()-1;
    std::vector<Lib::Processor> processors;
    std::vector< std::vector<std::string> > variables(dimension);
    bool status = true;
   {for(int dim=0;dim<dimension;dim++) {
      processors.push_back
        (Lib::Processor(aArgs[dim+1],fAccessorManager.out()));
      if(!processors[dim].findVariables(variables[dim])) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CalculateAction::execute :"
            << " can't find variables in " << inlib::sout(aArgs[dim+1])
            << std::endl;
        status = false;
      }
      if(variables[dim].size()==0) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::CalculateAction::execute :"
            << " No variables found in " << aArgs[dim+1] << std::endl;
        status = false;
      } else {
        if(!processors[dim].compile(variables[dim])) {
  	std::ostream& out = fAccessorManager.out();
          out << "Lib::CalculateAction::execute :"
              << " compilation of " << inlib::sout(aArgs[dim+1]) 
              << " failed." << std::endl;
          status = false;
        }
      }
    }}
  
    if(status) {
      Slash::Data::IHandlersIterator* hit =
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        std::vector<Slash::Core::IValue*> values;
        bool stop = false;
        for(int dim=0;dim<dimension;dim++) {
          unsigned int xn = variables[dim].size();
          std::vector<Lib::Value> vals(xn);
          for(unsigned int xi=0;xi<xn;xi++) {
            Slash::Core::IValue* value = 
              h->type()->findValue(h->object(),variables[dim][xi],h->tag());
            if(!value) {
  	    std::ostream& out = fAccessorManager.out();
              out << "Lib::CalculateAction::execute :"
                  << " Property " << inlib::sout(variables[dim][xi])
                  << " not found." << std::endl;
              stop = true;
              break;
            }
            if(!vals[xi].copy(*value)) { 
  	    std::ostream& out = fAccessorManager.out();
              out << "Lib::CalculateAction::execute :"
                  << " convertion of IValue " << inlib::sout(value->stype()) 
                  << " to a Lib::Variable failed."
                  << std::endl;
              stop = true;
              delete value;
              break;
            }
            delete value;
          }
          if(stop) break;
  
          Lib::Value var;
          std::string serror;
          if(!processors[dim].execute(vals,var,serror)) {
  	  std::ostream& out = fAccessorManager.out();
            out << "Lib::CalculateAction::execute :"
                << " execution failed"
                << std::endl;
            out << serror << std::endl;
            stop = true;
            break;
          }
          values.push_back(new Lib::Value(var));
        }
        if(!stop) {
          h->type()->calculate(h->object(),aArgs[0],values,h->tag());
        }
       {std::vector<Slash::Core::IValue*>::iterator it;
        for(it=values.begin();it!=values.end();++it) delete (*it);}
      }
      delete hit;
    }
  
    return "";
  }
  
public:
  inline CalculateAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_calculate)
  {}

  virtual ~CalculateAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

class InitAction : public virtual Slash::Core::IAction {
public: //IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::InitAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    //used in OnX::NetUI::steer ONX_PROTOCOL_DATA
    if(aArgs.size()<1) return "";
    const std::string& values = aArgs[0];
    fAccessorManager.eraseHandlers();

    std::vector<std::string> words = inlib::words(values,"\n");
    for(std::vector<std::string>::iterator it = words.begin();
      it!=words.end();++it) {
  
      Slash::Data::IAccessor* type = 0;
      void* id = 0;
      void* tag = 0;
      if(!fAccessorManager.type_id_tag(*it,type,id,tag)) {
        fAccessorManager.eraseHandlers();
        break;
      }
  
      fAccessorManager.addHandler(type,id,tag);
    }
  
    return "";
  }

public:
  inline InitAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_init)
  {}
  virtual ~InitAction(){}
private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

}

#include <Slash/Core/IAction.h>
#include <Slash/Data/IVisualizeAction.h>
#include <Slash/Data/IVisualizer.h>

namespace Lib {

class VisualizeAction
:public virtual Slash::Core::IAction 
,public virtual Slash::Data::IVisualizeAction 
{
public: //Slash::Core::IAction
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::VisualizeAction)
    else INLIB_IF_CAST(Slash::Core::IAction)
    else INLIB_IF_CAST(Slash::Data::IVisualizeAction)
    else return 0;
  }

  virtual std::string name() const {return fName;}

  virtual std::string execute(const std::vector<std::string>& aArgs){
    std::vector<Slash::Data::IAccessor*> types = 
      fAccessorManager.handlersAccessors();
    std::vector<Slash::Data::IAccessor*>::iterator it;
    for(it=types.begin();it!=types.end();++it) {
      Slash::Data::IAccessor* type = *it;
      Slash::Data::IVisualizer* visType = 
        INLIB_CAST(*type,Slash::Data::IVisualizer);
      if(!visType) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::AccessorManager::execute :"
            << " type " << inlib::sout(type->name()) 
            << " is not a Slash::Data::IVisualizer." 
            << std::endl;
        continue;
      }
  
      type->setExecuteArguments(aArgs);
  
      visType->beginVisualize();
  
      Slash::Data::IHandlersIterator* hit = 
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        if( h->type() == type ) {
          visType->visualize(h->object(),h->tag());
        }
      }
      visType->endVisualize();
  
      delete hit;
    }
    return "";
  }
  

public: //Slash::Core::IVisualizeAction
  virtual bool execute(const std::vector<std::string>& aArgs,const Slash::UI::IStyle& aStyle){
    std::vector<Slash::Data::IAccessor*> types = 
      fAccessorManager.handlersAccessors();
    std::vector<Slash::Data::IAccessor*>::iterator it;
    for(it=types.begin();it!=types.end();++it) {
      Slash::Data::IAccessor* type = *it;
      Slash::Data::IVisualizer* visType = 
        INLIB_CAST(*type,Slash::Data::IVisualizer);
      if(!visType) {
        std::ostream& out = fAccessorManager.out();
        out << "Lib::AccessorManager::execute :"
            << " type " << inlib::sout(type->name()) 
            << " is not a Slash::Data::IVisualizer." 
            << std::endl;
        continue;
      }
  
      type->setExecuteArguments(aArgs);
  
      visType->beginVisualize(aStyle);
  
      Slash::Data::IHandlersIterator* hit = 
        fAccessorManager.handlersIterator();
      for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
        if( h->type() == type ) {
          visType->visualize(h->object(),h->tag());
        }
      }
      visType->endVisualize();
  
      delete hit;
    }
    return true;
  }
  
public:
  inline VisualizeAction(Slash::Data::IProcessor& aAccessorManager)
  :fAccessorManager(aAccessorManager)
  ,fName(Slash_action_visualize)
  {}

  virtual ~VisualizeAction(){}

private:
  Slash::Data::IProcessor& fAccessorManager;
  std::string fName;
};

}

#endif




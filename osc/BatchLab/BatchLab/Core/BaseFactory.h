#ifndef BatchLab_BaseFactory_h
#define BatchLab_BaseFactory_h

#include <Slash/Store/ITree.h>
#include <inlib/args>

#include <Lib/Debug.h>

namespace BatchLab {

class BaseFactory  {
public:
  BaseFactory(std::ostream& a_out,Slash::Store::ITree& aTree)
  :f_out(a_out),fTree(aTree){
    Lib::Debug::increment("BatchLab::BaseFactory");
  }
  virtual ~BaseFactory(){Lib::Debug::decrement("BatchLab::BaseFactory");}
protected:
  bool manage(const std::string& aOptions) const {
    if(aOptions.empty()) return true; //optimization.
    // Managed by default.
    inlib::args args(aOptions,";",true);
    bool b;
    if(!args.find("manage",b)) return true;
    return b;
  }


  bool manageObject(Slash::Store::IObject* aObject,const std::string& aPath){
    // The string "/not_managed" appears too in path_not_managed().
    if(aPath=="/not_managed") return true; //Not managed;
    return fTree.manageObject(aObject,aPath);
  }
protected:
  std::ostream& f_out;
  Slash::Store::ITree& fTree;
};

}

#endif

#ifndef BatchLab_ArrayFactory_h
#define BatchLab_ArrayFactory_h

// Inheritance :
#include <BatchLab/Core/BaseFactory.h>
#include <Slash/Data/IArrayFactory.h>

#include <BatchLab/Core/Array.h>

namespace Slash {namespace Store {class ITree;}}

namespace BatchLab {

class ArrayFactory 
: public BaseFactory
,public virtual Slash::Data::IArrayFactory 
{
public: //Slash::Data::IArrayFactory
  virtual Slash::Data::IArray* create(const std::string& aPath,
                                      const std::string& aTitle,
                                      const std::vector<unsigned int>& aOrders,
                                      const std::string& aOptions = ""){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,aTitle,
                                          aOrders);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }

  //virtual bool destroy(AIDA::Slash::Data::IArray*);

  virtual Slash::Data::IArray* add(const std::string& aPath,
                                   const Slash::Data::IArray& aV1,
                                   const Slash::Data::IArray& aV2,
                                   const std::string& aOptions = ""){ 
    if(aV1.orders()!=aV2.orders()) return 0;
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aV1.orders());
    object->copy(aV1);
    object->add(aV2);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }

  virtual Slash::Data::IArray* subtract(const std::string& aPath,
                                        const Slash::Data::IArray& aV1,
                                        const Slash::Data::IArray& aV2,
                                        const std::string& aOptions = ""){ 
    if(aV1.orders()!=aV2.orders()) return 0;
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aV1.orders());
    object->copy(aV1);
    object->subtract(aV2);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }

  virtual Slash::Data::IArray* multiply(const std::string& aPath,
                                        const Slash::Data::IArray& aV1,
                                        const Slash::Data::IArray& aV2,
                                        const std::string& aOptions = ""){ 
    if(aV1.orders()!=aV2.orders()) return 0;
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aV1.orders());
    object->copy(aV1);
    object->multiply(aV2);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual Slash::Data::IArray* divide(const std::string& aPath,
                                      const Slash::Data::IArray& aV1,
                                      const Slash::Data::IArray& aV2,
                                      const std::string& aOptions = ""){ 
    if(aV1.orders()!=aV2.orders()) return 0;
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aV1.orders());
    object->copy(aV1);
    object->divide(aV2);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual Slash::Data::IArray* add(const std::string& aPath,
                                   const Slash::Data::IArray& aArray,
                                   double aValue,
                                   const std::string& aOptions = ""){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aArray.orders());
    object->copy(aArray);
    object->add(aValue);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
  virtual Slash::Data::IArray* multiply(const std::string& aPath,
                                        const Slash::Data::IArray& aArray,
                                        double aValue,
                                        const std::string& aOptions = ""){ 
    std::string path,name,suffix;
    if(!inlib::path_name_suffix(aPath,path,name,suffix)) return 0;
    BatchLab::Array* object = new BatchLab::Array(f_out,
                                          name,"",
                                          aArray.orders());
    object->copy(aArray);
    object->multiply(aValue);
    if(!manage(aOptions)) return object;
    if(manageObject(object,path)) return object;
    delete object;
    return 0;
  }
public:
  ArrayFactory(std::ostream& a_out,Slash::Store::ITree& aTree)
  :BaseFactory(a_out,aTree){
    Lib::Debug::increment("BatchLab::ArrayFactory");
  }
  virtual ~ArrayFactory(){
    Lib::Debug::decrement("BatchLab::ArrayFactory");
  }
};

}

#endif

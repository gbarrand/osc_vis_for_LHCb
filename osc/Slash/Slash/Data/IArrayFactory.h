#ifndef Slash_Data_IArrayFactory_h
#define Slash_Data_IArrayFactory_h

#include <string>
#include <vector>

namespace Slash {namespace Data { class IArray;}}

namespace Slash {

namespace Data {

class IArrayFactory {
public: 
  virtual ~IArrayFactory() {}
public:
  virtual IArray* create(const std::string & path, const std::string & title, const std::vector<unsigned int>& orders,const std::string& = "") = 0;
  //virtual bool destroy(IArray * vector) = 0;
  virtual IArray* add(const std::string&,const IArray&,const IArray&,const std::string& = "") = 0;
  virtual IArray* subtract(const std::string&,const IArray&,const IArray&,const std::string& = "") = 0;
  virtual IArray* multiply(const std::string&,const IArray&,const IArray&,const std::string& = "") = 0;
  virtual IArray* divide(const std::string&,const IArray&,const IArray&,const std::string& = "") = 0;
  virtual IArray* add(const std::string&,const IArray&,double,const std::string& = "") = 0;
  virtual IArray* multiply(const std::string&,const IArray&,double,const std::string& = "") = 0;
};

} //Data

} //Slash

#endif

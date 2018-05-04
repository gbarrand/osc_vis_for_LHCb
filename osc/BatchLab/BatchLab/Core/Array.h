#ifndef BatchLab_Array_h
#define BatchLab_Array_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <Slash/Data/IArray.h>
#include <Slash/Store/IStorable.h>

#include <BatchLab/Core/Annotation.h>
#include <inlib/array>

namespace BatchLab {

class Array 
:public BaseObject
,public virtual Slash::Data::IArray 
,public virtual Slash::Store::IStorable
{
public: //AIDA/IManagedObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Data::IArray)
    else INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(BatchLab::Array)
    else return BaseObject::cast(a_class);
  }
  //AIDA-v3r3p0
  virtual std::string type() const {return "Slash::Data::IArray";}
public: //BatchLab/Slash::Data::IArray
  virtual std::string title() const {return fTitle;}
  virtual bool setTitle(const std::string& aTitle) { 
    fTitle = aTitle;
    fAnnotation.setValue("Title",aTitle);
    return true;
  }

  virtual AIDA::IAnnotation& annotation() {return fAnnotation;}
  virtual const AIDA::IAnnotation& annotation() const {return fAnnotation;}

  virtual unsigned int dimension() const {return fArray.dimension();}
  virtual unsigned int size() const {return fArray.size();}

  virtual const std::vector<unsigned int>& orders() const {
    return fArray.orders();
  }

  virtual bool setValue(const std::vector<unsigned int>& aIs,double aValue) {
    return fArray.set_value(aIs,aValue);
  }

  virtual bool value(const std::vector<unsigned int>& aIs,
                     double& aValue) const {
    return fArray.value(aIs,aValue);
  }

  virtual bool fill(const std::vector<double>& aValues,Cut* aCut) {
    return fArray.fill(aValues,aCut);
  }

  virtual bool copy(const Slash::Data::IArray& aFrom) {
    const BatchLab::Array* from = INLIB_CONST_CAST(aFrom,BatchLab::Array);
    if(!from) return false;
    fArray.copy(from->fArray);
    return true;
  }

  virtual bool add(const Slash::Data::IArray& aArray,Cut* aCut = 0) {
    const BatchLab::Array* from = INLIB_CONST_CAST(aArray,BatchLab::Array);
    if(!from) return false;
    return fArray.add(from->fArray,aCut);
  }

  virtual bool subtract(const Slash::Data::IArray& aArray) {
    const BatchLab::Array* from = INLIB_CONST_CAST(aArray,BatchLab::Array);
    if(!from) return false;
    return fArray.subtract(from->fArray);
  }

  virtual bool multiply(const Slash::Data::IArray& aArray) {
    const BatchLab::Array* from = INLIB_CONST_CAST(aArray,BatchLab::Array);
    if(!from) return false;
    return fArray.multiply(from->fArray);
  }

  virtual bool divide(const Slash::Data::IArray& aArray) {
    const BatchLab::Array* from = INLIB_CONST_CAST(aArray,BatchLab::Array);
    if(!from) return false;
    return fArray.divide(from->fArray);
  }

  virtual bool add(double aValue,Cut* aCut = 0) {
    fArray.add(aValue,aCut);
    return true;
  }

  virtual bool multiply(double aValue) {
    fArray.multiply(aValue);
    return true;
  }

  virtual bool invert() {return fArray.invert();}

  virtual bool indices(unsigned int aOffset,
                       std::vector<unsigned int>& aIs) const {
    return fArray.indices(aOffset,aIs);
  }

  virtual const std::vector<double>& vector() const {return fArray.vector();}
  virtual std::vector<double>& vector() {return fArray.vector();}

  virtual bool accept(unsigned int aIndex,const Cut& aCut) const {
    return fArray.accept(aIndex,aCut);
  }

public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const {
    return new BatchLab::Array(*this);
  }
  virtual std::string storeClassName() const {return Array::s_class();}
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,Array::s_class(),Array::s_visit)) return false;
  
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
  
    if(!aVisitor.visit("fAnnotation",fAnnotation)) return false;
    if(!aVisitor.visit("fTitle",fTitle)) return false;
    if(!aVisitor.visit("fArray",fArray.vector())) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
  
  virtual bool read(Slash::Store::IVisitor& aVisitor) {
    if(!aVisitor.begin(*this)) return false;
  
    int version;
    if(!aVisitor.visit(version)) return false;
  
    if(!fAnnotation.read(aVisitor)) return false;
    if(!aVisitor.visit(fTitle)) return false;
    if(!aVisitor.visit(fArray.vector())) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
public:
  Array(std::ostream& a_out,
        const std::string& aName,const std::string& aTitle,
        const std::vector<unsigned int>& aOrders)
  :BaseObject(a_out,aName)
  ,fArray(aOrders){
    setTitle(aTitle);
  }
  virtual ~Array(){}
public:
  Array(const Array& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,Slash::Data::IArray(aFrom)
  ,Slash::Store::IStorable(aFrom)
  ,BaseObject(aFrom)
  ,fAnnotation(aFrom.fAnnotation)
  ,fTitle(aFrom.fTitle)
  ,fArray(aFrom.fArray)
  {}
private:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Array");return s_v;
  }
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Array* local = INLIB_CONST_CAST(aObject,BatchLab::Array);
    if(!local) return false;
    return local->Array::visit(aVisitor);
  }
private:
  Annotation fAnnotation;
  std::string fTitle;
  inlib::array<double> fArray;
};

}

#endif

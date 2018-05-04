#ifndef BatchLab_T_Vector_h
#define BatchLab_T_Vector_h

// Inheritance :
#include <vector>
#include <Slash/Store/IStorable.h>
#include <Slash/Store/IConstVisitor.h>
#include <Slash/Store/IVisitor.h>

#include <inlib/sto>

#include <inlib/cast>

namespace BatchLab {

template <class T>
class Vector 
:public std::vector<T>
,public virtual Slash::Store::IStorable {
public: //IVisited
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Store::IStorable)
    else if(a_class=="std::vector<"+fT+">") {
      return (void*)static_cast< const std::vector<T>* >(this);
    }
    else if(a_class=="BatchLab::Vector<"+fT+">") {
      return (void*)static_cast< const BatchLab::Vector<T>* >(this);
    }
    else return 0;
  }
private:
  static bool visit(const Slash::Store::IStorable& aObject,
                         Slash::Store::IConstVisitor& aVisitor){
    const BatchLab::Vector<T>* local = 
      dynamic_cast<  const BatchLab::Vector<T>* >(&aObject);
    if(!local) return false;
    return local->BatchLab::Vector<T>::visit(aVisitor);
  }
public:
  virtual bool visit(
    Slash::Store::IConstVisitor& aVisitor) const {

    if(!aVisitor.begin(*this,"BatchLab::Vector<"+fT+">",Vector<T>::visit)) 
      return false;

    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;

    unsigned int number = std::vector<T>::size();
    if(!aVisitor.visit("fSize",number)) return false;

    for(unsigned int index=0;index<number;index++) {
      const T& elem = (*this)[index];
      if(!aVisitor.visit(inlib::to<int>(index),elem)) return false;
    }

    if(!aVisitor.end(*this)) return false;
    return true;
  }
  virtual bool read(Slash::Store::IVisitor& aVisitor) {
    std::vector<T>::clear();
    
    if(!aVisitor.begin(*this)) return false;
    int version;
    if(!aVisitor.visit(version)) return false;

    unsigned int number;
    if(!aVisitor.visit(number)) return false;

    std::vector<T>::resize(number);
    for(unsigned int index=0;index<number;index++) {
      T& elem = (*this)[index];
      if(!elem.read(aVisitor)) return false;
    }

    if(!aVisitor.end(*this)) return false;
    return true;
  }
public:
  Vector(const std::string& aT):fT(aT){}
  Vector(const Vector& aFrom)
  :Slash::Store::IStorable(aFrom)
  ,std::vector<T>(aFrom)
  ,fT(aFrom.fT){}
  virtual ~Vector(){}
  Vector& operator=(const Vector& aFrom){ 
    std::vector<T>::operator=(aFrom);
    fT = aFrom.fT;
    return *this;
  }
private:
  std::string fT;
};

}

#endif

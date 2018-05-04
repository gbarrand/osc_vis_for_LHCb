#ifndef Lib_ObjectSizeGetter_h
#define Lib_ObjectSizeGetter_h

// Inheritance :
#include <Slash/Store/IConstVisitor.h>

#include <Slash/Store/IStorable.h>
#include <Slash/Store/IArray.h>

#include <list>
#include <cstring>

namespace Lib {

class ObjectSizeGetter : public virtual Slash::Store::IConstVisitor {
public: //Slash::Store::IConstVisitor
  virtual bool begin(const Slash::Store::IStorable&,const std::string&,
                     Slash::Store::IConstVisitor::Local){
    fStack.push_front(fCurrent);
    fCurrent = 0;
    return true;
  }

  virtual bool end(const Slash::Store::IStorable&){
    unsigned int sav = fCurrent;
    const_cast<unsigned int&>(fReturned) = fCurrent;
    const_cast<unsigned int&>(fCurrent) = fStack.front();
    // Back to parent :
    const_cast<unsigned int&>(fCurrent) += sav;
    const_cast< std::list<unsigned int>& >(fStack).pop_front();
    return true;
  }

  virtual bool visit(const std::string&,char) {
    fCurrent += LIB_SIZE_CHAR;
    return true;
  }
  virtual bool visit(const std::string&,bool) {
    fCurrent += LIB_SIZE_CHAR;
    return true;
  }
  virtual bool visit(const std::string&,short) {
    fCurrent += LIB_SIZE_SHORT;
    return true;
  }
  virtual bool visit(const std::string&,int) {
    fCurrent += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(const std::string&,Slash::int64) {
    fCurrent += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(const std::string&,Slash::uint64) {
    fCurrent += LIB_SIZE_INT64;
    return true;
  }
  virtual bool visit(const std::string&,unsigned int) {
    fCurrent += LIB_SIZE_INT;
    return true;
  }
  virtual bool visit(const std::string&,float) {
    fCurrent += LIB_SIZE_FLOAT;
    return true;
  }
  virtual bool visit(const std::string&,double) {
    fCurrent += LIB_SIZE_DOUBLE;
    return true;
  }

  virtual bool visit(const std::string&,const std::string& aData){
    fCurrent += LIB_SIZE_INT+(aData.size()+1)*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<bool>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<char>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<short>& aData){
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_SHORT;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<int>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_INT;
    return true;
  }

  virtual bool visit(const std::string&,
                     const std::vector<Slash::int64>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_INT64;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<float>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_FLOAT;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<double>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_DOUBLE;
    return true;
  }

  virtual bool visit(const std::string&,
                     const std::vector<unsigned char>& aData) {
    fCurrent += LIB_SIZE_INT + aData.size()*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit(const std::string&,
                     const std::vector< std::vector<double> >& aData){
    unsigned int n = aData.size();
    unsigned int dim = n?aData[0].size():0;
    unsigned int number = n*dim;
    unsigned int array_sz = number*LIB_SIZE_DOUBLE;
    fCurrent += 2*LIB_SIZE_INT+array_sz;
    return true;
  }

  virtual bool visit(const std::string&,const std::vector<std::string>& aData){
    unsigned int number = aData.size();
    unsigned int sz = 0;
    for(unsigned int index=0;index<number;index++) sz += aData[index].size()+1;
    sz++;
    fCurrent += LIB_SIZE_INT+sz*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit_double(const std::string&,
                            const Slash::Store::IArray& aData){ 
    Slash::Store::IArray::Orders orders = aData.orders();
    fCurrent += LIB_SIZE_INT + orders.size()*LIB_SIZE_INT;
    unsigned int array_sz = 1;
    unsigned int dimn = orders.size();
    for(unsigned int dimi=0;dimi<dimn;dimi++) array_sz *= orders[dimi];
    array_sz *= LIB_SIZE_DOUBLE;
    fCurrent += array_sz;
    return true;
  }

  virtual bool visit(const std::string&,const char* aData) {
    fCurrent += LIB_SIZE_INT+(::strlen(aData)+1)*LIB_SIZE_CHAR;
    return true;
  }

  virtual bool visit(const std::string&,const Slash::Store::IStorable& aData){ 
    // WARNING : can't be called in a object.visit()
    //   over a aData being a "super" of "object" .
    ObjectSizeGetter mAdder;
    if(!aData.visit(mAdder)) {
      fCurrent = 0;
      return false;
    }
    fCurrent += mAdder.size();
    return true;  
  }


public:
  ObjectSizeGetter():fCurrent(0),fReturned(0){}
  virtual ~ObjectSizeGetter(){}
  unsigned int size() const {return fReturned;}
private:
  static const int LIB_SIZE_CHAR = 1;
  static const int LIB_SIZE_SHORT = 2;
  static const int LIB_SIZE_INT = 4;
  static const int LIB_SIZE_INT64 = 8;
  static const int LIB_SIZE_FLOAT = 4;
  static const int LIB_SIZE_DOUBLE = 8;
private:
  unsigned int fCurrent;
  std::list<unsigned int> fStack;
  unsigned int fReturned;

};

}

#endif

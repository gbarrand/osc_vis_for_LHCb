#ifndef Rio_ObjArrayT_h
#define Rio_ObjArrayT_h

// Inheritance :
#include <vector>

#include <Rio/Interfaces/IBuffer.h>

namespace Rio {

class IDictionary;

template <class T>
class ObjArrayT : public std::vector<T*> {
public:
  typedef typename std::vector< T* >::iterator Iterator;
public:
  ObjArrayT(const IDictionary& aDictionary,std::ostream& aOut)
  :fDictionary(aDictionary),fOut(aOut){}
  ObjArrayT(const IDictionary& aDictionary,std::ostream& aOut,int aSize,T* aValue)
  :std::vector<T*>(aSize,aValue)
  ,fDictionary(aDictionary)
  ,fOut(aOut){}
  virtual ~ObjArrayT(){}
  inline bool read(IBuffer& aBuffer,const Arguments& aArgs,bool aNoNull = true){
    if (!aBuffer.isReading()) return false;
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    Object dummy_object(fDictionary);
    if(!dummy_object.stream(aBuffer)) return false;
    std::string name;
    if(!aBuffer.read(name)) return false;
    int nobjects;
    if(!aBuffer.read(nobjects)) return false;
    int lowerBound;
    if(!aBuffer.read(lowerBound)) return false;
    this->clear();
    for (int i = 0; i < nobjects; i++) {
      IObject* obj = 0;
      if(!aBuffer.readObject(aArgs,obj)){
        fOut << "Rio::ObjArrayT::read : can't read object." << std::endl;
        return false;
      }
      if(obj) {
        T* t = Rio::cast<T>(*obj);
        if(!t) {
          fOut << "Rio::ObjArrayT::read : bad object class." << std::endl;
        } else {
          this->push_back(t);
        }
      } else {
        if(!aNoNull) this->push_back(0);
      }
    }
    return aBuffer.checkByteCount(s, c,"TObjArray");
  }
  inline bool write(IBuffer& aBuffer){
    if (aBuffer.isReading()) return false;
#ifndef Rio_ObjectArray_Class_Version
#define Rio_ObjectArray_Class_Version 3
#endif
    unsigned int c;
    if(!aBuffer.writeVersion(Rio_ObjectArray_Class_Version,c)) return false;
    Object dummy_object(fDictionary);
    if(!dummy_object.stream(aBuffer)) return false;
    if(!aBuffer.write(std::string(""))) return false;
    int nobjects = this->size();
    if(!aBuffer.write(nobjects)) return false;
    //debug int lowerBound = 211;
    int lowerBound = 0;
    if(!aBuffer.write(lowerBound)) return false;
    unsigned int sz = this->size();
    for(unsigned int index=0;index<sz;index++) {
      if(!aBuffer.writeObject(this->operator[](index))) return false;
    }
    if(!aBuffer.setByteCount(c,true)) return false;
    return true;
  }
private:
  const IDictionary& fDictionary;
  std::ostream& fOut;
};

}

#endif

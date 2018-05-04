#ifndef Rio_Leaf_h
#define Rio_Leaf_h

#include <Rio/Interfaces/IBuffer.h>

#include <Rio/Tree/BaseLeaf.h>

#include <Rio/Tree/Branch.h>

namespace Rio {

template <class T> class Leaf : public BaseLeaf {
public: // Rio::ILeaf 
  virtual int number() const { return (int)fValue[0]; } //Rio
  virtual void* valuePointer() const { return fValue; }
  virtual bool fillBasket(IBuffer& aBuffer){
    int len = length();
    if (fPointer) fValue = *fPointer;
    return aBuffer.writeFastArray(fValue,len);
  }
  virtual bool readBasket(IBuffer& aBuffer){
    if (!fLeafCount && (fNdata == 1)) {
      if(!aBuffer.read(fValue[0])) {
        std::ostream& out = this->branch().out();
        out << "Rio::Leaf::readBasket : \"" << name() << "\" :"
            << " read value failed."
            << std::endl;
        return false;
      }
      return true;
    }else {
      if (fLeafCount) {
        int len = fLeafCount->number();
        if (len > fLeafCount->maximum()) {
	  std::ostream& out = this->branch().out();
          out << "Rio::Leaf::readBasket : \"" 
              << name() << "\", len = " << len << " and max = " 
              << fLeafCount->maximum() << std::endl;
          len = fLeafCount->maximum();
        }
        fNdata = len * fLength;
        if(!aBuffer.readFastArray(fValue,len * fLength)) {
          std::ostream& out = this->branch().out();
          out << "Rio::Leaf::readBasket : \"" << name() << "\" :"
              << " readFastArray failed."
              << std::endl;
          return false;
        }
        return true;
      } else {
        if(!aBuffer.readFastArray(fValue,fLength)) {
          std::ostream& out = this->branch().out();
          out << "Rio::Leaf::readBasket : \"" << name() << "\" :"
              << " readFastArray(2) failed."
              << std::endl;
          return false;
        }
        return true;
      }
    }
  }
  virtual bool setAddress(void* aAddress) {
    updateNdata();
    if(fNewValue) {
      delete [] fValue;
      fValue = 0;
    }
    if (aAddress) {
      fNewValue = false;
      if (isIndirectAddress()) {
        fPointer = (T**) aAddress;
        int ncountmax = fLength;
        if (fLeafCount) ncountmax = fLength * (fLeafCount->maximum() + 1);
        if (ncountmax > fNdata || *fPointer == 0) {
          if (*fPointer) delete [] *fPointer;
          if (ncountmax > fNdata) fNdata = ncountmax;
          *fPointer = new T[fNdata];
        }
        fValue = *fPointer;
      } else {
        fValue = (T*)aAddress;
      }
    } else {
      fNewValue = true;
      fValue = new T[fNdata];
      fValue[0] = 0;
    }
    return true;
  }
public:
  Leaf(const IDictionary& aDictionary,Branch& aBranch)
  :BaseLeaf(aDictionary,aBranch)
  ,fMinimum(0),fMaximum(0),fValue(0),fPointer(0){
    fLengthType = sizeof(T);
  }
  Leaf(const IDictionary& aDictionary,Branch& aBranch,const std::string& aName)
  :BaseLeaf(aDictionary,aBranch,aName)
  ,fMinimum(0),fMaximum(0),fValue(0),fPointer(0){
    fLengthType = sizeof(T);
  }
  virtual ~Leaf(){
    if (fNewValue) delete [] fValue;
  }
  virtual T value(int aIndex = 0) const { return fValue[aIndex]; }
protected:
  T fMinimum;    //Minimum value if leaf range is specified
  T fMaximum;    //Maximum value if leaf range is specified
  T* fValue;     //!Pointer to data buffer
  T** fPointer;  //!Addresss of pointer to data buffer!
};
 
#define RIO_LEAF_HEADER(aLeaf,aType)\
class aLeaf : public Leaf< aType > {\
  RIO_META_HEADER(aLeaf)\
public: /*Rio::IObject*/\
  virtual void* cast(const IClass&) const;\
  virtual bool stream(IBuffer&);\
public: /*Rio::ILeaf*/\
  virtual void printValue(std::ostream&,int = 0) const;	\
public:\
  aLeaf(const IDictionary&,Branch&);\
  aLeaf(const IDictionary&,Branch&,const std::string&);\
  virtual ~aLeaf();\
};

RIO_LEAF_HEADER(LeafFloat,float)
RIO_LEAF_HEADER(LeafDouble,double)
RIO_LEAF_HEADER(LeafShort,short)
RIO_LEAF_HEADER(LeafInteger,int)
RIO_LEAF_HEADER(LeafCharacter,char)

}

#endif


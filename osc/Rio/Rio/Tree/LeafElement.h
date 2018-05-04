#ifndef Rio_LeafElement_h
#define Rio_LeafElement_h

#include <Rio/Tree/BaseLeaf.h>

namespace Rio {

class LeafElement : public BaseLeaf {
  RIO_META_HEADER(LeafElement)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public: // Rio::ILeaf 
  virtual int number() const;
  virtual void* valuePointer() const;
  virtual bool fillBasket(IBuffer&);
  virtual bool readBasket(IBuffer&);
  virtual bool setAddress(void*);
  virtual void printValue(std::ostream&,int = 0) const;
public:
  LeafElement(const IDictionary&,Branch&);
  LeafElement(const IDictionary&,Branch&,const std::string&,int,int);
  virtual ~LeafElement();
private:
  int fID;           //element serial number in fInfo
  int fType;         //leaf type
};
 
}

#endif

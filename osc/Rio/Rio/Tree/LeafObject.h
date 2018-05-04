#ifndef Rio_LeafObject_h
#define Rio_LeafObject_h

#include <Rio/Tree/BaseLeaf.h>

namespace Rio {

class LeafObject : public BaseLeaf {
  RIO_META_HEADER(LeafObject)
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
  IObject* object() const;
public:
  LeafObject(const IDictionary&,Branch&);
  LeafObject(const IDictionary&,Branch&,const std::string&,const IClass&);
  virtual ~LeafObject();
  IClass* objectClass() const;
private:
  IClass* fObjectClass;        //! pointer to class
  IObject** fObjAddress; //! Address of Pointer to object
  bool fVirtual;         // Support for Virtuality
  IObject* fInternal;
};
 
}

#endif

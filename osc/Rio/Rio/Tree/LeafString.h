#ifndef Rio_LeafString_h
#define Rio_LeafString_h

#include <Rio/Tree/BaseLeaf.h>

namespace Rio {

class LeafString : public BaseLeaf {
  RIO_META_HEADER(LeafString)
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
  LeafString(const IDictionary&,Branch&);
  LeafString(const IDictionary&,Branch&,const std::string&);
  virtual ~LeafString();
  virtual std::string value() const;
private:
  int    fMinimum;       //Minimum value if leaf range is specified
  int    fMaximum;       //Maximum value if leaf range is specified
  char*  fValue;         //!Pointer to data buffer
  //char** fPointer;       //!Address of pointer to data buffer
};
 
}

#endif

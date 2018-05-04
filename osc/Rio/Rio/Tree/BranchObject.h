#ifndef Rio_BranchObject_h
#define Rio_BranchObject_h

#include <Rio/Tree/Branch.h>

namespace Rio {

class BranchObject : public Branch {
  RIO_META_HEADER(BranchObject)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  BranchObject(const IDictionary&,Tree&);
  BranchObject(const IDictionary&,Tree&,const std::string&,const std::string&,
               const IClass&,IObject**,
               int = 32000,int = -1);
  virtual ~BranchObject();
  virtual void setAutoDelete(bool = true);
  virtual bool fill(int&);
  virtual bool findEntry(int,int&);
  virtual void reset();
  virtual bool setAddress(void*);
  virtual void setBasketSize(int);
private:
  bool updateAddress();
private:
  std::string fClassName;
  IObject* fOldObject;
  //bool fWarn;
};

}

#endif

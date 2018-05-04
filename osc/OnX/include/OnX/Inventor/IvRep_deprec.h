#ifndef OnX_IvRep_deprec_h
#define OnX_IvRep_deprec_h

#include <OnX/Interfaces/IRepresentation_deprec.h>

#include <inlib/cast>

class SoNode;

namespace OnX {

//  Embarqued class : we put the code in the .h so
// that applications using this class will not be compelled
// to link against libOnXInventor (and then all 
// related libs).

class IvRep : public virtual IRepresentation {
public: //IRepresentation
  virtual void* cast(const std::string& aClass) const {
    INLIB_IF_CAST(OnX::IvRep) 
    else return 0;
  }
  virtual void* representation() const { return fPointer;}
public:
  IvRep(SoNode* aNode):fPointer(aNode){}
  //IvRep(void* aObject):fPointer(aObject){}
  virtual ~IvRep(){}
private:
  void* fPointer;
};

}

#endif

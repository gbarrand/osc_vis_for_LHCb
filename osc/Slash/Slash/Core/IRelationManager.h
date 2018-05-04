#ifndef Slash_Core_IRelationManager_h
#define Slash_Core_IRelationManager_h

namespace Slash {namespace Core { class IRelation;}}
namespace Slash {namespace Core { class IRelationVisitor;}}

namespace Slash {

namespace Core {

class IRelationManager {
public:
  virtual ~IRelationManager() {};
public:
  virtual void add(IRelation*) = 0;
  virtual void visit(IRelationVisitor&) = 0;
  virtual void remove(IRelationVisitor&,bool schedule = false) = 0;

  virtual void removeWith(void*) = 0;
  virtual bool isValid(const IRelation&) const = 0;
};

} //Core

} //Slash

#endif

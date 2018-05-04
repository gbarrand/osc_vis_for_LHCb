#ifndef Slash_Core_IRelationVisitor_h
#define Slash_Core_IRelationVisitor_h

namespace Slash {namespace Core { class IRelation;}}

namespace Slash {

namespace Core {

class IRelationVisitor {
public:
  virtual ~IRelationVisitor() {};
public:
  virtual bool visit(IRelation&) = 0;
};

} //Core

} //Slash

#endif

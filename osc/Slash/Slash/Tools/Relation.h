#ifndef Slash_Tools_Relation_h
#define Slash_Tools_Relation_h

#include <Slash/Core/IRelationVisitor.h>

#include <Slash/Core/IRelation.h>

namespace Slash {

class RV_Find : public virtual Slash::Core::IRelationVisitor {
public: //IRelationVisitor
  virtual bool visit(Slash::Core::IRelation& aRelation) {
    if(fFirst!=aRelation.first()) return true; //continue
    if(fSecond!=aRelation.second()) return true; //continue
    fFound = true;
    return false; //stop visit = found if first and second
  }
public:
  RV_Find(const void* aFirst,const void* aSecond)
  :fFirst(aFirst),fSecond(aSecond)
  ,fFound(false){}
  virtual ~RV_Find(){}
private:
  const void* fFirst;
  const void* fSecond;
public:
  bool fFound;
};

class RV_First_Find : public virtual Slash::Core::IRelationVisitor {
public: //IRelationVisitor
  virtual bool visit(Slash::Core::IRelation& aRelation) {
    if(fFirst!=aRelation.first()) return true; //continue
    fFound = true;
    return false; //stop visit = found first
  }
public:
  RV_First_Find(const void* aFirst)
  :fFirst(aFirst),fFound(false){}
  virtual ~RV_First_Find(){}
private:
  const void* fFirst;
public:
  bool fFound;
};

class RV_Remove : public virtual Slash::Core::IRelationVisitor {
public: //IRelationVisitor
  virtual bool visit(Slash::Core::IRelation& aRelation) {
    if(fFirst!=aRelation.first()) return false;
    if(fSecond!=aRelation.second()) return false;
    return true; //remove if first and second
  }
public:
  RV_Remove(const void* aFirst,const void* aSecond)
  :fFirst(aFirst),fSecond(aSecond){}
  virtual ~RV_Remove(){}
private:
  const void* fFirst;
  const void* fSecond;
};

class RV_Or_Remove : public virtual Slash::Core::IRelationVisitor {
public: //IRelationVisitor
  virtual bool visit(Slash::Core::IRelation& aRelation) {
    if(fFirst==aRelation.first()) return true; //remove
    if(fSecond==aRelation.second()) return true; //remove
    return false;
 }
public:
  RV_Or_Remove(const void* aFirst,const void* aSecond)
  :fFirst(aFirst),fSecond(aSecond){}
  virtual ~RV_Or_Remove(){}
private:
  const void* fFirst;
  const void* fSecond;
};

class RV_First_Schedule : public virtual Slash::Core::IRelationVisitor {
public: //IRelationVisitor
  virtual bool visit(Slash::Core::IRelation& aRelation) {
    if(fFirst!=aRelation.first()) return true;
    aRelation.schedule();
    return true;
 }
public:
  RV_First_Schedule(const void* aFirst)
  :fFirst(aFirst){}
  virtual ~RV_First_Schedule(){}
private:
  const void* fFirst;
};

}

#endif

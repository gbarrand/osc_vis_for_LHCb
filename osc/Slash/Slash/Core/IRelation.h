#ifndef Slash_Core_IRelation_h
#define Slash_Core_IRelation_h

#include <string>

namespace Slash {

namespace Core {

class IRelation {
public:
  virtual ~IRelation() {};
  virtual void* cast(const std::string&) const = 0;
public:
  /**
   * @return The name of the relation.
   */
  virtual std::string name() const = 0;

  virtual bool schedule() = 0;
  virtual void unSchedule() = 0;

  virtual void* first() const = 0;
  virtual void* second() const = 0;
};

} //Core

} //Slash

#endif

#ifndef Slash_Meta_IClass_h
#define Slash_Meta_IClass_h

#include <string>
#include <vector>

namespace Slash { namespace Meta {class IObject;}}

namespace Slash {

namespace Meta {

class IClass {
public:
  virtual ~IClass() {}

  /** @return The name of the described class. */
  virtual std::string name() const = 0;

  /** @return The version of the described class. */
  virtual unsigned int version() const = 0;

  /** Object factory for the described class. */
  virtual IObject* create() = 0;
};

} //Meta

} //Slash

#endif

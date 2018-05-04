#ifndef Slash_Meta_IDictionary_h
#define Slash_Meta_IDictionary_h

#include <string>

namespace Slash { namespace Meta {class IClass;}}
namespace Slash { namespace Meta {class IObject;}}

namespace Slash {

namespace Meta {

class IDictionary {
public:
  virtual ~IDictionary() {}
public:

  /** Find a class describer given its name.
   * @param aName : The name of the class to find.
   * @return The class describer (an IClass). 
   *         It returns 0 in case of failure. */
  virtual IClass* find(const std::string& aName) const = 0;

  /** Add a class describer to the dictionary.
   * @param aClass : The class describer. */
  virtual bool add(IClass* aClass) = 0;


  virtual unsigned int numberOfClasses() const = 0;
  virtual const IClass* getClass(unsigned int index) const = 0;

};

} //Meta

} //Slash

#endif

#ifndef Rio_IObject_h
#define Rio_IObject_h

#include <string>

namespace Rio {

class IClass;
class IBuffer;
class IDictionary;

/**
 * @class IObject
 *
 *  IObject is the interface to handle basic informations
 * in order that a C++ object be stored/retrieved in a file.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IObject {
public:
  virtual ~IObject() {}

  /** @return The class describer of the IObject. */
  virtual const IClass& isA() const = 0;

  /** @return The name of the object. */
  virtual const std::string& name() const = 0;

  /** @return The title of the object. */
  virtual const std::string& title() const = 0;

  /** Stream the object in the given buffer.
   * @param aBuffer : The buffer to write/read the data of the object. */
  virtual bool stream(IBuffer& aBuffer) = 0;

  /** Print the object.
   * @param aPrinter : The out stream to print on. */
  virtual void out(std::ostream& aPrinter) const = 0;

  /** Cast the object to the given class describer.
   *  This method permits to avoid good part of the 
   *  inefficient dynamic_cast.
   * 
   * @param aClass : The class describer to cast on.
   * @return The pointer resulting from the cast.
   *         Return 0 in case of failure. */
  virtual void* cast(const IClass& aClass) const = 0;

  virtual const IDictionary& dictionary() const = 0;
};


/** A helper template to use the IObject::cast method.
 *  For example, to know if a Rio::IObject is a directory :
 * @verbatim
 *    #include <Rio/Core/Directory.h>
 *  ...
 *    Rio::IObject* obj = ...
 *    Rio::Directory* dir = Rio::cast<Rio::Directory>(obj);
 * @endverbatim
 * 
 * @param aObject : object to cast.
 * @return The pointer resulting from the cast.
 *         Return 0 in case of failure. */

template <class T> 
inline T* cast(IObject& aObject) {
  //return (T*)aObject.cast(T::is(aObject.dictionary()));
  return dynamic_cast<T*>(&aObject);
}

// Used in implementations of interfaces cast methods.
#define Rio_SCast(aClass) (void*)static_cast<const aClass*>(this)

}

#endif

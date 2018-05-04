#ifndef Rio_IDirectory_h
#define Rio_IDirectory_h

#include <string>
#include <vector>
#include <ostream>

#include <Rio/Interfaces/Seek.h>

namespace Rio {

class IPrinter;
class IObject;
class IFile;
class IClass;
class IKey;

/**
 * @class IDirectory
 *
 *  IDirectory is the "user" interface to a Rio directory.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IDirectory {
public:
  virtual ~IDirectory(){}

  /** Create a subdirectory. 
   * @param aName : name of the created directory.
   * @param aTitle : title of the created directory. */
  virtual IDirectory* mkdir(const std::string& aName,
                            const std::string& aTitle = "") = 0;

  /** Add an object in the directory. 
   * @param aObject : the object to add. */
  virtual void appendObject(IObject* aObject) = 0;

  /** Remove an object in the directory. 
   * @param aObject : the object to remove. */
  virtual void removeObject(IObject* aObject) = 0;

  /** Find an object knowing its name. 
   * @param aName : the name of the searched object.
   * @return The found object. Return 0 if not found. */
  virtual IObject* find(const std::string& aName) = 0;

  /** Find a subdirectory knowing its name.
   * @param aName : the name of the searched directory.
   * @return The found directory. Return 0 if not found. */
  virtual IDirectory* findDirectory(const std::string& aName) = 0;

  /** Is a subdirectory.
   * @param aName : the name of the searched directory.
   * @return The found directory. Return 0 if not found. */
  virtual bool isADirectory(const std::string& aName) = 0;

  /** Print directory infos. 
   * @param aPrinter : a printer to write things on. */
  virtual void print(IPrinter& aPrinter) const = 0;

  /** @return The file containing this directory. */
  virtual IFile& file() const = 0;

  /** @return The out stream attached to the directory. */
  virtual std::ostream& out() const = 0;

  /** @return True if directory is writable, false if not. */
  virtual bool isWritable() const = 0;

  /** @return The name of the directory. */
  virtual const std::string& name() const = 0;

  /** @return The title of the directory. */
  virtual const std::string& title() const = 0;

  /** @return True if the file is in a closing phase. */
  virtual bool isClosing() const = 0;

  /** Save an object (write it in the file). */
  virtual bool saveObject(IObject&) = 0;

  /** Remove an object or a key knowing its name.*/
  virtual bool remove(const std::string&) = 0;

  /** Find an object in memory knowing its name. 
   * @param aName : name of the object to search.
   * @return The found object. Return 0 if not found. */
  virtual IObject* findInMemory(const std::string& aName) = 0;

  /** @return The parent directory. Return 0 if the directory has no parent.*/
  virtual IDirectory* parent() const = 0;

  /** @return The path from the root directory of the file.*/
  virtual std::string path() const = 0;

  /** @return The keys on file for this directory. */
  virtual std::vector<IObject*> objects() const = 0;

  /** @return The keys on file for this directory. */
  virtual std::vector<IKey*> keys() const = 0;

  /** @return Position in file of this directory. */
  virtual Seek seekDirectory() const = 0;

  /** See IObject::cast for comments. */
  virtual void* cast(const IClass&) const = 0;

  /** @return True if directory correctly created. */
  virtual bool isValid() const = 0;

};

template <class T> 
inline T* cast(IDirectory& aObject) {
  //return (T*)aObject.cast(T::is(aObject.file().dictionary()));
  return dynamic_cast<T*>(&aObject);
}

}

#endif

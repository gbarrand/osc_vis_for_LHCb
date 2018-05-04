#ifndef Rio_IFile_h
#define Rio_IFile_h

#include <string>
#include <ostream>

#include <Rio/Interfaces/Seek.h>

namespace Rio {

class IDictionary;
class IDirectory;
class IClass;

/**
 * @class IFile
 *
 *  IFile is the "user" interface to a Rio file.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IFile {
public:
  virtual ~IFile(){}

  /** @return The name of the file. */
  virtual const std::string& name() const = 0;

  /** @return The dictionary used to read/write objects. */
  virtual const IDictionary& dictionary() const = 0;

  /** @return The out stream used to print messages. */
  virtual std::ostream& out() const = 0;

  /** Set the verbose level : 
   *  - 0 = only error messages are dump.
   *  - 1 = error and information messages are dump. */
  virtual void setVerboseLevel(int) = 0;

  /** @return The verbose level. */
  virtual int verboseLevel() const = 0;

  /** Set the compression level.
   * @param aLevel : compression level. */
  virtual void setCompressionLevel(int aLevel = 1) = 0;

  /** @return The compression level. */
  virtual int compressionLevel() const = 0;

  /** @return The root directory of the file. */
  virtual IDirectory& directory() const = 0;

  /** Write data on file. */
  virtual bool write(int&) = 0;

  /** Close the file. */
  virtual bool close() = 0;

  /** @return True if file is open, false if not. */
  virtual bool isOpen() const = 0;

  /** @return True if file is writable, false if not. */
  virtual bool isWritable() const = 0;

  virtual Seek end() const = 0;

  /** See IObject::cast. */
  virtual void* cast(const IClass& aClass) const = 0;
};

template <class T> 
inline T* cast(IFile& aObject) {
  //return (T*)aObject.cast(T::is(aObject.dictionary()));
  return dynamic_cast<T*>(&aObject);
}

}

#endif

#ifndef Rio_IClass_h
#define Rio_IClass_h

#include <string>
#include <vector>
#include <Rio/Interfaces/typedefs.h>

namespace Rio { class IBuffer;} //FIXME : should not know IBuffer at all.
namespace Rio { class IObject;}

namespace Rio {

/**
 * @class IClass 
 *
 *  IClass is the interface for the Rio class description.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IClass {
public:
  virtual ~IClass() {}
public:
  /** @return The name of the described class. */
  virtual const std::string& name() const = 0;

  /** @return The name put in file of the described class. */
  virtual const std::string& inStoreName() const = 0;

  /** @return The version of the described class. */
  virtual short version() const = 0;

  /** Object factory for the described class.
   * @param aArgs : A vector of pair(int,void*) to pass arguments 
   *                to the constructor. 
   */
  virtual IObject* create(const Arguments& aArgs) = 0;

  /** Factory for objects which are not IObject.
   * @param aArgs : A vector of pair(int,void*) to pass arguments 
   *                to the constructor. 
   */
  virtual void* createOpaque(const Arguments& aArgs) = 0;

  /** Compute and return the class check sum.
   * The class ckecksum is used to uniquely identify a class version.
   * The check sum is built from the names/types of base classes and
   * data members
   * @return The checksum. */
  virtual unsigned int checkSum() const = 0;


  /** Used by Rio::BranchElement::fillLeaves.
   *  Similar action than the TStreamerInfo::WriteBuffer.
   * @return true if success. */
  virtual bool writeBuffer(IBuffer&,void*) const = 0;

  /** Used by Rio::BranchElement::readLeaves.
   *  Similar action than the TStreamerInfo::ReadBuffer.
   * @return true if success. */
  virtual bool readBuffer(IBuffer&,void*) const = 0;
};

}

#endif

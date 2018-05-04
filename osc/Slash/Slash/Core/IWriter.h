#ifndef Slash_Core_IWriter_h
#define Slash_Core_IWriter_h

/**
 * @class Slash::Core::IWriter
 *
 *  IWriter is the interface to handle a basic writer.
 *
 * @author Guy Barrand
 * @date 2002
 */

#include <string>

namespace Slash {

namespace Core {

class IWriter {
public:
  virtual ~IWriter() {}

  /** Write the given buffer.
   * @param aString : string to write. */
  virtual bool write(const std::string& aString) = 0;

  /** To enable / disable the printer. */
  virtual void disable() = 0;
  virtual void enable() = 0;
  virtual bool enabled() const = 0;

  virtual bool flush() = 0;

};

} //Core

} //Slash

#endif

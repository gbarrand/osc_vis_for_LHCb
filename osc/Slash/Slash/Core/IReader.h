#ifndef Slash_Core_IReader_h
#define Slash_Core_IReader_h

/**
 * @class Slash::Core::IReader
 *
 *  IReader is the interface to handle a basic reader.
 *
 * @author Guy Barrand
 * @date 2003
 */

#include <string>

namespace Slash {

namespace Core {

class IReader {
public:
  virtual ~IReader() {}

  /** Read the given buffer.
   * @param aBuffer : buffer to read into. 
   * @param aLength : length to read.
   * @param aReadLength : length read.
   * @return false in case of failure.
   */
  virtual bool read(void* aBuffer,unsigned int aLength,int& aReadLength) = 0;

  /** Read a string.
   * @param aBuffer : buffer to read into. 
   * @param aLength : length to read.
   * @param aString : returned string.
   * @return false in case of failure.
   */
  virtual bool read(void* aBuffer,unsigned int aLength,std::string& aString) = 0;

  /** Read a string. 
   * A null terminated string is returned in aBuffer.
   * @param aBuffer : buffer to read into. 
   * @param aLength : length to read.
   * @param aStringLength : the length of the string.
   * @return false in case of failure.
   */
  virtual bool sread(void* aBuffer,
                     unsigned int aLength,
                     unsigned int& aStringLength) = 0;

  /** Rewind the data container. */
  virtual bool rewind() = 0; 

  /** To enable / disable the reader. */
  virtual void disable() = 0;
  virtual void enable() = 0;
  virtual bool enabled() const = 0;

};

} //Core

} //Slash

#endif

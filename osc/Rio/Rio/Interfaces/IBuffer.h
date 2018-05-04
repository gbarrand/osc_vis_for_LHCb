#ifndef Rio_IBuffer_h
#define Rio_IBuffer_h

#include <string>
#include <vector>
#include <ostream>

#include <Rio/Interfaces/typedefs.h>

namespace Rio {

class IObject;

/**
 * @class IBuffer
 *
 *  IBuffer is the interface to data buffer 
 * used by object streamer methods.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IBuffer {
public:
  virtual ~IBuffer() {}

  /** Methods to read data from the buffer. */
  virtual bool read(char&) = 0;
  virtual bool read(bool&) = 0;
  virtual bool read(short&) = 0;
  virtual bool read(int&) = 0;
  virtual bool read(int64&) = 0;
  virtual bool read(float&) = 0;
  virtual bool read(double&) = 0;
  virtual bool read(unsigned char&) = 0;
  virtual bool read(unsigned short&) = 0;
  virtual bool read(unsigned int&) = 0;
  virtual bool read(uint64&) = 0;
  virtual bool read(std::string&) = 0;
  virtual bool read(std::vector<std::string>&) = 0;
  virtual bool readArray(unsigned char*&,int&) = 0;
  virtual bool readArray(unsigned int*&,int&) = 0;
  virtual bool readArray(uint64*&,int&) = 0;
  virtual bool readArray(char*&,int&) = 0;
  virtual bool readArray(short*&,int&) = 0;
  virtual bool readArray(int*&,int&) = 0;
  virtual bool readArray(int64*&,int&) = 0;
  virtual bool readArray(float*&,int&) = 0;
  virtual bool readArray(double*&,int&) = 0;
  virtual bool readFastArray(bool*,int) = 0;
  virtual bool readFastArray(char*,int) = 0;
  virtual bool readFastArray(int*,int) = 0;
  virtual bool readFastArray(int64*,int) = 0;
  virtual bool readFastArray(unsigned char*,int) = 0;
  virtual bool readFastArray(unsigned short*,int) = 0;
  virtual bool readFastArray(unsigned int*,int) = 0;
  virtual bool readFastArray(uint64*,int) = 0;
  virtual bool readFastArray(float*,int) = 0;
  virtual bool readFastArray(double*,int) = 0;
  virtual bool readFastArray(short*,int) = 0;

  /** Methods to write data in the buffer. */
  virtual bool write(char) = 0;
  virtual bool write(bool) = 0;
  virtual bool write(short) = 0;
  virtual bool write(int) = 0;
  virtual bool write(int64) = 0;
  virtual bool write(float) = 0;
  virtual bool write(double) = 0;
  virtual bool write(unsigned char) = 0;
  virtual bool write(unsigned short) = 0;
  virtual bool write(unsigned int) = 0;
  virtual bool write(uint64) = 0;
  //Warning : for constant strings, someone must do a :
  //     write(std::string("<string>"))
  // else a :
  //     write("<string>")
  // will be casted on write(bool) !!!
  virtual bool write(const std::string&) = 0;
  virtual bool write(const char*) = 0;
  virtual bool write(const std::vector<std::string>&) = 0;
  virtual bool writeArray(const unsigned char*,int) = 0;
  virtual bool writeArray(const unsigned int*,int) = 0;
  virtual bool writeArray(const uint64*,int) = 0;
  virtual bool writeArray(const char*,int) = 0;
  virtual bool writeArray(const short*,int) = 0;
  virtual bool writeArray(const int*,int) = 0;
  virtual bool writeArray(const int64*,int) = 0;
  virtual bool writeArray(const float*,int) = 0;
  virtual bool writeArray(const double*,int) = 0;
  virtual bool writeFastArray(const bool*,int) = 0;
  virtual bool writeFastArray(const char*,int) = 0;
  virtual bool writeFastArray(const float*,int) = 0;
  virtual bool writeFastArray(const int*,int) = 0;
  virtual bool writeFastArray(const int64*,int) = 0;
  virtual bool writeFastArray(const unsigned char*,int) = 0;
  virtual bool writeFastArray(const unsigned short*,int) = 0;
  virtual bool writeFastArray(const unsigned int*,int) = 0;
  virtual bool writeFastArray(const uint64*,int) = 0;
  virtual bool writeFastArray(const double*,int) = 0;
  virtual bool writeFastArray(const short*,int) = 0;

  /** @return true if buffer is in read mode, false if not.*/
  virtual bool isReading() const = 0;

  /** Below method is used in a streamer to write the 
   * version and the byte count of the streamed data. It is used in 
   * the following way :
   * @verbatim
   *   bool MyObject::stream(IBuffer& aBuffer) {
   *     if (!aBuffer.isReading()) {
   *       // If the "version" of the MyObject class is "2" :
   *       unsigned int c;
   *       if(!aBuffer.writeVersion(2,c)) return false;
   *       .... 
   *       write things with the write methods.
   *       ....
   *       if(!aBuffer.setByteCount(c,true)) return false;
   *      }
   *     return true;
   *   }
   * @endverbatim
   *
   * @param aVersion : the version. 
   * @return false in case of failure. */
  virtual bool writeVersion(short aVersion) = 0;

  /**
   * @param aVersion : the version. 
   * @param aPos : position where to store possible byte count.
   * @return false in case of failure. */
  virtual bool writeVersion(short aVersion,unsigned int& aPos) = 0;

  /** See writeVersion comment. */
  virtual bool setByteCount(unsigned int aPosition,
                            bool aUseByteCount = false) = 0;

  /** The below method is used in a streamer to read the 
   * version and the byte count of the streamed data. It is used in 
   * the following way :
   * @verbatim
   *   bool MyObject::stream(IBuffer& aBuffer) {
   *     if (aBuffer.isReading()) {
   *       unsigned int s, c;
   *       short version; 
   *       if(!aBuffer.readVersion(version,&s, &c)) return false;
   *       .... 
   *       read things according the version with the read methods.
   *       ....
   *       // If the stored name of the class is "MyObject"
   *       // (could be retreived with a MyObject::is().name()) :
   *       aBuffer.checkByteCount(s, c,"MyObject");
   *     }
   *     return true;
   *   }
   * @endverbatim
   */
  virtual bool readVersion(short& aVersion) = 0;

  virtual bool readVersion(short& aVersion,
                           unsigned int& aStartPosition, 
                           unsigned int& aByteCount) = 0;

  /** See readVersion comment. */
  virtual bool checkByteCount(unsigned int aStartPosition,
                              unsigned int aByteCount,
                              const std::string& aClassName) = 0;

  /** To read an IObject from the buffer. Internally the class name
   * is read from the buffer. From it, the dictionary is used 
   * to retrieve the IClass describing the class. The IClass::create
   * method is used to create an object. Some arguments may be passed
   * to the constructor. In Rio we are not limited to a default constructor
   * with no arguments.
   * 
   * @param aArgs : array of arguments to the constructors.
   * @param aObject : A pointer to the created object. If the IClass::create method
   *         put the object in a IDirectory, the caller do not have to delete
   *         the returned object. If not, the caller has the responsibility 
   *         of managing the object.
   */
  virtual bool readObject(const Arguments& aArgs,IObject*& aObject) = 0;

  /** Write an object. */
  virtual bool writeObject(IObject*) = 0;

  /** Length of written data. */
  virtual int length() const = 0;

  /** Internal usage only. 
   * It is not advised to manipulate directly the buffer. */
  virtual char* buffer() const = 0;

  virtual int bufferSize() const = 0;
  virtual char*& currentPosition() = 0;
  virtual char* maximumPosition() const = 0;
  virtual bool expand(int) = 0;

  /** Internal usage only. */
  virtual void setBufferOffset(int = 0) = 0;

  /** @return The out stream attached to the file. */
  virtual std::ostream& out() const = 0;

};


// Helpers :
template <class T>
inline bool readArray(Rio::IBuffer& aBuffer,std::vector<T>& aArray) {
  aArray.clear();
  T* buffer = 0;
  int n;
  if(!aBuffer.readArray(buffer,n)) return false;
  if(!buffer) return true;
  aArray.resize(n);
  for(int index=0;index<n;index++) {
    aArray[index] = buffer[index];
  }
  delete [] buffer;
  return true;
}

template <class T>
inline bool writeArray(Rio::IBuffer& aBuffer,const std::vector<T>& aArray) {
  return aBuffer.writeArray(&(aArray[0]),(int)aArray.size());
}

template <class T>
inline bool readArray2(Rio::IBuffer& aBuffer,std::vector< std::vector<T> >& aArray) {
  int n;
  if(!aBuffer.read(n)) return false;
  aArray.resize(n);
  for(int index=0;index<n;index++) {
    if(!readArray< T >(aBuffer,aArray[index])) return false;
  }
  return true;
}

template <class T>
inline bool writeArray2(Rio::IBuffer& aBuffer,const std::vector< std::vector<T> >& aArray) {
  if(!aBuffer.write((int)aArray.size())) return false;
  for(unsigned int index=0;index<aArray.size();index++) {
    if(!writeArray< T >(aBuffer,aArray[index])) return false;
  }
  return true;
}

}

#endif

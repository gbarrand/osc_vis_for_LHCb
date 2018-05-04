#ifndef Slash_Tools_StreamBuffer_h
#define Slash_Tools_StreamBuffer_h

// should not use this class.
// Only needed because of Slash::BaseSession::printer.

// inheritance :
#include <sstream>

#include <Slash/Core/IWriter.h>
#include <cstring> //memcpy
#include <cstdio> //EOF

namespace Slash {

class StreamBuffer : public std::streambuf {
public:
  inline StreamBuffer(Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter)
  ,fBuffer(0)
  ,fSize(4095)
  ,fCount(0)
  {
    fBuffer = new char[fSize+1];
  }

  virtual ~StreamBuffer(){delete [] fBuffer;}
private:
  inline StreamBuffer(const StreamBuffer& aFrom)
  :std::basic_streambuf<char, std::char_traits<char> >()
  ,fPrinter(aFrom.fPrinter){}

  inline StreamBuffer& operator=(const StreamBuffer&){return *this;}
public:
  inline int overflow(int aChar) {
    int result = 0;
    if(fCount>=fSize) {
      fBuffer[fCount] = '\0';
      fCount = 0;
      result = receiveString();
    }
    fBuffer[fCount] = aChar;
    fCount++;
    if(aChar=='\n') {
      fBuffer[fCount] = '\0';
      fCount = 0;
      result = receiveString();
    }
    return result;
  }

  inline int sync() {
    fBuffer[fCount] = '\0';
    fCount = 0;
    return receiveString ();
  }

#ifdef WIN32
  inline int underflow() {return 0;}
#endif
private:
  inline int receiveString() {
    std::string stringToSend = fBuffer;
    fPrinter.write(stringToSend);
    return 0;
  }

private:
  Slash::Core::IWriter& fPrinter;
  char* fBuffer;
  unsigned int fSize;
  unsigned int fCount;
};

}
 

#endif


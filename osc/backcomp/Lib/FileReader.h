#ifndef Lib_FileReader_h
#define Lib_FileReader_h

#include <Slash/Core/IReader.h>

#include <string>
#include <cstring>
#include <cstdio>

namespace Slash {namespace Core {class IWriter;}}

namespace Lib {

class FileReader : public virtual Slash::Core::IReader {
public: //Slash::Core::IReader
  virtual bool read(void* aBuffer,unsigned int aLength,std::string& aString) {
    if(!fFILE) {
      aString.resize(0);
      return false;
    }
  
    char* buffer = (char*)aBuffer;
    if(::fgets(buffer,aLength,(FILE*)fFILE)==NULL) {
      disable();
      aString.resize(0);
      return false; //EOF
    }
  
    size_t l = ::strlen(buffer);
    //  On Windows, editors when saving binary files,
    // put \r\n at place of \n ; we then look for \r\n.
    if( (l>=2) && (buffer[l-2]=='\r') && (buffer[l-1]=='\n') ) {
      buffer[l-2] = '\0';
    } else if( (l>=1) && (buffer[l-1]=='\n') ) {
      buffer[l-1] = '\0';
    }
  
    aString = buffer;
    return true;
  }

  virtual bool read(void* aBuffer,unsigned int aLength,int& aReadLength) {
    aReadLength = 0;
    if(!fFILE) return false;
    size_t len = ::fread(aBuffer,1,aLength,(FILE*)fFILE);
    aReadLength = (size_t)len;
    return true;
  }
  virtual bool rewind() {
    if(!fFILE) return false;
    ::rewind((FILE*)fFILE);
    return true;
  }


  virtual bool sread(void* aBuffer,unsigned int aLength,
                     unsigned int& aStringLength) {
    if(!fFILE) {
      aStringLength = 0;
      return false;
    }
  
    char* buffer = (char*)aBuffer;
    if(::fgets(buffer,aLength,(FILE*)fFILE)==NULL) {
      disable();
      aStringLength = 0;
      return false; //EOF
    }
  
    size_t l = ::strlen(buffer);
    //  On Windows, editors when saving binary files,
    // put \r\n at place of \n ; we then look for \r\n.
    if( (l>=2) && (buffer[l-2]=='\r') && (buffer[l-1]=='\n') ) {
      buffer[l-2] = '\0';
      l -= 2;
    } else if( (l>=1) && (buffer[l-1]=='\n') ) {
      buffer[l-1] = '\0';
      l -= 1;
    }
  
    aStringLength = l;
    return true;
  }

  virtual void disable()  {  
    if(!fFILE) return; // Already closed.
    if(fOwner) ::fclose((FILE*)fFILE);
    fFILE = 0;
  }
  virtual void enable() {
    if(fFILE) return; // Already open.
    if(fOwner) fFILE = ::fopen(fFileName.c_str(),"rb");
  }
  virtual bool enabled() const {return fFILE ? true : false;}

public:
  FileReader(const std::string& aFileName,Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter)
  ,fFileName(aFileName)
  ,fFILE(0)
  ,fOwner(true)
  {
    //////Lib::Debug::increment("Lib::FileReader");
    fFILE = ::fopen(aFileName.c_str(),"rb");
  }
  FileReader(void* aFILE,Slash::Core::IWriter& aPrinter)
  :fPrinter(aPrinter)
  ,fFILE(aFILE)
  ,fOwner(false)
  {
    //////Lib::Debug::increment("Lib::FileReader");
  }
  virtual ~FileReader() {
    if(fOwner && fFILE) ::fclose((FILE*)fFILE);
    //////Lib::Debug::decrement("Lib::FileReader");
  }

private:
  Slash::Core::IWriter& fPrinter;
  std::string fFileName;
  void* fFILE;
  bool fOwner;
};

}

#endif

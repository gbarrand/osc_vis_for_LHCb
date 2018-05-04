#ifndef Rio_Buffer_h
#define Rio_Buffer_h

//////////////////////////////////////////////////////////////////////////
// Buffer base class used for serializing objects.                      //
//////////////////////////////////////////////////////////////////////////

// Inheritance :
#include <Rio/Interfaces/IBuffer.h>

#include <string>
#include <Rio/Core/Map.h> //For inline resetMap.

#include <vector> //GB

namespace Rio {

class IDictionary;
class IObject;
class IClass;
class Map;

class Buffer : public virtual IBuffer  {
public: //Rio::IBuffer
  virtual bool read(char&);
  virtual bool read(bool&);
  virtual bool read(short&);
  virtual bool read(int&);
  virtual bool read(int64&);
  virtual bool read(float&);
  virtual bool read(double&);
  virtual bool read(unsigned char&);
  virtual bool read(unsigned short&);
  virtual bool read(unsigned int&);
  virtual bool read(uint64&);
  virtual bool read(std::string&);
  virtual bool read(std::vector<std::string>&);
  virtual bool readArray(unsigned char*&,int&);
  virtual bool readArray(unsigned int*&,int&);
  virtual bool readArray(uint64*&,int&);
  virtual bool readArray(char*&,int&);
  virtual bool readArray(short*&,int&);
  virtual bool readArray(int*&,int&);
  virtual bool readArray(int64*&,int&);
  virtual bool readArray(float*&,int&);
  virtual bool readArray(double*&,int&);
  virtual bool readFastArray(bool*,int);
  virtual bool readFastArray(char*,int);
  virtual bool readFastArray(int*,int);
  virtual bool readFastArray(int64*,int);
  virtual bool readFastArray(unsigned char*,int);
  virtual bool readFastArray(unsigned short*,int);
  virtual bool readFastArray(unsigned int*,int);
  virtual bool readFastArray(uint64*,int);
  virtual bool readFastArray(float*,int);
  virtual bool readFastArray(double*,int);
  virtual bool readFastArray(short*,int);

  virtual bool write(char);
  virtual bool write(bool);
  virtual bool write(short);
  virtual bool write(int);
  virtual bool write(int64);
  virtual bool write(float);
  virtual bool write(double);
  virtual bool write(unsigned char);
  virtual bool write(unsigned short);
  virtual bool write(unsigned int);
  virtual bool write(uint64);
  virtual bool write(const std::string&);
  virtual bool write(const char*);
  virtual bool write(const std::vector<std::string>&);
  virtual bool writeArray(const unsigned char*,int);
  virtual bool writeArray(const unsigned int*,int);
  virtual bool writeArray(const uint64*,int);
  virtual bool writeArray(const char*,int);
  virtual bool writeArray(const short*,int);
  virtual bool writeArray(const int*,int);
  virtual bool writeArray(const int64*,int);
  virtual bool writeArray(const float*,int);
  virtual bool writeArray(const double*,int);
  virtual bool writeFastArray(const bool*,int);
  virtual bool writeFastArray(const char*,int);
  virtual bool writeFastArray(const float*,int);
  virtual bool writeFastArray(const int*,int);
  virtual bool writeFastArray(const int64*,int);
  virtual bool writeFastArray(const unsigned char*,int);
  virtual bool writeFastArray(const unsigned short*,int);
  virtual bool writeFastArray(const unsigned int*,int);
  virtual bool writeFastArray(const uint64*,int);
  virtual bool writeFastArray(const double*,int);
  virtual bool writeFastArray(const short*,int);

  virtual bool isReading() const;
  virtual bool readVersion(short&);
  virtual bool readVersion(short&,unsigned int&,unsigned int&);
  virtual bool writeVersion(short);
  virtual bool writeVersion(short,unsigned int&);
  virtual bool checkByteCount(unsigned int ,unsigned int,const std::string&);
  virtual bool setByteCount(unsigned int,bool = false);
  virtual bool readObject(const Arguments&,IObject*&);
  virtual bool writeObject(IObject*);
  virtual void setBufferOffset(int = 0);
  virtual char* buffer() const;
  virtual int length() const;

  virtual int bufferSize() const;
  virtual char*& currentPosition();
  virtual char* maximumPosition() const;
  virtual bool expand(int);

  virtual std::ostream& out() const;
public:
  enum Mode { 
    READ = 0, 
    WRITE = 1 
  };
  enum Size { 
    INITIAL = 1024, 
    MINIMAL = 128 
  };
  Buffer(const IDictionary&,std::ostream&,Mode,int = INITIAL,char* = 0);
  virtual ~Buffer();
public:
  // Get/set :
  bool isWriting() const;
  int displacement() const;
  // inlined to optimize reading.
  inline void setDisplacement(int aSkipped) { 
    fDisplacement = (int)(inline_length() - aSkipped);
  }
  inline void setDisplacement() { fDisplacement = 0;}
  Mode mode() const;
  // Else :
  void setBuffer(void*,unsigned int = 0);
  void setReadMode();
  void setWriteMode();
  bool mapObject(const void*,unsigned int = 1);

  char* readString(char*,int);
  bool writeString(const char*);

  bool writeBuffer(const void*,int);

  // Optimization writing :
  inline int inline_length() const { return (int)(fBufCur - fBuffer);}
  inline bool inline_isReading() const { return (fMode & WRITE) == 0;}
  inline bool inline_isWriting() const { return (fMode & WRITE) != 0;}
  inline void resetMap() {
    if (inline_isWriting()) {
      delete fWriteMap;
      fWriteMap = 0;
    } else {
      delete fReadMap;
      fReadMap = 0;
    }
    fMapCount = 0;
    fDisplacement = 0;
  }
  // Optimization reading :
  inline void inline_setBufferOffset(int aOffset) { fBufCur = fBuffer+aOffset;}

  bool displaceMapped(unsigned int); //GB
private:
  // Buffer objects cannot be copied or assigned
  Buffer(const Buffer&);           // not implemented
  void operator=(const Buffer &);  // not implemented
private:
  bool writeClass(const IClass&);
  bool readClass(const IClass*,unsigned int&,IClass*&);
  bool initMap();
  void checkCount(unsigned int);
  bool checkClass(unsigned int,const IClass*); //GB
  bool checkObject(unsigned int,const IClass*); //GB : ret bool.
private:
  const IDictionary& fDictionary;
  std::ostream& fOut;
  int fVerboseLevel;
  Mode fMode;         //Read or write mode
  //
  int fVersion;       //Buffer format version
  int fBufSize;       //Size of buffer
  char* fBuffer;      //Buffer used to store objects
  char* fBufCur;      //Current position in buffer
  char* fBufMax;      //End of buffer
  int fMapCount;      //Number of objects or classes in map
  int fMapSize;       //Default size of map
  int fDisplacement;  //Value to be added to the map offsets
  // Maps :
  union {
    Map* fReadMap;    //Map containing id,object references during reading
    Map* fWriteMap;   //Map containing object,id pairs during writing
  };
  typedef unsigned int bufpos;
  std::vector< std::pair<bufpos,bufpos> > f_cls_mapped; //GB
  std::vector< std::pair<bufpos,bufpos> > f_obj_mapped; //GB
};

}

#endif

#ifndef Rio_StreamerInfo_h
#define Rio_StreamerInfo_h

#include <Rio/Core/Named.h>

namespace Rio {

class StreamerElement;
class ObjArray;

// sizeof(vtbl)         = 4
// sizeof(unsigned int) = 4
// sizeof(TObject)   = 12  = 2 * (unsigned int) + vtbl.
// sizeof(TString)   = 8   = char* + vtbl.
// sizeof(TNamed)    = 28  = TObject + 2 * TString.
// sizeof(TObjArray) = 40

class StreamerInfo : public Named {
  RIO_META_HEADER(StreamerInfo)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
  virtual void out(std::ostream&) const;
public:
  StreamerInfo(const IDictionary&,const IClass&,const std::string&);
  virtual ~StreamerInfo();
  // Get/Set :
  //inline int identifier() const{return fNumber;}
  // Else :
  void add(StreamerElement*);
private:
  //static int fgCount;
private:
  const IClass& fStreamedClass;
  unsigned int fCheckSum;    //checksum of original class
  int fStreamedClassVersion; //Class version identifier
  //int fNumber;               //!Unique identifier
  ObjArray* fElements;       //Array of TStreamerElements
};

}

#endif

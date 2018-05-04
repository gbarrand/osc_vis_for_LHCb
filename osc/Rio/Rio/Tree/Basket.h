#ifndef Rio_Basket_h
#define Rio_Basket_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Basket                                                               //
//                                                                      //
// The Basket objects are created at run time to collect Tree entries   //
// in buffers. When a Basket is full, it is written to the file.        //
// The Basket is kept in memory if there is enough space.               //
//  (see the fMaxVirtualsize of Tree).                                  //
//                                                                      //
// The Basket class derives from Key.                                   //
//////////////////////////////////////////////////////////////////////////

#include <string>

#include <Rio/Core/Object.h>
#include <Rio/Core/Key.h>

namespace Rio {

class Branch;
class IDirectory;

class Basket : public Object, public Key {
  RIO_META_HEADER(Basket)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  Basket(const IDictionary&,IDirectory&,Branch&);
  Basket(const IDictionary&,IDirectory&,Branch&,const std::string&,const std::string&);
  virtual ~Basket();
private:
  Basket(const Basket&);           // not implemented
  Basket& operator=(const Basket&);  // not implemented
public:
  // Get/set :
  int nevBuf() const;
  inline int nevBufSize() const { return fNevBufSize;}
  void setNevBufSize(int);
  inline int* entryOffset() const { return fEntryOffset;}
  inline int* displacement() const { return fDisplacement;}
  // Else :
  virtual void setReadMode();
  virtual void setWriteMode();
  virtual void update(int,int);
  virtual bool writeBuffer(int&);
  virtual int dropBuffers();
  bool readBasketBytes(Seek,int&);
  bool readBasketBuffers(Seek,int);
private:
  IDirectory& fDirectory;
  Branch& fBranch;
  Seek fFile_end; //GB
  int fBufferSize;      //fBuffer length in bytes
  int fNevBufSize;      //Length in Int_t of fEntryOffset
  int fNevBuf;          //Number of entries in basket
  int fLast;            //Pointer to last used byte in basket
  bool fHeaderOnly;     //True when only the basket header must be read/written
  int* fDisplacement;   //![fNevBuf] Displacement of entries in fBuffer(TKey)
  int* fEntryOffset;    //[fNevBuf] Offset of entries in fBuffer(TKey)
};

}

#endif

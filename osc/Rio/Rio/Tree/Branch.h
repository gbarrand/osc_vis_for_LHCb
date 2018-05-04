#ifndef Rio_Branch_h
#define Rio_Branch_h

#include <Rio/Interfaces/Seek.h>

#include <Rio/Core/Named.h>
#include <Rio/Core/ObjArrayT.h>

namespace Rio {

class Buffer;
class Tree;
class Basket;
class BaseLeaf;

class Branch : public Named {
  RIO_META_HEADER(Branch)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  Branch(const IDictionary&,Tree&);
  Branch(const IDictionary&,Tree&,const std::string&,const std::string&,
         void*,const std::string&,int,int);
  virtual ~Branch();
private:
  Branch(const Branch&);           // not implemented
  Branch& operator=(const Branch&);  // not implemented
public:
  virtual bool fill(int&);
  // Get/set :
  int basketSize() const;
  int entryOffsetLength() const;
  int writeBasket() const;
  void setEntryOffsetLength(int);
  Tree& tree() const;
  int compressionLevel() const;
  BaseLeaf* leaf(const std::string&);
  double entries() const;
  std::vector<BaseLeaf*>& leaves();
  char* address() const;
  //std::vector<Branch*>& branches();
  // Else :
  virtual bool fillLeaves(IBuffer&);
  virtual bool findEntry(int,int&);
  virtual bool readLeaves(IBuffer&);
  virtual std::ostream& out() const;
  virtual void reset();
  virtual bool setAddress(void*);
  bool isAutoDelete() const;
  virtual void setAutoDelete(bool = true);
  virtual void setBasketSize(int);
  virtual void dropBaskets();
private:
  Basket* getBasket(int);
protected:
  Tree& fTree;
private:
  bool fAutoDelete;
protected:
  ObjArrayT<Branch> fBranches;
  ObjArrayT<BaseLeaf> fLeaves;
  ObjArrayT<Basket> fBaskets;
  int fCompress;        //  (=1 branch is compressed, 0 otherwise)
  int fBasketSize;      //  Initial Size of  Basket Buffer
private:
  int fEntryOffsetLen;  //  Initial Length of fEntryOffset table in the basket buffers
  int fWriteBasket;     //  Last basket number written
protected:
  int fEntryNumber;     //  Current entry number (last one filled in this branch)
private:
  int fOffset;          //Offset of this branch
protected:
  int fMaxBaskets;      //  Maximum number of Baskets so far
private:
  int fSplitLevel;      //  Branch split level (v8 3.10.02)
protected:
  int fNleaves;         //! Number of leaves
private:
  int fReadBasket;      //! Current basket number when reading
protected:
  int fReadEntry;       //! Current entry number when reading
  double fEntries;      //  Number of entries
private:
  double fTotBytes;  //  Total number of bytes in all leaves before compression
  double fZipBytes;  //  Total number of bytes in all leaves after compression
  int fNBasketRAM;      //! Number of baskets in fBasketRAM
  int* fBasketRAM;      //! [fNBasketRAM] table of basket numbers in memory
protected:
  int* fBasketBytes;    //[fMaxBaskets] Lenght of baskets on file
  int* fBasketEntry;    //[fMaxBaskets] Table of first entry in eack basket
  Seek* fBasketSeek;    //[fMaxBaskets] Addresses of baskets on file
  // Stay with a char* (and not a void*) to follow the fancy ROOT logic.
  char* fAddress;       //! Address of 1st leaf (variable or object)
private:
  Buffer* fEntryBuffer; //! Buffer used to directly pass the content without streaming
};

}

#endif

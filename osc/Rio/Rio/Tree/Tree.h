#ifndef Rio_Tree_h
#define Rio_Tree_h

#include <Rio/Core/Named.h>
#include <Rio/Core/ObjArrayT.h>

namespace Rio {

class IDirectory;
class Branch;
class BaseLeaf;

class Tree : public Named {
  RIO_META_HEADER(Tree)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  Tree(const IDictionary&,IDirectory&);
  Tree(IDirectory&,const std::string&,const std::string&);
  virtual ~Tree();
private:
  Tree(const Tree&);           // not implemented
  Tree& operator=(const Tree&);  // not implemented
public:
  // Get/set :
  std::vector<BaseLeaf*>& leaves();
  std::vector<Branch*>& branches();
  IDirectory& directory() const;
  std::ostream& out() const;
  BaseLeaf* leaf(const std::string&);
  double entries() const;
  bool memoryFull(int) const;
  virtual int readEntry()  const;
  // Else :
  virtual Branch* createBranch(const std::string&,
                               void*,
                               const std::string&,
                               int = 32000);
  virtual Branch* createBranch(const std::string&,
                               const IClass&,IObject**,int = 32000);
  virtual Branch* createBranchElement(const std::string&,
                               const IClass&,void*,int = 32000);
  virtual void reset();
  virtual bool fill(int&);
  virtual void addTotBytes(int);
  virtual void addZipBytes(int);
  virtual bool autoSave();
  virtual bool findEntry(int,int&);
  virtual bool show(int = -1);
  virtual void incrementTotalBuffers(int);
  virtual void setAutoSave(int = 10000000);
  virtual Branch* findBranch(const std::string&);
  virtual int loadTree(int);
  virtual int maxVirtualSize() const;
  virtual void setMaxVirtualSize(int = 0);
private:
  IDirectory& fDirectory;
  ObjArrayT<Branch> fBranches;
  ObjArrayT<BaseLeaf> fLeaves;
  double fEntries;      //  Number of entries
  double fTotBytes;     //  Total number of bytes in all branches before compression
  double fZipBytes;     //  Total number of bytes in all branches after compression
  double fSavedBytes;   //  Number of autosaved bytes
  int fMaxVirtualSize;  //  Maximum total size of buffers kept in memory
  int fAutoSave;        //  Autosave tree when fAutoSave bytes produced
  int fReadEntry;       //! Number of the entry being processed
  int fTotalBuffers;    //! Total number of bytes in branch buffers
};

}

#endif

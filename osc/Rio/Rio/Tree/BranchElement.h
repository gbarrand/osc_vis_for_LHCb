#ifndef Rio_BranchElement_h
#define Rio_BranchElement_h

#include <Rio/Tree/Branch.h>

namespace Rio {class IElementClass;}

namespace Rio {

class BranchElement : public Branch {
  RIO_META_HEADER(BranchElement)
public: //Rio::IObject
  virtual void* cast(const IClass&) const;
  virtual bool stream(IBuffer&);
public:
  BranchElement(const IDictionary&,Tree&);
  BranchElement(const IDictionary&,Tree&,const std::string&,const std::string&,
                const IClass&,void*,
                int = 32000,int = -1);
  virtual ~BranchElement();
  std::string className() const;
public: //Branch
  virtual void setAutoDelete(bool = true);
  virtual bool fill(int&);
  virtual bool findEntry(int,int&);
  virtual void reset();
  virtual bool setAddress(void*);
  virtual void setBasketSize(int);
  virtual bool fillLeaves(IBuffer&);
  virtual bool readLeaves(IBuffer&);
private:
  std::string fClassName; //Class name of referenced object
  int fClassVersion;  //Version number of class
  int fID;            //element serial number in fInfo
  int fType;          //branch type
  int fStreamerType;  //branch streamer type
};

}

#endif

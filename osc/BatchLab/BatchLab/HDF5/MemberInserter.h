#ifndef BatchLab_HDF5_MemberInserter_h
#define BatchLab_HDF5_MemberInserter_h

// Inheritance :
#include <Slash/Store/IConstVisitor.h>

#include <exlib/hdf5/hdf5_h>

#include <list>

namespace BatchLab {

namespace HDF5 {

class Item {
public:
  Item()
  :fParent(0)
  ,fSize(0)
  ,fCompound(-1)
  ,fOffset(0)
  ,fName(""){}

  Item(const Item& aFrom) 
  :fParent(aFrom.fParent)
  ,fSize(aFrom.fSize)
  ,fCompound(aFrom.fCompound)
  ,fOffset(aFrom.fOffset)
  ,fName(aFrom.fName){}

  Item& operator=(const Item& aFrom) {
    fParent = aFrom.fParent;
    fSize = aFrom.fSize;
    fCompound = aFrom.fCompound;
    fOffset = aFrom.fOffset;
    fName = aFrom.fName;
    return *this;
  }
public:
  Item* fParent;
  unsigned int fSize;
  hid_t fCompound;
  unsigned int fOffset;
  std::string fName;
};

class MemberInserter : public virtual Slash::Store::IConstVisitor {
public: //Slash::Store::IConstVisitor
  virtual bool begin(const Slash::Store::IStorable&,
                     const std::string&,
                     Slash::Store::IConstVisitor::Local);
  virtual bool end(const Slash::Store::IStorable&);

  virtual bool visit(const std::string&,bool);
  virtual bool visit(const std::string&,char);
  virtual bool visit(const std::string&,short);
  virtual bool visit(const std::string&,int);
  virtual bool visit(const std::string&,unsigned int);
  virtual bool visit(const std::string&,Slash::int64);
  virtual bool visit(const std::string&,Slash::uint64);
  virtual bool visit(const std::string&,float);
  virtual bool visit(const std::string&,double);
  virtual bool visit(const std::string&,const std::string&);
  virtual bool visit(const std::string&,const char*);
  virtual bool visit(const std::string&,const std::vector<bool>&);
  virtual bool visit(const std::string&,const std::vector<char>&);
  virtual bool visit(const std::string&,const std::vector<short>&);
  virtual bool visit(const std::string&,const std::vector<int>&);
  virtual bool visit(const std::string&,const std::vector<Slash::int64>&);
  virtual bool visit(const std::string&,const std::vector<float>&);
  virtual bool visit(const std::string&,const std::vector<double>&);
  virtual bool visit(const std::string&,const std::vector<unsigned char>&);
  virtual bool visit(const std::string&,const std::vector<std::string>&);
  virtual bool visit(const std::string&,const std::vector< std::vector<double> >&);
  virtual bool visit_double(const std::string&,const Slash::Store::IArray&);
  virtual bool visit(const std::string&,const Slash::Store::IStorable&);
public:
  MemberInserter();
  virtual ~MemberInserter();  
  hid_t compound() const;
  unsigned int offset() const;
private:
  void clear();
private:
  Item fCurrent;
  std::list<Item> fStack; 
  Item fReturned;
};

}}

#endif

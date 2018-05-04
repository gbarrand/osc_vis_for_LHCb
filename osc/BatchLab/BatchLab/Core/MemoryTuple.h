#ifndef BatchLab_MemoryTuple_h
#define BatchLab_MemoryTuple_h

// Inheritance :
#include <BatchLab/Core/BaseTuple.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Meta/IObject.h>

#include <inlib/scast>

namespace BatchLab {

class MemoryTuple 
:public BaseTuple 
,public virtual Slash::Store::IStorable
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::MemoryTuple");return s_v;
  }
public: //AIDA/ITuple
  //fill :
  virtual bool fill(int,double);
  virtual bool fill(int,float);
  virtual bool fill(int,int);
  virtual bool fill(int,short);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual bool fill(int,long);
#else
  virtual bool fill(int,AIDA::int64);
  virtual bool fill(int,unsigned char);
#endif
  virtual bool fill(int,char);
  virtual bool fill(int,bool);
  virtual bool fill(int,const std::string&);
  virtual bool fill(int,const AIDA::ITupleEntry&);
  virtual bool fill(const std::vector<double>&);
  virtual bool fill(const std::vector<float>&);
  //get :
  virtual double getDouble(int) const;
  virtual float getFloat(int) const;
  virtual int getInt(int) const;
  virtual short getShort(int) const;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual long getLong(int) const;
#else
  virtual AIDA::int64 getLong(int) const;
#endif
  virtual char getChar(int) const;
  virtual unsigned char getByte(int) const; //AIDA-v3r3p0
  virtual bool getBoolean(int) const;
  virtual std::string getString(int) const;
  virtual const AIDA::ITupleEntry* getObject(int) const;
  //else :
  virtual int rows() const;
  virtual bool addRow();
  virtual void* cast(const std::string&) const;
  virtual double columnMin(int column) const;
  virtual double columnMax(int column) const;
  virtual double columnMean(int column) const;
  virtual double columnRms(int column) const;
  virtual const AIDA::ITuple* getTuple(int) const; //deprecated in AIDA-v3r3p0
  virtual AIDA::ITuple* getTuple(int); //deprecated in AIDA-v3r3p0
  virtual const AIDA::ITuple* findTuple(int) const; //AIDA-v3r3p0
  virtual AIDA::ITuple* findTuple(int); //AIDA-v3r3p0
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public:
  MemoryTuple(std::ostream&,const std::string& = "");
  MemoryTuple(std::ostream&,
              const std::string&,
              const std::string&,
              const std::string&);
  MemoryTuple(std::ostream&,
              const std::string&,
              const std::string&,
              const std::list<Lib::Value>&);
  MemoryTuple(const MemoryTuple&);
  virtual ~MemoryTuple();
private:
  bool getBooking(std::string&) const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const MemoryTuple* local = INLIB_CONST_CAST(aObject,BatchLab::MemoryTuple);
    if(!local) return false;
    return local->BatchLab::MemoryTuple::visit(aVisitor);
  }
private:
  template <class T,class TC>
  inline T get_value(int aColumn,const std::string& a_type) const {
    BaseColumn* column = findColumn1(aColumn);
    if(!column) {
      out() << "BatchLab::MemoryTuple::get_value(" << a_type << ") :"
            << " column " << aColumn << " not found."
            << std::endl;
      return 0;
    }
    if(column->type()!=a_type) {
      out() << "BatchLab::MemoryTuple::get_value(" << a_type << ") :"
          << " type mismatch (" << inlib::sout(column->type()) << ") "
          << " for column " << aColumn
          << " " << inlib::sout(column->name()) << "."
          << std::endl;
      return 0;
    }
    const TC* col = inlib::cast<BaseColumn,TC>(*column);
    if(!col) {
      out() << "BatchLab::MemoryTuple::get_value(" << a_type << ") :"
            << " BaseColumn cast failed."
            << std::endl;
      return 0;
    }
    T value;
    if(!col->value(fRow,value)) {
      out() << "BatchLab::MemoryTuple::get_value(" << a_type << ") :"
            << " can't get value."
            << std::endl;
      return 0;
    }
    return value;
  }

  template <class T,class TC>
  inline bool fill_value(int aColumn,const std::string& a_type,
                         T aValue) {
    BaseColumn* column = findColumn1(aColumn);
    if(!column) {
      out() << "BatchLab::MemoryTuple::fill_value(" << a_type << ") :"
            << " column " << aColumn << " not found."
            << std::endl;
      return false;
    }
    if(column->type()!=a_type) {
      out() << "BatchLab::MemoryTuple::fill_value(" << a_type << ") :"
          << " type mismatch (" << inlib::sout(column->type()) << ") "
          << " for column " << aColumn
          << " " << inlib::sout(column->name()) << "."
          << std::endl;
      return false;
    }
    TC* col = inlib::cast<BaseColumn,TC>(*column);
    if(!col) {
      out() << "BatchLab::MemoryTuple::fill_value(" << a_type << ") :"
            << " BaseColumn cast failed."
            << std::endl;
      return false;
    }
    return col->fill(aValue);
  }

private:
  template <class T,class TC>
  inline bool COLMIN(const BaseColumn& a_col,double& a_val) const {
    int rown = rows();
    if(rown<=0) {a_val = 0;return false;}
    const TC* col = inlib::cast<BaseColumn,TC>(a_col);
    if(!col) {a_val = 0;return false;}
    T value;
    if(!col->value(0,value)) {a_val = 0;return false;}
    T v = value;
    for(int row=1;row<rown;row++) {
      if(!col->value(row,value)) {a_val = 0;return false;}
      v = inlib::mn<T>(v,value);
    }
    a_val = (double)v;
    return true;
  }

  template <class T,class TC>
  inline bool COLMAX(const BaseColumn& a_col,double& a_val) const {
    int rown = rows();
    if(rown<=0) {a_val = 0;return false;}
    const TC* col = inlib::cast<BaseColumn,TC>(a_col);
    if(!col) {a_val = 0;return false;}
    T value;
    if(!col->value(0,value)) {a_val = 0;return false;}
    T v = value;
    for(int row=1;row<rown;row++) {
      if(!col->value(row,value)) {a_val = 0;return false;}
      v = inlib::mx<T>(v,value);
    }
    a_val = (double)v;
    return true;
  }

  template <class T,class TC>
  inline bool COLMEAN(const BaseColumn& a_col,double& a_val) const {
    int rown = rows();
    if(rown<=0) {a_val = 0;return false;}
    const TC* col = inlib::cast<BaseColumn,TC>(a_col);
    if(!col) {a_val = 0;return false;}
    double sv = 0;
    T value = 0;
    for(int row=0;row<rown;row++) {
      if(!col->value(row,value)) {a_val = 0;return false;}
      sv += (double)value;
    }
    a_val = sv/((double)rown);
    return true;
  }

  template <class T,class TC>
  inline bool COLRMS(const BaseColumn& a_col,double& a_val) const {
    int rown = rows();
    if(rown<=0) {a_val = 0;return false;}
    const TC* col = inlib::cast<BaseColumn,TC>(a_col);
    if(!col) {a_val = 0;return false;}
    double sv = 0;
    double sv2 = 0;
    T value = 0;
    for(int row=0;row<rown;row++) {
      if(!col->value(row,value)) {a_val = 0;return false;}
      sv += (double)value;
      sv2 += (double)value * (double)value;
    }
    double mean = sv/((double)rown);
    double rms = ::sqrt(::fabs(sv2/((double)rown) - mean * mean));
    a_val = rms;
    return true;
  }
protected:
  bool initialize(const std::list<Lib::Value>&);
};

}

#include "MemoryTuple.icc"

#endif


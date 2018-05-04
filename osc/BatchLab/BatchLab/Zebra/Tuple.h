#ifndef BatchLab_Zebra_Tuple_h
#define BatchLab_Zebra_Tuple_h

// Inheritance :
#include <BatchLab/Core/BaseTuple.h>

namespace BatchLab {

namespace Zebra {

class Tuple : public BaseTuple {
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
  virtual bool getBoolean(int) const;
  virtual std::string getString(int) const;
  virtual const AIDA::ITupleEntry* getObject(int) const;
  //else :
  virtual void* cast(const std::string&) const;
  virtual bool setTitle(const std::string&);
  virtual bool addRow();
  virtual int rows() const;
  virtual const AIDA::ITuple* getTuple(int) const;
  virtual AIDA::ITuple* getTuple(int);
  virtual const AIDA::ITuple* findTuple(int) const; //AIDA-v3r3p0
  virtual AIDA::ITuple* findTuple(int); //AIDA-v3r3p0
  virtual double columnMin(int column) const;
  virtual double columnMax(int column) const;
  virtual double columnMean(int column) const;
  virtual double columnRms(int column) const;
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(const Slash::Store::IConstVisitor&) const;
public: //BatchLab::BaseTuple
  virtual unsigned char getByte(int) const;
  virtual bool fill33(int,unsigned char);
public:
  Tuple(std::ostream&,const std::string&,int,
        const std::string&,const std::string&,int);
  virtual ~Tuple();
  bool isValid() const;
private:
  bool addColumn(const std::string&,const std::string&);
  void cd_beg() const;
  void cd_end() const;
private:
  int fVerboseLevel;
  std::string fPath;
  int fID;
  char* fBuffer;
  char* fPos;
  bool fIsValid;
  std::string f_tmp;
};

} //Zebra

} //BatchLab

#endif



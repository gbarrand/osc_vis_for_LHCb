#ifndef BatchLab_Rio_Tuple_h
#define BatchLab_Rio_Tuple_h

// Inheritance :
#include <BatchLab/Core/BaseTuple.h>

namespace Rio {class IDirectory;}
namespace Rio {class IObject;}
namespace Rio {class Tree;}

namespace BatchLab {

namespace Rio {

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
  virtual unsigned char getByte(int) const; //AIDA-v3r3p0
  virtual bool getBoolean(int) const;
  virtual std::string getString(int) const;
  virtual const AIDA::ITupleEntry* getObject(int) const;
  //else :
  virtual void* cast(const std::string&) const;
  virtual bool setTitle(const std::string&);
  virtual bool addRow();
  virtual int rows() const;
  virtual double columnMin(int column) const;
  virtual double columnMax(int column) const;
  virtual double columnMean(int column) const;
  virtual double columnRms(int column) const;
  virtual void reset();
  virtual const AIDA::ITuple* getTuple(int) const; //deprecated in AIDA-v3r3p0
  virtual AIDA::ITuple* getTuple(int); //deprecated in AIDA-v3r3p0
  virtual const AIDA::ITuple* findTuple(int) const; //AIDA-v3r3p0
  virtual AIDA::ITuple* findTuple(int); //AIDA-v3r3p0
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //BaseTuple
  virtual bool close();
public:
  Tuple(std::ostream&,::Rio::IDirectory&,
           const std::string&,const std::string&,
           const std::string&,
           int);
  Tuple(std::ostream&,::Rio::IDirectory&,
           const std::string&,const std::string&,
           const std::list<Lib::Value>&,
           int);
  Tuple(std::ostream&,const std::string&,::Rio::Tree*,
           const std::list<Lib::Value>&,int);
  Tuple(std::ostream&,const std::string&,::Rio::Tree*,int);
  virtual ~Tuple();
  bool isValid() const;
  void unmanage();
private:
  void create(::Rio::IDirectory&,
              const std::string&,
              const std::list<Lib::Value>&);
  void createColumns();
  bool createColumns(const std::list<Lib::Value>&);
  void addColumn(const std::string&,const std::string&,
                 const std::list<Lib::Value>&);
private:
  ::Rio::Tree* fRioTree;
  int fVerboseLevel;
  std::vector< ::Rio::IObject** > fObjectHolders;
  std::vector< std::vector<double>* > fVectors;
};

} //Rio

} //BatchLab

#endif



#ifndef BatchLab_ChainedTuple_h
#define BatchLab_ChainedTuple_h

// Inheritance :
#include <BatchLab/Core/BaseTuple.h>

namespace BatchLab {

class ChainedTuple 
:public BaseTuple
{
public: //AIDA/ITuple
  //fill : //FIXME
  virtual bool fill(int /*aColumn*/,float /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,double /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,bool /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,char /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,short /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,int /*aValue*/){return false;}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual bool fill(int /*aColumn*/,long /*aValue*/){return false;}
#else
  virtual bool fill(int /*aColumn*/,AIDA::int64 /*aValue*/){return false;}
  virtual bool fill(int /*aColumn*/,unsigned char /*aValue*/){return false;}
#endif
  virtual bool fill(int /*aColumn*/,const std::string& /*aValue*/){
    return false;
  }
  virtual bool fill(int /*aColumn*/,const AIDA::ITupleEntry& /*aValue*/){
    return false;
  }
  virtual bool fill(const std::vector<double>& /*aRow*/){return false;}
  virtual bool fill(const std::vector<float>& /*aRow*/){return false;}

  //get :
  virtual double getDouble(int aColumn) const {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getDouble(aColumn);
  }
  virtual float getFloat(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getFloat(aColumn);
  }
  virtual int getInt(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getInt(aColumn);
  }
  virtual short getShort(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getShort(aColumn);
  }

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual long getLong(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getLong(aColumn);
  }
#else
  virtual AIDA::int64 getLong(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getLong(aColumn);
  }
#endif
  virtual char getChar(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getChar(aColumn);
  }

  virtual unsigned char getByte(int aColumn) const {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    BaseTuple* baseTuple = INLIB_CAST(*tuple,BatchLab::BaseTuple);
    if(!baseTuple) {
      out() << "BatchLab::ChainedTuple::getByte : "
            << " tuple not a BatchLab::BaseTuple."
            << std::endl;
      return 0;
    }
    tuple->setRow(row);
    return baseTuple->getByte(aColumn);
  }

  virtual bool getBoolean(int aColumn) const  {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getBoolean(aColumn);
  }
  virtual std::string getString(int aColumn) const {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    tuple->setRow(row);
    return tuple->getString(aColumn);
  }

  virtual const AIDA::ITupleEntry* getObject(int aColumn) const {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
    return tuple->getObject(aColumn);
  }

  //else :
  virtual int rows() const {
    int rown = 0;
    unsigned int tuplen = fTuples.size();
    for(unsigned int tuplei=0;tuplei<tuplen;tuplei++) 
      rown += fTuples[tuplei]->rows();
    return rown;
  }

  virtual bool addRow(){
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return false;
    AIDA::ITuple* tuple = fTuples[tuplen-1];
    tuple->addRow();
    return true;
  }

  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::ChainedTuple)
    else return BaseTuple::cast(a_class);
  }

  virtual const AIDA::ITuple* getTuple(int aColumn) const {
    return findTuple(aColumn);
  }
  virtual AIDA::ITuple* getTuple(int aColumn) {return findTuple(aColumn);}

  virtual const AIDA::ITuple* findTuple(int aColumn) const { //AIDA-v3r3p0
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
    return tuple->getTuple(aColumn);
#else
    return tuple->findTuple(aColumn);
#endif
  }
  virtual AIDA::ITuple* findTuple(int aColumn) { //AIDA-v3r3p0
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return 0;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
    return tuple->getTuple(aColumn);
#else
    return tuple->findTuple(aColumn);
#endif
  }

  //Override BaseTuple :
  virtual int findColumn(const std::string& aName) const {
    static const int NotFound = -1;
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return NotFound;
    AIDA::ITuple* ref = fTuples[0];
    return ref->findColumn(aName);
  }

  virtual int columns() const {
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return 0;
    AIDA::ITuple* ref = fTuples[0];
    return ref->columns();
  }
  virtual std::string columnName(int aIndex) const {
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return "";
    AIDA::ITuple* ref = fTuples[0];
    return ref->columnName(aIndex);
  }
  virtual std::vector<std::string> columnNames() const {
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return std::vector<std::string>();
    AIDA::ITuple* ref = fTuples[0];
    return ref->columnNames();
  }
  virtual std::string columnType(int aIndex) const {
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return "";
    AIDA::ITuple* ref = fTuples[0];
    return ref->columnType(aIndex);
  }
  virtual std::vector<std::string> columnTypes() const {
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return std::vector<std::string>();
    AIDA::ITuple* ref = fTuples[0];
    return ref->columnTypes();
  }

  virtual void resetRow() {
    AIDA::ITuple* tuple = 0;
    int row = 0;
    if(!findRow(fRow,tuple,row)) return;
    return tuple->resetRow();
  }
  virtual void reset() { 
    unsigned int tuplen = fTuples.size();
    for(unsigned int tuplei=0;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      tuple->reset();
    }
  }

  virtual double columnMin(int aColumn) const { 
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return 0;
    AIDA::ITuple* tuple = fTuples[0];
    double vmin = tuple->columnMin(aColumn);
    for(unsigned int tuplei=1;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      if(tuple->rows()) {
        double v = tuple->columnMin(aColumn);
        vmin = inlib::mn<double>(vmin,v);
      }
    }
    return vmin;
  }

  virtual double columnMax(int aColumn) const { 
    unsigned int tuplen = fTuples.size();
    if(!tuplen) return 0;
    AIDA::ITuple* tuple = fTuples[0];
    double vmax = tuple->columnMax(aColumn);
    for(unsigned int tuplei=1;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      if(tuple->rows()) {
        double v = tuple->columnMax(aColumn);
        vmax = inlib::mx<double>(vmax,v);
      }
    }
    return vmax;
  }

  virtual double columnMean(int aColumn) const { 
    int rown = 0;
    double sv = 0;
    unsigned int tuplen = fTuples.size();
    for(unsigned int tuplei=0;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      int trown = tuple->rows();
      if(trown) {
        double tmean = tuple->columnMean(aColumn);
        double tsv = tmean * trown;
        rown += trown;
        sv += tsv;
      }
    }
    if(!rown) return 0;
    return sv/rown;
  }

  virtual double columnRms(int aColumn) const{ 
    double sv = 0;
    double sv2 = 0;
    int rown = 0;
    unsigned int tuplen = fTuples.size();
    for(unsigned int tuplei=0;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      int trown = tuple->rows();
      if(trown) {
        double tmean = tuple->columnMean(aColumn);
        double trms = tuple->columnRms(aColumn);
        // Find back tsv and tsv2 :
        double tsv = tmean * trown;
        double tsv2 = (trms * trms + tmean * tmean) * trown;
        rown += trown;
        sv += tsv;
        sv2 += tsv2;
      }
    }
    if(!rown) return 0;
    double mean = sv/((double)rown);
    double rms = ::sqrt(::fabs(sv2/((double)rown) - mean * mean));
    return rms;
  }
  
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const {
    return new BatchLab::ChainedTuple(*this);
  }
  virtual std::string storeClassName() const {return "BatchLab::ChainedTuple";}
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const {return false;}
public:
  ChainedTuple(std::ostream& a_out,
               const std::string& aName,const std::string& aTitle,
               const std::vector<AIDA::ITuple*>& aTuples)
  :BaseTuple(a_out,aName,aTitle)
  ,fTuples(aTuples){
    //Check that tuples are compatibles :
    unsigned int tuplen = aTuples.size();
    if(!tuplen) return;
    AIDA::ITuple* ref = aTuples[0];
    for(unsigned int tuplei=1;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = aTuples[tuplei];
      if(!amanip::compatible(out(),*ref,*tuple)) {
        out() << "BatchLab::ChainedTuple::ChainedTuple :"
            << " booking mismatch for tuple " << tuplei << "." 
            << std::endl;
        fTuples.clear();
        return;
      }
    }
  }
  virtual ~ChainedTuple(){}
public:
  ChainedTuple(const ChainedTuple& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,AIDA::ITuple(aFrom)
  ,AIDA::ITupleEntry(aFrom)
  ,BaseTuple(aFrom)
  ,fTuples(aFrom.fTuples)
  {}

private:
  bool findRow(int aRow,AIDA::ITuple*& aTuple,int& aTupleRow) const{
    unsigned int tuplen = fTuples.size();
    if(!tuplen) {
      aTuple = 0;
      aTupleRow = 0;
      return false;
    }
    int rown = 0;
    for(unsigned int tuplei=0;tuplei<tuplen;tuplei++) {
      AIDA::ITuple* tuple = fTuples[tuplei];
      int nrown = rown+tuple->rows();
      if((aRow>=rown)&&(aRow<nrown)) {
        aTuple = tuple;
        aTupleRow = aRow-rown;
        return true;
      } 
      rown = nrown;
    }
    return false;
  }
private:
  std::vector<AIDA::ITuple*> fTuples;
};

}

#endif



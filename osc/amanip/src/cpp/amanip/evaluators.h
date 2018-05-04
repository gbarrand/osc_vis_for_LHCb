#ifndef amanip_evaluators_h
#define amanip_evaluators_h

// Inheritance :
#include <AIDA/IEvaluator.h>

#include <AIDA/ITuple.h>
#include <AIDA/IConstants.h>

#include <ostream>

namespace amanip {

class evaluator_1D : public virtual AIDA::IEvaluator {
public: //AIDA/IEvaluator
  virtual bool initialize(AIDA::ITuple& aTuple){ 
    if(&aTuple!=&fTuple) return false;
    if(fTuple.columns()<=0) {
      f_out << "amanip::evaluator_1D::initialize :"
            << " tuple has no columns." << std::endl;
      fType = NONE;
      return false;
    }
    if( (fIndex<0) || (fIndex>=fTuple.columns()) ) {
      f_out << "amanip::evaluator_1D::initialize :"
            << " column index out of range ; given " << fIndex
            << " range [0," << fTuple.columns() << "]." 
            << std::endl;
      fType = NONE;
      return false;
    }
    std::string type = fTuple.columnType(fIndex);
    if(type=="float") {
      fType = FLOAT;
    } else if(type=="double") {
      fType = DOUBLE;
    } else if(type=="char") {
      fType = CHAR;
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    } else if(type=="byte") {
      fType = BYTE;
#endif
    } else if(type=="short") {
      fType = SHORT;
    } else if(type=="int") {
      fType = INT;
    } else if(type=="long") {
      fType = LONG;
    } else if(type=="boolean") {
      fType = BOOL;
    } else {
      f_out << "amanip::evaluator_1D::initialize :"
            << " unknown column type \"" << type << "\"." 
            << std::endl;
      fType = NONE;
      return false;
    } 
    return true;
  }
  virtual double evaluateDouble() const {
    if(fType==FLOAT) {
      return fTuple.getFloat(fIndex);
    } else if(fType==DOUBLE) {
      return fTuple.getDouble(fIndex);
    } else if(fType==CHAR) {
      return fTuple.getChar(fIndex);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    } else if(fType==BYTE) {
      return fTuple.getByte(fIndex);
#endif
    } else if(fType==SHORT) {
      return fTuple.getShort(fIndex);
    } else if(fType==INT) {
      return fTuple.getInt(fIndex);
    } else if(fType==LONG) {
      return (double)fTuple.getLong(fIndex);
    } else if(fType==BOOL) {
      return fTuple.getBoolean(fIndex);
    }
    return 0;
  }
  virtual std::string expression() const { return "";}
private:
  enum TYPE {
    NONE = 0,
    FLOAT = 1,
    DOUBLE = 2,
    CHAR = 3,
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    BYTE = 4,
#endif
    SHORT = 5,
    INT = 6,    
    LONG = 7,
    BOOL = 8
  };
public:
  evaluator_1D(std::ostream& a_out,const AIDA::ITuple& aTuple,int aIndex)
  :f_out(a_out),fTuple(aTuple),fIndex(aIndex),fType(NONE){}
  virtual ~evaluator_1D(){}
private:
  std::ostream& f_out;
  const AIDA::ITuple& fTuple;
  int fIndex;
  TYPE fType;
};

}

#endif

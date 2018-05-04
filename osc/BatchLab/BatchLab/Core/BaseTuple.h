#ifndef BatchLab_BaseTuple_h
#define BatchLab_BaseTuple_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <AIDA/ITuple.h>
#include <AIDA/ITupleEntry.h> //FIXME
#include <AIDA/IConstants.h> //For AIDA_VERSION
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
#include <AIDA/ITupleColumn.h>
#endif

#include <Lib/Value.h>
#include <Lib/Columns.h>   //for booking()
//#include <Lib/Columns_no_yacc.h>   //for booking()

#include <BatchLab/Core/Annotation.h>

#include <inlib/mnmx>
#include <inlib/touplow>
#include <inlib/srep>
#include <inlib/strip>
#include <inlib/words>

#include <amanip/filters.h>
#include <amanip/project.h>
#include <amanip/tuple.h>

#include <list>
#include <cmath>

//for projections:
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IHistogram3D.h>

#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>

#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>

namespace BatchLab {

class BaseColumn 
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
:public virtual AIDA::IBaseTupleColumn
#endif
{
public: //AIDA::IBaseTupleColumn
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::BaseColumn)
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
    else INLIB_IF_CAST(AIDA::IBaseTupleColumn)
#endif
    else return 0;
  }

  virtual std::string name() const {return fLabel;}
  virtual std::string type() const {return fType;}

  virtual double minimum() const {
    unsigned int number = size();
    if(!number) return 0; //FIXME : throw.
    Lib::Value val;
    double d;
    if(!value(0,val)) {} //FIXME :throw
    if(!val.to_double(d)) {} //FIXME : throw
    double mn = d;
    for(unsigned int index=1;index<number;index++) {
      if(!value(index,val)) {} //FIXME :throw
      if(!val.to_double(d)) {} //FIXME : throw
      mn = inlib::mn<double>(d,mn);
    }
    return mn;
  }

  virtual double maximum() const {
    unsigned int number = size();
    if(!number) return 0; //FIXME : throw.
    Lib::Value val;
    double d;
    if(!value(0,val)) {} //FIXME :throw
    if(!val.to_double(d)) {} //FIXME : throw
    double mx = d;
    for(unsigned int index=1;index<number;index++) {
      if(!value(index,val)) {} //FIXME :throw
      if(!val.to_double(d)) {} //FIXME : throw
      mx = inlib::mx<double>(d,mx);
    }
    return mx;
  }

  virtual double mean() const {
    unsigned int number = size();
    if(!number) return 0; //FIXME : throw.
    Lib::Value val;
    double d;
    if(!value(0,val)) {} //FIXME :throw
    if(!val.to_double(d)) {} //FIXME : throw
    double sum = d;
    for(unsigned int index=1;index<number;index++) {
      if(!value(index,val)) {} //FIXME :throw
      if(!val.to_double(d)) {} //FIXME : throw
      sum += d;
    }
    return sum/double(number);
  }

  virtual double rms() const {
    unsigned int number = size();
    if(!number) return 0; //FIXME : throw.
    Lib::Value val;
    double d;
    if(!value(0,val)) {} //FIXME :throw
    if(!val.to_double(d)) {} //FIXME : throw
    double sum = d;
    double sum2 = d*d;
    for(unsigned int index=1;index<number;index++) {
      if(!value(index,val)) {} //FIXME :throw
      if(!val.to_double(d)) {} //FIXME : throw
      sum += d;
      sum2 += d*d;
    }
    double mean = sum/double(number);
    return ::sqrt(::fabs(sum2/double(number) - mean * mean));
  }
  
public:
  virtual unsigned int size() const = 0;
  virtual void reset() = 0;
  virtual void add() = 0;
  virtual BaseColumn* copy() const = 0;
  virtual void resetFilled() = 0;
  virtual std::string stringDefaultValue() const = 0;
  virtual bool isDefaultDefault() const = 0;
  virtual bool append(void*) = 0;
  virtual bool value(int aIndex,Lib::Value& aValue) const = 0;
public:
  virtual bool read(Slash::Store::IVisitor&) = 0;
protected:
  BaseColumn(std::ostream& a_out,
             const std::string& aLabel,const std::string& aType)
  :f_out(a_out),fLabel(aLabel),fType(aType),fRow(-1){
    Lib::Debug::increment("BatchLab::BaseColumn");
  }
public:
  virtual ~BaseColumn(){
    Lib::Debug::decrement("BatchLab::BaseColumn");
  }
public:
  BaseColumn(const BaseColumn& aFrom)
  :f_out(aFrom.f_out),fLabel(aFrom.fLabel)
  ,fType(aFrom.fType),fRow(-1){
    Lib::Debug::increment("BatchLab::BaseColumn");
  }
public:
  void setRow(int aRow){fRow = aRow;}
protected:
  std::ostream& f_out;
  std::string fLabel;
  std::string fType;
  int fRow;
};

class BaseTuple 
:public BaseObject
,public virtual AIDA::ITuple
,public virtual AIDA::ITupleEntry  //FIXME
{
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(AIDA::ITuple)
    else INLIB_IF_CAST(AIDA::ITupleEntry)
    else INLIB_IF_CAST(BatchLab::BaseTuple)
    else return BaseObject::cast(a_class);
  }
  virtual std::string type() const {return "AIDA::ITuple";}
public: //AIDA/ITuple
  virtual std::string title() const {return fTitle;}
  virtual bool setTitle(const std::string& aTitle) {
    fTitle = aTitle;
    fAnnotation.setValue("Title",aTitle);
    return true;
  }

  virtual AIDA::IAnnotation& annotation() {return fAnnotation;}
  virtual const AIDA::IAnnotation& annotation() const {return fAnnotation;}

  virtual int findColumn(const std::string& aName) const {
    // Not case sensitive (for OpenPAW).
    unsigned int number = fColumns.size();
    // case sensitive :
    //for(unsigned int index=0;index<number;index++) {
    //  if(fColumns[index]->label()==aName) return index;
    //}
    // Not case sensitive :
    std::string as = aName;
    inlib::touppercase(as);
    for(unsigned int index=0;index<number;index++) {
      std::string s = fColumns[index]->name();
      inlib::touppercase(s);
      if(s==as) return index;
    }
    return NotFound;
  }

  virtual void resetRow() {
    unsigned int number = fColumns.size();
    for(unsigned int index=0;index<number;index++) 
      fColumns[index]->resetFilled();
  }

  virtual void reset() { 
    unsigned int number = fColumns.size();
    for(unsigned int index=0;index<number;index++) fColumns[index]->reset();
  }

  virtual void start() {fRow = -1;setColumnsRow(fRow);}

  virtual bool next() { 
    if((fRow+1)>=rows()) return false;
    fRow++;
    setColumnsRow(fRow);
    return true;
  }

  virtual bool skip(int aRows) { 
    if((fRow+aRows)>=rows()) return false;
    fRow += aRows;
    setColumnsRow(fRow);
    return true;
  }

  virtual bool setRow(int aRow) { 
    if(aRow<0) return false;
    if(aRow>=rows()) return false;
    fRow = aRow;
    setColumnsRow(fRow);
    return true;
  }

  virtual int columns() const {return fColumns.size();}

  virtual std::string columnName(int aIndex) const {
    BaseColumn* column = findColumn1(aIndex);
    if(!column) return "";
    return column->name();
  }

  virtual std::vector<std::string> columnNames() const {
    unsigned int number = fColumns.size();
    std::vector<std::string> ss(number);
    for(unsigned int index=0;index<number;index++) {
      ss[index] = fColumns[index]->name();
    }
    return ss;
  }

  virtual std::string columnType(int aIndex) const {
    BaseColumn* column = findColumn1(aIndex);
    if(!column) return "";
    return column->type();
  }

  virtual std::vector<std::string> columnTypes() const {
    unsigned int number = fColumns.size();
    std::vector<std::string> ss(number);
    for(unsigned int index=0;index<number;index++) {
      ss[index] = fColumns[index]->type();
    }
    return ss;
  }

  /////////////////////////////////////////////////////////////////////////
  // project in histogram 1D //////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::IHistogram1D& aFillable,AIDA::IEvaluator& aX) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram1D>
             (out(),*this,aFillable,aX,filter);
  }

  virtual bool project(AIDA::IHistogram1D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IFilter& aFilter){
    return amanip::project<AIDA::IHistogram1D>
             (out(),*this,aFillable,aX,aFilter);
  }

  virtual bool project(AIDA::IHistogram1D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IFilter& aFilter,
                       AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::IHistogram1D>
             (out(),*this,aFillable,aX,aFilter,aW);
  }

  virtual bool project(AIDA::IHistogram1D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram1D>
             (out(),*this,aFillable,aX,filter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in histogram 2D //////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::IHistogram2D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IEvaluator& aY){
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram2D>
             (out(),*this,aFillable,aX,aY,filter);
  }

  virtual bool project(AIDA::IHistogram2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IFilter& aFilter){
    return amanip::project<AIDA::IHistogram2D>
             (out(),*this,aFillable,aX,aY,aFilter);
  }

  virtual bool project(AIDA::IHistogram2D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IEvaluator& aY,
                       AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram2D>
      (out(),*this,aFillable,aX,aY,filter,aW);
  }
  virtual bool project(AIDA::IHistogram2D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IEvaluator& aY,
                       AIDA::IFilter& aFilter,
                       AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::IHistogram2D>
      (out(),*this,aFillable,aX,aY,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in histogram 3D //////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  virtual bool project(AIDA::IHistogram3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ){
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram3D>
             (out(),*this,aFillable,aX,aY,aZ,filter);
  }

  virtual bool project(AIDA::IHistogram3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IFilter& aFilter){
    return amanip::project<AIDA::IHistogram3D>
             (out(),*this,aFillable,aX,aY,aZ,aFilter);
  }

  virtual bool project(AIDA::IHistogram3D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IEvaluator& aY,
                       AIDA::IEvaluator& aZ,
                       AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IHistogram3D>
      (out(),*this,aFillable,aX,aY,aZ,filter,aW);
  }
  virtual bool project(AIDA::IHistogram3D& aFillable,
                       AIDA::IEvaluator& aX,
                       AIDA::IEvaluator& aY,
                       AIDA::IEvaluator& aZ,
                       AIDA::IFilter& aFilter,
                       AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::IHistogram3D>
      (out(),*this,aFillable,aX,aY,aZ,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in cloud 1D //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::ICloud1D& aFillable,AIDA::IEvaluator& aX) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud1D>(out(),*this,aFillable,aX,filter);
  }

  virtual bool project(AIDA::ICloud1D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IFilter& aFilter) {
    return amanip::project<AIDA::ICloud1D>(out(),*this,aFillable,aX,aFilter);
  }

  virtual bool project(AIDA::ICloud1D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud1D>(out(),*this,aFillable,aX,filter,aW);
  }

  virtual bool project(AIDA::ICloud1D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IFilter& aFilter,AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::ICloud1D>
             (out(),*this,aFillable,aX,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in cloud 2D //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::ICloud2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud2D>(out(),*this,aFillable,aX,aY,filter);
  }

  virtual bool project(AIDA::ICloud2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IFilter& aFilter) {
    return amanip::project<AIDA::ICloud2D>
             (out(),*this,aFillable,aX,aY,aFilter);
  }

  virtual bool project(AIDA::ICloud2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud2D>
             (out(),*this,aFillable,aX,aY,filter,aW);
  }
  virtual bool project(AIDA::ICloud2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,
            AIDA::IFilter& aFilter,AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::ICloud2D>
      (out(),*this,aFillable,aX,aY,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in cloud 3D //////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::ICloud3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud3D>
             (out(),*this,aFillable,aX,aY,aZ,filter);
  }

  virtual bool project(AIDA::ICloud3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IFilter& aFilter) {
    return amanip::project<AIDA::ICloud3D>
             (out(),*this,aFillable,aX,aY,aZ,aFilter);
  }

  virtual bool project(AIDA::ICloud3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::ICloud3D>
      (out(),*this,aFillable,aX,aY,aZ,filter,aW);
  }

  virtual bool project(AIDA::ICloud3D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IFilter& aFilter,AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::ICloud3D>
      (out(),*this,aFillable,aX,aY,aZ,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in profile 1D ////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::IProfile1D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IEvaluator& aY) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IProfile1D>
             (out(),*this,aFillable,aX,aY,filter);
  }

  virtual bool project(AIDA::IProfile1D& aFillable,
                       AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,
                       AIDA::IFilter& aFilter) {
    return amanip::project<AIDA::IProfile1D>
             (out(),*this,aFillable,aX,aY,aFilter);
  }

  virtual bool project(AIDA::IProfile1D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IProfile1D>
      (out(),*this,aFillable,aX,aY,filter,aW);
  }

  virtual bool project(AIDA::IProfile1D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,
            AIDA::IFilter& aFilter,AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::IProfile1D>
      (out(),*this,aFillable,aX,aY,aFilter,aW);
  }

  /////////////////////////////////////////////////////////////////////////
  // project in profile 2D ////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  virtual bool project(AIDA::IProfile2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IProfile2D>
             (out(),*this,aFillable,aX,aY,aZ,filter);
  }

  virtual bool project(AIDA::IProfile2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IFilter& aFilter) {
    return amanip::project<AIDA::IProfile2D>
             (out(),*this,aFillable,aX,aY,aZ,aFilter);
  }

  virtual bool project(AIDA::IProfile2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IEvaluator& aW) {
    ::amanip::accept_all filter;
    return amanip::project<AIDA::IProfile2D>
      (out(),*this,aFillable,aX,aY,aZ,filter,aW);
  }

  virtual bool project(AIDA::IProfile2D& aFillable,
            AIDA::IEvaluator& aX,AIDA::IEvaluator& aY,AIDA::IEvaluator& aZ,
            AIDA::IFilter& aFilter,AIDA::IEvaluator& aW) {
    return amanip::project<AIDA::IProfile2D>
      (out(),*this,aFillable,aX,aY,aZ,aFilter,aW);
  }
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////

  virtual std::string columnDefaultValue(int aIndex) const {
    BaseColumn* column = findColumn1(aIndex);
    if(!column) return "";
    return column->stringDefaultValue();
  }


  virtual double evaluateMin(AIDA::IEvaluator& aEval) {
    //AIDA-v3r3p0
    double xmin,xmax;
    ::amanip::accept_all filter;
    amanip::min_max(out(),*this,aEval,filter,xmin,xmax);
    return xmin;
  } 

  virtual double evaluateMin(AIDA::IEvaluator& aEval,AIDA::IFilter& aFilter) {
    //AIDA-v3r3p0
    double xmin,xmax;
    amanip::min_max(out(),*this,aEval,aFilter,xmin,xmax);
    return xmin;
  } 

  virtual double evaluateMax(AIDA::IEvaluator& aEval) {
    //AIDA-v3r3p0
    double xmin,xmax;
    ::amanip::accept_all filter;
    amanip::min_max(out(),*this,aEval,filter,xmin,xmax);
    return xmax;
  } 

  virtual double evaluateMax(AIDA::IEvaluator& aEval,AIDA::IFilter& aFilter) {
    //AIDA-v3r3p0
    double xmin,xmax;
    amanip::min_max(out(),*this,aEval,aFilter,xmin,xmax);
    return xmax;
  } 

#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual unsigned char getByte(int) const = 0;
#else

  virtual AIDA::IBaseTupleColumn* column(const std::string& aName) const {
    std::vector<BaseColumn*>::const_iterator it;
    for(it=fColumns.begin();it!=fColumns.end();++it) {
      if(aName==(*it)->name()) {
        return INLIB_CAST(*(*it),AIDA::IBaseTupleColumn);
      }   
    }
    return 0;
  }

  virtual AIDA::IBaseTupleColumn* column(int aIndex) const {
    BaseColumn* column = findColumn1(aIndex);
    if(!column) return 0;
    return INLIB_CAST(*column,AIDA::IBaseTupleColumn);
  }

#endif
public:
  virtual bool isDefaultDefault(int aIndex) const {
    BaseColumn* column = findColumn1(aIndex);
    if(!column) return false;
    return column->isDefaultDefault();
  }

  virtual int rowIndex() const {return fRow;}
  virtual bool close() {return true;}
public:
  //FIXME : should be protected, but used in HDF5.
  BaseColumn* findColumn1(int aIndex) const {
    int number = fColumns.size();
    if((aIndex<0)||(aIndex>=number)) return 0;
    return fColumns[aIndex];
  }
protected:
  BaseTuple(std::ostream& a_out,
            const std::string& aName,const std::string& aTitle)
  :BaseObject(a_out,aName),fRow(-1){
    setTitle(aTitle);
  }
  virtual ~BaseTuple() {clear();}
protected:
  BaseTuple(const BaseTuple& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,AIDA::ITuple(aFrom)
  ,AIDA::ITupleEntry(aFrom)
  ,BaseObject(aFrom)
  ,fAnnotation(aFrom.fAnnotation)
  ,fTitle(aFrom.fTitle)
  ,fRow(-1)
  {
    unsigned int number = aFrom.fColumns.size();
    for(unsigned int index=0;index<number;index++) {
      BaseColumn* column = aFrom.fColumns[index]->copy();
      if(!column) {
        out() << "BatchLab::BaseTuple::BaseTuple :"
            << " can't copy column."
            << std::endl;
        fColumns.clear();
        return;
      }
      fColumns.push_back(column);
    }
  }

protected:
  //annotation() is ambiguous vs the AIDA one.
  Annotation& getAnnotation(){return fAnnotation;}
  void setAnnotation(const BatchLab::Annotation& aFrom) {fAnnotation = aFrom;}
protected:
  static bool is_column_type(const std::string& aString) {
    std::string type = aString;
    inlib::tolowercase(type);
    if( (type=="float") ||
        (type=="double") ||
        (type=="char") ||
        (type=="short") ||
        (type=="int") ||
        (type=="long") ||
        (type=="boolean") ||
        (type=="string") ||
        (type=="ITuple") ) return true;
    return false;
  }

  static bool booking(std::ostream& a_out,
                      const std::string& aString,
                      std::list<Lib::Value>& aVariables) {
    aVariables.clear();
  
    if(aString.empty()) return true; //it happens when reading a from a file.
  
    int verboseLevel = 0;
    if(verboseLevel) {
      a_out << "BatchLab::BaseTuple::booking :"
            << " parse " << inlib::sout(aString) << "."
            << std::endl;
    }
  
    //printf("debug : BatchLab::BaseTuple::booking : \"%s\".\n",aString.c_str());
  
    if(aString.find(";")!=std::string::npos) {
  
      std::string snew = aString;
      inlib::replace(snew,";",",");
  
      Lib::Columns proc(snew,a_out,verboseLevel);
      if(!proc.findVariables()) {
        a_out << "BatchLab::BaseTuple::booking :"
            << " Bad syntax in " << inlib::sout(aString) << "."
            << std::endl;
        return false;
      }
      aVariables = proc.result();
  
    } else if(aString.find(",")==std::string::npos) {
      if(aString.find("\t")!=std::string::npos) {
        // Handle hippodraw format "col1\tcol2\tcol3".
        // "double" will be the type by default.
        std::vector<std::string> names;
        inlib::words(aString,"\t",false,names);
        inlib::strip(names);
        inlib::replace(names," ","_");
  
        Lib::Value v;
        v.setType(Slash::Core::IValue::DOUBLE);
        unsigned int namen = names.size();
        for(unsigned int index=0;index<namen;index++) {
          //printf("debug : %d \"%s\"\n",index,aNames[index].c_str());
          v.setLabel(names[index]);
          aVariables.push_back(v);
        }
      } else {
        std::vector<std::string> names;
        inlib::words(aString," ",false,names);
        if( ( (names.size()==2) || (names.size()==4) ) && 
            (is_column_type(names[0])) ) {
          // Could be one column at new format :
          //   <type> <name>
          //   <type> <name> = <value>
          Lib::Columns proc(aString,a_out,verboseLevel);
          if(!proc.findVariables()) {
            a_out << "BatchLab::BaseTuple::booking :"
                << " Bad syntax in " << inlib::sout(aString) << "."
                << std::endl;
            return false;
          }
          aVariables = proc.result();
        } else {
          // Old format is "col1 col2 col3".
          // "double" will be the type by default.
          Lib::Value v;
          v.setType(Slash::Core::IValue::DOUBLE);
          unsigned int namen = names.size();
          for(unsigned int index=0;index<namen;index++) {
            //printf("debug : %d \"%s\"\n",index,aNames[index].c_str());
            v.setLabel(names[index]);
            aVariables.push_back(v);
          }
        }
      }
  
    } else {
      Lib::Columns proc(aString,a_out,verboseLevel);
      if(!proc.findVariables()) {
        a_out << "BatchLab::BaseTuple::booking :"
            << " Bad syntax in " << inlib::sout(aString) << "."
            << std::endl;
        return false;
      }
      aVariables = proc.result();
    }
    if(!aVariables.size()) {
      a_out << "BatchLab::BaseTuple::booking :"
          << " Bad syntax in " << inlib::sout(aString) << "."
          << std::endl;
      return false;
    }
  
    if(verboseLevel) {
      int index = 0;
      std::list<Lib::Value>::iterator it;
      for(it=aVariables.begin();it!=aVariables.end();++it,index++){
        Lib::Value& var = (*it);
        a_out << "BatchLab::BaseTuple::booking :"
            << " variable " << index 
            <<  " (" << (unsigned int)aVariables.size() << ")"
            << " name = " << inlib::sout(var.label())
            << " type = " << inlib::sout(var.stype()) << "."
            << std::endl;
      }
    }
  
    return true;
  }
  
protected:
  void clear() {
    unsigned int number = fColumns.size();
    for(unsigned int index=0;index<number;index++) delete fColumns[index];
    fColumns.clear();
    fRow = -1;
    setColumnsRow(fRow);
  }

  std::string convertType(Slash::Core::IValue::Type aType) const {
    switch(aType) {
    case Slash::Core::IValue::SHORT: return "short";
    case Slash::Core::IValue::INT: return "int";
    case Slash::Core::IValue::INT64: return "long"; //and not int64.
    case Slash::Core::IValue::DOUBLE:  return "double";
    //AIDA3.3 : case BOOLEAN: return "bool";
    case Slash::Core::IValue::BOOL: return "boolean";
    case Slash::Core::IValue::FLOAT: return "float";
    case Slash::Core::IValue::UNSIGNED_CHAR: return "byte";
    case Slash::Core::IValue::CHAR: return "char";
    case Slash::Core::IValue::STRING: return "string";
    case Slash::Core::IValue::VOID_STAR: return "ITuple";
  
    case Slash::Core::IValue::ARRAY_DOUBLE: return "double[]";
  
    case Slash::Core::IValue::UNSIGNED_INT:
    case Slash::Core::IValue::UNSIGNED_INT64:
    default:
      out() << "BatchLab::BaseTuple::convertType :"
          << " unhandle Lib::Variable type " << ((int)aType) << "."
          << std::endl;
      return "";
    }
  }

  void setColumnsRow(int aRow) {
    std::vector<BaseColumn*>::iterator it;
    for(it=fColumns.begin();it!=fColumns.end();++it) {
      (*it)->setRow(aRow);   
    }
  }
protected:
  static const int NotFound = -1;
protected:
  Annotation fAnnotation;
  std::string fTitle;
  int fRow;
  std::vector<BaseColumn*> fColumns;
};

}

#endif

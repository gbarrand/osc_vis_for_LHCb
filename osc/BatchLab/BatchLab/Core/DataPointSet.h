#ifndef BatchLab_DataPointSet_h
#define BatchLab_DataPointSet_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <AIDA/IDataPointSet.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Store/IArray.h>
#include <Slash/Meta/IObject.h>

#include <BatchLab/Core/Annotation.h>
#include <BatchLab/Core/DataPoint.h>
#include <inlib/mnmx>

#include <Lib/Sizes.h>

namespace BatchLab {

class DataPointSet 
:public BaseObject
,public virtual AIDA::IDataPointSet
,public virtual Slash::Store::IStorable
,public virtual Slash::Store::IArray //For the IO.
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::DataPointSet");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(AIDA::IDataPointSet)
    else INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(Slash::Meta::IObject)
    else INLIB_IF_CAST(BatchLab::DataPointSet)
    else return BaseObject::cast(a_class);
  }

  virtual std::string type() const {return "AIDA::IDataPointSet";}//AIDA-v3r3p0

public: //AIDA/IDataPointSet
  virtual std::string title() const {
    lock();
    std::string s = fTitle;
    unlock();
    return s;
  }

  virtual bool setTitle(const std::string& aTitle) { 
    lock();
    fTitle = aTitle;
    fAnnotation.setValue("Title",aTitle);
    unlock();
    return true;
  }

  virtual AIDA::IAnnotation& annotation() {return fAnnotation;}
  virtual const AIDA::IAnnotation& annotation() const {return fAnnotation;}

  virtual int dimension() const {return fDimension;}

  virtual void clear() {lock();fPoints.clear();unlock();}

  virtual int size() const { 
    lock();
    int n = fPoints.size();
    unlock();
    return n;
  }

  virtual AIDA::IDataPoint* point(int aIndex) { 
    lock();
    AIDA::IDataPoint* p = 0;
    if((aIndex>=0) && (aIndex<(int)fPoints.size()) ) {
      p = &(fPoints[aIndex]);
    }
    unlock();
    return p;
  }

  virtual const AIDA::IDataPoint* point(int aIndex) const { 
    lock();
    const AIDA::IDataPoint* p = 0;
    if((aIndex>=0) && (aIndex<(int)fPoints.size()) ) {
      p = &(fPoints[aIndex]);
    }
    unlock();
    return p;
  }

  virtual AIDA::IDataPoint* addPoint() { 
    lock();
    fPoints.push_back(DataPoint(fDimension));
    const DataPoint& dataPoint = fPoints.back();
    AIDA::IDataPoint* pt = (AIDA::IDataPoint*)&dataPoint;
    unlock();
    emitUpdate();
    return pt;
  }

  virtual bool addPoint(const AIDA::IDataPoint& aPoint) { 
    lock();
    fPoints.push_back(DataPoint(aPoint));
    unlock();
    emitUpdate();
    return true;
  }

  virtual bool removePoint(int aIndex) { 
    lock();
    bool stat = false;
    if( (aIndex>=0) && (aIndex<(int)fPoints.size()) ){
      std::vector<DataPoint>::iterator it = fPoints.begin();
      for(int i=0;i<aIndex;i++) it++;
      fPoints.erase(it);
      stat = true;
    }
    unlock();
    if(stat) emitUpdate();
    return stat;
  }

  virtual double lowerExtent(int aCoordinate) const { 
    lock();
    double value = 0;//FIXME nan.
    if((aCoordinate>=0) && (aCoordinate<fDimension) && (fPoints.size()>0) ){
      std::vector<DataPoint>::const_iterator it = fPoints.begin();
      const AIDA::IMeasurement* m = (*it).coordinate(aCoordinate);
      value = m->value();
      ++it;
      for(;it!=fPoints.end();++it) {
        const AIDA::IMeasurement* m = (*it).coordinate(aCoordinate);
        value = inlib::mn<double>(value,m->value());
      }
    }
    unlock();
    return value;
  }
  
  virtual double upperExtent(int aCoordinate) const { 
    lock();
    double value = 0;//FIXME nan.
    if((aCoordinate>=0) && (aCoordinate<fDimension) && (fPoints.size()>0) ){
      std::vector<DataPoint>::const_iterator it = fPoints.begin();
      const AIDA::IMeasurement* m = (*it).coordinate(aCoordinate);
      value = m->value();
      ++it;
      for(;it!=fPoints.end();++it) {
        const AIDA::IMeasurement* m = (*it).coordinate(aCoordinate);
        value = inlib::mx<double>(value,m->value());
      }
    }
    unlock();
    return value;
  }

  virtual bool scale(double aScaleFactor) { 
    lock();
    std::vector<DataPoint>::iterator it;
    for(it=fPoints.begin();it!=fPoints.end();++it) {
      for(int index=0;index<fDimension;index++) {
        AIDA::IMeasurement* m = (*it).coordinate(index);
        m->setValue(m->value() * aScaleFactor);
        m->setErrorPlus(m->errorPlus() * aScaleFactor);
        m->setErrorMinus(m->errorMinus() * aScaleFactor);
      }
    }
    unlock();
    if(fPoints.size()) emitUpdate();
    return true;
  }

  virtual bool scaleValues(double aScaleFactor) { 
    lock();
    std::vector<DataPoint>::iterator it;
    for(it=fPoints.begin();it!=fPoints.end();++it) {
      for(int index=0;index<fDimension;index++) {
        AIDA::IMeasurement* m = (*it).coordinate(index);
        m->setValue(m->value() * aScaleFactor);
      }
    }
    unlock();
    if(fPoints.size()) emitUpdate();
    return true;
  }

  virtual bool scaleErrors(double aScaleFactor) { 
    lock();
    std::vector<DataPoint>::iterator it;
    for(it=fPoints.begin();it!=fPoints.end();++it) {
      for(int index=0;index<fDimension;index++) {
        AIDA::IMeasurement* m = (*it).coordinate(index);
        m->setErrorPlus(m->errorPlus() * aScaleFactor);
        m->setErrorMinus(m->errorMinus() * aScaleFactor);
      }
    }
    unlock();
    if(fPoints.size()) emitUpdate();
    return true;
  }
  
  virtual bool setCoordinate(int aCoordinate,
                             const std::vector<double>& aValues,
                             const std::vector<double>& aErrors){
    lock();
    bool stat = false;
    unsigned int number = aValues.size();
    if(   (aCoordinate>=0) 
       && (aCoordinate<fDimension) 
       && number 
       && (aErrors.size()==number) ){
      if(fPoints.empty()){
        for(unsigned int index=0;index<number;index++) addPoint();
      }
      if(fPoints.size()==number) {
        std::vector<DataPoint>::const_iterator it;
        unsigned int index = 0;
        for(it=fPoints.begin();it!=fPoints.end();++it) {
          AIDA::IMeasurement* m = 
            (AIDA::IMeasurement*)(*it).coordinate(aCoordinate);
          m->setValue(aValues[index]);
          m->setErrorPlus(aErrors[index]);
          m->setErrorMinus(aErrors[index]);
          index++;
        }
        stat = true;
      }
    }
    unlock();
    if(stat) emitUpdate();
    return stat;
  }

  virtual bool setCoordinate(int aCoordinate,
                             const std::vector<double>& aValues,
                             const std::vector<double>& aErrorsPlus,
                             const std::vector<double>& aErrorsMinus){
    lock();
    bool stat = false;
    unsigned int number = aValues.size();
    if(   (aCoordinate>=0) 
       && (aCoordinate<fDimension) 
       && number 
       && (aErrorsPlus.size()==number) 
       && (aErrorsMinus.size()==number) ){
      if(fPoints.empty()){
        fPoints.resize(number,DataPoint(fDimension));
      }
      if(fPoints.size()==number) {
        std::vector<DataPoint>::const_iterator it;
        unsigned int index = 0;
        for(it=fPoints.begin();it!=fPoints.end();++it) {
          AIDA::IMeasurement* m = 
            (AIDA::IMeasurement*)(*it).coordinate(aCoordinate);
          m->setValue(aValues[index]);
          m->setErrorPlus(aErrorsPlus[index]);
          m->setErrorMinus(aErrorsMinus[index]);
          index++;
        }
        stat = true;
      }
    }
    unlock();
    if(stat) emitUpdate();
    return stat;
  }

public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const{
    return new BatchLab::DataPointSet(*this);
  }
  virtual std::string storeClassName() const{return "BatchLab::DataPointSet";}
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const{
    if(!aVisitor.begin(*this,"BatchLab::DataPointSet",s_visit)) return false;
  
    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
  
    if(!aVisitor.visit("fAnnotation",fAnnotation)) return false;
    if(!aVisitor.visit("fTitle",fTitle)) return false;
    if(!aVisitor.visit("fDimension",fDimension)) return false;
    if(!aVisitor.visit_double("fPoints",*this)) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }
  
  virtual bool read(Slash::Store::IVisitor& aVisitor){
    if(!aVisitor.begin(*this)) return false;
  
    int version;
    if(!aVisitor.visit(version)) return false;
  
    if(!fAnnotation.read(aVisitor)) return false;
    if(!aVisitor.visit(fTitle)) return false;
    if(!aVisitor.visit(fDimension)) return false;
    if(!aVisitor.visit_double(*this)) return false;
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }

public: //IArray
  virtual Slash::Store::IArray::Orders orders() const {
    lock();
    std::vector<unsigned int> dims(3);
    dims[0] = fPoints.size();
    dims[1] = fDimension;
    dims[2] = 3;
    unlock();
    return dims;
  }

  virtual bool write(char*& aPos,char* aEnd) const{
    lock();
    bool stat = false;
    unsigned int number = fPoints.size();
    unsigned int dim = fDimension;
    unsigned int array_sz = number*dim*3*LIB_SIZE_DOUBLE;
    if((aPos+array_sz)>aEnd) {
    } else {
      if(array_sz) {
        double* data = (double*)aPos;
        for(unsigned int index=0;index<number;index++) {
          const DataPoint& point = fPoints[index];
          for(unsigned int icoord=0;icoord<dim;icoord++) {
            const AIDA::IMeasurement* measurement = point.coordinate(icoord);
            *data = measurement->value();data++;
            *data = measurement->errorMinus();data++;
            *data = measurement->errorPlus();data++;
          }
        }
        aPos += array_sz;
      }
      stat = true;
    }
    unlock();
    return stat;
  }
  
  virtual bool read(const Slash::Store::IArray::Orders& aOrders,
                    char*& aPos,char* aEnd){
    lock();
    bool stat = false;
    unsigned int array_sz = 1;
    unsigned int dimn = aOrders.size();
    for(unsigned int dimi=0;dimi<dimn;dimi++) array_sz *= aOrders[dimi];
    array_sz *= LIB_SIZE_DOUBLE;
    if((aPos+array_sz)>aEnd) {
      fDimension = 0;
      fPoints.clear();
    } else {
      fDimension = aOrders[1];
      unsigned int number = aOrders[0];
      fPoints.resize(number,DataPoint(fDimension));
      if(array_sz) {
        double* data = (double*)aPos;
        std::vector<DataPoint>::const_iterator it;
        for(it=fPoints.begin();it!=fPoints.end();++it) {
          for(int icoord=0;icoord<fDimension;icoord++) {
            AIDA::IMeasurement* m = 
              (AIDA::IMeasurement*)(*it).coordinate(icoord);
            m->setValue(*data);data++;
            m->setErrorMinus(*data);data++;
            m->setErrorPlus(*data);data++;
          }
        }
        aPos += array_sz;
      }
      stat = true;
    }
    unlock();
    return stat;
  }

public:
  DataPointSet(std::ostream& a_out,const std::string& aName = "")
  :BaseObject(a_out,aName),fDimension(0){}

  DataPointSet(std::ostream& a_out,
               const std::string& aName,const std::string& aTitle,
               int aDimension)
  :BaseObject(a_out,aName),fDimension(aDimension){
    setTitle(aTitle);
  }

  virtual ~DataPointSet(){}
public:
  DataPointSet(const DataPointSet& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,AIDA::IDataPointSet(aFrom)
  ,Slash::Store::IStorable(aFrom)
  ,Slash::Store::IArray(aFrom)
  ,Slash::Meta::IObject(aFrom)
  ,BaseObject(aFrom)
  ,fAnnotation(aFrom.fAnnotation)
  ,fTitle(aFrom.fTitle)
  ,fDimension(aFrom.fDimension)
  ,fPoints(aFrom.fPoints)
  {}

public:
  Annotation& getAnnotation(){return fAnnotation;}
  void setAnnotation(const Annotation& aFrom){fAnnotation = aFrom;}

  std::vector<BatchLab::DataPoint>& points(){return fPoints;} //for the IO.

  bool setPoints(unsigned int aNumber,const double* aData){ //for the IO.
    // Expected in aData :
    //   aData[0 + 3 * icoord + 3 * dimension * ipoint] = value
    //   aData[1 + 3 * icoord + 3 * dimension * ipoint] = error minus
    //   aData[2 + 3 * icoord + 3 * dimension * ipoint] = error plus
    // with :
    //   icoord in [0,dimension[
    //   ipoint in [0,aNumber[
    lock();
    bool stat = false;
    if(aNumber) {
      if(fPoints.empty()) fPoints.resize(aNumber,DataPoint(fDimension));
      if(fPoints.size()==aNumber) {
        std::vector<DataPoint>::const_iterator it;
        double* data = (double*)aData;
        for(it=fPoints.begin();it!=fPoints.end();++it) {
          for(int icoord=0;icoord<fDimension;icoord++) {
            AIDA::IMeasurement* m = 
              (AIDA::IMeasurement*)(*it).coordinate(icoord);
            m->setValue(*data);data++;
            m->setErrorMinus(*data);data++;
            m->setErrorPlus(*data);data++;
          }
        }
        stat = true;
      }
    }
    unlock();
    if(stat) emitUpdate();
    return stat;
  }

private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const DataPointSet* local = 
      INLIB_CONST_CAST(aObject,BatchLab::DataPointSet);
    if(!local) return false;
    return local->BatchLab::DataPointSet::visit(aVisitor);
  }
private:
  Annotation fAnnotation;
  std::string fTitle;
  int fDimension;
  std::vector<DataPoint> fPoints;
};

}

#endif

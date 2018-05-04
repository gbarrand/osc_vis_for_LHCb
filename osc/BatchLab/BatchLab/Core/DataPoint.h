#ifndef BatchLab_DataPoint_h
#define BatchLab_DataPoint_h

#include <AIDA/IDataPoint.h>

#include <vector>

#include <Lib/Debug.h>

#include <BatchLab/Core/Measurement.h>

namespace BatchLab {

class DataPoint : public virtual AIDA::IDataPoint {
public: //AIDA/IDataPoint
  virtual int dimension() const {return fDimension;}
  virtual AIDA::IMeasurement* coordinate(int aCoordinate) {
    if(aCoordinate<0) return 0;
    if(aCoordinate>=fDimension) return 0;
    return &(fMeasurements[aCoordinate]);
  }
  virtual const AIDA::IMeasurement* coordinate(int aCoordinate) const {
    if(aCoordinate<0) return 0;
    if(aCoordinate>=fDimension) return 0;
    return &(fMeasurements[aCoordinate]);
  }
public:
  inline DataPoint(int aDimension)
  :fDimension(aDimension),fMeasurements(aDimension){
    Lib::Debug::increment("BatchLab::DataPoint");
  }
  inline DataPoint(const AIDA::IDataPoint& aPoint)
  :fDimension(aPoint.dimension())
  ,fMeasurements(aPoint.dimension())
  {
    Lib::Debug::increment("BatchLab::DataPoint");
    initialize(aPoint);
  }
public:
  inline DataPoint(const DataPoint& aPoint)
  :AIDA::IDataPoint(aPoint)
  ,fDimension(aPoint.dimension())
  ,fMeasurements(aPoint.dimension())
  {
    Lib::Debug::increment("BatchLab::DataPoint");
    initialize(aPoint);
  }
  inline DataPoint& operator=(const DataPoint& aPoint) {
    fDimension = aPoint.dimension();
    fMeasurements.resize(aPoint.dimension());
    initialize(aPoint);
    return *this;
  }
  virtual ~DataPoint() {
    Lib::Debug::decrement("BatchLab::DataPoint");
  }
private:
  inline void initialize(const AIDA::IDataPoint& aPoint) {
    for(int index=0;index<fDimension;index++) {
      const AIDA::IMeasurement* measurement = aPoint.coordinate(index);
      if(measurement) {
        fMeasurements[index] = Measurement(measurement->value(),
                                           measurement->errorPlus(),
                                           measurement->errorMinus());
      }
    }
  }
private:
  int fDimension;
  std::vector<Measurement> fMeasurements;
};

}

#endif

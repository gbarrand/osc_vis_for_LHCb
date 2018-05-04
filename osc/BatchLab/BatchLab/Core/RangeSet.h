#ifndef BatchLab_RangeSet_h
#define BatchLab_RangeSet_h

// Inheritance :
#include <AIDA/IRangeSet.h>

namespace BatchLab {

class RangeSet : public virtual AIDA::IRangeSet {
public: 
  virtual const std::vector<double>& lowerBounds() { return fMins;}
  virtual const std::vector<double>& upperBounds() { return fMaxs;}
  virtual void include(double /*xMin*/, double /*xMax*/) {}
  virtual void exclude(double /*xMin*/, double /*xMax*/) {}
  virtual void includeAll() {}
  virtual void excludeAll() {}
  virtual bool isInRange(double /*point*/) { return false;}
  virtual int size() { return 0;}
  virtual double PLUS_INF() { return 0;}
  virtual double MINUS_INF() { return 0;}
public:
  virtual ~RangeSet(){}
private:
  std::vector<double> fMins;
  std::vector<double> fMaxs;
};

}

#endif

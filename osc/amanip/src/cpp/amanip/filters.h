#ifndef amanip_filters_h
#define amanip_filters_h

// Inheritance :
#include <AIDA/IFilter.h>

#include <AIDA/IEvaluator.h>

namespace amanip {

//used in BatchLab.
class accept_all : public virtual AIDA::IFilter {
public: //AIDA/IFilter
  virtual bool initialize(AIDA::ITuple&){ return true;}
  virtual bool accept() const {return true;}
  virtual std::string expression() const { return "";}
public:
  accept_all(){}
  virtual ~accept_all(){}
};

//used in OnXLab/TupleExplorer.cxx
class filter_2D : public virtual AIDA::IFilter {
public: //AIDA/IFilter
  virtual bool initialize(AIDA::ITuple& aTuple){ 
    if(!m_x.initialize(aTuple)) return false;
    if(!m_y.initialize(aTuple)) return false;
    return true;
  }
  virtual bool accept() const {
    double x = m_x.evaluateDouble();
    double y = m_y.evaluateDouble();
    if( (m_x_min<=x)&&(x<m_x_max) &&
        (m_y_min<=y)&&(y<m_y_max) ){
      return true;
    }
    return false;
  }
  virtual std::string expression() const { return "";}
public:
  filter_2D(AIDA::IEvaluator& aX,double aXmin,double aXmax,
           AIDA::IEvaluator& aY,double aYmin,double aYmax)
  :m_x(aX),m_x_min(aXmin),m_x_max(aXmax)
  ,m_y(aY),m_y_min(aYmin),m_y_max(aYmax)
  {}
  virtual ~filter_2D(){}
private:
  AIDA::IEvaluator& m_x;
  double m_x_min,m_x_max;
  AIDA::IEvaluator& m_y;
  double m_y_min,m_y_max;
};

class filter_3D : public virtual AIDA::IFilter {
public: //AIDA/IFilter
  virtual bool initialize(AIDA::ITuple& aTuple){ 
    if(!m_x.initialize(aTuple)) return false;
    if(!m_y.initialize(aTuple)) return false;
    if(!m_z.initialize(aTuple)) return false;
    return true;
  }
  virtual bool accept() const {
    double x = m_x.evaluateDouble();
    double y = m_y.evaluateDouble();
    double z = m_z.evaluateDouble();
    if( (m_x_min<=x)&&(x<m_x_max) &&
        (m_y_min<=y)&&(y<m_y_max) &&
        (m_z_min<=z)&&(z<m_z_max) ){
      return true;
    }
    return false;
  }
  virtual std::string expression() const { return "";}
public:
  filter_3D(AIDA::IEvaluator& aX,double aXmin,double aXmax,
           AIDA::IEvaluator& aY,double aYmin,double aYmax,
           AIDA::IEvaluator& aZ,double aZmin,double aZmax)
  :m_x(aX),m_x_min(aXmin),m_x_max(aXmax)
  ,m_y(aY),m_y_min(aYmin),m_y_max(aYmax)
  ,m_z(aZ),m_z_min(aZmin),m_z_max(aZmax)
  {}
  virtual ~filter_3D(){}
private:
  AIDA::IEvaluator& m_x;
  double m_x_min,m_x_max;
  AIDA::IEvaluator& m_y;
  double m_y_min,m_y_max;
  AIDA::IEvaluator& m_z;
  double m_z_min,m_z_max;
};

}

#endif

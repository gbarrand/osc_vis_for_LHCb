#ifndef BatchLab_HistogramAllocator_h
#define BatchLab_HistogramAllocator_h

// Inheritance :
#include <Slash/Core/IManager.h>

#include <BatchLab/Core/Cloud.h>

namespace BatchLab {

class HistogramAllocator : public virtual Slash::Core::IManager {
public: 
  HistogramAllocator(std::ostream& a_out)
  :f_out(a_out),fName("BatchLab::HistogramAllocator")
  {}
  virtual ~HistogramAllocator(){}
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const { 
    INLIB_IF_CAST(BatchLab::HistogramAllocator)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  // new_ methods having same signature than constructors :
  virtual Histogram1D* new_Histogram1D(std::ostream& a_out,
                                       const std::string& aName,
                                       inlib::histo::h1d& aHclHisto){
    BatchLab::Histogram1D* object = 
      new BatchLab::Histogram1D(a_out,aName,aHclHisto);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram1D* new_Histogram1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax){
    BatchLab::Histogram1D* object = 
      new BatchLab::Histogram1D(a_out,aName,aTitle,aXnumber,aXmin,aXmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_")) {
      delete object;
      return 0;
    }
    return object;
  }
  virtual Histogram1D* new_Histogram1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdges){
    BatchLab::Histogram1D* object = 
      new BatchLab::Histogram1D(a_out,aName,aTitle,aEdges);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram1D* new_Histogram1D(const Histogram1D& aFrom) {
    return create_copy(aFrom);
  }

  virtual Histogram2D* new_Histogram2D(std::ostream& a_out,
                                       const std::string& aName,
                                       inlib::histo::h2d& aHclHisto){
    BatchLab::Histogram2D* object = 
      new BatchLab::Histogram2D(a_out,aName,aHclHisto);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram2D* new_Histogram2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax,
                                       int aYnumber,double aYmin,double aYmax){
    BatchLab::Histogram2D* object = 
      new BatchLab::Histogram2D(a_out,aName,aTitle,
                                aXnumber,aXmin,aXmax,
                                aYnumber,aYmin,aYmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram2D* new_Histogram2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdgesX,
                                       const std::vector<double>& aEdgesY){
    BatchLab::Histogram2D* object = 
      new BatchLab::Histogram2D(a_out,aName,aTitle,aEdgesX,aEdgesY);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram2D* new_Histogram2D(const Histogram2D& aFrom){
    return create_copy(aFrom);
  }

  virtual Histogram3D* new_Histogram3D(std::ostream& a_out,
                                       const std::string& aName,
                                       inlib::histo::h3d& aHclHisto){
    BatchLab::Histogram3D* object = 
      new BatchLab::Histogram3D(a_out,aName,aHclHisto);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram3D")) {
      delete object;
      return 0;
    }
    return object;
  }


  virtual Histogram3D* new_Histogram3D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,
                                       double aXmin,
                                       double aXmax,
                                       int aYnumber,
                                       double aYmin,
                                       double aYmax,
                                       int aZnumber,
                                       double aZmin,
                                       double aZmax){
    BatchLab::Histogram3D* object = 
      new BatchLab::Histogram3D(a_out,aName,aTitle,
                                aXnumber,aXmin,aXmax,
                                aYnumber,aYmin,aYmax,
                                aZnumber,aZmin,aZmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram3D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram3D* new_Histogram3D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdgesX,
                                       const std::vector<double>& aEdgesY,
                                       const std::vector<double>& aEdgesZ){
    BatchLab::Histogram3D* object = 
      new BatchLab::Histogram3D(a_out,aName,aTitle,aEdgesX,aEdgesY,aEdgesZ);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Histogram3D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Histogram3D* new_Histogram3D(const Histogram3D& aFrom){
    return create_copy(aFrom);
  }

  virtual Profile1D* new_Profile1D(std::ostream& a_out,
                                       const std::string& aName,
                                       inlib::histo::p1d& aHclHisto){
    BatchLab::Profile1D* object = 
      new BatchLab::Profile1D(a_out,aName,aHclHisto);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile1D* new_Profile1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax){
    BatchLab::Profile1D* object = 
      new BatchLab::Profile1D(a_out,aName,aTitle,aXnumber,aXmin,aXmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile1D* new_Profile1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax,
                                       double aVmin,double aVmax){
    BatchLab::Profile1D* object = 
      new BatchLab::Profile1D(a_out,aName,aTitle,
                              aXnumber,aXmin,aXmax,aVmin,aVmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile1D* new_Profile1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdges){
    BatchLab::Profile1D* object = 
      new BatchLab::Profile1D(a_out,aName,aTitle,aEdges);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile1D* new_Profile1D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdges,
                                       double aVmin,double aVmax){
    BatchLab::Profile1D* object = 
      new BatchLab::Profile1D(a_out,aName,aTitle,aEdges,aVmin,aVmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Prfile1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile1D* new_Profile1D(const Profile1D& aFrom){
    return create_copy(aFrom);
  }

  virtual Profile2D* new_Profile2D(std::ostream& a_out,
                                       const std::string& aName,
                                       inlib::histo::p2d& aHclHisto){
    BatchLab::Profile2D* object = 
      new BatchLab::Profile2D(a_out,aName,aHclHisto);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile2D* new_Profile2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax,
                                       int aYnumber,double aYmin,double aYmax){
    BatchLab::Profile2D* object = 
      new BatchLab::Profile2D(a_out,aName,aTitle,
                              aXnumber,aXmin,aXmax,
                              aYnumber,aYmin,aYmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile2D* new_Profile2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       int aXnumber,double aXmin,double aXmax,
                                       int aYnumber,double aYmin,double aYmax,
                                       double aVmin,double aVmax){
    BatchLab::Profile2D* object = 
      new BatchLab::Profile2D(a_out,aName,aTitle,
                              aXnumber,aXmin,aXmax,
                              aYnumber,aYmin,aYmax,
                              aVmin,aVmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile2D* new_Profile2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdgesX,
                                       const std::vector<double>& aEdgesY){
    BatchLab::Profile2D* object = 
      new BatchLab::Profile2D(a_out,aName,aTitle,aEdgesX,aEdgesY);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile2D* new_Profile2D(std::ostream& a_out,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::vector<double>& aEdgesX,
                                       const std::vector<double>& aEdgesY,
                                       double aVmin,double aVmax){
    BatchLab::Profile2D* object = 
      new BatchLab::Profile2D(a_out,aName,aTitle,aEdgesX,aEdgesY,aVmin,aVmax);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Profile2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Profile2D* new_Profile2D(const Profile2D& aFrom){
    return create_copy(aFrom);
  }

  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  virtual Cloud1D* new_Cloud1D(std::ostream& a_out,
                                         const std::string& aName,
                                         const std::string& aTitle,
                                         int aLimit = -1){
    BatchLab::Cloud1D* object = 
      new BatchLab::Cloud1D(a_out,aName,aTitle,aLimit);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Cloud1D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Cloud1D* new_Cloud1D(const Cloud1D& aFrom){
    return create_copy(aFrom);
  }

  virtual Cloud2D* new_Cloud2D(std::ostream& a_out,
                                         const std::string& aName,
                                         const std::string& aTitle,
                                         int aLimit = -1){
    BatchLab::Cloud2D* object = 
      new BatchLab::Cloud2D(a_out,aName,aTitle,aLimit);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Cloud2D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Cloud2D* new_Cloud2D(const Cloud2D& aFrom){
    return create_copy(aFrom);
  }

  virtual Cloud3D* new_Cloud3D(std::ostream& a_out,
                                         const std::string& aName,
                                         const std::string& aTitle,
                                         int aLimit = -1){
    BatchLab::Cloud3D* object = 
      new BatchLab::Cloud3D(a_out,aName,aTitle,aLimit);
    if(!CHECK_HISTO(object,"BatchLab::HistogramAllocator::new_Cloud3D")) {
      delete object;
      return 0;
    }
    return object;
  }

  virtual Cloud3D* new_Cloud3D(const Cloud3D& aFrom){
    return create_copy(aFrom);
  }
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
private:
  template <class T>
  inline bool CHECK_HISTO(T* object,const std::string& aCMT) {
    if(!object) return false;
    if(!object->isValid()) {
      f_out << aCMT << " :"
          << " unable to create a valid histogram."
          << " Check the booking parameters."
          << std::endl;
      return false;
    }
    return true;
  }

  template <class T>
  T* create_copy(const T& a_from) {
    T* object = new T(a_from);
    if(!object) return 0;
    if(!object->isValid()) {
      f_out << " create_copy :"
          << " unable to create a valid object."
          << " Check the booking parameters."
          << std::endl;
      delete object;
      return 0;
    }
    return object;
  }
private:
  std::ostream& f_out;
  std::string fName;
};

}

#endif

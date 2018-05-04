#ifndef BatchLab_Histogram_h
#define BatchLab_Histogram_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <AIDA/IAxis.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IHistogram3D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Meta/IObject.h>

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <inlib/histo/h3d>
#include <inlib/histo/p1d>
#include <inlib/histo/p2d>
#include <inlib/scast>

#include <BatchLab/Core/Annotation.h>

namespace BatchLab {

class Axis 
:public virtual AIDA::IAxis 
,public virtual Slash::Store::IStorable {
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Axis");return s_v;
  }
public: //AIDA::IAxis
  virtual bool isFixedBinning() const;
  virtual double lowerEdge() const;
  virtual double upperEdge() const;
  virtual int bins() const;
  virtual double binLowerEdge(int) const;
  virtual double binUpperEdge(int) const;
  virtual double binWidth(int) const;
  virtual double binCenter(int index) const; //AIDA-v3r3p0
  virtual int coordToIndex(double) const;
public: //IVisited
  virtual void* cast(const std::string&) const;
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public:
  Axis();
  Axis(const Axis&);
  virtual ~Axis();
  void copy(const inlib::histo::axis<double>&);
  const inlib::histo::axis<double>& bareAxis() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Axis* local = inlib::cast<Slash::Store::IStorable,Axis>(aObject);
    if(!local) return false;
    return local->BatchLab::Axis::visit(aVisitor);
  }
private:
  inlib::histo::axis<double> fAxis;
};

class BaseHistogram 
:public BaseObject 
,public virtual Slash::Store::IStorable
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::BaseHistogram");return s_v;
  }
public: //IVisited
  virtual void* cast(const std::string&) const;
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public:
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const = 0; //for Rio_THistogram.
  virtual bool isValid() const = 0;
protected:
  BaseHistogram(std::ostream&,const std::string&);
  BaseHistogram(const BaseHistogram&);
public:
  virtual ~BaseHistogram();
  // The belows is used in BatchLab::Rio::THistogram.
  Annotation& getAnnotation(); //annotation() is ambiguous vs the AIDA one.
  const Annotation& getAnnotation() const;
  void setAnnotation(const Annotation&);
protected:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const BaseHistogram* local = 
      inlib::cast<Slash::Store::IStorable,BaseHistogram>(aObject);
    if(!local) return false;
    return local->BatchLab::BaseHistogram::visit(aVisitor);
  }
protected:
  Annotation fAnnotation;
};

class Histogram1D 
:public BaseHistogram
,public virtual AIDA::IHistogram1D
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Histogram1D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual bool reset();
public: //AIDA/IHistogram
  virtual int allEntries() const;
  virtual int extraEntries() const;
  virtual double equivalentBinEntries() const;
  virtual double sumBinHeights() const;
  virtual double sumAllBinHeights() const;
  virtual double sumExtraBinHeights() const;
  virtual double minBinHeight() const;
  virtual double maxBinHeight() const;
  virtual bool scale(double);
public: //AIDA/IHistogram1D
  virtual bool fill(double,double);
  virtual double binMean(int) const;
  virtual double binStdDev(int) const; //Not AIDA
  virtual int binEntries(int) const;
  virtual double binHeight(int) const;
  virtual double binError(int) const;
  virtual double mean() const;
  virtual double rms() const;
  virtual const AIDA::IAxis& axis() const;
  virtual int coordToIndex(double) const;
  virtual bool add(const AIDA::IHistogram1D&);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BatchLab::BaseHistogram
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const {
    return fHistogram.get_histo_data();
  }
  virtual bool isValid() const;
public:
  Histogram1D(std::ostream&,const std::string& = "");
  Histogram1D(std::ostream&,const std::string&,const inlib::histo::h1d&);
  Histogram1D(std::ostream&,
              const std::string&,
              const std::string&,
              int,double,double);
  Histogram1D(std::ostream&,
              const std::string&,
              const std::string&,
              const std::vector<double>&);
  Histogram1D(const Histogram1D&);
  virtual ~Histogram1D();
  const inlib::histo::h1d& hclHistogram() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Histogram1D* local = 
      inlib::cast<Slash::Store::IStorable,Histogram1D>(aObject);
    if(!local) return false;
    return local->BatchLab::Histogram1D::visit(aVisitor);
  }
private:
  inlib::histo::h1d fHistogram; 
  Axis fAxis;
};

class Histogram2D 
:public BaseHistogram
,public virtual AIDA::IHistogram2D
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Histogram2D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual bool reset();
public: //AIDA/IHistogram
  virtual int allEntries() const;
  virtual int extraEntries() const;
  virtual double equivalentBinEntries() const;
  virtual double sumBinHeights() const;
  virtual double sumAllBinHeights() const;
  virtual double sumExtraBinHeights() const;
  virtual double minBinHeight() const;
  virtual double maxBinHeight() const;
  virtual bool scale(double);
public: //AIDA/IHistogram2D
  virtual bool fill(double,double,double);
  virtual double binMeanX(int,int) const;
  virtual double binMeanY(int,int) const;
  virtual double binStdDevX(int,int) const;
  virtual double binStdDevY(int,int) const;
  virtual int binEntries(int,int) const;
  virtual int binEntriesX(int) const;
  virtual int binEntriesY(int) const;
  virtual double binHeight(int,int) const;
  virtual double binHeightX(int) const;
  virtual double binHeightY(int) const;
  virtual double binError(int,int) const;
  virtual double meanX() const;
  virtual double meanY() const;
  virtual double rmsX() const;
  virtual double rmsY() const;
  virtual const AIDA::IAxis& xAxis() const;
  virtual const AIDA::IAxis& yAxis() const;
  virtual int coordToIndexX(double) const;
  virtual int coordToIndexY(double) const;
  virtual bool add(const IHistogram2D&);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BatchLab::BaseHistogram
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const {
    return fHistogram.get_histo_data();
  }
  virtual bool isValid() const;
public:
  Histogram2D(std::ostream&,const std::string& = "");
  Histogram2D(std::ostream&,const std::string&,const inlib::histo::h2d&);
  Histogram2D(std::ostream&,
              const std::string&,
              const std::string&,
              int,double,double,
              int,double,double);
  Histogram2D(std::ostream&,
              const std::string&,
              const std::string&,
              const std::vector<double>&,
              const std::vector<double>&);
  Histogram2D(const Histogram2D&);
  virtual ~Histogram2D();
  const inlib::histo::h2d& hclHistogram() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Histogram2D* local = 
      inlib::cast<Slash::Store::IStorable,Histogram2D>(aObject);
    if(!local) return false;
    return local->BatchLab::Histogram2D::visit(aVisitor);
  }
private:
  inlib::histo::h2d fHistogram; 
  Axis fAxisX;
  Axis fAxisY;
};

class Histogram3D 
:public BaseHistogram
,public virtual AIDA::IHistogram3D
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Histogram3D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual bool reset();
public: //AIDA/IHistogram
  virtual int allEntries() const;
  virtual int extraEntries() const;
  virtual double equivalentBinEntries() const;
  virtual double sumBinHeights() const;
  virtual double sumAllBinHeights() const;
  virtual double sumExtraBinHeights() const;
  virtual double minBinHeight() const;
  virtual double maxBinHeight() const;
  virtual bool scale(double);
public: //AIDA/IHistogram3D
  virtual bool fill(double,double,double,double);
  virtual double binMeanX(int,int,int) const;
  virtual double binMeanY(int,int,int) const;
  virtual double binMeanZ(int,int,int) const;
  virtual double binStdDevX(int,int,int) const;
  virtual double binStdDevY(int,int,int) const;
  virtual double binStdDevZ(int,int,int) const;
  virtual int binEntries(int,int,int) const;
  virtual int binEntriesX(int) const;
  virtual int binEntriesY(int) const;
  virtual int binEntriesZ(int) const;
  virtual double binHeight(int,int,int) const;
  virtual double binHeightX(int) const;
  virtual double binHeightY(int) const;
  virtual double binHeightZ(int) const;
  virtual double binError(int,int,int) const;
  virtual double meanX() const;
  virtual double meanY() const;
  virtual double meanZ() const;
  virtual double rmsX() const;
  virtual double rmsY() const;
  virtual double rmsZ() const;
  virtual const AIDA::IAxis& xAxis() const;
  virtual const AIDA::IAxis& yAxis() const;
  virtual const AIDA::IAxis& zAxis() const;
  virtual int coordToIndexX(double) const;
  virtual int coordToIndexY(double) const;
  virtual int coordToIndexZ(double) const;
  virtual bool add(const IHistogram3D&);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BatchLab::BaseHistogram
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const {
    return fHistogram.get_histo_data();
  }
  virtual bool isValid() const;
public:
  Histogram3D(std::ostream&,const std::string& = "");
  Histogram3D(std::ostream&,const std::string&,const inlib::histo::h3d&);
  Histogram3D(std::ostream&,
              const std::string&,
              const std::string&,
              int,double,double,
              int,double,double,
              int,double,double);
  Histogram3D(std::ostream&,
              const std::string&,
              const std::string&,
              const std::vector<double>&,
              const std::vector<double>&,
              const std::vector<double>&);
  Histogram3D(const Histogram3D&);
  virtual ~Histogram3D();
  const inlib::histo::h3d& hclHistogram() const;
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Histogram3D* local = 
      inlib::cast<Slash::Store::IStorable,Histogram3D>(aObject);
    if(!local) return false;
    return local->BatchLab::Histogram3D::visit(aVisitor);
  }
private:
  inlib::histo::h3d fHistogram; 
  Axis fAxisX;
  Axis fAxisY;
  Axis fAxisZ;
};

class Profile1D 
:public BaseHistogram
,public virtual AIDA::IProfile1D
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Profile1D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual bool reset();
public: //AIDA/IHistogram
  virtual int allEntries() const;
  virtual int extraEntries() const;
  virtual double equivalentBinEntries() const;
  virtual double sumBinHeights() const;
  virtual double sumAllBinHeights() const;
  virtual double sumExtraBinHeights() const;
  virtual double minBinHeight() const;
  virtual double maxBinHeight() const;
  virtual bool scale(double);
public: //AIDA/IProfile1D
  virtual bool fill(double,double,double);
  virtual double binMean(int) const;
  virtual double binStdDev(int) const; //Not AIDA
  virtual int binEntries(int) const;
  virtual double binHeight(int) const;
  virtual double binError(int) const;
  virtual double binRms(int) const;
  virtual double mean() const;
  virtual double rms() const;
  virtual const AIDA::IAxis& axis() const;
  virtual int coordToIndex(double) const;
  virtual bool add(const AIDA::IProfile1D&);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BatchLab::BaseHistogram
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const {
    return fHistogram.get_histo_data();
  }
  virtual bool isValid() const;
public:
  Profile1D(std::ostream&,const std::string& = "");
  Profile1D(std::ostream&,const std::string&,const inlib::histo::p1d&);
  Profile1D(std::ostream&,
            const std::string&,
            const std::string&,
            int,double,double);
  Profile1D(std::ostream&,
            const std::string&,
            const std::string&,
            int,double,double,
            double,double);
  Profile1D(std::ostream&,
            const std::string&,
            const std::string&,
            const std::vector<double>&);
  Profile1D(std::ostream&,
            const std::string&,
            const std::string&,
            const std::vector<double>&,
            double,double);
  Profile1D(const Profile1D&);
  virtual ~Profile1D();
  const inlib::histo::p1d& hclHistogram() const {return fHistogram;}
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Profile1D* local = 
      inlib::cast<Slash::Store::IStorable,Profile1D>(aObject);
    if(!local) return false;
    return local->BatchLab::Profile1D::visit(aVisitor);
  }
private:
  inlib::histo::p1d fHistogram; 
  Axis fAxis;
};

class Profile2D 
:public BaseHistogram
,public virtual AIDA::IProfile2D
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Profile2D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual bool reset();
public: //AIDA/IHistogram
  virtual int allEntries() const;
  virtual int extraEntries() const;
  virtual double equivalentBinEntries() const;
  virtual double sumBinHeights() const;
  virtual double sumAllBinHeights() const;
  virtual double sumExtraBinHeights() const;
  virtual double minBinHeight() const;
  virtual double maxBinHeight() const;
  virtual bool scale(double);
public: //AIDA/IProfile2D
  virtual bool fill(double,double,double,double);
  virtual double binMeanX(int,int) const;
  virtual double binMeanY(int,int) const;
  virtual double binStdDevX(int,int) const; //Not AIDA
  virtual double binStdDevY(int,int) const; //Not AIDA
  virtual int binEntries(int,int) const;
  virtual int binEntriesX(int) const;
  virtual int binEntriesY(int) const;
  virtual double binRms(int,int) const;
  virtual double binHeight(int,int) const;
  virtual double binHeightX(int) const;
  virtual double binHeightY(int) const;
  virtual double binError(int,int) const;
  virtual double meanX() const;
  virtual double meanY() const;
  virtual double rmsX() const;
  virtual double rmsY() const;
  virtual const AIDA::IAxis& xAxis() const;
  virtual const AIDA::IAxis& yAxis() const;
  virtual int coordToIndexX(double) const;
  virtual int coordToIndexY(double) const;
  virtual bool add(const IProfile2D&);
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public: //BatchLab::BaseHistogram
  virtual inlib::histo::profile_data<double,unsigned int,double,double> histo_data() const {
    return fHistogram.get_histo_data();
  }
  virtual bool isValid() const;
public:
  Profile2D(std::ostream&,const std::string& = "");
  Profile2D(std::ostream&,const std::string&,const inlib::histo::p2d&);
  Profile2D(std::ostream&,
              const std::string&,
              const std::string&,
              int,double,double,
              int,double,double);
  Profile2D(std::ostream&,
              const std::string&,
              const std::string&,
              int,double,double,
              int,double,double,
              double,double);
  Profile2D(std::ostream&,
              const std::string&,
              const std::string&,
              const std::vector<double>&,
              const std::vector<double>&);
  Profile2D(std::ostream&,
              const std::string&,
              const std::string&,
              const std::vector<double>&,
              const std::vector<double>&,
              double,double);
  Profile2D(const Profile2D&);
  virtual ~Profile2D();
  const inlib::histo::p2d& hclHistogram() const {return fHistogram;}
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Profile2D* local = 
      inlib::cast<Slash::Store::IStorable,Profile2D>(aObject);
    if(!local) return false;
    return local->BatchLab::Profile2D::visit(aVisitor);
  }
private:
  inlib::histo::p2d fHistogram; 
  Axis fAxisX;
  Axis fAxisY;
};

}

#include "Histogram.icc"

#endif

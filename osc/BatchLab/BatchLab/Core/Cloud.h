#ifndef BatchLab_Cloud_h
#define BatchLab_Cloud_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <Slash/Store/IStorable.h>
#include <Slash/Meta/IObject.h>

#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>

namespace AIDA {class IHistogram;}

#include <BatchLab/Core/Annotation.h>
#include <BatchLab/Core/Histogram.h> // For autoconvertion.
#include <inlib/mnmx>
#include <inlib/scast>

namespace BatchLab {

class Histogram1D;
class Histogram2D;
class Histogram3D;

class BaseCloud : public BaseObject {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(BatchLab::BaseCloud)
    else return BaseObject::cast(a_class);
  }
protected:
  BaseCloud(std::ostream& a_out,const std::string& aName,int aLimit)
  :BaseObject(a_out,aName)
  ,fLimit(aLimit)
  ,fSw(0)
  {}

  BaseCloud(const BaseCloud& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,BaseObject(aFrom)
  ,fAnnotation(aFrom.fAnnotation)
  ,fTitle(aFrom.fTitle)
  ,fLimit(aFrom.fLimit)
  ,fSw(aFrom.fSw)
  ,fWs(aFrom.fWs)
  {}
public:
  virtual ~BaseCloud(){}
public:
  bool isValid() const {return true;}
  Annotation& getAnnotation(){return fAnnotation;}
  const Annotation& getAnnotation() const{return fAnnotation;}
  void setAnnotation(const Annotation& aFrom){fAnnotation = aFrom;}
  std::vector<double>& ws(){return fWs;} //For IO (HDF5).
protected:
  Annotation fAnnotation;
  std::string fTitle;
  int fLimit;
  double fSw;
  std::vector<double> fWs;
};

class Cloud1D 
:public BaseCloud
,public virtual AIDA::ICloud1D 
,public virtual Slash::Store::IStorable
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Cloud1D");return s_v;
  }
public: //AIDA/IManagedObject
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(AIDA::ICloud1D)
    else INLIB_IF_CAST(AIDA::ICloud)
    else INLIB_IF_CAST(AIDA::IBaseHistogram)
    else INLIB_IF_CAST(Slash::Store::IStorable)
    else INLIB_IF_CAST(Slash::Meta::IObject)
    else INLIB_IF_CAST(BatchLab::Cloud1D)
    else return BaseCloud::cast(a_class);
  }
  virtual std::string type() const {return "AIDA::ICloud1D";} //AIDA-v3r3p0
public: //AIDA/IBaseHistogram
  virtual std::string title() const { 
    lock();
    std::string s = fTitle;
    unlock();
    return s;
  }

  virtual bool setTitle(const std::string& aTitle){
    lock();
    fTitle = aTitle;
    fAnnotation.setValue("Title",aTitle);
    if(fHistogram) fHistogram->setTitle(aTitle);
    unlock();
    return true;
  }

  virtual AIDA::IAnnotation& annotation() {return fAnnotation;}
  virtual const AIDA::IAnnotation& annotation() const {return fAnnotation;}
  virtual int dimension() const {return 1;}
  virtual bool reset() { 
    lock();
    clear(); //no lock/unlock inside
    delete fHistogram;
    fHistogram = 0;
    unlock();
    return true;
  }
  virtual int entries() const { 
    lock();
    int v = fHistogram ? fHistogram->allEntries() : fWs.size();
    unlock();
    return v;
  }
  virtual int nanEntries() const {return 0;} //FIXME
public: //AIDA/ICloud
  virtual double sumOfWeights() const { 
    lock();
    double v = (fHistogram ? fHistogram->sumBinHeights() : fSw);
    unlock();
    return v;
  }
  virtual bool convertToHistogram(){
    lock();
    bool stat = convert_to_histogram();
    unlock();
    return stat;
  }

  virtual bool isConverted() const {return fHistogram ? true : false;}
  virtual int maxEntries() const {return fLimit;}
  virtual bool scale(double aScale) {
    lock();
    bool stat = false;
    if(fHistogram) {
      stat = fHistogram->scale(aScale);
    } else {
      unsigned int number = fWs.size();
      for(unsigned int index=0;index<number;index++) fWs[index] *= aScale;
      fSw *= aScale;
      fSxw *= aScale;
      fSx2w *= aScale;
      stat = true;
    }
    unlock();
    return stat;
  }

public: //AIDA/ICloud1D
  virtual bool fill(double aX,double aW = 1){
    lock();
    if(!fHistogram && (fLimit!=BATCHLAB_UNLIMITED) && 
       ((int)fXs.size()>=fLimit)){
      convert_to_histogram();
    }
  
    bool stat = false;
    bool emit = false;
    if(fHistogram) {
      stat = fHistogram->fill(aX,aW);
    } else {
      if(fXs.size()) {
        fLowerX = inlib::mn<double>(aX,fLowerX);
        fUpperX = inlib::mx<double>(aX,fUpperX);
      } else {
        fLowerX = aX;
        fUpperX = aX;
      }
      fXs.push_back(aX);
      fWs.push_back(aW);
      fSw += aW;
      double xw = aX * aW;
      fSxw += xw;
      fSx2w += aX * xw;
      stat = true;
      emit = true;
    }
    unlock();
    if(emit) emitUpdate();
    return stat;
  }

  virtual double lowerEdge() const { 
    lock();
    double v = (fHistogram ? fHistogram->axis().lowerEdge() : fLowerX);
    unlock();
    return v;
  }
  virtual double upperEdge() const { 
    lock();
    double v = (fHistogram ? fHistogram->axis().upperEdge() : fUpperX);
    unlock();
    return v;
  }
  virtual double value(int aIndex) const {
    lock();
    double v = (fHistogram ? 0 : fXs[aIndex]);
    unlock();
    return v;
  }
  virtual double weight(int aIndex) const {
    lock();
    double v = (fHistogram ? 0 : fWs[aIndex]);
    unlock();
    return v;
  }
  virtual double mean() const {
    lock();
    double v = (fHistogram ? fHistogram->mean() : (fSw?fSxw/fSw:0));
    unlock();
    return v;
  }
  virtual double rms() const {
    lock();
    double rms = 0; //FIXME nan.
    if(fHistogram) {
      rms = fHistogram->rms();
    } else {
      if(fSw==0) {
      } else {
        double mean = fSxw / fSw;
        rms = ::sqrt(::fabs( (fSx2w / fSw) - mean * mean));
      }
    }
    unlock();
    return rms;
  }

  virtual bool convert(int aBins,double aLowerEdge,double aUpperEdge){
    lock();
    bool stat = private_convert(aBins,aLowerEdge,aUpperEdge);
    unlock();
    return stat;
  }

  virtual bool convert(const std::vector<double>& aEdges) {
    lock();
    bool stat = true;
    if(!fHistogram) {
      fHistogram = new BatchLab::Histogram1D(out(),
                                        this->name(),this->title(),
                                        aEdges);
      if(fHistogram) {
        fill_h_1D(*fHistogram);
        clear(); //no lock/unlock inside
      } else {
        stat = false;
      }
    }
    unlock();
    return stat;
  }

  virtual const AIDA::IHistogram1D& histogram() const {
    lock();
    INLIB_SELF(BatchLab::Cloud1D);
    if(!fHistogram) self.convert_to_histogram();
    unlock();
    return *fHistogram;
  }
  virtual bool fillHistogram(AIDA::IHistogram1D& aHistogram) const {
    lock();
    fill_h_1D(aHistogram);
    unlock();
    return true;
  }
  virtual bool setConversionParameters(int aCnvXnumber,
                                       double aCnvXmin,double aCnvXmax){
    //AIDA-v3r3p0
    lock();
    fCnvXnumber = aCnvXnumber;
    fCnvXmin = aCnvXmin;
    fCnvXmax = aCnvXmax;
    unlock();
    return true;
  }

public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const{
    lock();
    Slash::Store::IObject* obj = new BatchLab::Cloud1D(*this);
    unlock();
    return obj;
  }

  virtual std::string storeClassName() const{return "BatchLab::Cloud1D";}
  virtual void setName(const std::string& aName){
    lock();
    BaseObject::setName(aName);
    if(fHistogram) fHistogram->setName(aName);
    unlock();
  }
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor& aVisitor) const {
    if(!aVisitor.begin(*this,"BatchLab::Cloud1D",s_visit)) return false;

    int version = 1;
    if(!aVisitor.visit("fVersion",version)) return false;
  
    // BaseCloud :
    if(!aVisitor.visit("fAnnotation",fAnnotation)) return false;
    if(!aVisitor.visit("fTitle",fTitle)) return false;
    if(!aVisitor.visit("fLimit",fLimit)) return false;
    if(!aVisitor.visit("fSw",fSw)) return false;
    if(!aVisitor.visit("fWs",fWs)) return false;
  
    // Cloud1D :
    if(!aVisitor.visit("fXs",fXs)) return false;
    if(!aVisitor.visit("fLowerX",fLowerX)) return false;
    if(!aVisitor.visit("fUpperX",fUpperX)) return false;
    if(!aVisitor.visit("fSxw",fSxw)) return false;
    if(!aVisitor.visit("fSx2w",fSx2w)) return false;
    if(!aVisitor.visit("fCnvXnumber",fCnvXnumber)) return false;
    if(!aVisitor.visit("fCnvXmin",fCnvXmin)) return false;
    if(!aVisitor.visit("fCnvXmax",fCnvXmax)) return false;
  
    if(!aVisitor.visit("fIsConverted",fHistogram?true:false)) return false;
    if(fHistogram) {
      if(!aVisitor.visit("fHistogram",*fHistogram)) return false;
    }
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }

  virtual bool read(Slash::Store::IVisitor& aVisitor){
    if(!aVisitor.begin(*this)) return false;

    int version;
    if(!aVisitor.visit(version)) return false;
  
    // BaseCloud :
    if(!fAnnotation.read(aVisitor)) return false;
    if(!aVisitor.visit(fTitle)) return false;
    if(!aVisitor.visit(fLimit)) return false;
    if(!aVisitor.visit(fSw)) return false;
    if(!aVisitor.visit(fWs)) return false;
  
    // Cloud1D :
    if(!aVisitor.visit(fXs)) return false;
    if(!aVisitor.visit(fLowerX)) return false;
    if(!aVisitor.visit(fUpperX)) return false;
    if(!aVisitor.visit(fSxw)) return false;
    if(!aVisitor.visit(fSx2w)) return false;
    if(!aVisitor.visit(fCnvXnumber)) return false;
    if(!aVisitor.visit(fCnvXmin)) return false;
    if(!aVisitor.visit(fCnvXmax)) return false;
   {bool b;
    if(!aVisitor.visit(b)) return false;
    if(b) {
      fHistogram = new BatchLab::Histogram1D(out(),this->name());
      if(!fHistogram->read(aVisitor)) return false;
    }}
  
    if(!aVisitor.end(*this)) return false;
    return true;
  }

public:
  Cloud1D(std::ostream& a_out,const std::string& aName = "")
  :BaseCloud(a_out,aName,-1)
  ,fLowerX(0),fUpperX(0)
  ,fSxw(0),fSx2w(0)
  ,fCnvXnumber(0),fCnvXmin(0),fCnvXmax(0),fHistogram(0)
  {}
  
  Cloud1D(std::ostream& a_out,
          const std::string& aName,const std::string& aTitle,int aLimit = -1)
  :BaseCloud(a_out,aName,aLimit)
  ,fLowerX(0),fUpperX(0)
  ,fSxw(0),fSx2w(0)
  ,fCnvXnumber(0),fCnvXmin(0),fCnvXmax(0),fHistogram(0)
  {
    setTitle(aTitle);
  }

  Cloud1D(const Cloud1D& aFrom)
  :AIDA::IManagedObject(aFrom)
  ,Slash::Store::IObject(aFrom)
  ,AIDA::IBaseHistogram(aFrom)
  ,AIDA::ICloud(aFrom)
  ,AIDA::ICloud1D(aFrom)
  ,Slash::Store::IStorable(aFrom)
  ,Slash::Meta::IObject(aFrom)
  ,BaseCloud(aFrom)
  ,fXs(aFrom.fXs)
  ,fLowerX(aFrom.fLowerX)
  ,fUpperX(aFrom.fUpperX)
  ,fSxw(aFrom.fSxw)
  ,fSx2w(aFrom.fSx2w)
  ,fCnvXnumber(aFrom.fCnvXnumber)
  ,fCnvXmin(aFrom.fCnvXmin)
  ,fCnvXmax(aFrom.fCnvXmax)
  ,fHistogram(0)
  {
    if(aFrom.fHistogram) {
      fHistogram = new BatchLab::Histogram1D(*aFrom.fHistogram);
    }
  }

  virtual ~Cloud1D(){delete fHistogram;}
public:
  bool setHistogram(BatchLab::Histogram1D* aHistogram){ 
    //used in XML_DataReader.
    if(aHistogram->hasMutex()) {
      out() << "BatchLab::Cloud1D::setHistogram :"
          << " failed, histogram has a mutex." 
          << std::endl;
      return false;
    }
    lock();
    clear(); //no lock/unlock inside
    fHistogram = aHistogram;
    unlock();
    return true;
  }
  std::vector<double>& xs(){return fXs;} //For IO (HDF5).

private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Cloud1D* local = 
      inlib::cast<Slash::Store::IStorable,Cloud1D>(aObject);
    if(!local) return false;
    return local->BatchLab::Cloud1D::visit(aVisitor);
  }
private:
  //void update(); //IO
  void clear(){
    fLowerX = 0;
    fUpperX = 0;
    fSw = 0;
    fSxw = 0;
    fSx2w = 0;
    fXs.clear();
    fWs.clear();
  }

  static const int BATCHLAB_UNLIMITED = -1;
  static const int BINS = 100;

  bool convert_to_histogram(){
    if( (fCnvXnumber<=0) || (fCnvXmax<=fCnvXmin) ) {
      // Cloud min, max should be included in the histo.
      double dx = 0.01 * (upperEdge() - lowerEdge())/BINS;
      return private_convert(BINS,lowerEdge(),upperEdge() + dx);
    } else {
      return private_convert(fCnvXnumber,fCnvXmin,fCnvXmax);
    }
  }

  bool private_convert(int aBins,double aLowerEdge,double aUpperEdge){
    if(fHistogram) return true;
    fHistogram = new BatchLab::Histogram1D(out(),
                                      this->name(),this->title(),
                                      aBins,aLowerEdge,aUpperEdge);
    if(!fHistogram) return false;
    fill_h_1D(*fHistogram);
    clear(); //no lock/unlock inside
    return true;
  }
private:
  inline void fill_h_1D(AIDA::IHistogram1D& a_histo) const {
    unsigned int number = fXs.size();
    for(unsigned int index=0;index<number;index++) {
      a_histo.fill(fXs[index],fWs[index]);
    }
  }
private:
  std::vector<double> fXs;
  double fLowerX;
  double fUpperX;
  double fSxw;
  double fSx2w;
  //
  int fCnvXnumber;
  double fCnvXmin;
  double fCnvXmax;
  Histogram1D* fHistogram;
};

class Cloud2D 
:public BaseCloud
,public virtual AIDA::ICloud2D
,public virtual Slash::Store::IStorable
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Cloud2D");return s_v;
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
  virtual bool reset();
  virtual int entries() const;
  virtual int nanEntries() const;
public: //AIDA/ICloud
  virtual double sumOfWeights() const;
  virtual bool convertToHistogram();
  virtual bool isConverted() const;
  virtual int maxEntries() const;
  virtual bool scale(double);
public: //ICloud2D
  virtual bool fill(double,double,double = 1);
  virtual double lowerEdgeX() const;
  virtual double upperEdgeX() const;
  virtual double lowerEdgeY() const;
  virtual double upperEdgeY() const;
  virtual double valueX(int) const;
  virtual double valueY(int) const;
  virtual double weight(int) const;
  virtual double meanX() const;
  virtual double meanY() const;
  virtual double rmsX() const;
  virtual double rmsY() const;
  virtual bool convert(int,double,double,int,double,double);
  virtual bool convert(const std::vector<double>&,const std::vector<double>&);
  virtual const AIDA::IHistogram2D& histogram() const;
  virtual bool fillHistogram(AIDA::IHistogram2D&) const;
  virtual bool setConversionParameters(int,double,double,  
                                       int,double,double); //AIDA-v3r3p0
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
  virtual void setName(const std::string&);
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public:
  Cloud2D(std::ostream&,const std::string& = "");
  Cloud2D(std::ostream&,
          const std::string&,const std::string&,int=-1);
  Cloud2D(const Cloud2D&);
  virtual ~Cloud2D();
public:
  bool setHistogram(Histogram2D*); //used in XML_DataReader.
  std::vector<double>& xs(); //For IO (HDF5).
  std::vector<double>& ys(); //For IO (HDF5).
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Cloud2D* local =
      inlib::cast<Slash::Store::IStorable,Cloud2D>(aObject);
    if(!local) return false;
    return local->BatchLab::Cloud2D::visit(aVisitor);
  }
private:
  //void update(); //IO
  void clear();
  bool convert_to_histogram();
  bool private_convert(int,double,double,int,double,double);
private:
  inline void fill_h_2D(AIDA::IHistogram2D& a_histo) const {
    unsigned int number = fXs.size();
    for(unsigned int index=0;index<number;index++) {
      a_histo.fill(fXs[index],fYs[index],fWs[index]);
    }
  }
private:
  static const int BATCHLAB_UNLIMITED = -1;
  static const int BINS = 100;
private:
  std::vector<double> fXs;
  std::vector<double> fYs;
  double fLowerX;
  double fUpperX;
  double fLowerY;
  double fUpperY;
  double fSxw;
  double fSx2w;
  double fSyw;
  double fSy2w;
  //
  int fCnvXnumber;
  double fCnvXmin;
  double fCnvXmax;
  int fCnvYnumber;
  double fCnvYmin;
  double fCnvYmax;
  Histogram2D* fHistogram;
};

class Cloud3D
:public BaseCloud
,public virtual AIDA::ICloud3D
,public virtual Slash::Store::IStorable
,public virtual Slash::Meta::IObject
{
public:
  inline static const std::string& s_class() {
    static const std::string s_v("BatchLab::Cloud3D");return s_v;
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
  virtual bool reset();
  virtual int entries() const;
  virtual int nanEntries() const;
public: //AIDA/ICloud
  virtual double sumOfWeights() const;
  virtual bool convertToHistogram();
  virtual bool isConverted() const;
  virtual int maxEntries() const;
  virtual bool scale(double);
public: //AIDA/ICloud3D
  virtual bool fill(double,double,double,double = 1);
  virtual double lowerEdgeX() const;
  virtual double upperEdgeX() const;
  virtual double lowerEdgeY() const;
  virtual double upperEdgeY() const;
  virtual double lowerEdgeZ() const;
  virtual double upperEdgeZ() const;
  virtual double valueX(int) const;
  virtual double valueY(int) const;
  virtual double valueZ(int) const;
  virtual double weight(int) const;
  virtual double meanX() const;
  virtual double meanY() const;
  virtual double meanZ() const;
  virtual double rmsX() const;
  virtual double rmsY() const;
  virtual double rmsZ() const;
  virtual bool convert(int,double,double,int,double,double,int,double,double);
  virtual bool convert(const std::vector<double>&,
                       const std::vector<double>&,
                       const std::vector<double>&);
  virtual const AIDA::IHistogram3D& histogram() const;
  virtual bool fillHistogram(AIDA::IHistogram3D&) const;
  virtual bool setConversionParameters(int,double,double,
                                       int,double,double,
                                       int,double,double); //AIDA-v3r3p0
public: //Slash::Store::IObject
  virtual Slash::Store::IObject* copy() const;
  virtual std::string storeClassName() const;
  virtual void setName(const std::string&);
public: //IVisited
  virtual bool visit(Slash::Store::IConstVisitor&) const;
  virtual bool read(Slash::Store::IVisitor&);
public:
  Cloud3D(std::ostream&,const std::string& = "");
  Cloud3D(std::ostream&,
          const std::string&,const std::string&,int=-1);
  Cloud3D(const Cloud3D&);
  virtual ~Cloud3D();
public:
  bool setHistogram(Histogram3D*); //used in XML_DataReader.
  std::vector<double>& xs(); //For IO (HDF5).
  std::vector<double>& ys(); //For IO (HDF5).
  std::vector<double>& zs(); //For IO (HDF5).
private:
  //SLASH_STORE_HEADER
  static bool s_visit(const Slash::Store::IStorable& aObject,
                      Slash::Store::IConstVisitor& aVisitor){
    const Cloud3D* local =
      inlib::cast<Slash::Store::IStorable,Cloud3D>(aObject);
    if(!local) return false;
    return local->BatchLab::Cloud3D::visit(aVisitor);
  }
private:
  //void update(); //IO
  void clear();
  bool convert_to_histogram();
  bool private_convert(int,double,double,int,double,double,int,double,double);
private:
  inline void fill_h_3D(AIDA::IHistogram3D& a_histo) const {
    unsigned int number = fXs.size();
    for(unsigned int index=0;index<number;index++) {
      a_histo.fill(fXs[index],fYs[index],fZs[index],fWs[index]);
    }
  }
private:
  static const int BATCHLAB_UNLIMITED = -1;
  static const int BINS = 100;
private:
  std::vector<double> fXs;
  std::vector<double> fYs;
  std::vector<double> fZs;
  double fLowerX;
  double fUpperX;
  double fLowerY;
  double fUpperY;
  double fLowerZ;
  double fUpperZ;
  double fSxw;
  double fSx2w;
  double fSyw;
  double fSy2w;
  double fSzw;
  double fSz2w;
  //
  int fCnvXnumber;
  double fCnvXmin;
  double fCnvXmax;
  int fCnvYnumber;
  double fCnvYmin;
  double fCnvYmax;
  int fCnvZnumber;
  double fCnvZmin;
  double fCnvZmax;
  Histogram3D* fHistogram;
};

}

#include "Cloud.icc"

#endif

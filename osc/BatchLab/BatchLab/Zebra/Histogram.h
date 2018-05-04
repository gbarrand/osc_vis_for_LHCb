#ifndef BatchLab_Zebra_Histogram_h
#define BatchLab_Zebra_Histogram_h

// Inheritance :
#include <BatchLab/Core/BaseObject.h>
#include <AIDA/IAxis.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IHistogram3D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>

#include <vector>

#include <BatchLab/Core/Annotation.h>

namespace BatchLab {

namespace Zebra {

class Axis : public virtual AIDA::IAxis {
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
public:
  Axis(const std::string& aPath,int aID,int aDimension,bool aIsX,bool aFixed);
  Axis(const Axis&);
  virtual ~Axis();
private:
  void cd_beg() const;
  void cd_end() const;
private:
  std::string fPath;
  int fID;
  int fDimension;
  bool fIsX;
  bool fFixed;
  std::string f_tmp;
};

class BaseHistogram : public BaseObject {
protected:
  BaseHistogram(std::ostream&,const std::string&,int);
  BaseHistogram(std::ostream&,const BaseHistogram&);
public:
  virtual ~BaseHistogram();
  Annotation& annotation();
  void setAnnotation(const Annotation&);
protected:
  void cd_beg() const;
  void cd_end() const;
protected:
  Annotation fAnnotation;
  std::string fPath;
  int fID;
  std::string f_tmp;
};

class Histogram1D 
:public BaseHistogram
,public virtual AIDA::IHistogram1D
{
public: //AIDA/IBaseHistogram
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
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
public:
  Histogram1D(std::ostream&,
              const std::string&,int,
              const std::string&,
              int,double,double);
  Histogram1D(std::ostream&,
              const std::string&,int,
              const std::string&,
              const std::vector<double>&);
  Histogram1D(const Histogram1D&);
  virtual ~Histogram1D();
private:
  Axis fAxis;
};

class Histogram2D 
:public BaseHistogram
,public virtual AIDA::IHistogram2D
{
public: //AIDA/IBaseHistogram
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int nanEntries() const; //AIDA-v3r3p0
  virtual int entries() const;
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
public:
  Histogram2D(std::ostream&,
              const std::string&,int,
              const std::string&,
              int,double,double,
              int,double,double);
  Histogram2D(const Histogram2D&);
  virtual ~Histogram2D();
private:
  Axis fAxisX;
  Axis fAxisY;
};

class Profile1D 
:public BaseHistogram
,public virtual AIDA::IProfile1D
{
public: //AIDA/IBaseHistogram
  virtual void* cast(const std::string&) const;
  virtual std::string type() const; //AIDA-v3r3p0
  virtual std::string title() const;
  virtual bool setTitle(const std::string&);
  virtual AIDA::IAnnotation& annotation();
  virtual const AIDA::IAnnotation& annotation() const;
  virtual int dimension() const;
  virtual int entries() const;
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
public:
  Profile1D(std::ostream&,
            const std::string&,int,
            const std::string&,
            int,double,double,
            double,double);
  Profile1D(const Profile1D&);
  virtual ~Profile1D();
private:
  Axis fAxis;
};

}

}

#endif

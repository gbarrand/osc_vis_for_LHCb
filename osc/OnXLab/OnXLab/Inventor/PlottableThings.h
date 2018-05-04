#ifndef OnXLab_PlottableThings_h
#define OnXLab_PlottableThings_h 

// Inheritance :
#include <HEPVis/SbPlottableThings.h>

#include <string>
#include <vector>

namespace AIDA {
  class IHistogram;
  class IHistogram1D;
  class IHistogram2D;
  class IProfile1D;
  class IProfile2D;
  class IFunction;
  class ICloud;
  class ICloud1D;
  class ICloud2D;
  class ICloud3D;
  class IDataPointSet;
}

#include <BatchLab/Core/Annotation.h>

namespace BatchLab {class BaseFunction;}

#define PLOTTABLE_HEADER \
public: /*SbPlottableObject*/\
  virtual void* cast(const char*) const;\
  virtual bool isValid() const;\
  /*virtual void* nativeObject() const;*/\
  virtual const char* getInfos(const char*);\
  virtual const char* getName();\
  virtual const char* getLabel();\
  virtual const char* getLegend();\
  virtual int getDimension() const;\
private:\
  std::string fName;\
  std::string fTitle;\
  std::string fLegend;\
  std::string fInfos;\
  INotifier* fNotifier;\
  AIDA::IAnnotation* f_annotation;\
  bool fRetain;\
  std::string f_lf;/*optimization*/\
public:

namespace OnXLab {

class INotifier {
public:
  virtual ~INotifier() {}
public:
  virtual bool isValid() const = 0;
};

class PlottableHistogram1D 
:public virtual SbPlottableBins1D {
  PLOTTABLE_HEADER
public: //SbPlottableBins1D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBins() const;
  virtual float getAxisMinimum() const;
  virtual float getAxisMaximum() const;
  virtual float getBinLowerEdge(int) const;
  virtual float getBinUpperEdge(int) const;
  virtual int getBinNumberOfEntries(int) const;
  virtual float getBinSumOfWeights(int) const;
  virtual float getBinBar(int) const;
public:
  PlottableHistogram1D(const std::string&,
                       const AIDA::IHistogram1D*,
                       INotifier*,bool);
  virtual ~PlottableHistogram1D();
private:
  const AIDA::IHistogram1D* fData;
  // Retain mode :
  double f_minBinHeight;
  double f_maxBinHeight;
  //axis:
  int f_axis_bins;
  double f_axis_lowerEdge;
  double f_axis_upperEdge;
  std::vector<double> f_axis_binLowerEdge;
  std::vector<double> f_axis_binUpperEdge;
  //bins:
  std::vector<int> f_binEntries;
  std::vector<double> f_binHeight;
  std::vector<double> f_binError;
  //infos:
  int f_allEntries;
  double f_mean;
  double f_rms;
  double f_under;
  double f_over;  
};

class PlottableHistogram2D 
:public virtual SbPlottableBins2D {
  PLOTTABLE_HEADER
public: //SbPlottableBins2D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBinsX() const;
  virtual float getAxisMinimumX() const;
  virtual float getAxisMaximumX() const;
  virtual int getAxisNumberOfBinsY() const;
  virtual float getAxisMinimumY() const;
  virtual float getAxisMaximumY() const;
  virtual float getBinLowerEdgeX(int) const;
  virtual float getBinUpperEdgeX(int) const;
  virtual float getBinLowerEdgeY(int) const;
  virtual float getBinUpperEdgeY(int) const;
  virtual int getBinNumberOfEntries(int,int) const;
  virtual float getBinSumOfWeights(int,int) const;
  virtual float getBinBar(int,int) const;
public:
  PlottableHistogram2D(const std::string&,
                       const AIDA::IHistogram2D*,
                       INotifier*,bool);
  virtual ~PlottableHistogram2D();
private:
  const AIDA::IHistogram2D* fData;
  // Retain mode :
  double f_minBinHeight;
  double f_maxBinHeight;
  //axes:
  int f_xAxis_bins;
  double f_xAxis_lowerEdge;
  double f_xAxis_upperEdge;
  std::vector<double> f_xAxis_binLowerEdge;
  std::vector<double> f_xAxis_binUpperEdge;
  int f_yAxis_bins;
  double f_yAxis_lowerEdge;
  double f_yAxis_upperEdge;
  std::vector<double> f_yAxis_binLowerEdge;
  std::vector<double> f_yAxis_binUpperEdge;
  //bins:
  std::vector<int> f_binEntries;
  std::vector<double> f_binHeight;
  std::vector<double> f_binError;
  //infos:
  int f_allEntries;
  double f_meanX;
  double f_meanY;
  double f_rmsX;
  double f_rmsY;
};

class PlottableProfile1D 
:public virtual SbPlottableBins1D {
  PLOTTABLE_HEADER
public: //SbPlottableBins1D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBins() const;
  virtual float getAxisMinimum() const;
  virtual float getAxisMaximum() const;
  virtual float getBinLowerEdge(int) const;
  virtual float getBinUpperEdge(int) const;
  virtual int getBinNumberOfEntries(int) const;
  virtual float getBinSumOfWeights(int) const;
  virtual float getBinBar(int) const;
public:
  PlottableProfile1D(const std::string&,
                       const AIDA::IProfile1D*,
                       INotifier*,bool);
  virtual ~PlottableProfile1D();
private:
  const AIDA::IProfile1D* fData;
};

class PlottableProfile2D 
:public virtual SbPlottableBins2D {
  PLOTTABLE_HEADER
public: //SbPlottableBins2D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBinsX() const;
  virtual float getAxisMinimumX() const;
  virtual float getAxisMaximumX() const;
  virtual int getAxisNumberOfBinsY() const;
  virtual float getAxisMinimumY() const;
  virtual float getAxisMaximumY() const;
  virtual float getBinLowerEdgeX(int) const;
  virtual float getBinUpperEdgeX(int) const;
  virtual float getBinLowerEdgeY(int) const;
  virtual float getBinUpperEdgeY(int) const;
  virtual int getBinNumberOfEntries(int,int) const;
  virtual float getBinSumOfWeights(int,int) const;
  virtual float getBinBar(int,int) const;
public:
  PlottableProfile2D(const std::string&,
                     const AIDA::IProfile2D*,
                     INotifier*,bool);
  virtual ~PlottableProfile2D();
private:
  const AIDA::IProfile2D* fData;
};

class PlottableFunction1D 
:public virtual SbPlottableFunction1D {
  PLOTTABLE_HEADER
public: //SbPlottableFunction1D
  virtual bool value(float,float&) const;
  virtual int getNumberOfStepsX() const;
  virtual float getMinimumX() const;
  virtual float getMaximumX() const;
public:
  PlottableFunction1D(const std::string&,
                      const AIDA::IFunction*,
                      INotifier*,bool);
  virtual ~PlottableFunction1D();
private:
  const AIDA::IFunction* fData;
  const BatchLab::BaseFunction* fBaseFunction;
};

class PlottableFunction2D 
:public virtual SbPlottableFunction2D {
  PLOTTABLE_HEADER
public: //SbPlottableFunction2D
  virtual bool value(float,float,float&) const;
  virtual int getNumberOfStepsX() const;
  virtual float getMinimumX() const;
  virtual float getMaximumX() const;
  virtual int getNumberOfStepsY() const;
  virtual float getMinimumY() const;
  virtual float getMaximumY() const;
  virtual int getNumberOfPoints() const;
  virtual void getIthPoint(int,float&,float&,bool&) const;
  virtual bool setIthPoint(int,float,float);
  virtual bool dragger_update_points() const;
public:
  PlottableFunction2D(const std::string&,
                     const AIDA::IFunction*,
                     INotifier*,bool);
  virtual ~PlottableFunction2D();
private:
  const AIDA::IFunction* fData;
  const BatchLab::BaseFunction* fBaseFunction;
};

class PlottableInsidePolygon 
:public virtual SbPlottableFunction2D {
  PLOTTABLE_HEADER
public: //SbPlottableFunction2D
  virtual bool value(float,float,float&) const;
  virtual int getNumberOfStepsX() const;
  virtual float getMinimumX() const;
  virtual float getMaximumX() const;
  virtual int getNumberOfStepsY() const;
  virtual float getMinimumY() const;
  virtual float getMaximumY() const;
  virtual int getNumberOfPoints() const;
  virtual void getIthPoint(int,float&,float&,bool&) const;
  virtual bool setIthPoint(int,float,float);
  virtual bool dragger_update_points() const;
public:
  PlottableInsidePolygon(const std::string&,
                   const AIDA::IFunction*,
                   INotifier*,bool);
  virtual ~PlottableInsidePolygon();
private:
  const AIDA::IFunction* fData;
  const BatchLab::BaseFunction* fBaseFunction;
  // Retain mode :
  int f_entries;
  double f_lowerEdgeX;
  double f_upperEdgeX;
  double f_lowerEdgeY;
  double f_upperEdgeY;
  typedef std::pair<double,double> Point;
  std::vector<Point> f_points;
};

class PlottableInsideEllipse 
:public virtual SbPlottableFunction2D {
  PLOTTABLE_HEADER
public: //SbPlottablePoints2D
  virtual bool value(float,float,float&) const;
  virtual int getNumberOfStepsX() const;
  virtual float getMinimumX() const;
  virtual float getMaximumX() const;
  virtual int getNumberOfStepsY() const;
  virtual float getMinimumY() const;
  virtual float getMaximumY() const;
  virtual int getNumberOfPoints() const;
  virtual void getIthPoint(int,float&,float&,bool&) const;
  virtual bool setIthPoint(int,float,float);
  virtual bool dragger_update_points() const;
public:
  PlottableInsideEllipse(const std::string&,
                   const AIDA::IFunction*,
                   INotifier*,bool);
  virtual ~PlottableInsideEllipse();
private:
  const AIDA::IFunction* fData;
  const BatchLab::BaseFunction* fBaseFunction;
  int fNumber;
  double f_a;
  double f_b;
};

class PlottableCloud1D 
:public virtual SbPlottableBins1D {
  PLOTTABLE_HEADER
public: //SbPlottableBins1D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBins() const;
  virtual float getAxisMinimum() const;
  virtual float getAxisMaximum() const;
  virtual float getBinLowerEdge(int) const;
  virtual float getBinUpperEdge(int) const;
  virtual int getBinNumberOfEntries(int) const;
  virtual float getBinSumOfWeights(int) const;
  virtual float getBinBar(int) const;
public:
  PlottableCloud1D(const std::string&,
                   const AIDA::ICloud1D*,
                   INotifier*,bool);
  virtual ~PlottableCloud1D();
private:
  const AIDA::ICloud1D* fData;
};

class PlottableCloud2D 
:public virtual SbPlottablePoints2D {
  PLOTTABLE_HEADER
public: //SbPlottablePoints2D
  virtual float getAxisMinimumX() const;
  virtual float getAxisMaximumX() const;
  virtual float getAxisMinimumY() const;
  virtual float getAxisMaximumY() const;
  virtual int getNumberOfPoints() const;
  virtual void getIthPoint(int,float&,float&) const;
public:
  PlottableCloud2D(const std::string&,
                   const AIDA::ICloud2D*,
                   INotifier*,bool);
  virtual ~PlottableCloud2D();
private:
  const AIDA::ICloud2D* fData;
  // Retain mode :
  int f_entries;
  double f_lowerEdgeX;
  double f_upperEdgeX;
  double f_lowerEdgeY;
  double f_upperEdgeY;
  std::vector<double> f_valueX;
  std::vector<double> f_valueY;
};

class PlottableCloud3D 
:public virtual SbPlottablePoints3D {
  PLOTTABLE_HEADER
public: //SbPlottableCloud
  virtual float getAxisMinimumX() const;
  virtual float getAxisMaximumX() const;
  virtual float getAxisMinimumY() const;
  virtual float getAxisMaximumY() const;
  virtual float getAxisMinimumZ() const;
  virtual float getAxisMaximumZ() const;
  virtual int getNumberOfPoints() const;
  virtual void getIthPoint(int,float&,float&,float&)const;
public:
  PlottableCloud3D(const std::string&,
                   const AIDA::ICloud3D*,
                   INotifier*,bool);
  virtual ~PlottableCloud3D();
private:
  const AIDA::ICloud3D* fData;
};

class PlottableDataPointSet 
:public virtual SbPlottableBins1D {
  PLOTTABLE_HEADER
public: //SbPlottableBins1D
  virtual void getBinsSumOfWeightsRange(float&,float&) const;
  virtual int getAxisNumberOfBins() const;
  virtual float getAxisMinimum() const;
  virtual float getAxisMaximum() const;
  virtual float getBinLowerEdge(int) const;
  virtual float getBinUpperEdge(int) const;
  virtual int getBinNumberOfEntries(int) const;
  virtual float getBinSumOfWeights(int) const;
  virtual float getBinBar(int) const;
public:
  PlottableDataPointSet(const std::string&,
                        const AIDA::IDataPointSet*,
                        INotifier*,bool);
  virtual ~PlottableDataPointSet();
private:
  const AIDA::IDataPointSet* fData;
  int fCoordinate;
  int fRep;
  // Retain mode :
  int f_size;
  std::vector<double> f_xs;
  std::vector<double> f_xms;
  std::vector<double> f_xps;
  std::vector<double> f_vs;
  std::vector<double> f_vms;
  std::vector<double> f_vps;
};

}

#endif

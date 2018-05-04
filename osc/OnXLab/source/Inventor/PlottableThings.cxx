// this :
#include <OnXLab/Inventor/PlottableThings.h>

#include <HEPVis/SbPlottableThings.h>

#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/IProfile1D.h>
#include <AIDA/IProfile2D.h>
#include <AIDA/IAxis.h>
#include <AIDA/ITree.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>
#include <AIDA/IFunction.h>
#include <AIDA/IDataPointSet.h>
#include <AIDA/IDataPoint.h>
#include <AIDA/IMeasurement.h>
#include <AIDA/IAnnotation.h>

#include <inlib/math>
#include <cmath>

#include <inlib/cast>
#include <Lib/Debug.h>

#include <BatchLab/Core/Function.h>

#include <string.h>

#define MINIMUM(a,b) ((a)<(b)?a:b)
#define MAXIMUM(a,b) ((a)>(b)?a:b)

#define PLOTTABLE_SOURCE(a_class) \
  OnXLab::a_class::~a_class(){\
    if(fRetain) delete f_annotation;\
    delete fNotifier;\
    Lib::Debug::decrement(std::string("OnXLab::")+std::string(#a_class));\
  }\
  /*void* OnXLab::a_class::nativeObject() const{return (void*)fData;}*/\
  bool OnXLab::a_class::isValid() const{\
    if(fRetain) return true;\
    if(!fNotifier) return false;\
    return fNotifier->isValid();\
  }\
  const char* OnXLab::a_class::getName(){return fName.c_str();}\
  const char* OnXLab::a_class::getLabel(){return fTitle.c_str();}\
  const char* OnXLab::a_class::getLegend(){\
    fLegend = f_annotation->value("Legend");\
    return fLegend.c_str();\
  }

#define INIT_CSTOR \
:fName(aName)\
,fTitle(aData->title())\
,fNotifier(aNotifier)\
,f_annotation(0)\
,fRetain(aRetain)\
,f_lf("\n")\
,fData(aData)

#define ANNOT_SOURCE \
  if(fRetain) {\
    f_annotation = new BatchLab::Annotation();\
    int number = fData->annotation().size();\
    for(int index=0;index<number;index++) {\
      f_annotation->addItem(fData->annotation().key(index),\
                            fData->annotation().value(index));\
    }\
  } else {\
    f_annotation = (AIDA::IAnnotation*)&(fData->annotation());\
  }

#define IF_NAME \
    if((*it)=="name") {\
      if(fInfos.size()) fInfos += f_lf;\
      fInfos += "Name\n"+fName;

#define ELSE_IF_SB_PLOTTER \
    } else if((*it)==Sb_plotter) {\
      std::string prefix("plotter.");\
      unsigned int lpfx = prefix.size();\
      int number = f_annotation->size();\
      for(int index=0;index<number;index++) {\
        std::string key = f_annotation->key(index);\
        if(key.substr(0,lpfx)==prefix) {\
          std::string s = key.substr(lpfx,key.size()-lpfx);\
          if(fInfos.size()) fInfos += f_lf;\
          /*NOTE : space and not lf to match SbStyle string format.*/\
          fInfos += s+" "+f_annotation->value(index);\
        }\
      }

#define L_ANN 11
#define ELSE_IF_SB_ANNOTATION \
    } else if((*it).substr(0,L_ANN)=="annotation.") {\
      std::string s = (*it).substr(L_ANN,(*it).size()-L_ANN);\
      int number = f_annotation->size();\
      for(int index=0;index<number;index++) {\
        if(s==f_annotation->key(index)) {\
          if(fInfos.size()) fInfos += f_lf;\
          fInfos += s+f_lf+f_annotation->value(index);\
          break;\
        }\
      }\
    } else if((*it).substr(0,L_ANN)=="annotation=") {\
      std::string s = (*it).substr(L_ANN,(*it).size()-L_ANN);\
      int number = f_annotation->size();\
      for(int index=0;index<number;index++) {\
        if(s==f_annotation->key(index)) {\
          if(fInfos.size()) fInfos += f_lf;\
          fInfos += f_annotation->value(index);\
          break;\
        }\
      }

#define END_IF \
    }

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableHistogram1D::PlottableHistogram1D(
 const std::string& aName
,const AIDA::IHistogram1D* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
// Retain :
,f_minBinHeight(0)
,f_maxBinHeight(0)
,f_axis_bins(0)
,f_axis_lowerEdge(0)
,f_axis_upperEdge(0)
,f_allEntries(0)
,f_mean(0)
,f_rms(0)
,f_under(0)
,f_over(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableHistogram1D");

  ANNOT_SOURCE

  if(fRetain) { //PAW
    //FIXME : in case of thread, the whole below should be
    //        in between lock/unlock !
    f_minBinHeight = fData->minBinHeight();
    f_maxBinHeight = fData->maxBinHeight();
    //axis:
    f_axis_bins = fData->axis().bins();
    f_axis_lowerEdge = fData->axis().lowerEdge();
    f_axis_upperEdge = fData->axis().upperEdge();
    f_axis_binLowerEdge.resize(f_axis_bins);
    f_axis_binUpperEdge.resize(f_axis_bins);
   {for(int i=0;i<f_axis_bins;i++) {
      f_axis_binLowerEdge[i] = fData->axis().binLowerEdge(i);
      f_axis_binUpperEdge[i] = fData->axis().binUpperEdge(i);
    }}
    //bins:
    int n = f_axis_bins;
    f_binEntries.resize(n);
    f_binHeight.resize(n);
    f_binError.resize(n);
   {for(int i=0;i<n;i++) {
      f_binEntries[i] = fData->binEntries(i);
      f_binHeight[i] = fData->binHeight(i);
      f_binError[i] = fData->binError(i);
    }}
    //infos:
    f_allEntries = fData->allEntries();
    f_mean = fData->mean();
    f_rms = fData->rms();
    f_under = fData->binHeight(AIDA::IAxis::UNDERFLOW_BIN);
    f_over = fData->binHeight(AIDA::IAxis::OVERFLOW_BIN);
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  }
}

int OnXLab::PlottableHistogram1D::getDimension() const{return 1;}

void* OnXLab::PlottableHistogram1D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins1D_s)) {
    return INLIB_SCAST(SbPlottableBins1D);
  } else if(fData) { //used by Accessors.cxx
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableHistogram1D)

const char* OnXLab::PlottableHistogram1D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;


  // See BatchLab::BaseFitter::setFitInfos for the annotation setup.
  bool show_fit_errors = false;
  for(it=words.begin();it!=words.end();++it) {
    if((*it)=="fit_errors") {show_fit_errors = true;break;}
  }
  bool show_fit_ndf = false;
  for(it=words.begin();it!=words.end();++it) {
    if((*it)=="fit_ndf") {show_fit_ndf = true;break;}
  }

  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>
        (fData ? fData->allEntries() : f_allEntries);

    } else if((*it)=="mean") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Mean\n"+inlib::to<double>(fData ? fData->mean() : f_mean);

    } else if((*it)=="rms") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "RMS\n"+inlib::to<double>(fData ? fData->rms() : f_rms);

    } else if((*it)=="overflow") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "UDFLW\n"+inlib::to<double>
        (fData ? fData->binHeight(AIDA::IAxis::UNDERFLOW_BIN) : f_under);

    } else if((*it)=="underflow") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "OVFLW\n"+inlib::to<double>
        (fData?fData->binHeight(AIDA::IAxis::OVERFLOW_BIN):f_over);

    } else if((*it)=="fit_quality") {

      int ndf = 0;
      inlib::to<int>(f_annotation->value("fit.ndf"),ndf);

      double quality = 0;
      inlib::to<double>(f_annotation->value("fit.quality"),quality);

      double chi2 = quality * ndf;
      //Need PAW encoding rendering for chi2 :
      if(show_fit_ndf) {
        if(fInfos.size()) fInfos += f_lf;
        fInfos += "[h]^2! / ndf\n"+ 
	  inlib::to<double>(chi2)+" / "+inlib::to<int>(ndf);
      } else {
        if(fInfos.size()) fInfos += f_lf;
        fInfos += "[h]^2!\n"+inlib::to<double>(chi2);
      }

    } else if((*it)=="fit_parameters") {

      int number = f_annotation->size();
      for(int index=0;index<number;index++) {
        std::string key = f_annotation->key(index);
        if(key.substr(0,10)=="fit.param.") { 
          std::string name = key.substr(10,key.size()-10);
          std::vector<std::string> words;
          inlib::words(f_annotation->value(index)," ",false,words);
          if(words.size()==2) { //value and error.
            double v;
            inlib::to<double>(words[0],v);
            double e;
            inlib::to<double>(words[1],e);
            if(show_fit_errors) {
              //Need PAW encoding rendering for +/- :
              if(fInfos.size()) fInfos += f_lf;
              fInfos += name+f_lf+
                inlib::to<double>(v)+" +&_ "+inlib::to<double>(e);
            } else {
              if(fInfos.size()) fInfos += f_lf;
              fInfos += name+f_lf+inlib::to<double>(v);
            }
          }
        }
      }

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab::PlottableHistogram1D::getBinsSumOfWeightsRange(
 float& aMin
,float& aMax
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fData) {
    aMin = (float)fData->minBinHeight();
    aMax = (float)fData->maxBinHeight();
  } else {
    aMin = (float)f_minBinHeight;
    aMax = (float)f_maxBinHeight;
  }
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableHistogram1D::getAxisNumberOfBins(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? fData->axis().bins() : f_axis_bins;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getAxisMinimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->axis().lowerEdge() : (float)f_axis_lowerEdge;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getAxisMaximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->axis().upperEdge() : (float)f_axis_upperEdge;
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableHistogram1D::getBinNumberOfEntries(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? fData->binEntries(aI) : f_binEntries[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getBinLowerEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->axis().binLowerEdge(aI) : 
                 (float)f_axis_binLowerEdge[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getBinUpperEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->axis().binUpperEdge(aI) : 
                 (float)f_axis_binUpperEdge[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getBinSumOfWeights(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->binHeight(aI) : (float)f_binHeight[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram1D::getBinBar(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->binError(aI) : (float)f_binError[aI];
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableHistogram2D::PlottableHistogram2D(
 const std::string& aName
,const AIDA::IHistogram2D* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
// Retain :
,f_minBinHeight(0)
,f_maxBinHeight(0)
,f_xAxis_bins(0)
,f_xAxis_lowerEdge(0)
,f_xAxis_upperEdge(0)
,f_yAxis_bins(0)
,f_yAxis_lowerEdge(0)
,f_yAxis_upperEdge(0)
,f_allEntries(0)
,f_meanX(0)
,f_meanY(0)
,f_rmsX(0)
,f_rmsY(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableHistogram2D");

  ANNOT_SOURCE

  if(fRetain) { //PAW
    //FIXME : in case of thread, the whole below should be
    //        in between lock/unlock !
    f_minBinHeight = fData->minBinHeight();
    f_maxBinHeight = fData->maxBinHeight();
    //axes:
    f_xAxis_bins = fData->xAxis().bins();
    f_xAxis_lowerEdge = fData->xAxis().lowerEdge();
    f_xAxis_upperEdge = fData->xAxis().upperEdge();
    f_xAxis_binLowerEdge.resize(f_xAxis_bins);
    f_xAxis_binUpperEdge.resize(f_xAxis_bins);
   {for(int i=0;i<f_xAxis_bins;i++) {
      f_xAxis_binLowerEdge[i] = fData->xAxis().binLowerEdge(i);
      f_xAxis_binUpperEdge[i] = fData->xAxis().binUpperEdge(i);
    }}
    f_yAxis_bins = fData->yAxis().bins();
    f_yAxis_lowerEdge = fData->yAxis().lowerEdge();
    f_yAxis_upperEdge = fData->yAxis().upperEdge();
    f_yAxis_binLowerEdge.resize(f_yAxis_bins);
    f_yAxis_binUpperEdge.resize(f_yAxis_bins);
   {for(int j=0;j<f_yAxis_bins;j++) {
      f_yAxis_binLowerEdge[j] = fData->yAxis().binLowerEdge(j);
      f_yAxis_binUpperEdge[j] = fData->yAxis().binUpperEdge(j);
    }}
    //bins:
    int n = f_xAxis_bins * f_yAxis_bins;
    f_binEntries.resize(n);
    f_binHeight.resize(n);
    f_binError.resize(n);
   {for(int j=0;j<f_yAxis_bins;j++) {
      for(int i=0;i<f_xAxis_bins;i++) {
        int off = i + j * f_xAxis_bins;
        f_binEntries[off] = fData->binEntries(i,j);
        f_binHeight[off] = fData->binHeight(i,j);
        f_binError[off] = fData->binError(i,j);
      }
    }}
    //infos:
    f_allEntries = fData->allEntries();
    f_meanX = fData->meanX();
    f_meanY = fData->meanY();
    f_rmsX = fData->rmsX();
    f_rmsY = fData->rmsY();
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  }
}

int OnXLab::PlottableHistogram2D::getDimension()const{return 2;}

void* OnXLab::PlottableHistogram2D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins2D_s)) {
    return INLIB_SCAST(SbPlottableBins2D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableHistogram2D)

const char* OnXLab::PlottableHistogram2D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>
        (fData?fData->allEntries():f_allEntries);
    } else if((*it)=="mean") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "MeanX\n"+inlib::to<double>(fData?fData->meanX():f_meanX);
      fInfos += f_lf;
      fInfos += "MeanY\n"+inlib::to<double>(fData?fData->meanY():f_meanY);
    } else if((*it)=="rms") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "RMS X\n"+inlib::to<double>(fData?fData->rmsX():f_rmsX);
      fInfos += f_lf;
      fInfos += "RMS Y\n"+inlib::to<double>(fData?fData->rmsY():f_rmsY);

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab::PlottableHistogram2D::getBinsSumOfWeightsRange(
 float& aMin
,float& aMax
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fData) {
    aMin = (float)fData->minBinHeight();
    aMax = (float)fData->maxBinHeight();
  } else {
    aMin = (float)f_minBinHeight;
    aMax = (float)f_maxBinHeight;
  }
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableHistogram2D::getAxisNumberOfBinsX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? fData->xAxis().bins() : f_xAxis_bins;
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableHistogram2D::getAxisNumberOfBinsY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? fData->yAxis().bins() : f_yAxis_bins;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getAxisMinimumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->xAxis().lowerEdge() : 
                 (float)f_xAxis_lowerEdge;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getAxisMinimumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->yAxis().lowerEdge() :
                 (float)f_yAxis_lowerEdge;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getAxisMaximumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->xAxis().upperEdge() :
                 (float)f_xAxis_upperEdge;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getAxisMaximumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->yAxis().upperEdge() : 
                 (float)f_yAxis_upperEdge;
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableHistogram2D::getBinNumberOfEntries(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? fData->binEntries(aI,aJ) : 
                 f_binEntries[aI + aJ * f_xAxis_bins];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinLowerEdgeX(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->xAxis().binLowerEdge(aI) :
                 (float)f_xAxis_binLowerEdge[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinUpperEdgeX(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->xAxis().binUpperEdge(aI) :
                 (float)f_xAxis_binUpperEdge[aI];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinLowerEdgeY(
 int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->yAxis().binLowerEdge(aJ) :
                 (float)f_yAxis_binLowerEdge[aJ];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinUpperEdgeY(
 int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->yAxis().binUpperEdge(aJ) :
                 (float)f_yAxis_binUpperEdge[aJ];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinSumOfWeights(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->binHeight(aI,aJ) :
                 (float)f_binHeight[aI + aJ * f_xAxis_bins];
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableHistogram2D::getBinBar(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData ? (float)fData->binError(aI,aJ) :
                 (float)f_binError[aI + aJ * f_xAxis_bins];
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableProfile1D::PlottableProfile1D(
 const std::string& aName
,const AIDA::IProfile1D* aData
,INotifier* aNotifier
,bool aRetain //FIXME
)
INIT_CSTOR
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableProfile1D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
}

int OnXLab::PlottableProfile1D::getDimension()const{return 1;}

void* OnXLab::PlottableProfile1D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins1D_s)) {
    return INLIB_SCAST(SbPlottableBins1D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableProfile1D)

const char* OnXLab::PlottableProfile1D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData->allEntries());
    } else if((*it)=="mean") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Mean\n"+inlib::to<double>(fData->mean());
    } else if((*it)=="rms") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "RMS\n"+inlib::to<double>(fData->rms());

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableProfile1D::getAxisNumberOfBins(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->axis().bins();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getAxisMinimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->axis().lowerEdge();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getAxisMaximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->axis().upperEdge();
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab::PlottableProfile1D::getBinsSumOfWeightsRange(
 float& aMin
,float& aMax
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aMin = (float)fData->minBinHeight();
  aMax = (float)fData->maxBinHeight();
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableProfile1D::getBinNumberOfEntries(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->binEntries(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getBinLowerEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->axis().binLowerEdge(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getBinUpperEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->axis().binUpperEdge(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getBinSumOfWeights(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->binHeight(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile1D::getBinBar(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->binError(aI);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableProfile2D::PlottableProfile2D(
 const std::string& aName
,const AIDA::IProfile2D* aData
,INotifier* aNotifier
,bool aRetain //FIXME
)
INIT_CSTOR
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableProfile2D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
}

int OnXLab::PlottableProfile2D::getDimension()const{return 2;}

void* OnXLab::PlottableProfile2D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins2D_s)) {
    return INLIB_SCAST(SbPlottableBins2D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableProfile2D)

const char* OnXLab::PlottableProfile2D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData->allEntries());
    } else if((*it)=="mean") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "MeanX\n"+inlib::to<double>(fData->meanX());
      fInfos += f_lf;
      fInfos += "MeanY\n"+inlib::to<double>(fData->meanY());
    } else if((*it)=="rms") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "RMS X\n"+inlib::to<double>(fData->rmsX());
      fInfos += f_lf;
      fInfos += "RMS Y\n"+inlib::to<double>(fData->rmsY());

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

//////////////////////////////////////////////////////////////////////////////
void OnXLab::PlottableProfile2D::getBinsSumOfWeightsRange(
 float& aMin
,float& aMax
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aMin = (float)fData->minBinHeight();
  aMax = (float)fData->maxBinHeight();
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableProfile2D::getAxisNumberOfBinsX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->xAxis().bins();
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableProfile2D::getAxisNumberOfBinsY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->yAxis().bins();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getAxisMinimumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->xAxis().lowerEdge();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getAxisMinimumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->yAxis().lowerEdge();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getAxisMaximumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->xAxis().upperEdge();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getAxisMaximumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->yAxis().upperEdge();
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableProfile2D::getBinNumberOfEntries(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->binEntries(aI,aJ);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinLowerEdgeX(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->xAxis().binLowerEdge(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinUpperEdgeX(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->xAxis().binUpperEdge(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinLowerEdgeY(
 int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->yAxis().binLowerEdge(aJ);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinUpperEdgeY(
 int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->yAxis().binUpperEdge(aJ);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinSumOfWeights(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->binHeight(aI,aJ);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableProfile2D::getBinBar(
 int aI
,int aJ
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->binError(aI,aJ);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableFunction1D::PlottableFunction1D(
 const std::string& aName
,const AIDA::IFunction* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
,fBaseFunction(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableFunction1D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
  fBaseFunction = INLIB_CONST_CAST(*fData,BatchLab::BaseFunction);
}

int OnXLab::PlottableFunction1D::getDimension()const{return 1;}

void* OnXLab::PlottableFunction1D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableFunction1D_s)) {
    return INLIB_SCAST(SbPlottableFunction1D);
  } else { // special usage : (see Accessor.cxx)
    if(!::strncmp(aClass,Sb_data,4) && fData) { //data::<class> format.
      return (void*)fData->cast(aClass+4+2);
    }
  }
  return 0;
}

PLOTTABLE_SOURCE(PlottableFunction1D)

const char* OnXLab::PlottableFunction1D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME
    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

//////////////////////////////////////////////////////////////////////////////
bool OnXLab::PlottableFunction1D::value(
 float aX
,float& aValue
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<double> xs(1);
  xs[0] = (double)aX;
  if(fBaseFunction) {
    double value;
    if(!fBaseFunction->value(xs,value)) {
      aValue = 0;
      return false;
    }
    aValue = float(value);
    return true;
  } else if(fData) {
    aValue = (float)fData->value(xs); //FIXME : throw in case of problem.
    return true;
  }
  return false;
}

#define RET_ANNOT_INT(a_key) \
  std::string s = f_annotation->value(a_key);\
  if(s.size()) {int v;if(inlib::to<int>(s,v)) {return v;}}

#define RET_ANNOT_FLOAT(a_key) \
  std::string s = f_annotation->value(a_key);\
  if(s.size()) {float v;if(inlib::to<float>(s,v)) {return v;}}

// Used by OpenPAW to fix the extend of the function really drawn.
int OnXLab::PlottableFunction1D::getNumberOfStepsX() const{
  RET_ANNOT_INT("domain.xAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableFunction1D::getMinimumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMinimum")
  return 0;
}
float OnXLab::PlottableFunction1D::getMaximumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMaximum");
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableFunction2D::PlottableFunction2D(
 const std::string& aName
,const AIDA::IFunction* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
,fBaseFunction(0)
{
  Lib::Debug::increment("OnXLab::PlottableFunction2D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
  //if(aRetain) {
  //  //FIXME : the aFunction may not be copiable.
  //  const Slash::Store::IObject* aobj = 
  //    INLIB_CONST_CAST(*aFunction,Slash::Store::IObject);
  //  Slash::Store::IObject* obj = aobj->copy();
  //  fData = INLIB_CONST_CAST(*obj,AIDA::IFunction);
  //}
  fBaseFunction = INLIB_CONST_CAST(*fData,BatchLab::BaseFunction);
}

int OnXLab::PlottableFunction2D::getDimension()const{return 2;}

void* OnXLab::PlottableFunction2D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableFunction2D_s)) {
    return INLIB_SCAST(SbPlottableFunction2D);
  } else { // special usage : (see Accessor.cxx)
    if(!::strncmp(aClass,Sb_data,4) && fData) { //data::<class> format.
      return (void*)fData->cast(aClass+4+2);
    }
  }
  return 0;
}

PLOTTABLE_SOURCE(PlottableFunction2D)

const char* OnXLab::PlottableFunction2D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME
    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

bool OnXLab::PlottableFunction2D::value(float aX,float aY,float& aValue) const{
  std::vector<double> xs(2);
  xs[0] = (double)aX;
  xs[1] = (double)aY;
  if(fBaseFunction) {
    double value;
    if(!fBaseFunction->value(xs,value)) {
      aValue = 0;
      return false;
    }
    aValue = float(value);
    return true;
  } else if(fData) {
    aValue = (float)fData->value(xs);  //FIXME : throw in case of problem.
    return true;
  }
  return false;
}

// Used by OpenPAW to fix the extend of the function really drawn.
int OnXLab::PlottableFunction2D::getNumberOfStepsX() const{
  RET_ANNOT_INT("domain.xAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableFunction2D::getMinimumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMinimum")
  return 0;
}
float OnXLab::PlottableFunction2D::getMaximumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMaximum");
  return 0;
}
int OnXLab::PlottableFunction2D::getNumberOfStepsY() const{
  RET_ANNOT_INT("domain.yAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableFunction2D::getMinimumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMinimum")
  return 0;
}
float OnXLab::PlottableFunction2D::getMaximumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMaximum");
  return 0;
}

//modeling polygon
int OnXLab::PlottableFunction2D::getNumberOfPoints() const{return 0;}

void OnXLab::PlottableFunction2D::getIthPoint(int aIndex,float& aX,float& aY,bool& aIsControl) const {aX = aY = 0;aIsControl = false;}
bool OnXLab::PlottableFunction2D::setIthPoint(int aIndex,float aX,float aY){return false;}
bool OnXLab::PlottableFunction2D::dragger_update_points() const{return false;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableInsidePolygon::PlottableInsidePolygon(
 const std::string& aName
,const AIDA::IFunction* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
,fBaseFunction(0)
// Retain :
,f_entries(0)
,f_lowerEdgeX(0)
,f_upperEdgeX(0)
,f_lowerEdgeY(0)
,f_upperEdgeY(0)
{
  Lib::Debug::increment("OnXLab::PlottableInsidePolygon");

  ANNOT_SOURCE

  if(fRetain) { //PAW
    //FIXME : in case of thread, the whole below should be
    //        in between lock/unlock !
    const std::vector<double>& params = aData->parameters();
    f_entries = params.size()/2;
    //if(f_entries*2!=params.size()) //throw
    if(f_entries) {
      f_lowerEdgeX = params[0];
      f_upperEdgeX = params[0];
      f_lowerEdgeY = params[1];
      f_upperEdgeY = params[1];
    } else {
      f_lowerEdgeX = 0;
      f_upperEdgeX = 0;
      f_lowerEdgeY = 0;
      f_upperEdgeY = 0;
    }
    f_points.resize(f_entries);
    for(unsigned int index=0;index<f_entries;index++) {
      double x = params[2*index];
      double y = params[2*index+1];
      f_lowerEdgeX = MINIMUM(x,f_lowerEdgeX);
      f_upperEdgeX = MAXIMUM(x,f_upperEdgeX);
      f_lowerEdgeY = MINIMUM(y,f_lowerEdgeY);
      f_upperEdgeY = MAXIMUM(y,f_upperEdgeY);
      f_points[index] = Point(x,y);
    }
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  } else {
    fBaseFunction = INLIB_CONST_CAST(*fData,BatchLab::BaseFunction);
  }
}

int OnXLab::PlottableInsidePolygon::getDimension()const{return 2;}

void* OnXLab::PlottableInsidePolygon::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableFunction2D_s)) {
    return INLIB_SCAST(SbPlottableFunction2D);
  } else { // special usage : (see Accessor.cxx)
    if(!::strncmp(aClass,Sb_data,4) && fData) { //data::<class> format.
      return (void*)fData->cast(aClass+4+2);
    }
  }
  return 0;
}

PLOTTABLE_SOURCE(PlottableInsidePolygon)

const char* OnXLab::PlottableInsidePolygon::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME
    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

bool OnXLab::PlottableInsidePolygon::value(float aX,float aY,float& aValue) const{
  if(fBaseFunction) {
    std::vector<double> xs(2);
    xs[0] = (double)aX;
    xs[1] = (double)aY;
    double value;
    if(!fBaseFunction->value(xs,value)) {
      aValue = 0;
      return false;
    }
    aValue = float(value);
    return true;
  } else if(fData) {
    std::vector<double> xs(2);
    xs[0] = (double)aX;
    xs[1] = (double)aY;
    aValue = (float)fData->value(xs);  //FIXME : throw in case of problem.
    return true;
  } else {
    //FIXME : optimize.
    inlib::func::in_polygon f;
    f.set(f_points);
    aValue = (float)f.value(Point(aX,aY));
    return true;
  }
  return false;
}

// Used by OpenPAW to fix the extend of the function really drawn.
int OnXLab::PlottableInsidePolygon::getNumberOfStepsX() const{
  RET_ANNOT_INT("domain.xAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableInsidePolygon::getMinimumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMinimum")
  return 0;
}
float OnXLab::PlottableInsidePolygon::getMaximumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMaximum");
  return 0;
}
int OnXLab::PlottableInsidePolygon::getNumberOfStepsY() const{
  RET_ANNOT_INT("domain.yAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableInsidePolygon::getMinimumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMinimum")
  return 0;
}
float OnXLab::PlottableInsidePolygon::getMaximumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMaximum");
  return 0;
}

// modeling polygon :
int OnXLab::PlottableInsidePolygon::getNumberOfPoints() const{
  if(fData) {
    const std::vector<double>& params = fData->parameters();
    return params.size()/2;
  } else {
    return f_entries;
  }
}

void OnXLab::PlottableInsidePolygon::getIthPoint(int aIndex,float& aX,float& aY,bool& aIsControl) const {
  aIsControl = true;
  if(fData) {
    const std::vector<double>& params = fData->parameters();
    aX = (float)params[2*aIndex];
    aY = (float)params[2*aIndex+1];
  } else {
    aX = (float)f_points[aIndex].first;
    aY = (float)f_points[aIndex].second;
  }
}
bool OnXLab::PlottableInsidePolygon::setIthPoint(int aIndex,float aX,float aY){
  if(fData) {
    std::vector<double> params = fData->parameters();
    params[2*aIndex] = aX;
    params[2*aIndex+1] = aY;
    const_cast<AIDA::IFunction*>(fData)->setParameters(params);
  } else {
    f_points[aIndex] = Point(aX,aY);
  }
  return true;
}
bool OnXLab::PlottableInsidePolygon::dragger_update_points() const{return false;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableInsideEllipse::PlottableInsideEllipse(
 const std::string& aName
,const AIDA::IFunction* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
,fBaseFunction(0)
,fNumber(4*25)
,f_a(1)
,f_b(1)
{
  Lib::Debug::increment("OnXLab::PlottableInsideEllipse");

  ANNOT_SOURCE

  const std::vector<double>& params = aData->parameters();
  if(params.size()!=2) {
    //throw
  } else {
    f_a = params[0];
    f_b = params[1];
  }
  if(fRetain) { //PAW
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  } else {
    fBaseFunction = INLIB_CONST_CAST(*fData,BatchLab::BaseFunction);
  }
}

int OnXLab::PlottableInsideEllipse::getDimension()const{return 2;}

void* OnXLab::PlottableInsideEllipse::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableFunction2D_s)) {
    return INLIB_SCAST(SbPlottableFunction2D);
  } else { // special usage : (see Accessor.cxx)
    if(!::strncmp(aClass,Sb_data,4) && fData) { //data::<class> format.
      return (void*)fData->cast(aClass+4+2);
    }
  }
  return 0;
}

PLOTTABLE_SOURCE(PlottableInsideEllipse)

const char* OnXLab::PlottableInsideEllipse::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME
    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

bool OnXLab::PlottableInsideEllipse::value(float aX,float aY,float& aValue) const{
  if(fBaseFunction) {
    std::vector<double> xs(2);
    xs[0] = (double)aX;
    xs[1] = (double)aY;
    double value;
    if(!fBaseFunction->value(xs,value)) {
      aValue = 0;
      return false;
    }
    aValue = float(value);
    return true;
  } else if(fData) {
    std::vector<double> xs(2);
    xs[0] = (double)aX;
    xs[1] = (double)aY;
    aValue = (float)fData->value(xs);  //FIXME : throw in case of problem.
    return true;
  } else { //retain
    inlib::func::in_ellipse f;
    f.set_from_a_b(f_a,f_b);
    aValue = (float)f.value(aX,aY);
    return true;
  }
  return false;
}

// Used by OpenPAW to fix the extend of the function really drawn.
int OnXLab::PlottableInsideEllipse::getNumberOfStepsX() const{
  RET_ANNOT_INT("domain.xAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableInsideEllipse::getMinimumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMinimum")
  return 0;
  //return -f_a;
}
float OnXLab::PlottableInsideEllipse::getMaximumX() const{
  RET_ANNOT_FLOAT("domain.xAxisMaximum");
  return 0;
  //return f_a;
}
int OnXLab::PlottableInsideEllipse::getNumberOfStepsY() const{
  RET_ANNOT_INT("domain.yAxisNumberOfSteps")
  return 100;
}
float OnXLab::PlottableInsideEllipse::getMinimumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMinimum")
  return 0;
  //return -f_b;
}
float OnXLab::PlottableInsideEllipse::getMaximumY() const{
  RET_ANNOT_FLOAT("domain.yAxisMaximum");
  return 0;
  //return f_b;
}

// modeling polygon :
int OnXLab::PlottableInsideEllipse::getNumberOfPoints() const{return fNumber;}

void OnXLab::PlottableInsideEllipse::getIthPoint(int aIndex,float& aX,float& aY,bool& aIsControl) const {
  //
  //use the polar form :
  //  cs = cos(theta)
  //  sn = sin(theta)
  //  r = 1/sqrt(cs*cs/(a*a)+sn*sn/(b*b))

  if(aIndex==0) {aIsControl = true;}
  else if(aIndex==(fNumber/4)) {aIsControl = true;}
  else {aIsControl = false;}

  double angle = double(aIndex)*2.0*inlib::pi()/double(fNumber);

  aX = float(f_a*::cos(angle));
  aY = float(f_b*::sin(angle));

}
bool OnXLab::PlottableInsideEllipse::setIthPoint(int aIndex,float aX,float aY){
  if(aIndex==0) {
    if(aX<=0) return false;
    if(fData) const_cast<AIDA::IFunction*>(fData)->setParameter("a",aX);
    f_a = aX;
  } else if(aIndex==(fNumber/4)) {
    if(aY<=0) return false;
    if(fData) const_cast<AIDA::IFunction*>(fData)->setParameter("b",aY);
    f_b = aY;
  }
  return true;
}
bool OnXLab::PlottableInsideEllipse::dragger_update_points() const{return true;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//FIXME : two ways to display Cloud1D (x,w) or (count,x) (assuming w = 1)
// For the moment, we display (count,x). The xAxis is in [1,n+1]
// where n is the number of entries of the Cloud.
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableCloud1D::PlottableCloud1D(
 const std::string& aName
,const AIDA::ICloud1D* aData
,INotifier* aNotifier
,bool aRetain //FIXME
)
INIT_CSTOR
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableCloud1D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
}

int OnXLab::PlottableCloud1D::getDimension()const{return 1;}

void* OnXLab::PlottableCloud1D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins1D_s)) {
    return INLIB_SCAST(SbPlottableBins1D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableCloud1D)

const char* OnXLab::PlottableCloud1D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData->entries());

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}
//////////////////////////////////////////////////////////////////////////////
void OnXLab::PlottableCloud1D::getBinsSumOfWeightsRange(
 float& aMin
,float& aMax
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aMin = (float)fData->lowerEdge();
  aMax = (float)fData->upperEdge();
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableCloud1D::getAxisNumberOfBins(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData->entries();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getAxisMinimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)1;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getAxisMaximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->entries()+1;
}
//////////////////////////////////////////////////////////////////////////////
int OnXLab::PlottableCloud1D::getBinNumberOfEntries(
 int //aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 1;
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getBinLowerEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(aI+1);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getBinUpperEdge(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(aI+2);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getBinSumOfWeights(
 int aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->value(aI);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud1D::getBinBar(
 int //aI
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableCloud2D::PlottableCloud2D(
 const std::string& aName
,const AIDA::ICloud2D* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
// Retain :
,f_entries(0)
,f_lowerEdgeX(0)
,f_upperEdgeX(0)
,f_lowerEdgeY(0)
,f_upperEdgeY(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableCloud2D");

  ANNOT_SOURCE

  if(fRetain) { //PAW
    //FIXME : in case of thread, the whole below should be
    //        in between lock/unlock !
    f_entries = fData->entries();
    f_lowerEdgeX = fData->lowerEdgeX();
    f_lowerEdgeY = fData->lowerEdgeY();
    f_upperEdgeX = fData->upperEdgeX();
    f_upperEdgeY = fData->upperEdgeY();
    f_valueX.resize(f_entries);
    f_valueY.resize(f_entries);
    for(int i=0;i<f_entries;i++) {
      f_valueX[i] = fData->valueX(i);
      f_valueY[i] = fData->valueY(i);
    }
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  }

}

int OnXLab::PlottableCloud2D::getDimension()const{return 2;}

void* OnXLab::PlottableCloud2D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottablePoints2D_s)) {
    return INLIB_SCAST(SbPlottablePoints2D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableCloud2D)

const char* OnXLab::PlottableCloud2D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData?fData->entries():f_entries);

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud2D::getAxisMinimumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(fData?fData->lowerEdgeX():f_lowerEdgeX);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud2D::getAxisMinimumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(fData?fData->lowerEdgeY():f_lowerEdgeY);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud2D::getAxisMaximumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(fData?fData->upperEdgeX():f_upperEdgeX);
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud2D::getAxisMaximumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)(fData?fData->upperEdgeY():f_upperEdgeY);
}

int OnXLab::PlottableCloud2D::getNumberOfPoints() const{return (fData?fData->entries():f_entries);}

void OnXLab::PlottableCloud2D::getIthPoint(int aIndex,float& aX,float& aY) const{
  if(fData) {
    aX = (float)fData->valueX(aIndex);
    aY = (float)fData->valueY(aIndex);
  } else {
    aX = (float)f_valueX[aIndex];
    aY = (float)f_valueY[aIndex];
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlottableCloud3D::PlottableCloud3D(
 const std::string& aName
,const AIDA::ICloud3D* aData
,INotifier* aNotifier
,bool aRetain //FIXME
)
INIT_CSTOR
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("OnXLab::PlottableCloud3D");
  ANNOT_SOURCE
  fRetain = false; //FIXME
}

int OnXLab::PlottableCloud3D::getDimension()const{return 3;}

void* OnXLab::PlottableCloud3D::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottablePoints3D_s)) {
    return INLIB_SCAST(SbPlottablePoints3D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableCloud3D)

const char* OnXLab::PlottableCloud3D::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData->entries());

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMinimumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->lowerEdgeX();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMinimumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->lowerEdgeY();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMinimumZ(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->lowerEdgeZ();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMaximumX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->upperEdgeX();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMaximumY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->upperEdgeY();
}
//////////////////////////////////////////////////////////////////////////////
float OnXLab::PlottableCloud3D::getAxisMaximumZ(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (float)fData->upperEdgeZ();
}

int OnXLab::PlottableCloud3D::getNumberOfPoints() const{return fData->entries();}

void OnXLab::PlottableCloud3D::getIthPoint(int aIndex,float& aX,float& aY,float& aZ) const{
  aX = (float)fData->valueX(aIndex);
  aY = (float)fData->valueY(aIndex);
  aZ = (float)fData->valueZ(aIndex);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

OnXLab::PlottableDataPointSet::PlottableDataPointSet(
 const std::string& aName
,const AIDA::IDataPointSet* aData
,INotifier* aNotifier
,bool aRetain
)
INIT_CSTOR
,fCoordinate(0) // Displayed coordinate.
//,fRep(0)
,fRep(1)
// Retain :
,f_size(0)
{
  // fRep = 0
  //   Expect a DPS of any dimension>0.
  //   X axis will be from [1,dps.size()+1] with dps.size() bins.
  //   Y axis will display the fCoordinate measurement.
  // fRep = 1 (used in OpenPAW /GRAPHICS/HPLOT/ERRORS).
  //   Expect a DPS of dim 2. 
  //   Coordinate 0 will be displayed on plotter X axis.
  //   Coordinate 1 will be displayed on plotter Y axis.
  Lib::Debug::increment("OnXLab::PlottableDataPointSet");

  ANNOT_SOURCE

  if(fData->dimension()!=2) fRep = 0;

  if(fRetain) { //PAW
    //FIXME : in case of thread, the whole below should be
    //        in between lock/unlock !

    f_size = fData->size();
    
    f_vs.resize(f_size);
    f_vms.resize(f_size);
    f_vps.resize(f_size);
    if(fRep==1) {
      f_xs.resize(f_size);
      f_xms.resize(f_size);
      f_xps.resize(f_size);
      for(int index=0;index<f_size;index++) {
        const AIDA::IDataPoint* point = fData->point(index);
        f_xs[index] = point->coordinate(0)->value();
        f_xms[index] = point->coordinate(0)->errorMinus();
        f_xps[index] = point->coordinate(0)->errorPlus();

        f_vs[index] = point->coordinate(1)->value();
        f_vms[index] = point->coordinate(1)->errorMinus();
        f_vps[index] = point->coordinate(1)->errorPlus();
      }
    } else {
      for(int index=0;index<f_size;index++) {
        const AIDA::IDataPoint* point = fData->point(index);
        f_vs[index] = point->coordinate(fCoordinate)->value();
        f_vms[index] = point->coordinate(fCoordinate)->errorMinus();
        f_vps[index] = point->coordinate(fCoordinate)->errorPlus();
      }
    }
    //forget data !
    fData = 0;
    delete fNotifier;
    fNotifier = 0;
  }
}

int OnXLab::PlottableDataPointSet::getDimension()const{return 1;}

void* OnXLab::PlottableDataPointSet::cast(const char* aClass) const{
  if(!::strcmp(aClass,SbPlottableObject_s)) {
    return INLIB_SCAST(SbPlottableObject);
  } else if(!::strcmp(aClass,SbPlottableBins1D_s)) {
    return INLIB_SCAST(SbPlottableBins1D);
  } else if(fData) {
    return (void*)fData->cast(aClass);
  } else {
    return 0;
  }
}

PLOTTABLE_SOURCE(PlottableDataPointSet)

const char* OnXLab::PlottableDataPointSet::getInfos(const char* aOptions){
  fInfos.clear();

  std::vector<std::string> words;
  inlib::words(aOptions," ",false,words);
  std::vector<std::string>::const_iterator it;
  for(it=words.begin();it!=words.end();++it) {
    IF_NAME

    } else if((*it)=="entries") {
      if(fInfos.size()) fInfos += f_lf;
      fInfos += "Entries\n"+inlib::to<int>(fData?fData->size():f_size);

    ELSE_IF_SB_PLOTTER
    ELSE_IF_SB_ANNOTATION
    END_IF
  }

  return fInfos.c_str();
}

void OnXLab::PlottableDataPointSet::getBinsSumOfWeightsRange(float& aMin,float& aMax) const {
  aMin = 0;
  aMax = 0;
  if(fData) {
    int number = fData->size();
    if(number<=0) return;
    int coord = (fRep==1?1:fCoordinate);
    float vmin = (float)fData->point(0)->coordinate(coord)->value();
    float vmax = (float)fData->point(0)->coordinate(coord)->value();
    for(int index=1;index<number;index++) {
      float value = (float)fData->point(index)->coordinate(coord)->value();
      vmin = MINIMUM(vmin,value);
      vmax = MAXIMUM(vmax,value);
    }
    aMin = vmin;
    aMax = vmax;
  } else {
    if(f_size<=0) return;
    float vmin = (float)f_vs[0];
    float vmax = vmin;
    for(int index=1;index<f_size;index++) {
      float value = (float)f_vs[index];
      vmin = MINIMUM(vmin,value);
      vmax = MAXIMUM(vmax,value);
    }
    aMin = vmin;
    aMax = vmax;
  }
}

int OnXLab::PlottableDataPointSet::getAxisNumberOfBins() const {
  if(fData) {
    return fData->size();
  } else {
    return f_size;
  }
}

float OnXLab::PlottableDataPointSet::getAxisMinimum() const {
  if(fData) {
    if(fRep==1) {
      int number = fData->size();
      if(number<=0) return 0;

      const AIDA::IDataPoint* point = fData->point(0);
      if(!point) return 0;
      const AIDA::IMeasurement* mes = point->coordinate(0);
      if(!mes) return 0;
      float mn = (float)(mes->value()-mes->errorMinus());

      for(int index=1;index<number;index++) {
        const AIDA::IDataPoint* point = fData->point(index);
        if(!point) return 0;
        const AIDA::IMeasurement* mes = point->coordinate(0);
        if(!mes) return 0;
        mn = MINIMUM(mn,(float)(mes->value()-mes->errorMinus()));
      }

      return mn;
    } else {
      return (float)1;
    }
  } else {
    if(fRep==1) {
      if(!f_size) return 0;
      float mn = float(f_xs[0]-f_xms[0]);
      for(int index=1;index<f_size;index++) {
        mn = MINIMUM(mn,float(f_xs[index]-f_xms[index]));
      }
      return mn;
    } else {
      return (float)1;
    }
  }
}

float OnXLab::PlottableDataPointSet::getAxisMaximum() const {
  if(fData) {
    if(fRep==1) {
      int number = fData->size();
      if(number<=0) return 0;

      const AIDA::IDataPoint* point = fData->point(0);
      if(!point) return 0;
      const AIDA::IMeasurement* mes = point->coordinate(0);
      if(!mes) return 0;
      float mx = (float)(mes->value()+mes->errorPlus());

      for(int index=1;index<number;index++) {
        const AIDA::IDataPoint* point = fData->point(index);
        if(!point) return 0;
        const AIDA::IMeasurement* mes = point->coordinate(0);
        if(!mes) return 0;
        mx = MAXIMUM(mx,(float)(mes->value()+mes->errorPlus()));
      }

      return mx;
    } else {
      return (float)(fData->size()+1);
    }
  } else {
    if(fRep==1) {
      if(!f_size) return 0;
      float mx = float(f_xs[0]+f_xps[0]);
      for(int index=1;index<f_size;index++) {
        mx = MAXIMUM(mx,float(f_xs[index]+f_xps[index]));
      }
      return mx;
    } else {
      return (float)(f_size+1);
    }
  }
}

int OnXLab::PlottableDataPointSet::getBinNumberOfEntries(int) const {
  return 1;
}

float OnXLab::PlottableDataPointSet::getBinLowerEdge(int aI) const { 
  if(fData) {
    if(fRep==1) {
      //NOTE : points may be not ordered according coordinate(0) !
      const AIDA::IDataPoint* point = fData->point(aI);
      if(!point) return 0;
      const AIDA::IMeasurement* mes = point->coordinate(0);
      if(!mes) return 0;
      return (float)(mes->value() - mes->errorMinus());
    } else {
      return (float)(aI+1);
    }
  } else {
    if(fRep==1) {
      return float(f_xs[aI]-f_xms[aI]);
    } else {
      return (float)(aI+1);
    }
  }
}

float OnXLab::PlottableDataPointSet::getBinUpperEdge(int aI) const {
  if(fData) {
    if(fRep==1) {
      //NOTE : points may be not ordered according coordinate(0) !
      const AIDA::IDataPoint* point = fData->point(aI);
      if(!point) return 0;
      const AIDA::IMeasurement* mes = point->coordinate(0);
      if(!mes) return 0;
      return (float)(mes->value() + mes->errorPlus());
    } else {
      return (float)(aI+2);
    }
  } else {
    if(fRep==1) {
      return float(f_xs[aI]+f_xps[aI]);
    } else {
      return (float)(aI+2);
    }
  }
}

float OnXLab::PlottableDataPointSet::getBinSumOfWeights(int aI) const {
  if(fData) {
    const AIDA::IDataPoint* point = fData->point(aI);
    if(!point) return 0;
    int coord = (fRep==1?1:fCoordinate);
    const AIDA::IMeasurement* mes = point->coordinate(coord);
    if(!mes) return 0;
    return (float)mes->value();
  } else {
    return (float)f_vs[aI];
  }
}

float OnXLab::PlottableDataPointSet::getBinBar(int aI) const {
  if(fData) {
    //FIXME : for fRep=1, the SoPlotter will display "bin bar" 
    //        half/half of the value, which is not correct if
    //        errorMinus is not the same than errorPlus.
    const AIDA::IDataPoint* point = fData->point(aI);
    if(!point) return 0;
    int coord = (fRep==1?1:fCoordinate);
    const AIDA::IMeasurement* mes = point->coordinate(coord);
    if(!mes) return 0;
    return float(mes->errorMinus()+mes->errorPlus());
  } else {
    return (float)(f_vms[aI]+f_vps[aI]);
  }
}

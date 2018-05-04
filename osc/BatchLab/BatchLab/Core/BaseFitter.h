#ifndef BatchLab_BaseFitter_h
#define BatchLab_BaseFitter_h

// Inheritance :
#include <AIDA/IFitter.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

namespace AIDA {class ITupleFactory;}

#include <AIDA/IFunctionFactory.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/IFunction.h>
#include <AIDA/IAxis.h>
#include <AIDA/IDataPointSet.h>
#include <AIDA/IDataPoint.h>
#include <AIDA/IMeasurement.h>
#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>

#include <BatchLab/Core/FitResult.h>
#include <cmath>
#include <inlib/sprintf>
#include <inlib/mnmx>
#include <inlib/poldeg>

#include <BatchLab/Core/FitParameterSettings.h>

namespace BatchLab {

class BaseFitter : public virtual AIDA::IFitter {
public: //AIDA::IFitter
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual AIDA::IFitResult* fit(AIDA::IFitData&,AIDA::IFunction&);
  virtual AIDA::IFitResult* fit(AIDA::IFitData&,const std::string&);
  virtual AIDA::IFitResult* fit(AIDA::IFitData&,const std::string&,
                                const std::vector<double>&);

  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram&,AIDA::IFunction&);
  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram&,const std::string&);
  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram&,const std::string&,
                                const std::vector<double>&);

  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet&,AIDA::IFunction&);
  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet&,const std::string&);
  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet&,const std::string&,
                                const std::vector<double>&);
#else
  virtual AIDA::IFitResult* fit(AIDA::IFitData& fitData,
                                AIDA::IFunction& func,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IFitData& fitData,
                                const std::string& model,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IFitData& fitData,
                                const std::string& model,
                                const std::vector<double>& initialParams,
                                const std::string& range = ""); //AIDA-v3r3p0

  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram& hist,
                                AIDA::IFunction& func,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram& fitData,
                                const std::string& model,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IBaseHistogram& fitData,
                                const std::string& model,
                                const std::vector<double>& initialParams,
                                const std::string& range = ""); //AIDA-v3r3p0

  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet& hist,
                                AIDA::IFunction& func,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet& fitData,
                                const std::string& model,
                                const std::string& range = ""); //AIDA-v3r3p0
  virtual AIDA::IFitResult* fit(AIDA::IDataPointSet& fitData,
                                const std::string& model,
                                const std::vector<double>& initialParams,
                                const std::string& range = ""); //AIDA-v3r3p0
#endif
  virtual bool setEngine(const std::string&);
  virtual std::string engineName();
  virtual AIDA::IFitParameterSettings& fitParameterSettings(const std::string&);
  virtual std::vector<std::string> listParameterSettings();
  virtual void resetParameterSettings();
  virtual bool setConstraint(const std::string&);
  virtual std::vector<std::string> constraints();
  virtual void resetConstraints();
  virtual bool setFitMethod(const std::string&);
  virtual std::string fitMethodName();
  virtual AIDA::IDataPointSet* createScan1D(AIDA::IFitData&,
                                            AIDA::IFunction&,
                                            const std::string&,
                                            int,double,double);
  virtual AIDA::IDataPointSet* createContour(AIDA::IFitData&,
                                             AIDA::IFitResult&,
                                             const std::string&,
                                             const std::string&,int,double);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual void setUseFunctionGradient(bool);
#else
  virtual bool setUseFunctionGradient(bool);
#endif
  virtual bool useFunctionGradient();
public: //BatchLab::BaseFitter :
  virtual bool baseFit(AIDA::IFunction&,
                       const std::vector< std::vector<double> >&,
                       const std::vector<double>&,
                       const std::vector<double>&,
                       double&,std::vector<double>&) = 0;
public:
  BaseFitter(std::ostream&,
             AIDA::IFunctionFactory&,
             AIDA::ITupleFactory&,
             const std::string&);
  virtual ~BaseFitter();
  virtual void* cast(const std::string&) const;
public:
  static void setFitInfos(AIDA::IAnnotation&,AIDA::IFitResult&);
private:
  static double userFunction(double,const std::vector<double>&,void*);

private:
  template <class T>
  inline static bool HalfLowerMeanPoint(T& aHisto,double& aX,double& aV){
    int binn = aHisto.axis().bins();
    int end = binn/2;
    unsigned int n = 0;
    aX = 0;
    aV = 0;
    for(int count=0;count<end;count++) { 
      if(aHisto.binEntries(count)==0) continue;  // Do not take empty bins.
      // Take middle of Bin.
      aX += 
      (aHisto.axis().binLowerEdge(count)+aHisto.axis().binUpperEdge(count))/2.;
      aV += aHisto.binHeight(count);
      n++;
    }
    if(!n) {
      aX = 0;
      aV = 0;
      return false;
    }
    aX /= n;
    aV /= n;
    return true;
  }

  template <class T>
  inline static bool HalfUpperMeanPoint(T& aHisto,double& aX,double& aV){
    //  Used to find an end point for linear, exponential fit.
    int binn = aHisto.axis().bins();
    int begin = binn/2;
    unsigned int n = 0;
    aX = 0;
    aV = 0;
    for(int count=begin;count<binn;count++) {
      if(aHisto.binEntries(count)==0) continue;  // Do not take empty bins.
      // Take middle of Bin.
      aX += 
      (aHisto.axis().binUpperEdge(count)+aHisto.axis().binLowerEdge(count))/2.;
      aV += aHisto.binHeight(count);
      n++;
    }
    if(!n) {
      aX = 0;
      aV = 0;
      return false;
    }
    aX /= n;
    aV /= n;
    return true;
  }

private: //histograms, profiles.
  template <class T>
  inline static AIDA::IFitResult* _fit1D_(BaseFitter& aFitter,
                                          std::ostream& a_out,
                                          T& aHistogram, //histo1D or profile1D
                                          AIDA::IFunction& aFunction) {
    if(aHistogram.entries()==0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " histogram has no entries." << std::endl;
      return 0;
    }
  
    int nbin = aHistogram.axis().bins();
    if(nbin<=0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " has no bins." << std::endl;
      return 0;
    }
  
    int number = 0;
   {for(int count=0;count<nbin;count++) { 
      int nent = aHistogram.binEntries(count);
      if(nent==0) continue;  // Do not take bin without entry.
      number++;
    }}
  
    std::vector<double> pt;
    pt.resize(1,0);
    std::vector< std::vector<double> > pts;
    pts.resize(number,pt);
  
    std::vector<double> vs(number);
    std::vector<double> es(number);
    int bini = 0;
    for(int index=0;index<nbin;index++) { 
      int nent = aHistogram.binEntries(index);
      if(nent==0) continue;  // Do not take bin without entry.
      // Take middle of bin.
      double xx = 
        (aHistogram.axis().binLowerEdge(index)+
         aHistogram.axis().binUpperEdge(index))/2.;
      double val = aHistogram.binHeight(index);
      double err = ::sqrt(::fabs(val)); // Take statistical error.
      if(err==0.) { 
        a_out << "BatchLab::BaseFitter::fit :"
            << " null error for bin " << index 
            << " x " << xx << " value " << val
            << ". Set error to 0.01."
            << std::endl;
        //return 0;
        err = 0.01; //FIXME
      }
      pts[bini][0] = xx;
      vs[bini] = val;
      es[bini] = err;
      bini++;
    }
    if(pts.size()==0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " no starting points." << std::endl;
      return 0;
    }
  
    double chi2;
    std::vector<double> errs;
    if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
    const std::vector<double>& func_params = aFunction.parameters();
    int degreeOfFreedom = pts.size()-func_params.size();
    double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
    BatchLab::FitResult* result = new BatchLab::FitResult
      (aFunction,errs,degreeOfFreedom,quality);
  
    setFitInfos(aHistogram.annotation(),*result);
  
    return result;
  }

  template <class T>
  inline static AIDA::IFitResult* _fit2D_(BaseFitter& aFitter,
                                          std::ostream& a_out,
                                          T& aHistogram, //histo2D or profile2D
                                          AIDA::IFunction& aFunction) {
    if(aHistogram.entries()==0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " histogram has no entries." << std::endl;
      return 0;
    }
  
    int nx = aHistogram.xAxis().bins();
    if(nx<=0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " has no x bins." << std::endl;
      return 0;
    }
  
    int ny = aHistogram.yAxis().bins();
    if(ny<=0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " has no y bins." << std::endl;
      return 0;
    }
  
    int number = 0;
   {for(int i=0;i<nx;i++) { 
      for(int j=0;j<ny;j++) { 
        int nent = aHistogram.binEntries(i,j);
        if(nent==0) continue;  // Do not take bin without entry.
        number++;
      }
    }}
  
    std::vector<double> pt;
    pt.resize(2,0);
    std::vector< std::vector<double> > pts;
    pts.resize(number,pt);
  
    std::vector<double> vs(number);
    std::vector<double> es(number);
    int bini = 0;
   {for(int i=0;i<nx;i++) { 
      // Take middle of bin.
      double xx = 
        (aHistogram.xAxis().binLowerEdge(i)+
         aHistogram.xAxis().binUpperEdge(i))/2.;
      for(int j=0;j<ny;j++) { 
        int nent = aHistogram.binEntries(i,j);
        if(nent==0) continue;  // Do not take bin without entry.
  
        double yy = 
          (aHistogram.yAxis().binLowerEdge(j)+
           aHistogram.yAxis().binUpperEdge(j))/2.;
  
        double val = aHistogram.binHeight(i,j);
        double err = ::sqrt(::fabs(val)); // Take statistical error.
        if(err==0.) { 
          a_out << "BatchLab::BaseFitter::fit :"
              << " null error for bin"
              << " i " << i << " x " << xx 
              << " j " << j << " y " << yy
              << " value " << val
              << ". Set error to 0.01."
              << std::endl;
          //return 0;
          err = 0.01; //FIXME
        }
        pts[bini][0] = xx;
        pts[bini][1] = yy;
        vs[bini] = val;
        es[bini] = err;
        bini++;
      }
    }}
    if(pts.size()==0) {
      a_out << "BatchLab::BaseFitter::fit :"
          << " no starting points." << std::endl;
      return 0;
    }
  
    double chi2;
    std::vector<double> errs;
    if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
    const std::vector<double>& func_params = aFunction.parameters();
    int degreeOfFreedom = pts.size()-func_params.size();
    double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
    BatchLab::FitResult* result = new BatchLab::FitResult
      (aFunction,errs,degreeOfFreedom,quality);
  
    setFitInfos(aHistogram.annotation(),*result);
  
    return result;
  }
  
  template <class T>
  inline static AIDA::IFitResult* _fit_(BaseFitter& aFitter,
                                        std::ostream& a_out,
                                    AIDA::IFunctionFactory& aFunctionFactory,
                                    int& aCounter,T& aHistogram,
                                    const std::string& aModel) {
    //AIDA::IAnnotation& annotation = aHistogram.annotation();
    //std::string name = annotation.value("fit_function_name");
  
    // Build a name :
    std::string name;
    inlib::sprintf(name,128,"fitHistogram_%d",aCounter);
    aCounter++;
    std::string title = name; //FIXME
    std::string model = aModel;
    inlib::touppercase(model);
    int degree;
  
    if( (model=="G") || (model=="GAUSSIAN") ) {
  
      double h = aHistogram.maxBinHeight() - aHistogram.minBinHeight();
      double m = aHistogram.mean();
      double w = aHistogram.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"G");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = m;
      ps[2] = w;
      function->setParameters(ps);
  
      //NOTE : fit.function is used in OpenPAW /HISTOGRAM/PLOT.
      aHistogram.annotation().removeItem("fit.function");
      aHistogram.annotation().addItem("fit.function",name);
  
      return _fit1D_(aFitter,a_out,aHistogram,*function);
  
    } else if( (model=="BW") || (model=="BREITWIGNER") ) {
  
      double h = aHistogram.maxBinHeight() - aHistogram.minBinHeight();
      double m = aHistogram.mean();
      double w = aHistogram.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"BW");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = m;
      ps[2] = w;
      function->setParameters(ps);
  
      aHistogram.annotation().removeItem("fit.function");
      aHistogram.annotation().addItem("fit.function",name);
  
      return _fit1D_(aFitter,a_out,aHistogram,*function);
  
    } else if( (model=="E") || (model=="EXPONENTIAL") ) {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aHistogram,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aHistogram,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      if(!val1) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      double dx = xx2 - xx1;
      if(!dx) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      // Deduce p0,p1 of p0*exp(p1*x) with the two upper points :
      double p1 = ::log(val2/val1) / dx;
      double p0 = ::exp(p1*xx1);
      if(!p0) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }    
      p0 = val1 / p0;
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"E");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = p0;
      ps[1] = p1;
  
      function->setParameters(ps);
  
      aHistogram.annotation().removeItem("fit.function");
      aHistogram.annotation().addItem("fit.function",name);
  
      return _fit1D_(aFitter,a_out,aHistogram,*function);
  
    } else if(model=="EHBOOK") {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aHistogram,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aHistogram,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      
      // Deduce a,b of exp(a+bx) with the two upper points :
      double b = ::log(val2/val1) / ( xx2 - xx1);
      double a = ::log( val1 / exp ( b * xx1 ) );
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"EHBOOK");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = a;
      ps[1] = b;
      function->setParameters(ps);
  
      aHistogram.annotation().removeItem("fit.function");
      aHistogram.annotation().addItem("fit.function",name);
  
      return _fit1D_(aFitter,a_out,aHistogram,*function);
  
    } else if(inlib::polynomial_degree(model,degree)) {
  
      double h = aHistogram.maxBinHeight() - aHistogram.minBinHeight();
      double m = aHistogram.mean();
      double w = aHistogram.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,model);
      if(!function) return 0;
      std::vector<double> ps;
      ps.resize(degree+1,1);
      if(degree==2) {
        ps[0] = h;
        ps[1] = m;
        ps[2] = w;
      } 
      function->setParameters(ps);
  
      aHistogram.annotation().removeItem("fit.function");
      aHistogram.annotation().addItem("fit.function",name);
  
      return _fit1D_(aFitter,a_out,aHistogram,*function);
  
    } 
  
    a_out << "BatchLab::BaseFitter::fit :"
        << " unknown fit model " << inlib::sout(model)
        << std::endl;
  
    return 0;
  }

private: //clouds
  inline static AIDA::IFitResult* cloud_fit(BaseFitter& aFitter,
                                            std::ostream& a_out,
                                            AIDA::ICloud1D& aCloud,
                                            AIDA::IFunction& aFunction) {
    if(aCloud.isConverted()) {
  
      //break const :
      AIDA::IHistogram1D* histo = (AIDA::IHistogram1D*)&(aCloud.histogram());
      return _fit1D_(aFitter,a_out,*histo,aFunction);
      
    } else {
  
      int number = aCloud.entries();
      if(number==0) {
        a_out << "BatchLab::BaseFitter::fit(Cloud1D) :"
            << " cloud has no entries." << std::endl;
        return 0;
      }
  
      std::vector<double> pt;
      pt.resize(1,0);
      std::vector< std::vector<double> > pts;
      pts.resize(number,pt);
  
      std::vector<double> vs(number);
      std::vector<double> es(number);
  
      for(int index=0;index<number;index++) { 
        double xx = aCloud.value(index);
        double val = aCloud.weight(index);
        double err = ::sqrt(::fabs(val)); // Take statistical error.
        if(err==0.) { 
          a_out << "BatchLab::BaseFitter::fit(Cloud1D) :"
              << " null error for point " << index
              << " x " << xx << " value " << val
              << ". Set error to 0.01."
              << std::endl;
          //return 0;
          err = 0.01; //FIXME
        }
        pts[index][0] = xx;
        vs[index] = val;
        es[index] = err;
      }
  
      double chi2;
      std::vector<double> errs;
      if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
      const std::vector<double>& func_params = aFunction.parameters();
      int degreeOfFreedom = pts.size()-func_params.size();
      double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
      BatchLab::FitResult* result = new BatchLab::FitResult
        (aFunction,errs,degreeOfFreedom,quality);
  
      setFitInfos(aCloud.annotation(),*result);
  
      return result;
    }
  }

  inline static AIDA::IFitResult* cloud_fit(BaseFitter& aFitter,
                                            std::ostream& a_out,
                                            AIDA::ICloud2D& aCloud,
                                            AIDA::IFunction& aFunction) {
    if(aCloud.isConverted()) {
  
      //return BatchLab_BaseFitter_fit
      //         (aFitter,a_out,aCloud.histogram(),aFunction);
      a_out << "BatchLab::BaseFitter::fit(Cloud2D) :"
          << " don't know how to fit an histogram 2D." << std::endl;
      return 0;
  
    } else {
  
      int number = aCloud.entries();
      if(number==0) {
        a_out << "BatchLab::BaseFitter::fit(Cloud2D) :"
              << " cloud has no entries." << std::endl;
        return 0;
      }
  
      std::vector<double> pt;
      pt.resize(1,0);
      std::vector< std::vector<double> > pts;
      pts.resize(number,pt);
  
      std::vector<double> vs(number);
      std::vector<double> es(number);
  
      for(int index=0;index<number;index++) { 
        double xx = aCloud.valueX(index);
        double val = aCloud.valueY(index);
        double err = ::sqrt(::fabs(val)); // Take statistical error.
        if(err==0.) { 
          a_out << "BatchLab::BaseFitter::fit(Cloud2D) :"
              << " null error for point " << index
              << " x " << xx << " value " << val
              << ". Set error to 0.01."
              << std::endl;
          //return 0;
          err = 0.01; //FIXME
        }
        pts[index][0] = xx;
        vs[index] = val;
        es[index] = err;
      }
  
      double chi2;
      std::vector<double> errs;
      if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
      const std::vector<double>& func_params = aFunction.parameters();
      int degreeOfFreedom = pts.size()-func_params.size();
      double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
      BatchLab::FitResult* result = new BatchLab::FitResult
        (aFunction,errs,degreeOfFreedom,quality);
  
      setFitInfos(aCloud.annotation(),*result);
  
      return result;
    }
  }

  inline static bool HalfLowerMeanPoint(AIDA::ICloud1D& aCloud,
                                        double& aX,double& aV){
    double m = aCloud.mean();
    unsigned int n = 0;
    aX = 0;
    aV = 0;
   {int number = aCloud.entries();
    for(int index=0;index<number;index++) { 
      double xx = aCloud.value(index);
      if(xx>m) continue;
      aX += xx;
      aV += aCloud.weight(index);
      n++;
    }}
    if(!n) {
      aX = 0;
      aV = 0;
      return false;
    }
    aX /= n;
    aV /= n;
    return true;
  }
  
  inline static bool HalfUpperMeanPoint(AIDA::ICloud1D& aCloud,
                                        double& aX,double& aV){
    double m = aCloud.mean();
    unsigned int n = 0;
    aX = 0;
    aV = 0;
   {int number = aCloud.entries();
    for(int index=0;index<number;index++) { 
      double xx = aCloud.value(index);
      if(xx<=m) continue;
      aX += xx;
      aV += aCloud.weight(index);
      n++;
    }}
    if(!n) {
      aX = 0;
      aV = 0;
      return false;
    }
    aX /= n;
    aV /= n;
    return true;
  }

  inline static AIDA::IFitResult* cloud_fit(BaseFitter& aFitter,
                                            std::ostream& a_out,
                                      AIDA::IFunctionFactory& aFunctionFactory,
                                      int& aCounter,
                                      AIDA::ICloud1D& aCloud,
                                      const std::string& aModel) {
    // Build a name :
    std::string name;
    inlib::sprintf(name,128,"fitCloud_%d",aCounter);
    aCounter++;
    std::string title = name; //FIXME
    std::string model = aModel;
    inlib::touppercase(model);
    int degree;
  
    if( (model=="G") || (model=="GAUSSIAN") ) {
  
      double h = 0;
     {int number = aCloud.entries();
      for(int index=0;index<number;index++) { 
        h = inlib::mx(h,aCloud.weight(index));
      }}
      double m = aCloud.mean();
      double w = aCloud.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"G");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = m;
      ps[2] = w;
      function->setParameters(ps);
  
      return cloud_fit(aFitter,a_out,aCloud,*function);
  
    } else if( (model=="BW") || (model=="BREITWIGNER") ) {
  
      double h = 0;
     {int number = aCloud.entries();
      for(int index=0;index<number;index++) { 
        h = inlib::mx(h,aCloud.weight(index));
      }}
      double m = aCloud.mean();
      double w = aCloud.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"BW");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = m;
      ps[2] = w;
      function->setParameters(ps);
  
      return cloud_fit(aFitter,a_out,aCloud,*function);
  
    } else if( (model=="E") || (model=="EXPONENTIAL") ) {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aCloud,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aCloud,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      
      // Deduce p0,p1 of p0*exp(p1*x) with the two upper points :
      double p1 = ::log(val2/val1) / ( xx2 - xx1);
      double p0 = val1 / exp ( p1 * xx1 );
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"E");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = p0;
      ps[1] = p1;
      function->setParameters(ps);
  
      return cloud_fit(aFitter,a_out,aCloud,*function);
  
    } else if(model=="EHBOOK") {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aCloud,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aCloud,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      
      // Deduce a,b of exp(a+bx) with the two upper points :
      double b = ::log(val2/val1) / ( xx2 - xx1);
      double a = ::log( val1 / exp ( b * xx1 ) );
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"EHBOOK");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = a;
      ps[1] = b;
      function->setParameters(ps);
  
      return cloud_fit(aFitter,a_out,aCloud,*function);
  
    } else if(inlib::polynomial_degree(model,degree)) {
  
      double h = 0;
     {int number = aCloud.entries();
      for(int index=0;index<number;index++) { 
        h = inlib::mx(h,aCloud.weight(index));
      }}
      double m = aCloud.mean();
      double w = aCloud.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,model);
      if(!function) return 0;
      std::vector<double> ps;
      ps.resize(degree+1,1);
      if(degree==2) {
        ps[0] = h;
        ps[1] = m;
        ps[2] = w;
      } 
      function->setParameters(ps);
  
      return cloud_fit(aFitter,a_out,aCloud,*function);
  
    } 
  
    a_out << "BatchLab::BaseFitter::fit :"
        << " unknown fit model " << inlib::sout(model)
        << std::endl;
  
    return 0;
  }

private: //data point sets
  inline static AIDA::IFitResult* dps_fit(BaseFitter& aFitter,
                                          std::ostream& a_out,
                                          AIDA::IDataPointSet& aDataPointSet,
                                          int a_x_index,int a_v_index,
                                          AIDA::IFunction& aFunction) {
    int number = aDataPointSet.size();
    if(number<=0) return 0;
  
    int dim = aDataPointSet.dimension();
  
    if((a_x_index<0)||(dim<=a_x_index)) {
      a_out << "BatchLab_BaseFitter_fit :"
          << " dataPointSet index " << a_x_index
          << " is out of range. Dimension is " << dim << "."
          << std::endl;
      return 0;
    }
    if((a_v_index<0)||(dim<=a_v_index)) {
      a_out << "BatchLab_BaseFitter_fit :"
          << " dataPointSet index " << a_v_index
          << " is out of range. Dimension is " << dim << "."
          << std::endl;
      return 0;
    }
  
    std::vector<double> pt;
    pt.resize(1,0);
    std::vector< std::vector<double> > pts;
    pts.resize(number,pt);
  
    std::vector<double> vs(number);
    std::vector<double> es(number);
    for(int index=0;index<number;index++) {
      double x = aDataPointSet.point(index)->coordinate(a_x_index)->value();
      double v = aDataPointSet.point(index)->coordinate(a_v_index)->value();
      double e = ::sqrt(::fabs(v));
      if(e==0) e = 0.01; //WARNING : must not be 0.
      pts[index][0] = x;
      vs[index] = v;
      es[index] = e;
    }
    
    double chi2;
    std::vector<double> errs;
    if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
    const std::vector<double>& func_params = aFunction.parameters();
    int degreeOfFreedom = pts.size()-func_params.size();
    double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
    BatchLab::FitResult* result = 
      new BatchLab::FitResult(aFunction,errs,degreeOfFreedom,quality);
  
    setFitInfos(aDataPointSet.annotation(),*result);
  
    return result;
  }

private: //tuple
  inline static AIDA::IFitResult* tuple_fit(BaseFitter& aFitter,
                                            std::ostream& a_out,
                                            AIDA::ITuple& aTuple,
                                            AIDA::IEvaluator& aEvaluator,
                                            AIDA::IFilter& aFilter,
                                            AIDA::IFunction& aFunction){
    //
    // for one column we fit {(row, col[row])}
    //
    int number = aTuple.rows();
    if(number<=0) {
      a_out << "BatchLab_BaseFitter_fit :"
          << " Tuple " 
          << inlib::sout(aTuple.title()) << " empty." 
          << std::endl;
      return 0;
    }
  
    if(!aEvaluator.initialize(aTuple)) {
      a_out << "BatchLab_BaseFitter_fit :"
          << " Evaluator initialization failed for tuple " 
          << inlib::sout(aTuple.title())
          << std::endl;
      return 0;
    }
  
    if(!aFilter.initialize(aTuple)) {
      a_out << "BatchLab_BaseFitter_fit :"
          << " Filter initialization failed for tuple " 
          << inlib::sout(aTuple.title())
          << std::endl;
      return 0;
    }
  
    std::vector<double> pt;
    pt.resize(1,0);
    std::vector< std::vector<double> > pts;
    pts.resize(number,pt);
  
    std::vector<double> vs(number);
    std::vector<double> es(number);
  
    unsigned int index = 0;
    aTuple.start();
    while(aTuple.next()) {
      if(!aFilter.accept()) continue;
      double xx = double(index);
      double val = aEvaluator.evaluateDouble();
      double err = ::sqrt(::fabs(val)); // Take statistical error.
      if(err==0.) { 
        a_out << "BatchLab_BaseFitter_fit :"
            << " null error for point " << index
            << " x " << xx << " value " << val
            << ". Set error to 0.01."
            << std::endl;
        //return 0;
        err = 0.01; //FIXME
      }
      pts[index][0] = xx;
      vs[index] = val;
      es[index] = err;
      index++;
    }
    pts.resize(index);
    vs.resize(index);
    es.resize(index);
  
    double chi2;
    std::vector<double> errs;
    if(!aFitter.baseFit(aFunction,pts,vs,es,chi2,errs)) return 0;
  
    const std::vector<double>& func_params = aFunction.parameters();
    int degreeOfFreedom = pts.size()-func_params.size();
    double quality = degreeOfFreedom?(chi2/degreeOfFreedom):0;
  
    BatchLab::FitResult* result = new BatchLab::FitResult
      (aFunction,errs,degreeOfFreedom,quality);
  
    BatchLab::BaseFitter::setFitInfos(aTuple.annotation(),*result);
  
    return result;
  }
  
  inline static AIDA::IFitResult* tuple_fit(BaseFitter& aFitter,
                                            std::ostream& a_out,
                                   AIDA::IFunctionFactory& aFunctionFactory,
                                            int& aCounter,
                                            AIDA::ITuple& aTuple,
                                            AIDA::IEvaluator& aEvaluator,
                                            AIDA::IFilter& aFilter,
                                            const std::string& aModel) {
    // Build a name :
    std::string name;
    inlib::sprintf(name,128,"fitTuple_%d",aCounter);
    aCounter++;
    std::string title = name; //FIXME
    std::string model = aModel;
    inlib::touppercase(model);
    int degree;
  
    if( (model=="G") || (model=="GAUSSIAN") ) {
  
      double h = 0;
      double mean = 0;
      double rms = 0;
     {double Sw = 0;
      double Sxw = 0;
      double Sx2w = 0;
      unsigned int index = 0;
      aTuple.start();
      while(aTuple.next()) {
        if(!aFilter.accept()) continue;
        double x = double(index);
        double w = aEvaluator.evaluateDouble();
        h = inlib::mx(h,w);
        Sw += w;
        double xw = x * w;
        Sxw += xw;
        Sx2w += x * xw;
        index++;
      }
      if(Sw!=0) {
        mean = Sxw/Sw;
        rms = ::sqrt(::fabs( (Sx2w / Sw) - mean * mean));
      }}
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"G");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = mean;
      ps[2] = rms;
      function->setParameters(ps);
  
      return tuple_fit(aFitter,a_out,aTuple,aEvaluator,aFilter,*function);
  
/*uuu
    } else if( (model=="BW") || (model=="BREITWIGNER") ) {
  
      double h = 0;
     {int number = aCloud.entries();
      for(int index=0;index<number;index++) { 
        h = inlib::mx(h,aCloud.weight(index));
      }}
      double m = aCloud.mean();
      double w = aCloud.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"BW");
      if(!function) return 0;
      std::vector<double> ps(3);
      ps[0] = h;
      ps[1] = m;
      ps[2] = w;
      function->setParameters(ps);
  
      return BatchLab_BaseFitter_fit(aFitter,a_out,
        aTuple,aEvaluator,aFilter,*function);
  
    } else if( (model=="E") || (model=="EXPONENTIAL") ) {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aCloud,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aCloud,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      
      // Deduce p0,p1 of p0*exp(p1*x) with the two upper points :
      double p1 = ::log(val2/val1) / ( xx2 - xx1);
      double p0 = val1 / exp ( p1 * xx1 );
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"E");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = p0;
      ps[1] = p1;
      function->setParameters(ps);
  
      return BatchLab_BaseFitter_fit(aFitter,a_out,
        aTuple,aEvaluator,aFilter,*function);
  
    } else if(model=="EHBOOK") {
  
      double xx1,xx2;
      double val1,val2;
      bool status1 = HalfLowerMeanPoint(aCloud,xx1,val1);
      bool status2 = HalfUpperMeanPoint(aCloud,xx2,val2);
      if( !status1 || !status2 || ((xx2-xx1)==0.) ) {
        a_out << "BatchLab::BaseFitter::fit : can't compute starting values." 
            << std::endl;
        return 0;
      }
      
      // Deduce a,b of exp(a+bx) with the two upper points :
      double b = ::log(val2/val1) / ( xx2 - xx1);
      double a = ::log( val1 / exp ( b * xx1 ) );
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,"EHBOOK");
      if(!function) return 0;
      std::vector<double> ps(2);
      ps[0] = a;
      ps[1] = b;
      function->setParameters(ps);
  
      return BatchLab_BaseFitter_fit(aFitter,a_out,
        aTuple,aEvaluator,aFilter,*function);
  
    } else if(inlib::polynomial_degree(model,degree)) {
  
      double h = 0;
     {int number = aCloud.entries();
      for(int index=0;index<number;index++) { 
        h = inlib::mx(h,aCloud.weight(index));
      }}
      double m = aCloud.mean();
      double w = aCloud.rms();
  
      AIDA::IFunction* function = 
        aFunctionFactory.createFunctionByName(name,model);
      if(!function) return 0;
      std::vector<double> ps;
      ps.resize(degree+1,1);
      if(degree==2) {
        ps[0] = h;
        ps[1] = m;
        ps[2] = w;
      } 
      function->setParameters(ps);
  
      return BatchLab_BaseFitter_fit(aFitter,a_out,
        aTuple,aEvaluator,aFilter,*function);
  
  */
    } 
  
    a_out << "BatchLab::BaseFitter::fit :"
        << " unknown fit model " << inlib::sout(model)
        << std::endl;
  
    return 0;
  }
  
protected:
  std::ostream& f_out;
private:
  AIDA::IFunctionFactory& fFunctionFactory;
  AIDA::ITupleFactory& fTupleFactory;
  std::string fEngineName;
  std::vector<std::string> fParameterSettings;
  std::vector<std::string> fConstraints;
  std::string fFitMethodName;
  int fCounter;
  std::vector<FitParameterSettings> fFitParameterSettings;
};

}

#include "BaseFitter.icc"

#endif

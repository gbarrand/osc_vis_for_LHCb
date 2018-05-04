#ifndef BatchLab_FitData_h
#define BatchLab_FitData_h

// Inheritance :
#include <AIDA/IFitData.h>

#include <AIDA/IConstants.h> //For AIDA_VERSION

#include <BatchLab/Core/RangeSet.h>
#include <AIDA/IRangeSet.h>

#include <inlib/cast>

#include <cstdio>

namespace BatchLab {

class FitData : public virtual AIDA::IFitData {
public: 
  virtual bool create1DConnection(const AIDA::IHistogram1D& aHistogram) { 
    fHistogram1D = &aHistogram;
    return true;
  }
  virtual bool create1DConnection(const AIDA::ICloud1D& /*c*/) { 
    ::printf("debug : BatchLab::FitData::create1DConnection : dummy\n");
    return false;
  }
  virtual bool create1DConnection(const AIDA::IProfile1D& /*p*/) { 
    ::printf("debug : BatchLab::FitData::create1DConnection : dummy\n");
    return false;
  }
  virtual bool create1DConnection(const AIDA::IDataPointSet& a_dps,int a_x,int a_v) { 
    f_dps = &a_dps;
    f_dps_x = a_x;
    f_dps_v = a_v;
    return true;
  }
  virtual bool create1DConnection(const std::vector<double>&,
                                  const std::vector<double>&,
                                  const std::vector<double>&){ //AIDA-v3r3p0
    ::printf("debug : BatchLab::FitData::create1DConnection : dummy\n");
    return false;
  }

  virtual bool create2DConnection(const AIDA::IHistogram2D& aHistogram) { 
    fHistogram2D = &aHistogram;
    return true;
  }
  virtual bool create2DConnection(const AIDA::IHistogram2D& /*aHistogram*/,
                                  int /*xIndex*/, int /*yIndex*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create2DConnection(const AIDA::ICloud2D& /*c*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create2DConnection(const AIDA::ICloud2D& /*c*/,
                                  int /*xIndex*/, int /*yIndex*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create2DConnection(const AIDA::IProfile2D& /*p*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create2DConnection(const AIDA::IProfile2D& /*p*/,
                                  int /*xIndex*/, int /*yIndex*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create2DConnection(const AIDA::IDataPointSet& /*dp*/,
                       int /*xIndex*/, int /*yIndex*/, int /*valIndex*/) { 
    ::printf("debug : BatchLab::FitData::create2DConnection : dummy\n");
    return false;
  }
  virtual bool create3DConnection(const AIDA::IHistogram3D& /*h*/) { 
    ::printf("debug : BatchLab::FitData::create3DConnection : dummy\n");
    return false;
  }
  virtual bool create3DConnection(const AIDA::IHistogram3D& /*h*/,
                       int /*xIndex*/, int /*yIndex*/, int /*zIndex*/) { 
    ::printf("debug : BatchLab::FitData::create3DConnection : dummy\n");
    return false;
  }
  virtual bool create3DConnection(const AIDA::ICloud3D& /*c*/) { 
    ::printf("debug : BatchLab::FitData::create3DConnection : dummy\n");
    return false;
  }
  virtual bool create3DConnection(const AIDA::ICloud3D& /*c*/,
                       int /*xIndex*/, int /*yIndex*/, int /*zIndex*/) { 
    ::printf("debug : BatchLab::FitData::create3DConnection : dummy\n");
    return false;
  }
  virtual bool create3DConnection(const AIDA::IDataPointSet& /*dp*/,
         int /*xIndex*/, int /*yIndex*/, int /*zIndex*/, int /*valIndex*/) { 
    ::printf("debug : BatchLab::FitData::create3DConnection : dummy\n");
    return false;
  }
  virtual bool createConnection(const AIDA::ITuple& /*t*/,
                 std::vector<AIDA::IEvaluator *>& /*evals*/) { 
    ::printf("debug : BatchLab::FitData::createConnection : dummy\n");
    return false;
  }

//////////////////////////////////////////////////////////////////
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
  virtual bool createConnection(const AIDA::ITuple& aTuple,
                                std::vector<std::string>  aCols) { 
    fTuple = &aTuple;
    fCols = aCols;
    return true;
  }
  virtual bool createConnection(const AIDA::IDataPointSet& /*dp*/,
                            std::vector<int>  /*indeces*/, int /*valIndex*/) { 
    ::printf("debug : BatchLab::FitData::createConnection : dummy\n");
    return false;
  }
  virtual AIDA::IRangeSet& range(int /*index*/) const { 
    INLIB_SELF(FitData);
    return self.fRangeSet;
  }

//////////////////////////////////////////////////////////////////
#else
  virtual bool createConnection(const AIDA::ITuple& /*t*/,
                           const std::vector<std::string>&  /*colNames*/) { 
    ::printf("debug : BatchLab::FitData::createConnection : dummy\n");
    return false;
  }
  virtual bool createConnection(const AIDA::IDataPointSet& /*dp*/,
                    const std::vector<int>&  /*indeces*/, int /*valIndex*/) { 
    ::printf("debug : BatchLab::FitData::createConnection : dummy\n");
    return false;
  }
  virtual AIDA::IRangeSet& range(int /*index*/) {return fRangeSet;}
#endif
//////////////////////////////////////////////////////////////////

  virtual void reset() {
    ::printf("debug : BatchLab::FitData::reset : dummy\n");
    return;
  }
  virtual int dimension() const {
    ::printf("debug : BatchLab::FitData::dimension : dummy\n");
    return 0;
  }
  virtual std::string dataDescription() const { 
    ::printf("debug : BatchLab::FitData::dataDescription : dummy\n");
    return "";
  }
public: 
  FitData():fTuple(0),fHistogram1D(0),fHistogram2D(0),f_dps(0) {}
  virtual ~FitData() {}
public: 
  const AIDA::ITuple* tuple(std::vector<std::string>& aCols) const {
    aCols = fCols;
    return fTuple;
  }
  const AIDA::IHistogram1D* histogram1D() const {return fHistogram1D;}
  const AIDA::IHistogram2D* histogram2D() const {return fHistogram2D;}
  const AIDA::IDataPointSet* dps(int& a_x,int& a_v) const {
    a_x = f_dps_x;
    a_v = f_dps_v;
    return f_dps;
  }
private:
  RangeSet fRangeSet;

  const AIDA::ITuple* fTuple;
  std::vector<std::string> fCols;

  const AIDA::IHistogram1D* fHistogram1D;
  const AIDA::IHistogram2D* fHistogram2D;
  const AIDA::IDataPointSet* f_dps;
  int f_dps_x;
  int f_dps_v;
};

}

#endif

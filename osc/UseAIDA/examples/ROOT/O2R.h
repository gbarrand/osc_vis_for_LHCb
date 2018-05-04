#ifndef O2R_h
#define O2R_h

#include "H2R.h"

#include <BatchLab/Core/Histogram.h>

class OTH1D : public HCL_2_TH1D {
public:
  inline OTH1D(const AIDA::IHistogram1D& aHisto){
    BatchLab::Histogram1D* bhisto = 
      INLIB_SAFE_CAST(aHisto,BatchLab::Histogram1D);
    if(!bhisto) {
      std::cout << "OTH1D :"
              << " cast of AIDA::IHistogram1D to BatchLab::Histogram1D failed."
              << std::endl;
      return;
    }
    build(bhisto->hclHistogram());
  }
  virtual ~OTH1D(){}

  ClassDef(OTH1D,1)
};

class OTH2D : public HCL_2_TH2D {
public:
  inline OTH2D(const AIDA::IHistogram2D& aHisto){
    BatchLab::Histogram2D* bhisto = 
      INLIB_SAFE_CAST(aHisto,BatchLab::Histogram2D);
    if(!bhisto) {
      std::cout << "OTH2D :"
              << " cast of AIDA::IHistogram2D to BatchLab::Histogram2D failed."
              << std::endl;
      return;
    }
    build(bhisto->hclHistogram());
  }
  virtual ~OTH2D(){}

  ClassDef(OTH2D,1)
};

class OTProfile : public HCL_2_TProfile {
public:
  inline OTProfile(const AIDA::IProfile1D& aHisto){
    BatchLab::Profile1D* bhisto = INLIB_SAFE_CAST(aHisto,BatchLab::Profile1D);
    if(!bhisto) {
      std::cout << "OTProfile :"
              << " cast of AIDA::IProfile1D to BatchLab::Profile1D failed."
              << std::endl;
      return;
    }
    build(bhisto->hclHistogram());
  }
  virtual ~OTProfile(){}

  ClassDef(OTProfile,1)
};

class OTProfile2D : public HCL_2_TProfile2D {
public:
  inline OTProfile2D(const AIDA::IProfile2D& aHisto){
    BatchLab::Profile2D* bhisto = INLIB_SAFE_CAST(aHisto,BatchLab::Profile2D);
    if(!bhisto) {
      std::cout << "OTProfile :"
              << " cast of AIDA::IProfile2D to BatchLab::Profile2D failed."
              << std::endl;
      return;
    }
    build(bhisto->hclHistogram());
  }
  virtual ~OTProfile2D(){}

  ClassDef(OTProfile2D,1)
};

#endif

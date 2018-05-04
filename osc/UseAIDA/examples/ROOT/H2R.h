#ifndef H2R_h
#define H2R_h

#include <inlib/histo/h1d>

#include <TH1D.h>

class HCL_2_TH1D : public TH1D {
protected:
  inline HCL_2_TH1D(){}
public:
  inline HCL_2_TH1D(const inlib::histo::h1d& aHisto){build(aHisto);}
  virtual ~HCL_2_TH1D(){}
protected:
  inline void build(const inlib::histo::h1d& aHisto){
    fTitle = aHisto.title();

    const inlib::histo::axis& axis = aHisto.axis();

    fNcells = axis.bins()+2;
    fXaxis.Set(axis.bins(),axis.lower_edge(),axis.upper_edge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.get_all_entries();

    const std::vector<double>& Sw = aHisto.bins_sum_w();
    const std::vector<double>& Sw2 = aHisto.bins_sum_w2();

    Set(fNcells); //TArrayD::fArray
    fSumw2.Set(fNcells);
    for(int i=0;i<fNcells;i++) {
      SetAt(Sw[i],i);
      fSumw2.SetAt(Sw2[i],i);
    }

    fTsumw = aHisto.get_Sw();
    fTsumw2 = aHisto.get_Sw2();
    fTsumwx = aHisto.get_ith_axis_Sxw(0);
    fTsumwx2 = aHisto.get_ith_axis_Sx2w(0);
  }

  ClassDef(HCL_2_TH1D,1)
};

#include <inlib/histo/h2d>

#include <TH2D.h>

class HCL_2_TH2D : public TH2D {
protected:
  inline HCL_2_TH2D(){}
public:
  inline HCL_2_TH2D(const inlib::histo::h2d& aHisto){build(aHisto);}
  virtual ~HCL_2_TH2D(){}
protected:
  inline void build(const inlib::histo::h2d& aHisto){
    fTitle = aHisto.title();

    const inlib::histo::axis& xaxis = aHisto.axis_x();
    const inlib::histo::axis& yaxis = aHisto.axis_y();

    int xbinn = xaxis.bins();
    int ybinn = yaxis.bins();

    fNcells = (xbinn+2)*(ybinn+2);
    fXaxis.Set(xbinn,xaxis.lower_edge(),xaxis.upper_edge());
    fYaxis.Set(ybinn,yaxis.lower_edge(),yaxis.upper_edge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.get_all_entries();

    const std::vector<double>& Sw = aHisto.bins_sum_w();
    const std::vector<double>& Sw2 = aHisto.bins_sum_w2();

    Set(fNcells); //TArrayD::fArray.
    fSumw2.Set(fNcells);
    for(int i=0;i<fNcells;i++) {
      SetAt(Sw[i],i);
      fSumw2.SetAt(Sw2[i],i);
    }

    fTsumw = aHisto.get_Sw();
    fTsumw2 = aHisto.get_Sw2();
    fTsumwx = aHisto.get_ith_axis_Sxw(0);
    fTsumwx2 = aHisto.get_ith_axis_Sx2w(0);
    fTsumwy = aHisto.get_ith_axis_Sxw(1);
    fTsumwy2 = aHisto.get_ith_axis_Sx2w(1);

    fTsumwxy = 0; //FIXME
  }

  ClassDef(HCL_2_TH2D,1)
};

#include <inlib/histo/p1d>

#include <TProfile.h>

class HCL_2_TProfile : public TProfile {
protected:
  inline HCL_2_TProfile(){}
public:
  inline HCL_2_TProfile(const inlib::histo::p1d& aHisto){build(aHisto);}
  virtual ~HCL_2_TProfile(){}
protected:
  inline void build(const inlib::histo::p1d& aHisto){
    fTitle = aHisto.title();

    const inlib::histo::axis& axis = aHisto.axis();

    fNcells = axis.bins()+2;
    fXaxis.Set(axis.bins(),axis.lower_edge(),axis.upper_edge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.get_all_entries();

    //WARNING : the mapping inlib::histo::p1d / TProfile is not obvious.
    //HCL::fBinSvw  <---> TProfile::fArray
    //HCL::fBinSv2w <---> TProfile::fSumw2
    //HCL::fBinSw   <---> TProfile::fBinEntries

    const std::vector<double>& Svw = aHisto.bins_sum_vw();
    const std::vector<double>& Sv2w = aHisto.bins_sum_v2w();
    const std::vector<double>& Sw = aHisto.bins_sum_w();

    Set(fNcells); //TArrayD::fArray
    fSumw2.Set(fNcells);
    fBinEntries.Set(fNcells);
    for(int i=0;i<fNcells;i++) {
      SetAt(Svw[i],i);
      fSumw2.SetAt(Sv2w[i],i);
      fBinEntries.SetAt(Sw[i],i);
    }

    fTsumw = aHisto.get_Sw();
    fTsumw2 = aHisto.get_Sw2();
    fTsumwx = aHisto.get_ith_axis_Sxw(0);
    fTsumwx2 = aHisto.get_ith_axis_Sx2w(0);

    if(aHisto.cut_v()) {
      fYmin = aHisto.min_v();
      fYmax = aHisto.max_v();
    } else {
      fYmin = 0;
      fYmax = 0;
    }
  }

  ClassDef(HCL_2_TProfile,1)
};

#include <inlib/histo/p2d>

#include <TProfile2D.h>

class HCL_2_TProfile2D : public TProfile2D {
protected:
  inline HCL_2_TProfile2D(){}
public:
  inline HCL_2_TProfile2D(const inlib::histo::p2d& aHisto){build(aHisto);}
  virtual ~HCL_2_TProfile2D(){}
protected:
  inline void build(const inlib::histo::p2d& aHisto){
    fTitle = aHisto.title();

    const inlib::histo::axis& xaxis = aHisto.axis_x();
    const inlib::histo::axis& yaxis = aHisto.axis_y();

    int xbinn = xaxis.bins();
    int ybinn = yaxis.bins();

    fNcells = (xbinn+2)*(ybinn+2);
    fXaxis.Set(xbinn,xaxis.lower_edge(),xaxis.upper_edge());
    fYaxis.Set(ybinn,yaxis.lower_edge(),yaxis.upper_edge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.get_all_entries();

    //WARNING : the mapping inlib::histo::p1d / TProfile is not obvious.
    //HCL::fBinSvw  <---> TProfile::fArray
    //HCL::fBinSv2w <---> TProfile::fSumw2
    //HCL::fBinSw   <---> TProfile::fBinEntries

    const std::vector<double>& Svw = aHisto.bins_sum_vw();
    const std::vector<double>& Sv2w = aHisto.bins_sum_v2w();
    const std::vector<double>& Sw = aHisto.bins_sum_w();

    Set(fNcells); //TArrayD::fArray
    fSumw2.Set(fNcells);
    fBinEntries.Set(fNcells);
    for(int i=0;i<fNcells;i++) {
      SetAt(Svw[i],i);
      fSumw2.SetAt(Sv2w[i],i);
      fBinEntries.SetAt(Sw[i],i);
    }

    fTsumw = aHisto.get_Sw();
    fTsumw2 = aHisto.get_Sw2();
    fTsumwx = aHisto.get_ith_axis_Sxw(0);
    fTsumwx2 = aHisto.get_ith_axis_Sx2w(0);
    fTsumwy = aHisto.get_ith_axis_Sxw(1);
    fTsumwy2 = aHisto.get_ith_axis_Sx2w(1);

    fTsumwxy = 0; //FIXME

    if(aHisto.cut_v()) {
      fZmin = aHisto.min_v();
      fZmax = aHisto.max_v();
    } else {
      fZmin = 0;
      fZmax = 0;
    }
  }

  ClassDef(HCL_2_TProfile2D,1)
};

#endif

#ifndef A2R_h
#define A2R_h

//AT = AIDA to ROOT.

#include <AIDA/IAxis.h>

#include <AIDA/IHistogram1D.h>
#include <TH1D.h>

class ATH1D : public TH1D {
public:
  inline ATH1D(const AIDA::IHistogram1D& aHisto){
    fTitle = aHisto.title();

    const AIDA::IAxis& axis = aHisto.axis();

    int binn = axis.bins();

    fNcells = binn+2;
    fXaxis.Set(binn,axis.lowerEdge(),axis.upperEdge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.allEntries();
    fTsumw = aHisto.sumBinHeights();

    Set(fNcells); //TArrayD::fArray
    fSumw2.Set(fNcells);

    int i,iri;
    double sw,err,sw2;

    ////////////////////////////////////////////
    /// surrounding under/overflow border : ////
    ////////////////////////////////////////////
    sw = aHisto.binHeight(AIDA::IAxis::UNDERFLOW_BIN);
    SetAt(sw,0);
    err = aHisto.binError(AIDA::IAxis::UNDERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,0);

    sw = aHisto.binHeight(AIDA::IAxis::OVERFLOW_BIN);
    SetAt(sw,binn+1);
    err = aHisto.binError(AIDA::IAxis::OVERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,binn+1);

    ////////////////////////////////////////////
    /// in range bins : ////////////////////////
    ////////////////////////////////////////////
    //NOTE : with a direct HCL access, we could do :    
    //fTsumw2 = data->getSw2();
    //fTsumwx = data->getIthAxisSxw(0);
    //fTsumwx2 = data->getIthAxisSx2w(0);

    fTsumw2 = 0;
    fTsumwx = 0;
    for(i=1;i<=binn;i++) {
      iri = i-1; //in range index

      sw = aHisto.binHeight(iri);
      SetAt(sw,i);

      err = aHisto.binError(iri);
      sw2 = err*err;
      fSumw2.SetAt(sw2,i);

      fTsumw2 += sw2;
      fTsumwx += sw*aHisto.binMean(iri);
    }

    double rms = aHisto.rms();

    double mean = fTsumwx*fTsumw;
    fTsumwx2 = (rms*rms+mean*mean)*fTsumw;

  }
  virtual ~ATH1D(){}

  ClassDef(ATH1D,1)
};

#include <AIDA/IHistogram2D.h>
#include <TH2D.h>

class ATH2D : public TH2D {
public:
  inline ATH2D(const AIDA::IHistogram2D& aHisto){
    fTitle = aHisto.title();

    const AIDA::IAxis& xaxis = aHisto.xAxis();
    const AIDA::IAxis& yaxis = aHisto.yAxis();

    int xbinn = xaxis.bins();
    int ybinn = yaxis.bins();

    fNcells = (xbinn+2)*(ybinn+2);
    fXaxis.Set(xbinn,xaxis.lowerEdge(),xaxis.upperEdge());
    fYaxis.Set(ybinn,yaxis.lowerEdge(),yaxis.upperEdge());

    fBarOffset = (short)(1000 * 0.25);
    fBarWidth = (short)(1000 * 0.5);
    fEntries = (double)aHisto.allEntries();
    fTsumw = aHisto.sumBinHeights();

    Set(fNcells); //TArrayD::fArray.
    fSumw2.Set(fNcells);

    int i,j,icell,iri,irj;
    double sw,err,sw2;

    ////////////////////////////////////////////
    /// surrounding under/overflow border : ////
    ////////////////////////////////////////////
    // lower band :
    j = 0;
    i = 0;
    icell = i*(ybinn+2)+j;
    sw = aHisto.binHeight(AIDA::IAxis::UNDERFLOW_BIN,
			  AIDA::IAxis::UNDERFLOW_BIN);
    SetAt(sw,icell);
    err = aHisto.binError(AIDA::IAxis::UNDERFLOW_BIN,
                          AIDA::IAxis::UNDERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,icell);
    for(i=1;i<=xbinn;i++) {
      icell = i*(xbinn+2)+j;
      iri = i-1; //in range i index
      //irj = j-1; //in range j index
      sw = aHisto.binHeight(iri,AIDA::IAxis::UNDERFLOW_BIN);
      SetAt(sw,icell);
      err = aHisto.binError(iri,AIDA::IAxis::UNDERFLOW_BIN);
      sw2 = err*err;
      fSumw2.SetAt(sw2,icell);
    }
    j = 0;
    i = xbinn+1;
    icell = i*(ybinn+2)+j;
    sw = aHisto.binHeight(AIDA::IAxis::OVERFLOW_BIN,
			  AIDA::IAxis::UNDERFLOW_BIN);
    SetAt(sw,icell);
    err = aHisto.binError(AIDA::IAxis::OVERFLOW_BIN,
                          AIDA::IAxis::UNDERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,icell);

    // left side :
    i = 0;
    for(j=1;j<=ybinn;j++) {
      icell = i*(xbinn+2)+j;
      //iri = i-1; //in range i index
      irj = j-1; //in range j index
      sw = aHisto.binHeight(AIDA::IAxis::UNDERFLOW_BIN,irj);
      SetAt(sw,icell);
      err = aHisto.binError(AIDA::IAxis::UNDERFLOW_BIN,irj);
      sw2 = err*err;
      fSumw2.SetAt(sw2,icell);
    }

    // right side :
    i = xbinn+1;
    for(j=1;j<=ybinn;j++) {
      icell = i*(xbinn+2)+j;
      //iri = i-1; //in range i index
      irj = j-1; //in range j index
      sw = aHisto.binHeight(AIDA::IAxis::OVERFLOW_BIN,irj);
      SetAt(sw,icell);
      err = aHisto.binError(AIDA::IAxis::OVERFLOW_BIN,irj);
      sw2 = err*err;
      fSumw2.SetAt(sw2,icell);
    }

    // upper band :
    j = ybinn+1;
    i = 0;
    icell = i*(ybinn+2)+j;
    sw = aHisto.binHeight(AIDA::IAxis::UNDERFLOW_BIN,
                          AIDA::IAxis::OVERFLOW_BIN);
    SetAt(sw,icell);
    err = aHisto.binError(AIDA::IAxis::UNDERFLOW_BIN,
                          AIDA::IAxis::OVERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,icell);
    for(i=1;i<=xbinn;i++) {
      icell = i*(xbinn+2)+j;
      iri = i-1; //in range i index
      //irj = j-1; //in range j index
      sw = aHisto.binHeight(iri,AIDA::IAxis::OVERFLOW_BIN);
      SetAt(sw,icell);
      err = aHisto.binError(iri,AIDA::IAxis::OVERFLOW_BIN);
      sw2 = err*err;
      fSumw2.SetAt(sw2,icell);
    }
    j = 0;
    i = xbinn+1;
    icell = i*(ybinn+2)+j;
    sw = aHisto.binHeight(AIDA::IAxis::OVERFLOW_BIN,
                          AIDA::IAxis::OVERFLOW_BIN);
    SetAt(sw,icell);
    err = aHisto.binError(AIDA::IAxis::OVERFLOW_BIN,
                          AIDA::IAxis::OVERFLOW_BIN);
    sw2 = err*err;
    fSumw2.SetAt(sw2,icell);

    ////////////////////////////////////////////
    /// in range bins : ////////////////////////
    ////////////////////////////////////////////

    //NOTE : with a direct HCL access, we could do :    
    //fTsumw2 = data->getSw2();
    //fTsumwx = data->getIthAxisSxw(0);
    //fTsumwx2 = data->getIthAxisSx2w(0);
    //fTsumwy = data->getIthAxisSxw(1);
    //fTsumwy2 = data->getIthAxisSx2w(1);

    fTsumw2 = 0;
    fTsumwx = 0;
    fTsumwy = 0;
    fTsumwxy = 0;
    for(j=1;j<=ybinn;j++) {
      for(i=1;i<=xbinn;i++) {
        icell = i*(xbinn+2)+j;
        iri = i-1; //in range i index
        irj = j-1; //in range j index

        sw = aHisto.binHeight(iri,irj);
        SetAt(sw,icell);

        err = aHisto.binError(iri,irj);
        sw2 = err*err;
        fSumw2.SetAt(sw2,icell);

        fTsumw2 += sw2;
        fTsumwx += sw * aHisto.binMeanX(iri,irj);
        fTsumwy += sw * aHisto.binMeanY(iri,irj);

        fTsumwxy += 0; //FIXME
      }
    }

    double xrms = aHisto.rmsX();
    double xmean = fTsumwx*fTsumw;
    fTsumwx2 = (xrms*xrms+xmean*xmean)*fTsumw;

    double yrms = aHisto.rmsY();
    double ymean = fTsumwy*fTsumw;
    fTsumwy2 = (yrms*yrms+ymean*ymean)*fTsumw;
  }
  virtual ~ATH2D(){}

  ClassDef(ATH2D,1)
};

// We can't restore a TProfile from a AIDA::IProfile1D of AIDA/v3r2p1.

#endif

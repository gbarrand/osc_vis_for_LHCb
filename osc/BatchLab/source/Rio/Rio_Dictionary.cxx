// this :
#include <BatchLab/Rio/Dictionary.h>

BatchLab::Rio::Dictionary::Dictionary(std::ostream& aOut)
: ::Rio::TreeDictionary(aOut)
{}

BatchLab::Rio::Dictionary::~Dictionary(){}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Rio :
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IPrinter.h>
#include <Rio/Interfaces/IFile.h>
#include <Rio/Core/BaseClass.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>
//FIXME : use dictionary to find version.
#include <Rio/Core/Named.h>
#include <Rio/Core/Array_double.h>
#include <Rio/Core/Array_float.h>
#include <Rio/Graf/AttLine.h>
#include <Rio/Graf/AttFill.h>
#include <Rio/Graf/AttMarker.h>

// BatchLab data :
#include <BatchLab/Rio/THistogram.h>
#include <BatchLab/Rio/AttAxis.h>
#include <BatchLab/Rio/Axis.h>
#include <BatchLab/Rio/ROOT.h>

#include "Rio_Classes.icxx"
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Dictionary::initialize(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!::Rio::TreeDictionary::initialize()) return false;

    {
      ::Rio::IClass* iclass = new AttAxis_Class();
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      // Elements :
      //4 (virtual ~)
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fNdivisions","Number of divisions(10000*n3 + 100*n2 + n1)",4,::Rio::Streamer_Info::INT,"Int_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fAxisColor","color of the line axis",8,::Rio::Streamer_Info::SHORT,"Color_t")); //2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fLabelColor","color of labels",10,::Rio::Streamer_Info::SHORT,"Color_t")); //2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fLabelFont","font for labels",12,::Rio::Streamer_Info::SHORT,"Style_t")); //2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fLabelOffset","offset of labels",16,::Rio::Streamer_Info::FLOAT,"Float_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fLabelSize","size of labels",20,::Rio::Streamer_Info::FLOAT,"Float_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTickLength","length of tick marks",24,::Rio::Streamer_Info::FLOAT,"Float_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTitleOffset","offset of axis title",28,::Rio::Streamer_Info::FLOAT,"Float_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTitleSize","size of axis title",32,::Rio::Streamer_Info::FLOAT,"Float_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTitleColor","color of axis title",36,::Rio::Streamer_Info::SHORT,"Color_t")); //2 
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTitleFont","font for axis title",38,::Rio::Streamer_Info::SHORT,"Style_t")); //2
      //40
    }

    {
      ::Rio::IClass* iclass = new Rio::Axis_Class();
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      // Elements :
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TNamed","The basis for a named object (name, title)",0,::Rio::Named::is(*this).version())); //28
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TAttAxis","Axis attributes",28,Rio::AttAxis::is(*this).version())); //40
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fNbins","Number of bins",68,::Rio::Streamer_Info::INT,"Int_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fXmin","low edge of first bin",72,::Rio::Streamer_Info::DOUBLE,"Axis_t")); //8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fXmax","upper edge of last bin",80,::Rio::Streamer_Info::DOUBLE,"Axis_t")); //8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fXbins","Bin edges array in X",88,::Rio::Streamer_Info::OBJECT_ANY,"TArrayD")); //12
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fFirst","first bin to display",100,::Rio::Streamer_Info::INT,"Int_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fLast","last bin to display",104,::Rio::Streamer_Info::INT,"Int_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTimeDisplay","on/off displaying time values instead of numerics",108,::Rio::Streamer_Info::UNSIGNED_CHAR,"Bool_t")); //4 (Bool_t = 1 + 3 for alignement)
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTimeFormat","Date&time format, ex: 09/12/99 12:34:00",112,::Rio::Streamer_Info::TSTRING,"TString")); //8
      //offset 120 //4
      //offset 124 //4
      //128

      //v3-05-07 : 124 (stored) + 4  = 128
    }

    {
      ::Rio::IClass* iclass = 
        new BatchLab::Rio::THistogram_Class(fOut);
      addClass(iclass);
    }

    {
      ::Rio::IClass* iclass = new TH1_Class();
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TNamed","The basis for a named object (name, title)",0,::Rio::Named::is(*this).version())); //28
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TAttLine","Line attributes",28,::Rio::AttLine::is(*this).version())); //12
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TAttFill","Fill area attributes",40,::Rio::AttFill::is(*this).version())); //8
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TAttMarker","Marker attributes",48,::Rio::AttMarker::is(*this).version())); //12
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fNcells","number of bins(1D), cells (2D) +U/Overflows",60,::Rio::Streamer_Info::INT,"Int_t")); //4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fXaxis","X axis descriptor",64,::Rio::Streamer_Info::OBJECT_ANY,"TAxis")); //128
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fYaxis","Y axis descriptor",192,::Rio::Streamer_Info::OBJECT_ANY,"TAxis")); //128
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fZaxis","Z axis descriptor",320,::Rio::Streamer_Info::OBJECT_ANY,"TAxis")); //128
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fBarOffset","(1000*offset) for bar charts or legos",448,::Rio::Streamer_Info::SHORT,"Short_t"));//2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fBarWidth","(1000*width) for bar charts or legos",450,::Rio::Streamer_Info::SHORT,"Short_t"));//2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fEntries","Number of entries",452,::Rio::Streamer_Info::DOUBLE,"Stat_t"));//2
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumw","Total Sum of weights",460,::Rio::Streamer_Info::DOUBLE,"Stat_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumw2","Total Sum of squares of weights",468,::Rio::Streamer_Info::DOUBLE,"Stat_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumwx","Total Sum of weight*X",476,::Rio::Streamer_Info::DOUBLE,"Stat_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumwx2","Total Sum of weight*X*X",484,::Rio::Streamer_Info::DOUBLE,"Stat_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fMaximum","Maximum value for plotting",492,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fMinimum","Minimum value for plotting",500,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fNormFactor","Normalization factor",508,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fContour","Array to display contour levels",516,::Rio::Streamer_Info::OBJECT_ANY,"TArrayD"));//12
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fSumw2","Array of sum of squares of weights",528,::Rio::Streamer_Info::OBJECT_ANY,"TArrayD"));//12
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fOption","histogram options",540,::Rio::Streamer_Info::TSTRING,"TString"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fFunctions","->Pointer to list of functions (fits and user)",548,::Rio::Streamer_Info::OBJECT_ARROW,"TList*"));//4
      //!fDirectory offset 552 : //4
      //!fDimension offset 556 : //4
      //!fIntegral  offset 560 : //4
      //!fPainter   offset 564 : //4
      //568

      //v3-05-07 : 576  = 568 + 2 * 4 = ok
    }

    {
      ::Rio::IClass* iclass = new Rio::TH1F_Class(fOut);
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH1","1-Dim histogram base class",0,TH1_VERSION));//568
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TArrayF","Array of floats",568,::Rio::Array_float::is(*this).version()));//12
    }

    {
      ::Rio::IClass* iclass = new Rio::TH1D_Class(fOut);
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH1","1-Dim histogram base class",0,TH1_VERSION));//568
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TArrayD","Array of doubles",568,::Rio::Array_double::is(*this).version()));//12
      //580

      //v3-05-07 : 588 = 576 + 12 = ok
    }

    {
      ::Rio::IClass* iclass = new Rio::TProfile_Class(fOut);
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH1D","1-Dim histograms (one double per channel)",0,TH1D_VERSION));//580
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fBinEntries","number of entries per bin",580,::Rio::Streamer_Info::OBJECT_ANY,"TArrayD"));//12
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fErrorMode","Option to compute errors",592,::Rio::Streamer_Info::DOUBLE,"EErrorType"));//4
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fYmin","Lower limit in Y (if set)",596,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fYmax","Upper limit in Y (if set)",604,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      //612

      //v3-05-07 : 624 = 612 + 4 + 8 (diff TH1D) = ok
    }

    {
      ::Rio::IClass* iclass = new Rio::TH2_Class();
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH1","1-Dim histogram base class",0,TH1_VERSION));//568
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fScaleFactor","Scale Factor",576,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumwy","Total Sum of weight*Y",584,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumwy2","Total Sum of weight*Y*Y",592,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      streamerInfo->add(new ::Rio::StreamerBasicType(*this,"fTsumwxy","Total Sum of weight*X*Y",600,::Rio::Streamer_Info::DOUBLE,"Double_t"));//8
      //600

      //v3-05-07 : <to be checked>
    }

    {
      ::Rio::IClass* iclass = new Rio::TH2F_Class(fOut);
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH2","2-Dim histogram base class",0,TH2_VERSION));//600
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TArrayF","Array of floats",600,::Rio::Array_float::is(*this).version()));//12
      //612

    }

    {
      ::Rio::IClass* iclass = new Rio::TH2D_Class(fOut);
      addClass(iclass);

      ::Rio::StreamerInfo* streamerInfo = new ::Rio::StreamerInfo(*this,*iclass,"");
      fStreamerInfos.push_back(streamerInfo);
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TH2","2-Dim histogram base class",0,TH2_VERSION));//600
      streamerInfo->add(new ::Rio::StreamerBase(*this,"TArrayD","Array of doubles",600,::Rio::Array_double::is(*this).version()));//12
      //612

    }

    {
      ::Rio::IClass* iclass = new Rio::TVectorD_Class(fOut);
      addClass(iclass);
    }

    {
      ::Rio::IClass* iclass = new Rio::TMatrix_Class(fOut);
      addClass(iclass);
    }

  return true;
}

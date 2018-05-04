// this :
#include <BatchLab/Rio/ROOT.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Core/List.h>
#include <Rio/Core/Named.h>
#include <Rio/Core/Array_double.h>
#include <Rio/Core/Array_float.h>
#include <Rio/Graf/AttLine.h>
#include <Rio/Graf/AttFill.h>
#include <Rio/Graf/AttMarker.h>

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <inlib/histo/h3d>
#include <inlib/histo/p1d>
#include <inlib/histo/p2d>

// BatchLab :
#include <BatchLab/Rio/Axis.h>

#include <inlib/vmanip>
#include <inlib/sout>

#include <cstdlib> //rand.
#include <cmath>

inline bool null_epsil(double a_1,double a_2,double a_prec = -5) {
  return (::log10(::fabs(a_1-a_2))<a_prec?true:false);
}

BatchLab::Rio::TH::TH(const ::Rio::IDictionary& aDictionary)
:Named(aDictionary,"","")
,fEntries(0)
,fSw(0)
{}

//inlib::histo::base_histo* BatchLab::Rio::TH::hclHistogram() const {return fHistogram;}

const std::string& BatchLab::Rio::TH::annotation() const {return fAnnotation;}

typedef inlib::histo::histo_data<double,unsigned int,double> hd_data;
typedef inlib::histo::profile_data<double,unsigned int,double,double> pd_data;

bool BatchLab::Rio::TH::read1D(::Rio::IBuffer& aBuffer,hd_data& aData){
    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TH::streamTH1 : version %d\n",v);

    // Version 3 streaming (ROOT/v3-00-6).

    if(!Named::stream(aBuffer)) return false;

    aData.m_title = title();

    {::Rio::AttLine dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {::Rio::AttFill dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {::Rio::AttMarker dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    int Ncells;
    if(!aBuffer.read(Ncells)) return false;

    //fXAxis
   {Rio::Axis axis(dictionary(),aData.m_axes[0]);
    if(!axis.stream(aBuffer)) return false;}
    aData.m_axes[0].m_offset = 1;

    if(aData.m_dimension==2) {
      Rio::Axis yaxis(dictionary(),aData.m_axes[1]);
      if(!yaxis.stream(aBuffer)) return false; //fYAxis
      aData.m_axes[1].m_offset = 
        aData.m_axes[0].m_offset * (aData.m_axes[0].bins()+2);

      inlib::histo::axis<double> dummy;
      Rio::Axis zaxis(dictionary(),dummy);
      if(!zaxis.stream(aBuffer)) return false; //fZAxis
    } else {
      inlib::histo::axis<double> dummy;
      Rio::Axis yaxis(dictionary(),dummy);
      if(!yaxis.stream(aBuffer)) return false; //fYAxis
      Rio::Axis zaxis(dictionary(),dummy);
      if(!zaxis.stream(aBuffer)) return false; //fZAxis
    }

    short barOffset;
    if(!aBuffer.read(barOffset)) return false;

    short barWidth;
    if(!aBuffer.read(barWidth)) return false;

    if(!aBuffer.read(fEntries)) return false;

    if(!aBuffer.read(fSw)) return false;
    //printf("debug : BatchLab::Rio::TH::streamTH1 : \"%s\" %g %g\n",
      //aData.m_title.c_str(),fEntries,fSw);

    double sw2;
    if(!aBuffer.read(sw2)) return false;

    double xSxw;
    if(!aBuffer.read(xSxw)) return false;

    double xSx2w;
    if(!aBuffer.read(xSx2w)) return false;

    double max;
    if(!aBuffer.read(max)) return false;

    double min;
    if(!aBuffer.read(min)) return false;

    double NormFactor;
    if(!aBuffer.read(NormFactor)) return false;

    {::Rio::Array_double dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;} //fContour TArrayD

    ::Rio::Array_double sumw2(dictionary()); //fSumw2 TArrayD
    if(!sumw2.stream(aBuffer)) return false;
    
   {std::string opt;
    if(!aBuffer.read(opt)) return false; //TString fOption
    //look if it is an "annotation trick" :
    if(opt.size()&&(opt[0]==0)) {
      fAnnotation = opt.substr(1,opt.size()-1);
    }}

    {::Rio::List dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;} //Functions

    if(v>=4) {
      int BufferSize;
      if(!aBuffer.read(BufferSize)) return false;
      
      //Double_t[fBufferSize]
      {char isArray;
      if(!aBuffer.read(isArray)) return false;
      if(isArray!=0) {
        if(BufferSize) {
          double* Buffer = new double[BufferSize];
          if(!aBuffer.readFastArray(Buffer,BufferSize)) return false;
          delete [] Buffer;
        }
      }}
    }

    // Add two for outflows.
    if(aData.m_dimension==1) {
      aData.m_bin_number = aData.m_axes[0].m_number_of_bins + 2;
    } else if(aData.m_dimension==2) {
      aData.m_bin_number = 
        (aData.m_axes[0].m_number_of_bins + 2) * 
	(aData.m_axes[1].m_number_of_bins + 2);
    }

    unsigned int binn = aData.m_bin_number;
    aData.m_bin_Sw2.resize(binn,0);
    if(binn==sumw2.size()) {
      for(unsigned int index=0;index<binn;index++){
        aData.m_bin_Sw2[index] = sumw2.array()[index];
      }
    }

    if(!aBuffer.checkByteCount(s,c,"TH1")) return false;

  return true;
}

bool BatchLab::Rio::TH::write1D(::Rio::IBuffer& aBuffer,const hd_data& aData){
    if(!aBuffer.writeVersion(3)) return false;

    if(!Named::stream(aBuffer)) return false;

    {::Rio::AttLine dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {::Rio::AttFill dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {::Rio::AttMarker dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    if(!aBuffer.write((int)aData.m_bin_number)) return false;

    //fXAxis
   {inlib::histo::axis<double> haxis(aData.m_axes[0]); //have a copy because of const.
    Rio::Axis axis(dictionary(),haxis);
    axis.setName("xaxis");
    if(!axis.stream(aBuffer)) return false;}

    if(aData.m_dimension==2) {
     {inlib::histo::axis<double> haxis(aData.m_axes[1]); //have a copy because of const.
      Rio::Axis yaxis(dictionary(),haxis);
      yaxis.setName("yaxis");
      if(!yaxis.stream(aBuffer)) return false;}

      inlib::histo::axis<double> dummy;
      dummy.configure(1,0,1);
      Rio::Axis zaxis(dictionary(),dummy);
      zaxis.setName("zaxis");
      if(!zaxis.stream(aBuffer)) return false;
    } else {
      inlib::histo::axis<double> dummy;
      dummy.configure(1,0,1);

      Rio::Axis yaxis(dictionary(),dummy);
      yaxis.setName("yaxis");
      if(!yaxis.stream(aBuffer)) return false;

      Rio::Axis zaxis(dictionary(),dummy);
      zaxis.setName("zaxis");
      if(!zaxis.stream(aBuffer)) return false;
    }

    if(!aBuffer.write((short)(1000 * 0.25))) return false; //fBarOffset
    if(!aBuffer.write((short)(1000 * 0.5))) return false; //fBarWidth

    if(!aBuffer.write((double)aData.get_entries())) return false;
    if(!aBuffer.write(aData.get_Sw())) return false;
    if(!aBuffer.write(aData.get_Sw2())) return false;

   {double value;
    aData.get_ith_axis_Sxw(0,value);  
    if(!aBuffer.write(value)) return false;}

   {double value;
    aData.get_ith_axis_Sx2w(0,value);  
    if(!aBuffer.write(value)) return false;}

    if(!aBuffer.write((double)-1111)) return false; //fMaximum
    if(!aBuffer.write((double)-1111)) return false; //fMinimum
    if(!aBuffer.write((double)0)) return false; //NormFactor

    {::Rio::Array_double dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;} //fContour TArrayD

    {::Rio::Array_double sumw2(dictionary(),aData.m_bin_Sw2);
    if(!sumw2.stream(aBuffer)) return false;} //fSumw2 TArrayD

    // store annotation on fOption    
    // but try to fool ROOT in order that it does not
    // understand fOption as.. ROOT options !
   {std::string opt = " "+fAnnotation;   
    opt[0] = 0; //awfull trick
    if(!aBuffer.write(opt)) return false;} //TString fOption

    {::Rio::List dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;} //Functions

  return true;
}

bool BatchLab::Rio::TH::read2D(::Rio::IBuffer& aBuffer,hd_data& aData) {
  unsigned int s, c;
  short v;
  if(!aBuffer.readVersion(v,s,c)) return false;

  //printf("debug : BatchLab::Rio::TH::streamTH2 : version %d\n",v);

  // Version 3 streaming (ROOT/v3-00-6).

  if(!read1D(aBuffer,aData)) return false;

  double ScaleFactor;
  if(!aBuffer.read(ScaleFactor)) return false;
  double Tsumwy;
  if(!aBuffer.read(Tsumwy)) return false;
  double Tsumwy2;
  if(!aBuffer.read(Tsumwy2)) return false;
  double Tsumwxy;
  if(!aBuffer.read(Tsumwxy)) return false;

  if(!aBuffer.checkByteCount(s,c,"TH2")) return false;

  return true;
}

bool BatchLab::Rio::TH::write2D(::Rio::IBuffer& aBuffer,const hd_data& aData) {
  if(!aBuffer.writeVersion(3)) return false;

  if(!write1D(aBuffer,aData)) return false;

  if(!aBuffer.write((double)1)) return false; //ScaleFactor

 {double value;
  aData.get_ith_axis_Sxw(1,value);  
  if(!aBuffer.write(value)) return false;}
 {double value;
  aData.get_ith_axis_Sx2w(1,value);  
  if(!aBuffer.write(value)) return false;}

  if(!aBuffer.write((double)0)) return false; //Tsumwxy //FIXME.

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TH1F)

BatchLab::Rio::TH1F::TH1F(const ::Rio::IDictionary& aDictionary)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH1F::is(aDictionary))
,fHistogram(0)
{}

BatchLab::Rio::TH1F::TH1F(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::h1d* aHisto
,const std::string& aName
)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH1F::is(aDictionary))
,fHistogram(aHisto) //do not take ownership.
{
  setName(aName);
  setTitle(aHisto->get_title());
}

void* BatchLab::Rio::TH1F::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TH1F);
  else return Named::cast(a_class);
}
bool BatchLab::Rio::TH1F::stream(::Rio::IBuffer& aBuffer){
  if (aBuffer.isReading()) {

    fHistogram = 0;

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TH1F::stream : version %d\n",v);

    // Version 1 streaming (ROOT/v3-00-6).

    // Now we have to reconstruct a valid Histogram from aBuffer :
    hd_data data;

    data.m_dimension = 1;
    //data.m_coords.resize(data.m_dimension,0);
    //data.m_ints.resize(data.m_dimension,0);
    data.m_axes.resize(1);

    if(!read1D(aBuffer,data)) return false;

    ::Rio::Array_float bins(dictionary()); //fArray TArrayF
    if(!bins.stream(aBuffer)) return false; 

    unsigned int binn = data.m_bin_number;
    //printf("debug : BatchLab::Rio::TH1F::stream : histo bins %d\n",binn);
    data.m_bin_Sw.resize(binn,0);
    double asw = 0;
   {for(unsigned int index=0;index<binn;index++){
      double h = double(bins.array()[index]);
      data.m_bin_Sw[index] = h;
      asw += h;
     }}
   {double sw = data.get_Sw();
    //if(sw!=fSw) {
    if(!null_epsil(sw,fSw)) {
      std::ostream& out = aBuffer.out();
      out << "BatchLab::Rio::TH1F::stream : "
          << " WARNING : inconsistent total weight" 
          << " for histo with title " << inlib::sout(data.m_title) << " :"
          << std::endl
          << " read fSw is " << fSw  
          << " whilst sum of in-range bins weight is " << sw
          << " (diff is " << (sw-fSw) << ")."
          << std::endl;
    }}

    // Fill Sxw and Sx2w by using in range bins :
    std::vector<double> empty;
    empty.resize(1,0);
    data.m_bin_Sxw.resize(binn,empty);
    data.m_bin_Sx2w.resize(binn,empty);    
    std::vector<int> is(data.m_dimension);
   {for(unsigned int index=0;index<binn;index++){
      if(!data.is_out(index)) {
        float height = bins.array()[index];
        data.get_indices(index,is);
        for(unsigned int iaxis=0;iaxis<data.m_dimension;iaxis++) {
          double x = data.m_axes[iaxis].bin_center(is[iaxis]); 
          data.m_bin_Sxw[index][iaxis] = x * height;
          data.m_bin_Sx2w[index][iaxis] = x * x * height;
        }
      }
    }}

    //ROOT does not store the number of entries per bin.
    // We have the global number of entries and weight ; with that
    // we do our best....
    data.m_bin_entries.resize(binn,0);
    if(asw) {
      for(unsigned int index=0;index<binn;index++){
        float height = bins.array()[index];
        int number = (int)((fEntries * height) / asw);
        if(number<0) {
        } else {
          data.m_bin_entries[index] = number;
        }
      }
      int allEntries = data.get_all_entries();
      if(allEntries<int(fEntries)) { //FIXME
        //Correct some bins randomly (beurk, do you have a better idea ?) :
        int diff = int(fEntries)-allEntries;
        unsigned int nx = data.m_axes[0].m_number_of_bins;
        for(int i=0;i<diff;i++) {
          int ri = ::rand();
          // ibin in [1,nx-1]
          int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
          int offset = ibin;
          data.m_bin_entries[offset]++;
        }
      } else if(allEntries>int(fEntries)) { //FIXME
        std::ostream& out = aBuffer.out();
        out << "BatchLab::Rio::TH1F::stream : "
            << " WARNING : can't reemulate number of entries per bin " 
            << " for histo with title " << inlib::sout(data.m_title) << " :"
            << std::endl
            << " read fEntries (a double) is " << fEntries  
            << " whilst corrected bin entries is " << allEntries
            << " (diff is " << (allEntries-int(fEntries)) << ")."
            << std::endl;
      } else {
      }
    }

    if(!aBuffer.checkByteCount(s,c,"TH1F")) return false;

    fHistogram = new inlib::histo::h1d("",10,0,1);
    fHistogram->copy_from_data(data);
    fHistogram->update_fast_getters();
    // We have now a valid HCL histogram.

  } else {
    if(!aBuffer.writeVersion(1)) return false;

    hd_data data = fHistogram->get_histo_data();
    if(!write1D(aBuffer,data)) return false;

   {::Rio::Array_float 
     sumw(dictionary(),inlib::convert<double,float>(data.m_bin_Sw));
    if(!sumw.stream(aBuffer)) return false;} //TH1D::TArrayD::fArray

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TH1D)
BatchLab::Rio::TH1D::TH1D(const ::Rio::IDictionary& aDictionary)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH1D::is(aDictionary))
,fHistogram(0)
{}

BatchLab::Rio::TH1D::TH1D(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::h1d* aHisto
,const std::string& aAnnotation
,const std::string& aName
)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH1D::is(aDictionary))
,fHistogram(aHisto)
{
  fAnnotation = aAnnotation;
  setName(aName);
  setTitle(aHisto->get_title());
}

void* BatchLab::Rio::TH1D::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TH1D);
  else return Named::cast(a_class);
}
bool BatchLab::Rio::TH1D::stream(::Rio::IBuffer& aBuffer){
  return stream_it(aBuffer,false);
}
bool BatchLab::Rio::TH1D::stream_it(::Rio::IBuffer& aBuffer,bool a_profile){
  if (aBuffer.isReading()) {

    fHistogram = 0;

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TH1D::stream : version %d\n",v);

    // Version 1 streaming (ROOT/v3-00-6).

    // Now we have to reconstruct a valid Histogram from aBuffer :
    hd_data data;

    data.m_dimension = 1;
    //data.m_coords.resize(data.m_dimension,0);
    //data.m_ints.resize(data.m_dimension,0);
    data.m_axes.resize(1);

    if(!read1D(aBuffer,data)) return false;

    ::Rio::Array_double bins(dictionary()); //fArray TArrayD
    if(!bins.stream(aBuffer)) return false; 

    unsigned int binn = data.m_bin_number;
    //printf("debug : BatchLab::Rio::TH1D::stream : histo bins %d\n",binn);
    data.m_bin_Sw.resize(binn,0);
    double asw = 0;
   {for(unsigned int index=0;index<binn;index++){
      double h = bins.array()[index];
      data.m_bin_Sw[index] = h;
      asw += h;
    }}
    if(!a_profile) {
    double sw = data.get_Sw();
    if(!null_epsil(sw,fSw)) {
      std::ostream& out = aBuffer.out();
      out << "BatchLab::Rio::TH1D::stream : "
          << " WARNING : inconsistent total weight" 
          << " for histo with title " << inlib::sout(data.m_title) << " :"
          << std::endl
          << " read fSw is " << fSw  
          << " whilst sum of in-range bins weight is " << sw
          << " (diff is " << (sw-fSw) << ")."
          << std::endl;
    }}

    // Fill Sxw and Sx2w by using in range bins :
    std::vector<double> empty;
    empty.resize(1,0);
    data.m_bin_Sxw.resize(binn,empty);
    data.m_bin_Sx2w.resize(binn,empty);
    std::vector<int> is(data.m_dimension);
    for(unsigned int index=0;index<binn;index++){
      if(!data.is_out(index)) {
        double height = bins.array()[index];
        data.get_indices(index,is);
        for(unsigned int iaxis=0;iaxis<data.m_dimension;iaxis++) {
          double x = data.m_axes[iaxis].bin_center(is[iaxis]); 
          data.m_bin_Sxw[index][iaxis] = x * height;
          data.m_bin_Sx2w[index][iaxis] = x * x * height;
        }
      }
    }

    //ROOT does not store the number of entries per bin.
    // We have the global number of entries and weight ; with that
    // we do our best....
    data.m_bin_entries.resize(binn,0);
    if(!a_profile && asw) {
      for(int index=0;index<binn;index++){
        double height = bins.array()[index];
        int number = (int)((fEntries * height) / asw);        
        if(number<0) {
        } else {
          data.m_bin_entries[index] = number;
        }
      }
      int allEntries = data.get_all_entries();
      if(allEntries<int(fEntries)) { //FIXME
        //Correct some bins randomly (beurk, do you have a better idea ?) :
        int diff = int(fEntries)-allEntries;
        unsigned int nx = data.m_axes[0].m_number_of_bins;
        for(int i=0;i<diff;i++) {
          int ri = ::rand();
          int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
          int offset = ibin;
          data.m_bin_entries[offset]++;
        }
      } else if(allEntries>int(fEntries)) { //FIXME
        std::ostream& out = aBuffer.out();
        out << "BatchLab::Rio::TH1D::stream : "
            << " WARNING : can't reemulate number of entries per bin " 
            << " for histo with title " << inlib::sout(data.m_title) << " :"
            << std::endl
            << " read fEntries (a double) is " << fEntries  
            << " whilst corrected bin entries is " << allEntries
            << " (diff is " << (allEntries-int(fEntries)) << ")."
            << std::endl;
      } else {
      }
    }

    if(!aBuffer.checkByteCount(s,c,"TH1D")) return false;

    fHistogram = new inlib::histo::h1d("",10,0,1);
    fHistogram->copy_from_data(data);
    fHistogram->update_fast_getters();
    // We have now a valid HCL histogram.

  } else {

    if(!aBuffer.writeVersion(1)) return false;

    hd_data data = fHistogram->get_histo_data();
    if(!write1D(aBuffer,data)) return false;

    {::Rio::Array_double sumw(dictionary(),data.m_bin_Sw);
    if(!sumw.stream(aBuffer)) return false;} //fArray TArrayD

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TH2F)
BatchLab::Rio::TH2F::TH2F(const ::Rio::IDictionary& aDictionary)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH2F::is(aDictionary))
,fHistogram(0)
{}

BatchLab::Rio::TH2F::TH2F(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::h2d* aHisto
,const std::string& aName
)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH2F::is(aDictionary))
,fHistogram(aHisto)
{
  setName(aName);
  setTitle(aHisto->get_title());
}

void* BatchLab::Rio::TH2F::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TH2F);
  else return Named::cast(a_class);
}
bool BatchLab::Rio::TH2F::stream(::Rio::IBuffer& aBuffer){
  if (aBuffer.isReading()) {

    fHistogram = 0;

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TH2F::stream : version %d\n",v);

    // Version 3 streaming (ROOT/v3-00-6).

    // Now we have to reconstruct a valid Histogram from aBuffer :
    hd_data data;

    data.m_dimension = 2;
    //data.m_coords.resize(data.m_dimension,0);
    //data.m_ints.resize(data.m_dimension,0);
    data.m_axes.resize(2);

    if(!read2D(aBuffer,data)) return false;

    ::Rio::Array_float bins(dictionary()); //fArray TArrayF
    if(!bins.stream(aBuffer)) return false; 

    //printf("debug : BatchLab::Rio::TH2F::stream : histo bins %d\n",n);
    unsigned int binn = data.m_bin_number;
    data.m_bin_Sw.resize(binn,0);
    double asw = 0;
   {for(unsigned int index=0;index<binn;index++){
      double h = double(bins.array()[index]);
      data.m_bin_Sw[index] = h;
      asw += h;
    }}
   {double sw = data.get_Sw();
    //if(sw!=fSw) {
    if(!null_epsil(sw,fSw)) {
      std::ostream& out = aBuffer.out();
      out << "BatchLab::Rio::TH2F::stream : "
          << " WARNING : inconsistent total weight" 
          << " for histo with title " << inlib::sout(data.m_title) << " :"
          << std::endl
          << " read fSw is " << fSw  
          << " whilst sum of in-range bins weight is " << sw
          << " (diff is " << (sw-fSw) << ")."
          << std::endl;
    }}

    // Fill Sxw and Sx2w by using in range bins :
    std::vector<double> empty;
    empty.resize(2,0);
    data.m_bin_Sxw.resize(binn,empty);
    data.m_bin_Sx2w.resize(binn,empty);
    std::vector<int> is(data.m_dimension);
    for(unsigned int index=0;index<binn;index++){
      if(!data.is_out(index)) {
        float height = bins.array()[index];
        data.get_indices(index,is);
        for(unsigned int iaxis=0;iaxis<data.m_dimension;iaxis++) {
          double x = data.m_axes[iaxis].bin_center(is[iaxis]); 
          data.m_bin_Sxw[index][iaxis] = x * height;
          data.m_bin_Sx2w[index][iaxis] = x * x * height;
        }
      }
    }

    //ROOT does not store the number of entries per bin.
    // We have the global number of entries and weight ; with that
    // we do our best....
    data.m_bin_entries.resize(binn,0);
    if(asw) {
      for(unsigned int index=0;index<binn;index++){
        float height = bins.array()[index];
        int number = (int)((fEntries * height) / asw);
        if(number<0) {
        } else {
          data.m_bin_entries[index] = number;
        } 
      }
      int allEntries = data.get_all_entries();
      if(allEntries<(int)fEntries) { //FIXME
        //Correct some bins randomly (beurk, do you have a better idea ?) :
        int diff = int(fEntries-allEntries);
        unsigned int nx = data.m_axes[0].m_number_of_bins;
        unsigned int ny = data.m_axes[1].m_number_of_bins;
        for(int i=0;i<diff;i++) {
          int ri = ::rand();
          int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
          int rj = ::rand();
          int jbin =  int((ny-1) * ((double)rj/(double)RAND_MAX)) + 1;
          int offset = ibin + jbin * data.m_axes[1].m_offset;
          data.m_bin_entries[offset]++;
        }
      } else if(allEntries>int(fEntries)) { //FIXME
        std::ostream& out = aBuffer.out();
        out << "BatchLab::Rio::TH2F::stream : "
            << " WARNING : can't reemulate number of entries per bin " 
            << " for histo with title " << inlib::sout(data.m_title) << " :"
            << std::endl
            << " read fEntries (a double) is " << fEntries  
            << " whilst corrected bin entries is " << allEntries
            << " (diff is " << (allEntries-int(fEntries)) << ")."
            << std::endl;
      } else {
      }
    }
    
    if(!aBuffer.checkByteCount(s,c,"TH2F")) return false;

    fHistogram = new inlib::histo::h2d("",10,0,1,10,0,1);
    fHistogram->copy_from_data(data);
    fHistogram->update_fast_getters();
    // We have now a valid HCL histogram.

  } else {

    if(!aBuffer.writeVersion(3)) return false;

    hd_data data = fHistogram->get_histo_data();
    if(!write2D(aBuffer,data)) return false;

   {::Rio::Array_float 
       sumw(dictionary(),inlib::convert<double,float>(data.m_bin_Sw));
    if(!sumw.stream(aBuffer)) return false;} //fArray TArrayD

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TH2D)
BatchLab::Rio::TH2D::TH2D(const ::Rio::IDictionary& aDictionary)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH2D::is(aDictionary))
,fHistogram(0)
{}

BatchLab::Rio::TH2D::TH2D(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::h2d* aHisto
,const std::string& aAnnotation
,const std::string& aName
)
:BatchLab::Rio::TH(aDictionary)
,fClass(BatchLab::Rio::TH2D::is(aDictionary))
,fHistogram(aHisto)
{
  fAnnotation = aAnnotation;
  setName(aName);
  setTitle(aHisto->get_title());
}

void* BatchLab::Rio::TH2D::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TH2D);
  else return Named::cast(a_class);
}
bool BatchLab::Rio::TH2D::stream(::Rio::IBuffer& aBuffer) {
  if (aBuffer.isReading()) {

    fHistogram = 0;

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TH2D::stream : version %d\n",v);

    // Version 3 streaming (ROOT/v3-00-6).

    // Now we have to reconstruct a valid Histogram from aBuffer :
    hd_data data;

    data.m_dimension = 2;
    //data.m_coords.resize(data.m_dimension,0);
    //data.m_ints.resize(data.m_dimension,0);
    data.m_axes.resize(2);

    if(!read2D(aBuffer,data)) return false;

    ::Rio::Array_double bins(dictionary()); //fArray TArrayD
    if(!bins.stream(aBuffer)) return false; 

    //printf("debug : BatchLab::Rio::TH2D::stream : histo bins %d\n",n);
    unsigned int binn = data.m_bin_number;
    data.m_bin_Sw.resize(binn,0);
    double asw = 0;
   {for(unsigned int index=0;index<binn;index++){
      double h = bins.array()[index];
      data.m_bin_Sw[index] = h;
      asw += h;
     }}
   {double sw = data.get_Sw();
    //if(sw!=fSw) {
    if(!null_epsil(sw,fSw)) {
      std::ostream& out = aBuffer.out();
      out << "BatchLab::Rio::TH2D::stream : "
          << " WARNING : inconsistent total weight" 
          << " for histo with title " << inlib::sout(data.m_title) << " :"
          << std::endl
          << " read fSw is " << fSw  
          << " whilst sum of in-range bins weight is " << sw
          << " (diff is " << (sw-fSw) << ")."
          << std::endl;
    }}

    // Fill Sxw and Sx2w by using in range bins :
    std::vector<double> empty;
    empty.resize(2,0);
    data.m_bin_Sxw.resize(binn,empty);
    data.m_bin_Sx2w.resize(binn,empty);
    std::vector<int> is(data.m_dimension);
    for(unsigned int index=0;index<binn;index++){
      if(!data.is_out(index)) {
        double height = bins.array()[index];
        data.get_indices(index,is);
        for(unsigned int iaxis=0;iaxis<data.m_dimension;iaxis++) {
          double x = data.m_axes[iaxis].bin_center(is[iaxis]); 
          data.m_bin_Sxw[index][iaxis] = x * height;
          data.m_bin_Sx2w[index][iaxis] = x * x * height;
        }
      }
    }

    //ROOT does not store the number of entries per bin.
    // We have the global number of entries and weight ; with that
    // we do our best....
    data.m_bin_entries.resize(binn,0);
    if(asw) {
      for(unsigned int index=0;index<binn;index++){
        double height = bins.array()[index];
        int number = (int)((fEntries * height) / asw);
        if(number<0) {
        } else {
          data.m_bin_entries[index] = number;
        } 
      }
      int allEntries = data.get_all_entries();
      if(allEntries<(int)fEntries) { //FIXME
        //Correct some bins randomly (beurk, do you have a better idea ?) :
        int diff = int(fEntries-allEntries);
        unsigned int nx = data.m_axes[0].m_number_of_bins;
        unsigned int ny = data.m_axes[1].m_number_of_bins;
        for(int i=0;i<diff;i++) {
          int ri = ::rand();
          int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
          int rj = ::rand();
          int jbin =  int((ny-1) * ((double)rj/(double)RAND_MAX)) + 1;
          int offset = ibin + jbin * data.m_axes[1].m_offset;
          data.m_bin_entries[offset]++;
        }
      } else if(allEntries>int(fEntries)) { //FIXME
        std::ostream& out = aBuffer.out();
        out << "BatchLab::Rio::TH2D::stream : "
            << " WARNING : can't reemulate number of entries per bin " 
            << " for histo with title " << inlib::sout(data.m_title) << " :"
            << std::endl
            << " read fEntries (a double) is " << fEntries  
            << " whilst corrected bin entries is " << allEntries
            << " (diff is " << (allEntries-int(fEntries)) << ")."
            << std::endl;
      } else {
      }
    }
    
    if(!aBuffer.checkByteCount(s,c,"TH2D")) return false;

    fHistogram = new inlib::histo::h2d("",10,0,1,10,0,1);
    fHistogram->copy_from_data(data);
    fHistogram->update_fast_getters();
    // We have now a valid HCL histogram.

  } else {

    if(!aBuffer.writeVersion(3)) return false;

    hd_data data = fHistogram->get_histo_data();
    if(!write2D(aBuffer,data)) return false;

    {::Rio::Array_double sumw(dictionary(),data.m_bin_Sw);
    if(!sumw.stream(aBuffer)) return false;} //fArray TArrayD

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TProfile)
BatchLab::Rio::TProfile::TProfile(const ::Rio::IDictionary& aDictionary)
:BatchLab::Rio::TH1D(aDictionary)
,fClass(BatchLab::Rio::TProfile::is(aDictionary))
,fProfile(0)
{}

BatchLab::Rio::TProfile::TProfile(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::p1d* aHisto
,const std::string& aAnnotation
,const std::string& aName
)
:BatchLab::Rio::TH1D(aDictionary)
,fClass(BatchLab::Rio::TProfile::is(aDictionary))
,fProfile(aHisto)
{
  fAnnotation = aAnnotation;
  setName(aName);
  setTitle(aHisto->get_title());
}

void* BatchLab::Rio::TProfile::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TProfile);
  else return BatchLab::Rio::TH1D::cast(a_class);
}

bool BatchLab::Rio::TProfile::stream(::Rio::IBuffer& aBuffer){
  if (aBuffer.isReading()) {

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TVectorD::stream : version %d\n",v);

    // Version 3 streaming (ROOT/v3-00-6).

    //WARNING : the mapping inlib::histo::p1d / TProfile is not obvious.
    //HCL::m_bin_Svw  <---> TProfile::fArray
    //HCL::m_bin_Sv2w <---> TProfile::fSumw2
    //HCL::m_bin_Sw   <---> TProfile::fBinEntries

    if(!BatchLab::Rio::TH1D::stream_it(aBuffer,true)) return false;
    //NOTE : histo.m_bin_Sw <---> TH1D::TArrayD::fArray

    //WARNING : should have a valid Rio::TH1D::fHistogram
    //          being a inlib::histo::h1d.
    //          But the inlib::histo::p1d does not inherit inlib::histo::h1d.

    pd_data data(fHistogram->get_histo_data());
    delete fHistogram;
    fHistogram = 0;

    ::Rio::Array_double bins(dictionary()); //fBinEntries TArrayD
    if(!bins.stream(aBuffer)) return false; 
    int errorMode;
    if(!aBuffer.read(errorMode)) return false;
    double ymin;
    if(!aBuffer.read(ymin)) return false;
    double ymax;
    if(!aBuffer.read(ymax)) return false;

    if(v>=4) {
      double  sumwy;
      if(!aBuffer.read(sumwy)) return 0;
      double   sumwy2;
      if(!aBuffer.read(sumwy2)) return 0;
      ::Rio::Array_double bins_sumw2(dictionary()); //fBinSumw2 TArrayD
      if(!bins_sumw2.stream(aBuffer)) return 0;
    }

    data.m_is_profile = true;
    data.m_cut_v = true;
    data.m_min_v = ymin;
    data.m_max_v = ymax;

    unsigned int binn = data.m_bin_number;
    data.m_bin_Svw.resize(binn,0);
    data.m_bin_Sv2w.resize(binn,0);

    for(unsigned int index=0;index<binn;index++){
      double svw = data.m_bin_Sw[index];
      double sw = bins.array()[index];
      data.m_bin_entries[index] = (int)sw; //FIXME : ok for w = 1 only !
      data.m_bin_Sw[index] = (double)sw;
      //FIXME : data.m_bin_Sxw
      //FIXME : data.m_bin_Sx2w
      data.m_bin_Svw[index] = svw;
      data.m_bin_Sv2w[index] = 0; //FIXME
    }

    if(!aBuffer.checkByteCount(s,c,"TProfile")) return false;

    fProfile = new inlib::histo::p1d("",10,0,1);
    fProfile->copy_from_data(data);
    fProfile->update_fast_getters();
    // We have now a valid inlib::histo::p1d.

  } else {

    if(!aBuffer.writeVersion(3)) return false;

    //WARNING : the mapping inlib::histo::p1d / TProfile is not obvious.
    //HCL::m_bin_Svw  <---> TProfile::fArray
    //HCL::m_bin_Sv2w <---> TProfile::fSumw2
    //HCL::m_bin_Sw   <---> TProfile::fBinEntries

    pd_data data = fProfile->get_histo_data();
    //NOTE : histo.m_bin_Sw <---> TH1D::TArrayD::fArray
    //       then have to copy histo.m_bin_Svw on histo.m_bin_Sw
    //       before streaming.

    //save histo.m_bin_Sw on a ::Rio::Array_double :
    ::Rio::Array_double bins(dictionary()); //fBinEntries TArrayD
   {unsigned int binn = data.m_bin_number;
    bins.resize(binn);
    double* array = bins.array();
    for(unsigned int index=0;index<binn;index++){
      array[index] = data.m_bin_Sw[index];
    }}

    // copy histo.m_bin_Svw on histo.m_bin_Sw :
    data.m_bin_Sw = data.m_bin_Svw;    

    //WARNING : should create a valid Rio::TH1D::fHistogram
    //          being a inlib::histo::h1d.
    //          But the inlib::histo::p1d does not inherit inlib::histo::h1d.

    fHistogram = new inlib::histo::h1d("",10,0,1);
    fHistogram->copy_from_data(data);
    fHistogram->update_fast_getters();
    if(!BatchLab::Rio::TH1D::stream(aBuffer)) return false;
    delete fHistogram;
    fHistogram = 0;

    //TProfile specific :
    if(!bins.stream(aBuffer)) return false; //fBinEntries TArrayD
    int errorMode = 0;
    if(!aBuffer.write(errorMode)) return false;
    if(!aBuffer.write(data.m_min_v)) return false;
    if(!aBuffer.write(data.m_max_v)) return false;

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TVectorD)
BatchLab::Rio::TVectorD::TVectorD(const ::Rio::IDictionary& aDictionary)
: ::Rio::Object(aDictionary)
,fClass(BatchLab::Rio::TVectorD::is(aDictionary))
{}

void* BatchLab::Rio::TVectorD::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TVectorD);
  else return Object::cast(a_class);
}
bool BatchLab::Rio::TVectorD::stream(::Rio::IBuffer& aBuffer){
  if (aBuffer.isReading()) {

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TVectorD::stream : version %d\n",v);

    // Version 2 streaming (ROOT/v3-00-6).

    if(!Object::stream(aBuffer)) return false;

    int Rows;
    if(!aBuffer.read(Rows)) return false;
    int RowLwb;
    if(!aBuffer.read(RowLwb)) return false;

    //Double_t[fNrows]
    {char isArray;
    if(!aBuffer.read(isArray)) return false;
    if(isArray!=0) {
      if(Rows) {
        double* Elements = new double[Rows];
        if(!aBuffer.readFastArray(Elements,Rows)) return false;
        delete [] Elements;
      }
    }}

    if(!aBuffer.checkByteCount(s,c,"TVectorD")) return false;

  } else {

    //FIXME
    return false;

  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
RIO_META_SOURCE(BatchLab::Rio::TMatrix)
BatchLab::Rio::TMatrix::TMatrix(const ::Rio::IDictionary& aDictionary)
: ::Rio::Object(aDictionary)
,fClass(BatchLab::Rio::TMatrix::is(aDictionary))
{}

void* BatchLab::Rio::TMatrix::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) return Rio_SCast(BatchLab::Rio::TMatrix);
  else return Object::cast(a_class);
}
bool BatchLab::Rio::TMatrix::stream(::Rio::IBuffer& aBuffer){
  if (aBuffer.isReading()) {

    unsigned int s, c;
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;

    //printf("debug : BatchLab::Rio::TMatrix::stream : version %d\n",v);

    // Version 2 streaming (ROOT/v3-00-6).

    if(!Object::stream(aBuffer)) return false;

    int Nrows;
    if(!aBuffer.read(Nrows)) return false;
    int Ncols;
    if(!aBuffer.read(Ncols)) return false;
    int Nelems;
    if(!aBuffer.read(Nelems)) return false;
    int RowLwb;
    if(!aBuffer.read(RowLwb)) return false;
    int ColLwb;
    if(!aBuffer.read(ColLwb)) return false;

    //Real_t[fNelems]
    {char isArray;
    if(!aBuffer.read(isArray)) return false;
    if(isArray!=0) {
      if(Nelems) {
        float* Elements = new float[Nelems];
        if(!aBuffer.readFastArray(Elements,Nelems)) return false;
        delete [] Elements;
      }
    }}

    if(!aBuffer.checkByteCount(s,c,"TMatrix")) return false;

  } else {

    //FIXME
    return false;

  }
  return true;
}


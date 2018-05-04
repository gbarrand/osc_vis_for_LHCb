// this :
#include <BatchLab/Rio/THistogram.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <inlib/histo/h3d>
#include <inlib/histo/p1d>
#include <inlib/histo/p2d>
#include <inlib/vmanip>

// BatchLab :
#include <BatchLab/Core/Histogram.h>
#include <BatchLab/Rio/Annotation.h>

//typedef inlib::histo::histo_data<double,unsigned int,double> hd_data;
typedef inlib::histo::profile_data<double,unsigned int,double,double> pd_data;

static bool read_v0(::Rio::IBuffer&,pd_data&);
static bool read_v1(::Rio::IBuffer&,pd_data&);
static bool read_v2(::Rio::IBuffer&,pd_data&);
static bool read_v3(::Rio::IBuffer&,pd_data&);
static bool read_v4(::Rio::IBuffer&,pd_data&);
static bool read_v5(::Rio::IBuffer&,pd_data&);
static bool read_v6(::Rio::IBuffer&,pd_data&);
static bool read_v7(::Rio::IBuffer&,pd_data&);

static bool Axis_write(::Rio::IBuffer&,inlib::histo::axis<double>&);
static bool Axis_read_v0_v3(::Rio::IBuffer&,inlib::histo::axis<double>&);
static bool Axis_read_v4_v6(::Rio::IBuffer&,inlib::histo::axis<double>&);
static bool Axis_read_v7(::Rio::IBuffer&,inlib::histo::axis<double>&);

RIO_META_SOURCE(BatchLab::Rio::THistogram)
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::THistogram::THistogram(
 const ::Rio::IDictionary& aDictionary
,std::ostream& a_out
)
:Named(aDictionary,"","")
,fClass(BatchLab::Rio::THistogram::is(aDictionary))
,f_out(a_out)
,fData(0)
//////////////////////////////////////////////////////////////////////////////
// To read data.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::THistogram::~THistogram(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::BaseHistogram* BatchLab::Rio::THistogram::data(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fData;
}

void* BatchLab::Rio::THistogram::cast(const ::Rio::IClass& a_class) const {
  if(&a_class==&fClass) 
    return Rio_SCast(BatchLab::Rio::THistogram);
  else 
    return Named::cast(a_class);
}

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::THistogram::stream(
 ::Rio::IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {

    fData = 0;

    short v;
    if(!aBuffer.readVersion(v)) return false;
    if(!Named::stream(aBuffer)) return false;
    pd_data data;
    BatchLab::Annotation annotation;
    if(v==0) {
      if(!read_v0(aBuffer,data)) return false;
    } else if(v==1) {
      if(!read_v1(aBuffer,data)) return false;
    } else if(v==2) {
      if(!read_v2(aBuffer,data)) return false;
    } else if(v==3) {
      if(!read_v3(aBuffer,data)) return false;
    } else if(v==4) {
      if(!read_v4(aBuffer,data)) return false;
    } else if(v==5) {
      if(!read_v5(aBuffer,data)) return false;
    } else if(v==6) {
      if(!read_v6(aBuffer,data)) return false;
    } else if(v==7) {
      if(!read_v7(aBuffer,data)) return false;
    } else if(v==8) {
      if(!BatchLab::Rio::Annotation::read(aBuffer,annotation)) return false;
      if(!read_v7(aBuffer,data)) return false;
    } else {
      return false;
    }

    //data.m_coords.resize(data.m_dimension,0);
    //data.m_ints.resize(data.m_dimension,0);

    BatchLab::BaseHistogram* bobject = 0;
    unsigned int dim = data.m_dimension;
    bool isProfile = data.m_is_profile;
    if(dim==1) {
      if(isProfile) {
        inlib::histo::p1d hcl("",10,0,1);
        hcl.copy_from_data(data);
        hcl.update_fast_getters();
        bobject = new BatchLab::Profile1D(f_out,fName,hcl);
      } else {
        inlib::histo::h1d hcl("",10,0,1);
        hcl.copy_from_data(data);
        hcl.update_fast_getters();
        bobject = new BatchLab::Histogram1D(f_out,fName,hcl);
      }
    } else if(dim==2) {
      if(isProfile) {
        inlib::histo::p2d hcl("",10,0,1,10,0,1);
        hcl.copy_from_data(data);
        hcl.update_fast_getters();
        bobject = new BatchLab::Profile2D(f_out,fName,hcl);
      } else {
        inlib::histo::h2d hcl("",10,0,1,10,0,1);
        hcl.copy_from_data(data);
        hcl.update_fast_getters();
        bobject = new BatchLab::Histogram2D(f_out,fName,hcl);
      }
    } else if(dim==3) {
      inlib::histo::h3d hcl("",10,0,1,10,0,1,10,0,1);
      hcl.copy_from_data(data);
      hcl.update_fast_getters();
      bobject = new BatchLab::Histogram3D(f_out,fName,hcl);
    } else {
      f_out << "BatchLab::Rio::Histogram::streamer : object " 
          << inlib::sout(fName)
          << " of dimension " << dim << " not treated." 
          << std::endl;      
      return false;
    }
    bobject->setAnnotation(annotation);

    fData = bobject;

  } else {
    if(!aBuffer.writeVersion(fClass.version())) return false;
    if(!Named::stream(aBuffer)) return false;

    BatchLab::Annotation& annotation = fData->getAnnotation();
    if(!BatchLab::Rio::Annotation::write(aBuffer,annotation)) return false;

    pd_data data = fData->histo_data(); 

    if(!aBuffer.write(data.m_title)) return false;
    if(!aBuffer.write((int)data.m_dimension)) return false;
    if(!aBuffer.write((int)data.m_bin_number)) return false;

    if(!::Rio::writeArray<int>(aBuffer,
         inlib::convert<unsigned int,int>(data.m_bin_entries))) return false;

    if(!::Rio::writeArray<double>(aBuffer,data.m_bin_Sw)) return false;
    if(!::Rio::writeArray<double>(aBuffer,data.m_bin_Sw2)) return false;
    if(!::Rio::writeArray2<double>(aBuffer,data.m_bin_Sxw)) return false;
    if(!::Rio::writeArray2<double>(aBuffer,data.m_bin_Sx2w)) return false;
    for(int iaxis=0;iaxis<data.m_dimension;iaxis++) {
      inlib::histo::axis<double> haxis(data.m_axes[iaxis]);
      if(!Axis_write(aBuffer,haxis)) return false;
    }
   {int dummy = 0;
    if(!aBuffer.write(dummy)) return false;} //m_mode

    // Profile :
    if(!aBuffer.write(data.m_is_profile)) return false;
    if(data.m_is_profile) {
      if(!::Rio::writeArray<double>(aBuffer,data.m_bin_Svw)) return false;
      if(!::Rio::writeArray<double>(aBuffer,data.m_bin_Sv2w)) return false;
      if(!aBuffer.write(data.m_cut_v)) return false;
      if(!aBuffer.write(data.m_min_v)) return false;
      if(!aBuffer.write(data.m_max_v)) return false;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Axis_write(
 ::Rio::IBuffer& aBuffer
,inlib::histo::axis<double>& aAxis
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aBuffer.write(aAxis.m_offset)) return false;
  if(!aBuffer.write(aAxis.m_number_of_bins)) return false;
  if(!aBuffer.write(aAxis.m_minimum_value)) return false;
  if(!aBuffer.write(aAxis.m_maximum_value)) return false;
  if(!aBuffer.write(aAxis.m_fixed)) return false;
  if(!aBuffer.write(aAxis.m_bin_width)) return false;
  if(!aBuffer.write((int)aAxis.m_edges.size())) return false;
  for(unsigned int count=0;count<aAxis.m_edges.size();count++) {
    if(!aBuffer.write(aAxis.m_edges[count])) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
unsigned int new_bin_number(const std::vector< inlib::histo::axis<double> >& aAxes) {
  unsigned int number = 1;
  for(unsigned int iaxis=0;iaxis<aAxes.size();iaxis++) 
    number *= (aAxes[iaxis].bins()+2);
  return number;
}
template <class T> void add_outflow(
 const std::vector< inlib::histo::axis<double> >& aAxes
,std::vector<T>& aVector) {
  // aAxes[].m_offset contains the offset without outflow.
  unsigned int dim = aAxes.size();
  // new size and offsets :
  std::vector<int> aoff(dim);

  int newn = 1;
 {for(unsigned iaxis=0;iaxis<dim;iaxis++) newn *= (aAxes[iaxis].bins()+2);}

  aoff[0] = 1;
 {for(unsigned iaxis=1;iaxis<dim;iaxis++) {
    aoff[iaxis] = aoff[iaxis-1] * (aAxes[iaxis-1].bins()+2);
  }}
  // copy :
  std::vector<T> tmp = aVector;
  int oldn = (int)tmp.size();
  aVector.resize(newn,0);
  // Copy :
  std::vector<int> is(dim);
  int offset;
  for(int index=0;index<oldn;index++) {
    // Get new offset of index :
    offset = index;
   {for(int iaxis=dim-1;iaxis>=0;iaxis--) { 
      is[iaxis] = offset/aAxes[iaxis].m_offset;
      offset -= is[iaxis] * aAxes[iaxis].m_offset;
    }}
    // new offset :
    offset = 0;
   {for(unsigned iaxis=0;iaxis<dim;iaxis++) offset += is[iaxis] * aoff[iaxis];}
    aVector[offset] = tmp[index];
  }
}
void add_outflow(std::vector< inlib::histo::axis<double> >& aAxes) {
  // Restore new offsets :
  aAxes[0].m_offset = 1;
  for(unsigned int iaxis=1;iaxis<aAxes.size();iaxis++)
    aAxes[iaxis].m_offset = aAxes[iaxis-1].m_offset * (aAxes[iaxis-1].bins()+2);
}
//////////////////////////////////////////////////////////////////////////////
bool read_v0(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  std::string sdummy;
  if(!aBuffer.read(sdummy)) return false;
  if(!aBuffer.read(aData.m_title)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v0_v3(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
  inlib::histo::axis<double> axisOfValues;
  if(!Axis_read_v0_v3(aBuffer,axisOfValues)) return false;
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v0 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v1(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  if(!aBuffer.read(aData.m_title)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v0_v3(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
  inlib::histo::axis<double> axisOfValues;
  if(!Axis_read_v0_v3(aBuffer,axisOfValues)) return false;
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v1 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v2(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  std::string sdummy;
  if(!aBuffer.read(aData.m_title)) return false;
  if(!aBuffer.read(sdummy)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v0_v3(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
  inlib::histo::axis<double> axisOfValues;
  if(!Axis_read_v0_v3(aBuffer,axisOfValues)) return false;
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v2 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v3(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;

  int l;
  if(!aBuffer.read(l)) return false;
  char* str = new char[l+1];
  for (int i = 0; i < l; i++) {
    if(!aBuffer.read(str[i])) { delete [] str;return false;}
  }
  str[l] = '\0';
  aData.m_title = str;
  delete [] str;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v0_v3(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
  inlib::histo::axis<double> axisOfValues;
  if(!Axis_read_v0_v3(aBuffer,axisOfValues)) return false;
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v3 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v4(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;

  int l;
  if(!aBuffer.read(l)) return false;
  char* str = new char[l+1];
  for (int i = 0; i < l; i++) {
    if(!aBuffer.read(str[i])) { delete [] str;return false;}
  }
  str[l] = '\0';
  aData.m_title = str;
  delete [] str;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v4_v6(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v4 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v5(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  if(!aBuffer.read(aData.m_title)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v4_v6(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow(aData.m_axes);

  // Not here in v5 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty);
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  aData.m_is_profile = false;
  aData.m_bin_Svw.clear();
  aData.m_bin_Sv2w.clear();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v6(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  if(!aBuffer.read(aData.m_title)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  if(!aBuffer.read(idummy)) return false; //fEntries
  if(!aBuffer.read(idummy)) return false; //fOutFlow
  if(!aBuffer.read(ddummy)) return false; //fSw

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v4_v6(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode

  // Profile :
  if(!aBuffer.read(aData.m_is_profile)) return false;
  if(aData.m_is_profile) {
    if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Svw)) return false;
    if(aData.m_bin_Svw.size()!=aData.m_bin_number) return false;
    if(!aBuffer.read(aData.m_cut_v)) return false;
    if(!aBuffer.read(aData.m_min_v)) return false;
    if(!aBuffer.read(aData.m_max_v)) return false;
  }

  // Add outflow :
  aData.m_bin_number = new_bin_number(aData.m_axes);
  add_outflow<unsigned int>(aData.m_axes,aData.m_bin_entries);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw);
  add_outflow<double>(aData.m_axes,aData.m_bin_Sw2);
  add_outflow<double>(aData.m_axes,aData.m_bin_Svw);
  add_outflow(aData.m_axes);

  // Not here in v6 :
  std::vector<double> empty;
  empty.resize(aData.m_dimension,0);
  aData.m_bin_Sxw.resize(aData.m_bin_number,empty); //Forget to write in v6 !
  aData.m_bin_Sx2w.resize(aData.m_bin_number,empty);
  if(aData.m_is_profile) {
    aData.m_bin_Sv2w.resize(aData.m_bin_number,0);
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool read_v7(
 ::Rio::IBuffer& aBuffer
,pd_data& aData
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aBuffer.read(aData.m_title)) return false;

 {int dim;
  if(!aBuffer.read(dim)) return false;
  aData.m_dimension = dim;}

 {int nbin;
  if(!aBuffer.read(nbin)) return false;
  aData.m_bin_number = nbin;}

  std::vector<int> vec;
  if(!::Rio::readArray<int>(aBuffer,vec)) return false;
  aData.m_bin_entries = inlib::convert<int,unsigned int>(vec);

  if(aData.m_bin_entries.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw)) return false;
  if(aData.m_bin_Sw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sw2)) return false;
  if(aData.m_bin_Sw2.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray2<double>(aBuffer,aData.m_bin_Sxw)) return false;
  if(aData.m_bin_Sxw.size()!=aData.m_bin_number) return false;
  if(!::Rio::readArray2<double>(aBuffer,aData.m_bin_Sx2w)) return false;
  if(aData.m_bin_Sx2w.size()!=aData.m_bin_number) return false;
  if(aData.m_dimension>0) {
    aData.m_axes.resize(aData.m_dimension);
    for(unsigned int iaxis=0;iaxis<aData.m_dimension;iaxis++) {
      if(!Axis_read_v7(aBuffer,aData.m_axes[iaxis])) return false;
    }
  }
 {int dummy;
  if(!aBuffer.read(dummy)) return false;} //m_mode
  // Profile :
  if(!aBuffer.read(aData.m_is_profile)) return false;
  if(aData.m_is_profile) {
    if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Svw)) return false;
    if(aData.m_bin_Svw.size()!=aData.m_bin_number) return false;
    if(!::Rio::readArray<double>(aBuffer,aData.m_bin_Sv2w)) return false;
    if(aData.m_bin_Sv2w.size()!=aData.m_bin_number) return false;
    if(!aBuffer.read(aData.m_cut_v)) return false;
    if(!aBuffer.read(aData.m_min_v)) return false;
    if(!aBuffer.read(aData.m_max_v)) return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Axis_read_v0_v3(
 ::Rio::IBuffer& aBuffer
,inlib::histo::axis<double>& aAxis
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  if(!aBuffer.read(aAxis.m_minimum_value)) return false;
  if(!aBuffer.read(aAxis.m_maximum_value)) return false;
  if(!aBuffer.read(aAxis.m_offset)) return false;
  if(!aBuffer.read(aAxis.m_number_of_bins)) return false;
  if(!aBuffer.read(idummy)) return false; //fOverFlow
  if(!aBuffer.read(idummy)) return false; //fUnderFlow
  if(!aBuffer.read(aAxis.m_bin_width)) return false;
  if(!aBuffer.read(ddummy)) return false; //fSxw
  if(!aBuffer.read(ddummy)) return false; //fSx2w
  aAxis.m_fixed = true;
  aAxis.m_edges.clear();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Axis_read_v4_v6(
 ::Rio::IBuffer& aBuffer
,inlib::histo::axis<double>& aAxis
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int idummy;
  double ddummy;
  if(!aBuffer.read(aAxis.m_offset)) return false;
  if(!aBuffer.read(idummy)) return false; //fOverFlow
  if(!aBuffer.read(idummy)) return false; //fUnderFlow
  if(!aBuffer.read(ddummy)) return false; //fSxw
  if(!aBuffer.read(ddummy)) return false; //fSx2w
  if(!aBuffer.read(aAxis.m_number_of_bins)) return false;
  if(!aBuffer.read(aAxis.m_minimum_value)) return false;
  if(!aBuffer.read(aAxis.m_maximum_value)) return false;
  if(!aBuffer.read(aAxis.m_fixed)) return false;
  if(!aBuffer.read(aAxis.m_bin_width)) return false;
  int edgen;
  if(!aBuffer.read(edgen)) return false;
  for(int count=0;count<edgen;count++) {
    double value;
    if(!aBuffer.read(value)) return false;
    aAxis.m_edges.push_back(value);
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Axis_read_v7(
 ::Rio::IBuffer& aBuffer
,inlib::histo::axis<double>& aAxis
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!aBuffer.read(aAxis.m_offset)) return false;
  if(!aBuffer.read(aAxis.m_number_of_bins)) return false;
  if(!aBuffer.read(aAxis.m_minimum_value)) return false;
  if(!aBuffer.read(aAxis.m_maximum_value)) return false;
  if(!aBuffer.read(aAxis.m_fixed)) return false;
  if(!aBuffer.read(aAxis.m_bin_width)) return false;
  int edgen;
  if(!aBuffer.read(edgen)) return false;
  for(int count=0;count<edgen;count++) {
    double value;
    if(!aBuffer.read(value)) return false;
    aAxis.m_edges.push_back(value);
  }
  return true;
}

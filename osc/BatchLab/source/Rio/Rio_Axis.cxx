// this :
#include <BatchLab/Rio/Axis.h>

// Rio :
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Core/Named.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Array_double.h>

#include <inlib/histo/axis>

// BatchLab :
#include <BatchLab/Rio/AttAxis.h>

RIO_META_SOURCE(BatchLab::Rio::Axis)

BatchLab::Rio::Axis::Axis(
 const ::Rio::IDictionary& aDictionary
,inlib::histo::axis<double>& aAxis
)
: ::Rio::Named(aDictionary,"","")
,fClass(BatchLab::Rio::Axis::is(aDictionary))
,fAxis(aAxis)
{}

BatchLab::Rio::Axis::~Axis(){}

bool BatchLab::Rio::Axis::stream(::Rio::IBuffer& aBuffer){
  // Version 6 streaming (ROOT/v3-00-6).
  if (aBuffer.isReading()) {

    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;

    if(!Named::stream(aBuffer)) return false;

    {AttAxis dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    int number;
    if(!aBuffer.read(number)) return false;
    double min;
    if(!aBuffer.read(min)) return false;
    double max;
    if(!aBuffer.read(max)) return false;
    //printf("debug : BatchLab::RioTH::streamTAxis : %d %g %g\n",number,min,max);

    ::Rio::Array_double edges(dictionary());
    if(!edges.stream(aBuffer)) return false; //fXbins TArrayD

    int edgen = edges.size();
    if(edgen<=0) {
      fAxis.configure(number,min,max);
    } else {
      std::vector<double> vedges;
      for(int index=0;index<edgen;index++) {
        vedges.push_back(edges.array()[index]);
      }
      fAxis.configure(vedges);
    }

    int First;
    if(!aBuffer.read(First)) return false;
    int Last;
    if(!aBuffer.read(Last)) return false;

    if(v>=8) { //fBits2.
      unsigned short dummy;
      if(!aBuffer.read(dummy)) return false;
    }

    //Bool_t
    unsigned char TimeDisplay;
    if(!aBuffer.read(TimeDisplay)) return false;

    //TString
    std::string TimeFormat;
    if(!aBuffer.read(TimeFormat)) return false;

    if(v>=7) {
      //THashList*
      ::Rio::IObject* Labels = 0;
      ::Rio::Arguments args;
      ::Rio::Args_init(args);
      if(!aBuffer.readObject(args,Labels)) return false;
    }

    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;

  } else {

    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!Named::stream(aBuffer)) return false;

    {AttAxis dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    if(!aBuffer.write(fAxis.bins())) return false;
    if(!aBuffer.write(fAxis.lower_edge())) return false;
    if(!aBuffer.write(fAxis.upper_edge())) return false;

    // fXbins 
    if(fAxis.m_fixed) {
      ::Rio::Array_double dummy(dictionary());
      if(!dummy.stream(aBuffer)) return false; //TArrayD
    } else {
      ::Rio::Array_double edges(dictionary(),fAxis.m_edges);
      if(!edges.stream(aBuffer)) return false; //TArrayD
    }

    if(!aBuffer.write((int)0)) return false; //fFirst
    if(!aBuffer.write((int)0)) return false; //fLast

    //Bool_t
    if(!aBuffer.write((unsigned char)0)) return false;  //TimeDisplay

    //TString
    if(!aBuffer.write(std::string())) return false; //TimeFormat

    /*
    if(v>=7) {
      //THashList*
      ::Rio::IObject* Labels = 0;
      ::Rio::Arguments args;
      ::Rio::Args_init(args);
      if(!aBuffer.readObject(args,Labels)) return false;
    }
    */

    if(!aBuffer.setByteCount(c,true)) return false;

  }
  return true;
}

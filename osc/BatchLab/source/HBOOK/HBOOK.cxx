//this :
#include <BatchLab/HBOOK/HBOOK.h>

#include <Slash/Core/ISession.h>
#include <Slash/Tools/AIDA.h>

#include <AIDA/IAnnotation.h>
#include <AIDA/IAxis.h>

#include <amanip/find.h>
#include <amanip/create.h>

#ifdef WIN32
#define ohnorm OHNORM
extern "C" {
  void __stdcall ohnorm(int*,float*);
}
#else
#define ohnorm ohnorm_
extern "C" {
  void ohnorm(int*,float*);
}
#endif

#include <inlib/random>
#include <inlib/cast>
#include <inlib/sto>
#include <inlib/sout>

#include <iostream>

extern "C" {
  void HBOOK_Initialize(Slash::Core::ISession&);
  void HBOOK_Finalize(Slash::Core::ISession&);
}

//FIXME : can we avoid these statics ?
static Slash::Core::ISession* sSession = 0;
static AIDA::IAnalysisFactory* sAIDA = 0;
static AIDA::ITree* sMemoryTree = 0;
static inlib::random::flat* sRandomFlat = 0;

//FIXME : Darwin : BatchLabHBOOK can't be dynamically loaded 
// since it is also linked and then upper global variables
// are not going to be in one data space.

//FIXME : Linux : if having loaded the BatchLabHBOOK 
// (through the init of OpenPAW by executing BatchLabPAWInitialize)
// then a loading of the BatchLabZebra driver will embarque
// the below hlimit and NOT the CERNLIB/HBOOK one !
// A dlopen without RTLD_GLOBAL fixes that (but then
// problems with Python if libpythons.a is used).

//////////////////////////////////////////////////////////////////////////////
void HBOOK_Initialize(
 Slash::Core::ISession& aSession
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  sSession = &aSession;

  sAIDA = Slash::find_AIDA(aSession);
  if(!sAIDA) {
    std::ostream& out = aSession.cout();
    out << "BatchLabHBOOKInitialize :"
        << " AIDA not found." 
        << std::endl;
  }

  if(!sAIDA) return;

  sMemoryTree = amanip::find_mem(*sAIDA);
  if(!sMemoryTree) {
    std::ostream& out = aSession.cout();
    out << "BatchLabHBOOKInitialize :"
        << " memory tree not found." 
        << std::endl;
  }

  sRandomFlat = new inlib::random::flat();
}
//////////////////////////////////////////////////////////////////////////////
void HBOOK_Finalize(
 Slash::Core::ISession&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete sRandomFlat;
  sRandomFlat = 0;

  sMemoryTree = 0;
  sAIDA = 0;
  sSession = 0;
}

static void GetPartition(AIDA::IHistogram1D&,std::vector<double>&,std::ostream&);
static double GetRandom(AIDA::IHistogram1D&);
#define NotFound (-1)
int BinarySearch(AIDA::IHistogram1D&,double);

#ifdef WIN32
#define INLIB_STDCALL __stdcall
#else
#define INLIB_STDCALL
#endif

float INLIB_STDCALL hrndm1(int* aID) {
  if(!sMemoryTree) {
    std::cout << "hrndm1 :" 
              << " BatchLabHBOOK not initialized."
              << std::endl;
    return 0;
  }

  std::string ID = inlib::to<int>(*aID);

  AIDA::IHistogram1D* histogram =
    amanip::find_histogram_1D(*sMemoryTree,ID,false,sSession->cout());
  if(!histogram) {
    sSession->cout() << "hrndm1 :" 
        << " can't find " << inlib::sout(ID) 
        << " in memory tree."
        << std::endl;
    return 0;
  }

  if(histogram->annotation().value("Integrated")=="true") {
    // The histogram had been integrated :
    return (float)GetRandom(*histogram);
  }

  std::vector<double> partition;
  GetPartition(*histogram,partition,sSession->cout());
  if(!partition.size()) {
    sSession->cout() << "hrndm1 :" 
        << " problem when computing partition of histogram " 
        << inlib::sout(ID) 
        << std::endl;
    return 0;
  }

  // Recreate an histogram with same full path filled with 
  // the partition function.
  if(!sMemoryTree->mkdir("/tmp")) {
    sSession->cout() << "hrndm1 :" 
        << " can't create /tmp in MemoryTree."
        << std::endl;
    return 0;
  }

  AIDA::IManagedObject* mobject = INLIB_CAST(*histogram,AIDA::IManagedObject);
  std::string path = sMemoryTree->findPath(*mobject);
  if(!sMemoryTree->mv(path,"/tmp")) {
    sSession->cout() << "hrndm1 :" 
        << " can't move " << inlib::sout(path) 
        << " in " << inlib::sout("/tmp")
        << std::endl;
    return 0;
  }
  
  const AIDA::IAxis& axis = histogram->axis();

  AIDA::IHistogram1D* ihistogram = 
    amanip::create_histogram_1D(*sAIDA,*sMemoryTree,
      path,histogram->title(),
      axis.bins(),axis.lowerEdge(),axis.upperEdge());
  if(!ihistogram) {
    sSession->cout() << "hrndm1 :" 
        << " can't create histogram " << inlib::sout(path) 
        << std::endl;
    return 0;
  }

  int binn = axis.bins();
  for(int bini = 0;bini<binn;bini++) {
    double x = axis.binLowerEdge(bini) + 0.5 * axis.binWidth(bini);
    ihistogram->fill(x,partition[bini]);
  }

  ihistogram->annotation().addItem("Integrated","true");

  if(!sMemoryTree->rmdir("/tmp")) {
    sSession->cout() << "hrndm1 :" 
        << " can't remove /tmp in MemoryTree."
        << std::endl;
    return 0;
  }

  if(sSession->verboseLevel()) {
    sSession->cout() << "hrndm1 :" 
        << " Histogram " << inlib::sout(path) << " integrated."
        << std::endl;
  }

  // Return something not zero :
  double r;
  while((r = GetRandom(*ihistogram))==0) {}
  return (float)r;
}

void INLIB_STDCALL hfill(int* aID,float* aX,float* aY,float* aW){
  if(!sMemoryTree) {
    std::cout << "hfill :" 
              << " BatchLabHBOOK not initialized."
              << std::endl;
    return;
  }
 
  std::string ID = inlib::to<int>(*aID);
  //printf("debug : hfill %s %g %g\n",ID.c_str(),*aX,*aW);

  //FIXME : how to optimize to avoid the below find ?
  AIDA::IBaseHistogram* histogram =
    amanip::find_base_histogram(*sMemoryTree,ID,false,sSession->cout());
  if(!histogram) {
    sSession->cout() << "hfill :" 
        << " can't find " << inlib::sout(ID) 
        << " in memory tree."
        << std::endl;
    return;
  }

  if(histogram->dimension()==1) {
    AIDA::IHistogram1D* histogram1D = INLIB_CAST(*histogram,AIDA::IHistogram1D);
    if(histogram1D) histogram1D->fill(*aX,*aW);
  } else if(histogram->dimension()==2) {
    AIDA::IHistogram2D* histogram2D = INLIB_CAST(*histogram,AIDA::IHistogram2D);
    if(histogram2D) histogram2D->fill(*aX,*aY,*aW);
  } else {
    sSession->cout() << "hfill :" 
        << " histogram of dimension " << histogram->dimension()
        << " not supported."
        << std::endl;
    return;
  }
}
//////////////////////////////////////////////////////////////////////////////
void INLIB_STDCALL ohnorm(
 int* aID
,float* aArea
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!sMemoryTree) {
    std::cout << "ohnorm :" 
        << " BatchLabHBOOK not initialized."
        << std::endl;
    return;
  }

  std::string ID = inlib::to<int>(*aID);

  AIDA::IHistogram1D* histogram =
    amanip::find_histogram_1D(*sMemoryTree,ID,false,sSession->cout());
  if(!histogram) {
    sSession->cout() << "ohnorm :" 
        << " can't find " << inlib::sout(ID) 
        << " in memory tree."
        << std::endl;
    return;
  }

  double sum = histogram->sumBinHeights();
  if(sum!=0) histogram->scale(((double)*aArea)/sum);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void GetPartition(
 AIDA::IHistogram1D& aHistogram
,std::vector<double>& aPartition
,std::ostream& a_out
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nbins = aHistogram.axis().bins();
  if(nbins<=0) {
    a_out << "BatchLabHBOOK/GetPartition :"
        << " Can't compute histogram partition."
        << " Histogram without bins." 
        << std::endl;
    aPartition.clear();
    return;
  }

  aPartition.resize(nbins,0);

  double height = aHistogram.binHeight(0);
  if(height<0) {
    a_out << "BatchLabHBOOK/GetPartition :"
        << " Bin " << (int)0
        << " of histogram with title " <<inlib::sout(aHistogram.title())
        << " has zero height."
        << std::endl;
    aPartition.clear();
    return;
  }
  aPartition[0] = height;
  for(int ibin=1;ibin<nbins;ibin++) {
    double height = aHistogram.binHeight(ibin);
    if(height<0) {
      a_out << "BatchLabHBOOK/GetPartition :"
          << " Bin " << ibin
          << " of histogram with title " <<inlib::sout(aHistogram.title())
          << " has zero height."
          << std::endl;
      aPartition.clear();
      return;
    }
    aPartition[ibin] = aPartition[ibin-1] + height;
  }

  double sum = aPartition[nbins-1];
  // Normalize integral to 1
  if(sum==0) {
    a_out << "BatchLabHBOOK/GetPartition :"
        << " Integral of histogram is null." 
        << std::endl;
    aPartition.clear();
    return;
  }

 {for(int ibin=0;ibin<nbins;ibin++) aPartition[ibin] /= sum;}
}
//////////////////////////////////////////////////////////////////////////////
double GetRandom(
 AIDA::IHistogram1D& aHistogram
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Get a flat random number (avoid strict 1).
  //double r;
  //while((r = sRandomFlat->shoot())==1);

  double r = sRandomFlat->shoot(); // r is in [0,1]
  
  int ibin = BinarySearch(aHistogram,r);

  if(ibin==NotFound) { //r in [0,aHistogram.binHeight(0)]
    double dr = aHistogram.binHeight(0);
    if(dr==0) {
      return aHistogram.axis().binLowerEdge(0)
           + aHistogram.axis().binWidth(0)
           * r;
    } else {
      return aHistogram.axis().binLowerEdge(0)
           + aHistogram.axis().binWidth(0)
           * r/dr;
    }
  } else if(ibin>=(int)(aHistogram.axis().bins()-1)) {
    // We pass here when r is stricly equal to 1.
    return aHistogram.axis().upperEdge();
    // If histogrammed with same binning than aHistogram, 
    // it should go in the overflow.
  } else {
    // (ibin+1) < aHistogram.bins()
    double dr = aHistogram.binHeight(ibin+1) - aHistogram.binHeight(ibin);
    if(dr==0) {
      return aHistogram.axis().binLowerEdge(ibin+1)
           + aHistogram.axis().binWidth(ibin+1)
           * r;
    } else {
      return aHistogram.axis().binLowerEdge(ibin+1)
           + aHistogram.axis().binWidth(ibin+1)
           * (r-aHistogram.binHeight(ibin))/dr;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
int BinarySearch(
 AIDA::IHistogram1D& aHistogram
,double aValue
)
//////////////////////////////////////////////////////////////////////////////
// Binary search in an array of n values to locate value.
//
// Array is supposed  to be sorted prior to this call.
// If match is found, function returns position of element.
// If no match found, function gives nearest element smaller than value.
// If aArray is empty (-1) is returned.
// If aValue is stricly below first element (-1) is returned.
//
// From ROOT/TMath code.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nabove = aHistogram.axis().bins() + 1;
  int nbelow = 0;
  while(nabove-nbelow > 1) {
     int middle = (nabove+nbelow)/2;
     double value = aHistogram.binHeight(middle-1);
     if (aValue == value) return middle-1;
     if (aValue  < value) nabove = middle;
     else nbelow = middle;
  }
  return nbelow-1;
}

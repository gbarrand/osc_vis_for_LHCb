
#include <BatchLab/Zebra/Histogram.h>

#include <inlib/cast>

#include <BatchLab/Zebra/CHBOOK.h>

#include <cstdio> //printf

inline int hindex(int aIndex,const AIDA::IAxis& aAxis){
  if(aIndex==AIDA::IAxis::UNDERFLOW_BIN) 
    return 0;
  else if(aIndex==AIDA::IAxis::OVERFLOW_BIN) 
    return aAxis.bins()+1;
  else 
    return aIndex+1;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BatchLab::Zebra::Axis::Axis(
 const std::string& aPath
,int aID
,int aDimension
,bool aIsX
,bool aFixed
)
:fPath(aPath)
,fID(aID)
,fDimension(aDimension)
,fIsX(aIsX)
,fFixed(aFixed)
{
}

BatchLab::Zebra::Axis::Axis(const Axis& aFrom)
:AIDA::IAxis(aFrom)
,fPath(aFrom.fPath)
,fID(aFrom.fID)
,fDimension(aFrom.fDimension)
,fIsX(aFrom.fIsX)
,fFixed(aFrom.fFixed)
{}

BatchLab::Zebra::Axis::~Axis(){}

void BatchLab::Zebra::Axis::cd_beg() const {
  INLIB_SELF(BatchLab::Zebra::Axis);
  self.f_tmp = CHPWD();
  CHCDIR(fPath," ");
}
void BatchLab::Zebra::Axis::cd_end() const {CHCDIR(f_tmp," ");}

bool BatchLab::Zebra::Axis::isFixedBinning() const {return fFixed;}

double BatchLab::Zebra::Axis::lowerEdge() const {
  std::string s;
  int xn,yn;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,s,xn,xmin,xmax,yn,ymin,ymax);
  cd_end();
  return fIsX?xmin:ymin;
}

double BatchLab::Zebra::Axis::upperEdge() const {
  std::string s;
  int xn,yn;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,s,xn,xmin,xmax,yn,ymin,ymax);
  cd_end();
  return fIsX?xmax:ymax;
}

int BatchLab::Zebra::Axis::bins() const {
  std::string s;
  int xn,yn;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,s,xn,xmin,xmax,yn,ymin,ymax);
  cd_end();
  return fIsX?xn:yn;
}

double BatchLab::Zebra::Axis::binLowerEdge(int aIndex) const {
  int index = aIndex + 1; //HBOOK counts in [1,NX]
  if(fDimension==1) {
    cd_beg();
    double v = CHIX(fID,index);
    cd_end();
    return v;
  } else { //dimension 2
    float x,y;
    if(fIsX) {
      cd_beg();
      CHIJXY(fID,index,1,x,y);
      cd_end();
      return x;
    } else {
      cd_beg();
      CHIJXY(fID,1,index,x,y);
      cd_end();
      return y;
    }
  }
}

double BatchLab::Zebra::Axis::binUpperEdge(int aIndex) const {
  //FIXME : is it correct to ask for the lower bound of the next bin ?
  int index = aIndex + 1 + 1; //HBOOK counts in [1,NX]
  if(fDimension==1) {
    cd_beg();
    double v = CHIX(fID,index);
    cd_end();
    return v;
  } else { //dimension 2
    float x,y;
    if(fIsX) {
      cd_beg();
      CHIJXY(fID,index,1,x,y);
      cd_end();
      return x;
    } else {
      cd_beg();
      CHIJXY(fID,1,index,x,y);
      cd_end();
      return y;
    }
  }
}

double BatchLab::Zebra::Axis::binWidth(int aIndex) const {
  return binUpperEdge(aIndex) - binLowerEdge(aIndex);
}

int BatchLab::Zebra::Axis::coordToIndex(double aCoord) const {
  if(fDimension==1) {
    cd_beg();
    int v = CHXI(fID,float(aCoord))-1; //HBOOK counts in [1,NX]
    cd_end();
    return v;
  } else { //dimension 2
    int i,j;
    if(fIsX) {
      cd_beg();
      CHXYIJ(fID,float(aCoord),0,i,j);
      cd_end();
      return i-1;
    } else {
      cd_beg();
      CHXYIJ(fID,0,float(aCoord),i,j);
      cd_end();
      return j-1;
    }
  }
}

double BatchLab::Zebra::Axis::binCenter(int aIndex) const {
  return (binLowerEdge(aIndex) + binUpperEdge(aIndex))/2.0;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BatchLab::Zebra::BaseHistogram::BaseHistogram(
 std::ostream& a_out
,const std::string& aPath
,int aID
)
:BaseObject(a_out,inlib::to<int>(aID))
,fPath(aPath)
,fID(aID)
{}

BatchLab::Zebra::BaseHistogram::BaseHistogram(
 std::ostream& a_out //aPrinter
,const BaseHistogram& aFrom
)
:AIDA::IManagedObject(aFrom)
,Slash::Store::IObject(aFrom)
,BaseObject(aFrom)
,fAnnotation(aFrom.fAnnotation)
,fPath(aFrom.fPath)
,fID(aFrom.fID)
{}

BatchLab::Zebra::BaseHistogram::~BaseHistogram(){}

BatchLab::Annotation& BatchLab::Zebra::BaseHistogram::annotation() {
  return fAnnotation;
}

//////////////////////////////////////////////////////////////////////////////
void BatchLab::Zebra::BaseHistogram::setAnnotation(
 const BatchLab::Annotation& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation = aFrom;
}

void BatchLab::Zebra::BaseHistogram::cd_beg() const{
  INLIB_SELF(BatchLab::Zebra::BaseHistogram);
  self.f_tmp = CHPWD();
  CHCDIR(fPath," ");
}
void BatchLab::Zebra::BaseHistogram::cd_end() const {CHCDIR(f_tmp," ");}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram1D::Histogram1D(
 std::ostream& a_out
,const std::string& aPath
,int aID
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
)
:BaseHistogram(a_out,aPath,aID)
,fAxis(aPath,aID,1,true,true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fAxis.copy(fHistogram.getAxis(0));
  fAnnotation.setValue("Title",aTitle);    
  cd_beg();
  CHBOOK1(fID,aTitle,aXnumber,(float)aXmin,(float)aXmax);
  cd_end();
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram1D::Histogram1D(
 std::ostream& a_out
,const std::string& aPath
,int aID
,const std::string& aTitle
,const std::vector<double>& aEdges
)
:BaseHistogram(a_out,aPath,aID)
,fAxis(aPath,aID,1,true,false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation.setValue("Title",aTitle);
  unsigned int edgen = aEdges.size();
  std::vector<float> edges(edgen);
  for(unsigned int i=0;i<edgen;i++) edges[i] = float(aEdges[i]);
  cd_beg();
  CHBOOKB(fID,aTitle,edges);
  cd_end();
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram1D::Histogram1D(
 const Histogram1D& aFrom
)
:AIDA::IManagedObject(aFrom)
,Slash::Store::IObject(aFrom)
,AIDA::IBaseHistogram(aFrom)
,AIDA::IHistogram(aFrom)
,AIDA::IHistogram1D(aFrom)
,BaseHistogram(aFrom.out(),aFrom)
,fAxis(aFrom.fAxis)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : BatchLab::Zebra::Histogram1D::Histogram1D(3) : dummy\n");
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram1D::~Histogram1D(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Histogram1D::copy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return new BatchLab::Zebra::Histogram1D(*this);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram1D::storeClassName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Zebra::Histogram1D";
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Zebra::Histogram1D::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
       INLIB_IF_CAST(AIDA::IHistogram1D)
  else INLIB_IF_CAST(AIDA::IHistogram)
  else INLIB_IF_CAST(AIDA::IBaseHistogram)
  else INLIB_IF_CAST(BatchLab::Zebra::Histogram1D)
  else INLIB_IF_CAST(BatchLab::Zebra::BaseHistogram)
  else return BaseObject::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram1D::type(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return "AIDA::IHistogram1D";
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram1D::fill(
 double aX
,double aWeight
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHFILL(fID,(float)aX,0,(float)aWeight);
  cd_end();
  emitUpdate();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram1D::add(
 const AIDA::IHistogram1D& //aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : BatchLab::Zebra::Histogram1D::add : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram1D::title(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,title,ncx,xmin,xmax,ncy,ymin,ymax);
  cd_end();
  return title;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram1D::setTitle(
 const std::string& aTitle
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation.setValue("Title",aTitle);
  printf("debug : BatchLab::Zebra::Histogram1D::setTitle : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::IAnnotation& BatchLab::Zebra::Histogram1D::annotation(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAnnotation& BatchLab::Zebra::Histogram1D::annotation(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram1D::dimension(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{  
  return 1;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram1D::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHRESET(fID," ");
  cd_end();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram1D::allEntries(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  int v = CHNOENT(fID);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::minBinHeight(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHMIN(fID);
  cd_end();
  return v;  
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::maxBinHeight(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHMAX(fID);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::mean(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHSTATI(fID,1," ",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::rms(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHSTATI(fID,2," ",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAxis& BatchLab::Zebra::Histogram1D::axis(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAxis;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram1D::coordToIndex(
 double aCoord
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAxis.coordToIndex(aCoord);
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::binHeight(
 int aIndex
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHI(fID,hindex(aIndex,axis()));
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::sumBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX = axis().bins();
  float w = 0;
  cd_beg();
  for(int i=1;i<=NX;i++) {
    w += CHI(fID,i);
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::sumAllBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX1 = axis().bins()+1;
  float w = 0;
  cd_beg();
  for(int i=0;i<=NX1;i++) {
    w += CHI(fID,i);
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::sumExtraBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX1 = axis().bins()+1;
  float w = 0;
  cd_beg();
  w += CHI(fID,0);
  w += CHI(fID,NX1);
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram1D::binError(
 int aIndex
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHIE(fID,aIndex+1);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram1D::scale(
 double aScale
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : Do we want the E option ?
  cd_beg();
  CHOPERA(fID,"+E",fID,fID,aScale,0);
  cd_end();
  return true;
}

//FIXME : below are yet dummy.
int BatchLab::Zebra::Histogram1D::entries() const {
  printf("debug : BatchLab::Zebra::Histogram1D::entries : dummy\n");
  //return CHNOENT(fID);
  return 0;
}
int BatchLab::Zebra::Histogram1D::nanEntries() const {
  printf("debug : BatchLab::Zebra::Histogram1D::nanEntries : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram1D::extraEntries() const {
  printf("debug : BatchLab::Zebra::Histogram1D::extraEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram1D::equivalentBinEntries() const {
  printf("debug : BatchLab::Zebra::Histogram1D::equivalentBinEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram1D::binMean(int /*aIndex*/) const {
  printf("debug : BatchLab::Zebra::Histogram1D::binMean : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram1D::binStdDev(int /*aIndex*/) const {
  printf("debug : BatchLab::Zebra::Histogram1D::binStdDev : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram1D::binEntries(int /*aIndex*/) const {
  printf("debug : BatchLab::Zebra::Histogram1D::binEntries : dummy\n");
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Profile1D::Profile1D(
 std::ostream& a_out
,const std::string& aPath
,int aID
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,double aVmin
,double aVmax
)
:BaseHistogram(a_out,aPath,aID)
,fAxis(aPath,aID,1,true,true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation.setValue("Title",aTitle);
  cd_beg();
  CHBPROF(fID,aTitle,
          aXnumber,(float)aXmin,(float)aXmax,
          (float)aVmin,(float)aVmax,
          ""); //FIXME : or "S" or "T" ?
  cd_end();
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Profile1D::Profile1D(
 const Profile1D& aFrom
)
:AIDA::IManagedObject(aFrom)
,Slash::Store::IObject(aFrom)
,AIDA::IBaseHistogram(aFrom)
,AIDA::IProfile(aFrom)
,AIDA::IProfile1D(aFrom)
,BaseHistogram(aFrom.out(),aFrom)
,fAxis(aFrom.fAxis)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : BatchLab::Zebra::Profile1D::Profile1D(3) : dummy\n");
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Profile1D::~Profile1D(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Profile1D::copy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return new BatchLab::Zebra::Profile1D(*this);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Profile1D::storeClassName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Zebra::Profile1D";
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Zebra::Profile1D::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
       INLIB_IF_CAST(AIDA::IProfile1D)
  else INLIB_IF_CAST(AIDA::IProfile)
  else INLIB_IF_CAST(AIDA::IBaseHistogram)
  else INLIB_IF_CAST(BatchLab::Zebra::Profile1D)
  else INLIB_IF_CAST(BatchLab::Zebra::BaseHistogram)
  else return BaseObject::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Profile1D::type(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return "AIDA::IProfile1D";
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Profile1D::fill(
 double aX
,double aY
,double aWeight
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHFILL(fID,(float)aX,(float)aY,(float)aWeight);
  cd_end();
  emitUpdate();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Profile1D::add(
 const AIDA::IProfile1D& aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : BatchLab::Zebra::Profile1D::add : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Profile1D::title(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,title,ncx,xmin,xmax,ncy,ymin,ymax);
  cd_end();
  return title;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Profile1D::setTitle(
 const std::string& aTitle
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation.setValue("Title",aTitle);
  printf("debug : BatchLab::Zebra::Profile1D::setTitle : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::IAnnotation& BatchLab::Zebra::Profile1D::annotation(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAnnotation& BatchLab::Zebra::Profile1D::annotation(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Profile1D::dimension(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 1;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Profile1D::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHRESET(fID," ");
  cd_end();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Profile1D::allEntries(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  int v = CHNOENT(fID);
  cd_end();
  return v;
}

//FIXME : below are yet dummy.
int BatchLab::Zebra::Profile1D::entries() const {
  printf("debug : BatchLab::Zebra::Profile1D::entries : dummy\n");
  return 0;
}
int BatchLab::Zebra::Profile1D::extraEntries() const {
  printf("debug : BatchLab::Zebra::Profile1D::extraEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::equivalentBinEntries() const {
  printf("debug : BatchLab::Zebra::Profile1D::equivalentBinEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::sumBinHeights() const {
  printf("debug : BatchLab::Zebra::Profile1D::sumBinHeights : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::sumAllBinHeights() const {
  printf("debug : BatchLab::Zebra::Profile1D::sumAllBinHeights : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::sumExtraBinHeights() const {
  printf("debug : BatchLab::Zebra::Profile1D::sumExtraBinHeights : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::minBinHeight() const {
  printf("debug : BatchLab::Zebra::Profile1D::minBinHeight : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::maxBinHeight() const {
  printf("debug : BatchLab::Zebra::Profile1D::maxBinHeight : dummy\n");
  return 0;
}
bool BatchLab::Zebra::Profile1D::scale(double aScale) {
  printf("debug : BatchLab::Zebra::Profile1D::scale : dummy\n");
  return false;
}
double BatchLab::Zebra::Profile1D::binMean(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binMean : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::binStdDev(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binStdDev : dummy\n");
  return 0;
}
int BatchLab::Zebra::Profile1D::binEntries(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::binHeight(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binHeight : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::binError(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binError : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::binRms(int aIndex) const {
  printf("debug : BatchLab::Zebra::Profile1D::binRms : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::mean() const {
  printf("debug : BatchLab::Zebra::Profile1D::mean : dummy\n");
  return 0;
}
double BatchLab::Zebra::Profile1D::rms() const {
  printf("debug : BatchLab::Zebra::Profile1D::mean : rms\n");
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAxis& BatchLab::Zebra::Profile1D::axis(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAxis;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Profile1D::coordToIndex(
 double aCoord
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAxis.coordToIndex(aCoord);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram2D::Histogram2D(
 std::ostream& a_out
,const std::string& aPath
,int aID
,const std::string& aTitle
,int aXnumber
,double aXmin
,double aXmax
,int aYnumber
,double aYmin
,double aYmax
)
:BaseHistogram(a_out,aPath,aID)
,fAxisX(aPath,aID,2,true,true)
,fAxisY(aPath,aID,2,false,true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fAxisX.copy(fHistogram.getAxis(0));
  //fAxisY.copy(fHistogram.getAxis(1));
  fAnnotation.setValue("Title",aTitle);
  cd_beg();
  CHBOOK2(fID,aTitle,aXnumber,(float)aXmin,(float)aXmax,
                     aYnumber,(float)aYmin,(float)aYmax);
  cd_end();
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram2D::Histogram2D(
 const Histogram2D& aFrom
)
:AIDA::IManagedObject(aFrom)
,Slash::Store::IObject(aFrom)
,AIDA::IBaseHistogram(aFrom)
,AIDA::IHistogram(aFrom)
,AIDA::IHistogram2D(aFrom)
,BaseHistogram(aFrom.out(),aFrom)
,fAxisX(aFrom.fAxisX)
,fAxisY(aFrom.fAxisY)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  printf("debug : BatchLab::Zebra::Histogram2D::Histogram2D(3) : dummy\n");
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Histogram2D::~Histogram2D(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Histogram2D::copy(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return new BatchLab::Zebra::Histogram2D(*this);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram2D::storeClassName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Zebra::Histogram2D";
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Zebra::Histogram2D::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
       INLIB_IF_CAST(AIDA::IHistogram2D)
  else INLIB_IF_CAST(AIDA::IHistogram)
  else INLIB_IF_CAST(AIDA::IBaseHistogram)
  else INLIB_IF_CAST(BatchLab::Zebra::Histogram2D)
  else INLIB_IF_CAST(BatchLab::Zebra::BaseHistogram)
  else return BaseObject::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram2D::type(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return "AIDA::IHistogram2D";
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram2D::fill(
 double aX
,double aY
,double aWeight
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  cd_beg();
  CHFILL(fID,(float)aX,(float)aY,(float)aWeight);
  cd_end();
  emitUpdate();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram2D::add(
 const AIDA::IHistogram2D& //aHisto
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //BatchLab::Zebra::Histogram2D* batchlabHisto = 
  //   INLIB_CONST_CAST(aHisto,BatchLab::Zebra::Histogram2D);
  //if(!batchlabHisto) return false;
  //fHistogram.add(batchlabHisto->bareHistogram());
  printf("debug : BatchLab::Zebra::Histogram2D::add : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Histogram2D::title(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  cd_beg();
  CHGIVE(fID,title,ncx,xmin,xmax,ncy,ymin,ymax);
  cd_end();
  return title;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram2D::setTitle(
 const std::string& aTitle
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAnnotation.setValue("Title",aTitle);
  printf("debug : BatchLab::Zebra::Histogram2D::setTitle : dummy\n");
  return false;
}
//////////////////////////////////////////////////////////////////////////////
AIDA::IAnnotation& BatchLab::Zebra::Histogram2D::annotation(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAnnotation& BatchLab::Zebra::Histogram2D::annotation(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAnnotation;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram2D::dimension(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 2;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram2D::reset(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  CHRESET(fID," ");
  cd_end();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::meanX(
) const{
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
 
  cd_beg();
  double v = CHSTATI(fID,1,"PROX",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::meanY(
) const{
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
 
  cd_beg();
  double v = CHSTATI(fID,1,"PROY",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::rmsX(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  cd_beg();
  double v = CHSTATI(fID,2,"PROX",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::rmsY(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHSTATI(fID,2,"PROY",0);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAxis& BatchLab::Zebra::Histogram2D::xAxis(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fAxisX;
}
//////////////////////////////////////////////////////////////////////////////
const AIDA::IAxis& BatchLab::Zebra::Histogram2D::yAxis(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fAxisY;
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram2D::coordToIndexX(
 double aCoordX
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fAxisX.coordToIndex(aCoordX);
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram2D::coordToIndexY(
 double aCoordY
) const
/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fAxisY.coordToIndex(aCoordY);
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Zebra::Histogram2D::allEntries(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  cd_beg();
  int v = CHNOENT(fID);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::minBinHeight(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHMIN(fID);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::maxBinHeight(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  cd_beg();
  double v = CHMAX(fID);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::binHeight(
 int aIndexX
,int aIndexY
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  cd_beg();
  double v = CHIJ(fID,hindex(aIndexX,xAxis()),hindex(aIndexY,yAxis()));
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::sumBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX = xAxis().bins();
  int NY = yAxis().bins();
  float w = 0;
  cd_beg();
  for(int i=1;i<=NX;i++) {
    for(int j=1;j<=NY;j++) {
      w += CHIJ(fID,i,j);
    }
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::sumAllBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX1 = xAxis().bins()+1;
  int NY1 = yAxis().bins()+1;
  float w = 0;
  cd_beg();
  for(int i=0;i<=NX1;i++) {
    for(int j=0;j<=NY1;j++) {
      w += CHIJ(fID,i,j);
    }
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::sumExtraBinHeights(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int NX = xAxis().bins();
  int NY = yAxis().bins();
  int NX1 = NX+1;
  int NY1 = NY+1;
  float w = 0;
  cd_beg();
 {for(int j=0;j<=NY1;j++) {
    w += CHIJ(fID,0,j);
  }}
  for(int i=1;i<=NX;i++) {
    w += CHIJ(fID,i,0);
    w += CHIJ(fID,i,NY1);
  }
 {for(int j=0;j<=NY1;j++) {
    w += CHIJ(fID,NX1,j);
  }}
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::binError(
 int aIndexX
,int aIndexY
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  cd_beg();
  double v = CHIJE(fID,aIndexX+1,aIndexY+1);
  cd_end();
  return v;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::binHeightX(
 int aIndexX
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  int i = hindex(aIndexX,xAxis());
  int NY1 = yAxis().bins()+1;
  float w = 0;
  cd_beg();
  for(int j=0;j<=NY1;j++) {
    w += CHIJ(fID,i,j);
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
double BatchLab::Zebra::Histogram2D::binHeightY(
 int aIndexY
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  int j = hindex(aIndexY,yAxis());
  int NX1 = xAxis().bins()+1;
  float w = 0;
  cd_beg();
  for(int i=0;i<=NX1;i++) {
    w += CHIJ(fID,i,j);
  }
  cd_end();
  return w;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Histogram2D::scale(
 double aScale
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : Do we want the E option ?
  cd_beg();
  CHOPERA(fID,"+E",fID,fID,aScale,0);
  cd_end();
  return true;
}

//FIXME : below are yet dummy.
int BatchLab::Zebra::Histogram2D::entries() const {
  printf("debug : BatchLab::Zebra::Histogram2D::entries : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram2D::nanEntries() const {
  printf("debug : BatchLab::Zebra::Histogram2D::nanEntries : dummy\n");
  //return CHNOENT(fID);
  return 0;
}
int BatchLab::Zebra::Histogram2D::extraEntries() const {
  printf("debug : BatchLab::Zebra::Histogram2D::extraEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram2D::equivalentBinEntries() const {
  printf("debug : BatchLab::Zebra::Histogram2D::equivalentBinEntries : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram2D::binMeanX(int /*aIndexX*/,int /*aIndexY*/) const { 
  printf("debug : BatchLab::Zebra::Histogram2D::binMeanX : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram2D::binMeanY(int /*aIndexX*/,int /*aIndexY*/) const { 
  printf("debug : BatchLab::Zebra::Histogram2D::binMeanY : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram2D::binStdDevX(int /*aIndexX*/,int /*aIndexY*/) const {
  printf("debug : BatchLab::Zebra::Histogram2D::binStdDevX : dummy\n");
  return 0;
}
double BatchLab::Zebra::Histogram2D::binStdDevY(int /*aIndexX*/,int /*aIndexY*/) const {
  printf("debug : BatchLab::Zebra::Histogram2D::binStdDevY : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram2D::binEntries(int /*aIndexX*/,int /*aIndexY*/) const { 
  printf("debug : BatchLab::Zebra::Histogram2D::binEntries : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram2D::binEntriesX(int /*aIndexX*/) const { 
  printf("debug : BatchLab::Zebra::Histogram2D::binEntriesX : dummy\n");
  return 0;
}
int BatchLab::Zebra::Histogram2D::binEntriesY(int /*aIndexY*/) const { 
  printf("debug : BatchLab::Zebra::Histogram2D::binEntriesY : dummy\n");
  return 0;
}

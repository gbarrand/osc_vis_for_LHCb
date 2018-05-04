// this :
#include <BatchLab/Zebra/CHBOOK.h>

#include <string.h> //memset

// Define the names of the Fortran subroutine 
// and functions for the different OSs

#ifdef WIN32
# define uhtoc   UHTOC
# define zitoh   ZITOH
# define rzink   RZINK
# define hlimit  HLIMIT
# define hropen  HROPEN
# define hrend   HREND
# define hrin    HRIN
# define hrout   HROUT
# define hnoent  HNOENT
# define hfnt    HFNT
# define hgive   HGIVE
# define hgiven  HGIVEN
# define hgnpar  HGNPAR
# define hgnf    HGNF
# define hgnt    HGNT
# define hdcofl  HDCOFL
# define hdelet  HDELET
# define hntvar2 HNTVAR2
# define hbnt    HBNT
# define hbname  HBNAME
# define hbnamc  HBNAMC
# define hbnam   HBNAM
# define hbook1  HBOOK1
# define hbook2  HBOOK2
# define hbookb  HBOOKB
# define hbprof  HBPROF
# define hi      HI
# define hie     HIE
# define hij     HIJ
# define hije    HIJE
# define hix     HIX
# define hijxy   HIJXY
# define hxi     HXI
# define hxyij   HXYIJ
# define hmin    HMIN
# define hmax    HMAX
# define hcdir   HCDIR
# define hldir   HLDIR
# define hmdir   HMDIR
# define hddir   HDDIR
# define hfill   HFILL
# define hstati  HSTATI
# define hreset  HRESET
# define hopera  HOPERA
# define hkind   HKIND
//# define hidall  HIDALL
//# define hid1    HID1
//# define hid2    HID2
//# define hexist  HEXIST
# define hisid   HISID

# define type_of_call  __stdcall
# define DEFCHAR  const char*, const int
# define PASSCHAR(string) string, strlen(string)
#else
# define uhtoc   uhtoc_
# define zitoh   zitoh_
# define rzink   rzink_
# define hlimit  hlimit_
# define hropen  hropen_
# define hrend   hrend_
# define hrin    hrin_
# define hrout   hrout_
# define hnoent  hnoent_
# define hfnt    hfnt_
# define hgive   hgive_
# define hgiven  hgiven_
# define hgnpar  hgnpar_
# define hgnf    hgnf_
# define hgnt    hgnt_
# define hdcofl  hdcofl_
# define hdelet  hdelet_
# define hntvar2 hntvar2_
# define hbnt    hbnt_
# define hbname  hbname_
# define hbnamc  hbnamc_
# define hbnam   hbnam_
# define hbook1  hbook1_
# define hbook2  hbook2_
# define hbookb  hbookb_
# define hbprof  hbprof_
# define hi      hi_
# define hie     hie_
# define hij     hij_
# define hije    hije_
# define hix     hix_
# define hijxy   hijxy_
# define hxi     hxi_
# define hxyij   hxyij_
# define hmin    hmin_
# define hmax    hmax_
# define hcdir   hcdir_
# define hldir   hldir_
# define hmdir   hmdir_
# define hddir   hddir_
# define hfill   hfill_
# define hstati  hstati_
# define hreset  hreset_
# define hopera  hopera_
# define hkind   hkind_
//# define hidall  hidall_
//# define hid1    hid1_
//# define hid2    hid2_
//# define hexist  hexist_
# define hisid   hisid_

# define type_of_call
# define DEFCHAR  const char*
# define PASSCHAR(string) string
#endif

#ifdef WIN32
extern "C" void __stdcall uhtoc(int*,int*,const char*,const int,int*);
#else
extern "C" void uhtoc(int*,int*,const char*,int*,int);
#endif


extern "C" void  type_of_call hlimit(int*);
#ifdef WIN32
extern "C" void  type_of_call hropen(int*,DEFCHAR,DEFCHAR,DEFCHAR,
                        int*,int*);
#else
extern "C" void  type_of_call hropen(int*,DEFCHAR,DEFCHAR,DEFCHAR,
                        int*,int*,int,int,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hrend(DEFCHAR);
#else
extern "C" void  type_of_call hrend(DEFCHAR,int);
#endif

extern "C" void  type_of_call hrin(int*,int*,int*);
#ifdef WIN32
extern "C" void  type_of_call hrout(int*,int*,DEFCHAR);
#else
extern "C" void  type_of_call hrout(int*,int*,DEFCHAR,int);
#endif

extern "C" void  type_of_call rzink(int*,int*,const char *,int);
extern "C" void  type_of_call hdcofl();
extern "C" void  type_of_call hdelet(int*);
#ifdef WIN32
extern "C" void  type_of_call hcdir(DEFCHAR,DEFCHAR);
#else
extern "C" void  type_of_call hcdir(DEFCHAR,DEFCHAR ,int,int);
#endif
#ifdef WIN32
extern "C" void  type_of_call hldir(DEFCHAR,DEFCHAR);
#else
extern "C" void  type_of_call hldir(DEFCHAR,DEFCHAR ,int,int);
#endif
#ifdef WIN32
extern "C" void  type_of_call hmdir(DEFCHAR,DEFCHAR);
#else
extern "C" void  type_of_call hmdir(DEFCHAR,DEFCHAR ,int,int);
#endif
#ifdef WIN32
extern "C" void  type_of_call hddir(DEFCHAR);
#else
extern "C" void  type_of_call hddir(DEFCHAR,int);
#endif
extern "C" void  type_of_call zitoh(int*,int*,int*);


#ifdef WIN32
extern "C" void  type_of_call hbook1(int*,DEFCHAR,int*,float*,float*,float*);
#else
extern "C" void  type_of_call hbook1(int*,DEFCHAR,int*,float*,float*,float*,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hbook2(int*,DEFCHAR,int*,float*,float*,int*,float*,float*,float*);
#else
extern "C" void  type_of_call hbook2(int*,DEFCHAR,int*,float*,float*,int*,float*,float*,float*,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hbookb(int*,DEFCHAR,int*,float*,float*);
#else
extern "C" void  type_of_call hbookb(int*,DEFCHAR,int*,float*,float*,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hbprof(int*,DEFCHAR,int*,float*,float*,float*,float*,DEFCHAR);
#else
extern "C" void  type_of_call hbprof(int*,DEFCHAR,int*,float*,float*,float*,float*,DEFCHAR,int,int);
#endif

extern "C" float type_of_call hi(int*,int*);
extern "C" float type_of_call hie(int*,int*);
extern "C" float type_of_call hij(int*,int*,int*);
extern "C" float type_of_call hije(int*,int*,int*);
extern "C" void type_of_call hix(int*,int*,float*);
extern "C" void type_of_call hijxy(int*,int*,int*,float*,float*);
extern "C" void type_of_call hxi(int*,float*,int*);
extern "C" void type_of_call hxyij(int*,float*,float*,int*,int*);
extern "C" float type_of_call hmin(int*);
extern "C" float type_of_call hmax(int*);
extern "C" void  type_of_call hfill(int*,float*,float*,float*);
extern "C" void  type_of_call hnoent(int*,int*);
#ifdef WIN32
extern "C" void  type_of_call hgive(int*,DEFCHAR,int*,float*,float*,
   int*,float*,float*,int*,int*);
#else
extern "C" void  type_of_call hgive(int*,DEFCHAR,int*,float*,float*,
   int*,float*,float*,int*,int*,int);
#endif


#ifdef WIN32
extern "C" float type_of_call hstati(int*,int*,DEFCHAR,int*);
#else
extern "C" float type_of_call hstati(int*,int*,DEFCHAR,int*,int);
#endif

#ifdef WIN32
extern "C" void type_of_call hreset(int*,DEFCHAR);
#else
extern "C" void type_of_call hreset(int*,DEFCHAR,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hbnt(int*,DEFCHAR,DEFCHAR);
#else
extern "C" void  type_of_call hbnt(int*,DEFCHAR,DEFCHAR,int,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hgiven(int*,DEFCHAR,int*,DEFCHAR,
   float*,float*);
#else
extern "C" void  type_of_call hgiven(int*,DEFCHAR,int*,DEFCHAR,
   float*,float*,int,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hntvar2(int*,int*,DEFCHAR,DEFCHAR,DEFCHAR,int*,int*,int*,int*);
#else
extern "C" void  type_of_call hntvar2(int*,int*,DEFCHAR,DEFCHAR,DEFCHAR,int*,int*,int*,int*,int,int,int);
#endif

extern "C" void  type_of_call hgnpar(int*,const char *,int);
extern "C" void  type_of_call hgnf(int*,int*,float*,int*);
extern "C" void  type_of_call hgnt(int*,int*,int*);

#ifdef WIN32
extern "C" void  type_of_call hbnam(int*,DEFCHAR,int*,DEFCHAR,int*);
#else
extern "C" void  type_of_call hbnam(int*,DEFCHAR,int*,DEFCHAR,int*,int,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hbname(int*,DEFCHAR,int*,DEFCHAR);
#else
extern "C" void  type_of_call hbname(int*,DEFCHAR,int*,DEFCHAR,int,int);
#endif

extern "C" void  type_of_call hfnt(int*);

#ifdef WIN32
extern "C" void  type_of_call hopera(int*,DEFCHAR,int*,int*,float*,float*);
#else
extern "C" void  type_of_call hopera(int*,DEFCHAR,int*,int*,float*,float*,int);
#endif

#ifdef WIN32
extern "C" void  type_of_call hkind(int*,int*,DEFCHAR);
#else
extern "C" void  type_of_call hkind(int*,int*,DEFCHAR,int);
#endif

//extern "C" void  type_of_call hidall(int*,int*);
//extern "C" void  type_of_call hid1(int*,int*);
//extern "C" void  type_of_call hid2(int*,int*);
//extern "C" int type_of_call hexist(int*);

extern "C" int type_of_call hisid(int*);

#define STR const std::string&

//////////////////////////////////////////////////////////////////////////////
/// Package //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CHLIMIT(int a1) {
  int v1 = a1;
  hlimit(&v1);
}

void CHDELET(int aID){
  int id = aID;
  hdelet(&id);
}

void CHDCOFL() {
  hdcofl();
}

void CRZINK(int aKey,int aCycle,const std::string& aOpts) {
  int key = aKey;
  int cycle = aCycle;
#ifdef WIN32
  rzink(&key,&cycle,PASSCHAR(aOpts.c_str()));
#else
  rzink(&key,&cycle,PASSCHAR(aOpts.c_str()),aOpts.size());
#endif
}

void CZITOH(int* a1,int* a2,int* a3) {
  zitoh(a1,a2,a3);
}

//////////////////////////////////////////////////////////////////////////////
/// File /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int CHROPEN(int a1,STR a2,STR a3,STR a4,int a5) {
  int v1 = a1;
  int v5 = a5;
  int ier = 0;
#ifdef WIN32
  hropen(&v1,PASSCHAR(a2.c_str()),PASSCHAR(a3.c_str()),PASSCHAR(a4.c_str()),&v5,&ier);
#else
  hropen(&v1,PASSCHAR(a2.c_str()),PASSCHAR(a3.c_str()),PASSCHAR(a4.c_str()),&v5,&ier,a2.size(),a3.size(),a4.size());
#endif
  return ier;
}


void CHRIN(int aID,int aCycle,int aOffset){
  int id = aID;
  int cycle = aCycle;
  int offset = aOffset;
  hrin(&id,&cycle,&offset);
}

void CHROUT(int a1,int a2,STR a3) {
  int v1 = a1;
  int v2 = a2;
#ifdef WIN32
  hrout(&v1,&v2,PASSCHAR(a3.c_str()));
#else
  hrout(&v1,&v2,PASSCHAR(a3.c_str()),a3.size());
#endif
}

void CHREND(STR a1) {
#ifdef WIN32
  hrend(PASSCHAR(a1.c_str()));
#else
  hrend(PASSCHAR(a1.c_str()),a1.size());
#endif
}

void CHCDIR(STR a1,STR a2) {
#ifdef WIN32
  hcdir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()));
#else
  hcdir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()),a1.size(),a2.size());
#endif
}

void CHLDIR(STR a1,STR a2) {
#ifdef WIN32
  hldir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()));
#else
  hldir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()),a1.size(),a2.size());
#endif
}

std::string CHPWD() {
  int l = 1024;
  char* pwd = new char[l];
 {for(int i=0;i<l;i++) pwd[i] = 0;}
  std::string a2 = "R";  
#ifdef WIN32
  hcdir(pwd,l,PASSCHAR(a2.c_str()));
#else
  hcdir(pwd,PASSCHAR(a2.c_str()),l,a2.size());
#endif
 {for(int i=l-1;i>0;i--) {
    if(pwd[i]==0) continue;
    if(pwd[i]!=' ') break;
    pwd[i] = 0;
  }}
  std::string spwd(pwd);
  delete [] pwd;
  return spwd;
}

void CHMDIR(STR a1,STR a2) {
#ifdef WIN32
  hmdir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()));
#else
  hmdir(PASSCHAR(a1.c_str()),PASSCHAR(a2.c_str()),a1.size(),a2.size());
#endif
}

void CHDDIR(STR a1) {
#ifdef WIN32
  hddir(PASSCHAR(a1.c_str()));
#else
  hddir(PASSCHAR(a1.c_str()),a1.size());
#endif
}


//////////////////////////////////////////////////////////////////////////////
/// Histogram ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CHBOOK1(int aID,STR aTitle,int aXnumber,float aXmin,float aXmax,float aVMX) {
  int id = aID;
  int xnum = aXnumber;
  float xmin = aXmin;
  float xmax = aXmax;
  float vmx = aVMX;
#ifdef WIN32
  hbook1(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&vmx);
#else
  hbook1(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&vmx,aTitle.size());
#endif
}

void CHBOOK2(int aID,STR aTitle,int aXnumber,float aXmin,float aXmax,int aYnumber,float aYmin,float aYmax,float aVMX) {
  int id = aID;
  int xnum = aXnumber;
  float xmin = aXmin;
  float xmax = aXmax;
  int ynum = aYnumber;
  float ymin = aYmin;
  float ymax = aYmax;
  float vmx = aVMX;
#ifdef WIN32
  hbook2(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&ynum,&ymin,&ymax,&vmx);
#else
  hbook2(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&ynum,&ymin,&ymax,&vmx,aTitle.size());
#endif
}

bool CHBOOKB(
 int aID
,STR aTitle
,const std::vector<float>& aEdges
,float aVMX
) {
  if(!aEdges.size()) return false;
  int id = aID;  
  int edgen = (int)aEdges.size();
  float* edges = new float[edgen];
  for(int i=0;i<edgen;i++) edges[i] = aEdges[i];
  int ncx = edgen-1;
  float vmx = aVMX;
#ifdef WIN32
  hbookb(&id,PASSCHAR(aTitle.c_str()),&ncx,edges,&vmx);
#else
  hbookb(&id,PASSCHAR(aTitle.c_str()),&ncx,edges,&vmx,aTitle.size());
#endif
  delete [] edges;
  return true;
}

void CHBPROF(
 int aID
,STR aTitle
,int aXnumber,float aXmin,float aXmax
,float aYmin,float aYmax
,STR aOpts
) {
  int id = aID;
  int xnum = aXnumber;
  float xmin = aXmin;
  float xmax = aXmax;
  float ymin = aYmin;
  float ymax = aYmax;
#ifdef WIN32
  hbprof(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&ymin,&ymax,PASSCHAR(aOpts.c_str()));
#else
  hbprof(&id,PASSCHAR(aTitle.c_str()),&xnum,&xmin,&xmax,&ymin,&ymax,PASSCHAR(aOpts.c_str()),aTitle.size(),aOpts.size());
#endif
}

void CHGIVE(
 int aID
,std::string& aTitle
,int& aXnumber
,float& aXmin
,float& aXmax
,int& aYnumber
,float& aYmin
,float& aYmax
){
  int id = aID;
  char chtitl[128];
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  int nwt,idb;
#ifdef WIN32
  hgive(&id,chtitl,80,&ncx,&xmin,&xmax,&ncy,&ymin,&ymax,&nwt,&idb);
#else
  hgive(&id,chtitl,&ncx,&xmin,&xmax,&ncy,&ymin,&ymax,&nwt,&idb,80);
#endif
  chtitl[4*nwt] = 0;
  aTitle = chtitl;
  aXnumber = ncx;
  aXmin = xmin;
  aXmax = xmax;
  aYnumber = ncy;
  aYmin = ymin;
  aYmax = ymax;
}

int CHNOENT(int aID){
  int id = aID;
  int nentries;
  hnoent(&id,&nentries);
  return nentries;
}

void CHFILL(int aID,float aX,float aY,float aW){
  int id = aID;
  float x = aX;
  float y = aY;
  float w = aW;
  hfill(&id,&x,&y,&w);
}

float CHI(int aID,int aI){
  int id = aID;
  int i = aI;
  return hi(&id,&i);
}

float CHIE(int aID,int aI){
  int id = aID;
  int i = aI;
  return hie(&id,&i);
}

float CHIJ(int aID,int aI,int aJ){
  int id = aID;
  int i = aI;
  int j = aJ;
  return hij(&id,&i,&j);
}

float CHIJE(int aID,int aI,int aJ){
  int id = aID;
  int i = aI;
  int j = aJ;
  return hije(&id,&i,&j);
}

float CHIX(int aID,int aI){
  int id = aID;
  int i = aI;
  float x;
  hix(&id,&i,&x);
  return x;
}

void CHIJXY(int aID,int aI,int aJ,float& aX,float& aY){
  int id = aID;
  int i = aI;
  int j = aJ;
  float x,y;
  hijxy(&id,&i,&j,&x,&y);
  aX = x;
  aY = y;
}

int CHXI(int aID,float aX){
  int id = aID;
  float x = aX;
  int i;
  hxi(&id,&x,&i);
  return i;
}

void CHXYIJ(int aID,float aX,float aY,int& aI,int& aJ){
  int id = aID;
  float x = aX;
  float y = aY;
  int i,j;
  hxyij(&id,&x,&y,&i,&j);
  aI = i;
  aJ = j;
}

float CHMIN(int aID){
  int id = aID;
  return hmin(&id);
}

float CHMAX(int aID){
  int id = aID;
  return hmax(&id);
}

float CHSTATI(int aID,int aWhat,STR aChoice,int aNum){
  int id = aID;
  int what = aWhat;
  int num = aNum;
#ifdef WIN32
  return hstati(&id,&what,PASSCHAR(aChoice.c_str()),&num);
#else
  return hstati(&id,&what,PASSCHAR(aChoice.c_str()),&num,aChoice.size());
#endif
}

void CHRESET(int aID,STR aTitle){
  int id = aID;
#ifdef WIN32
  hreset(&id,PASSCHAR(aTitle.c_str()));
#else
  hreset(&id,PASSCHAR(aTitle.c_str()),aTitle.size());
#endif
}

//////////////////////////////////////////////////////////////////////////////
/// Tuple ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CHBNT(int a1,STR a2,STR a3) {
  int v1 = a1;
#ifdef WIN32
  hbnt(&v1,PASSCHAR(a2.c_str()),PASSCHAR(a3.c_str()));
#else
  hbnt(&v1,PASSCHAR(a2.c_str()),PASSCHAR(a3.c_str()),a2.size(),a3.size());
#endif
}

void CHBNAME(int a1,STR a2,void* a3,STR a4) {
  int v1 = a1;
#ifdef WIN32
  hbname(&v1,PASSCHAR(a2.c_str()),(int*)a3,PASSCHAR(a4.c_str()));
#else
  hbname(&v1,PASSCHAR(a2.c_str()),(int*)a3,PASSCHAR(a4.c_str()),a2.size(),a4.size());
#endif
}

void CHBNAM(int a1,STR a2,void* a3,STR a4,int a5) {
  int v1 = a1;
  int v5 = a5;
#ifdef WIN32
  hbnam(&v1,PASSCHAR(a2.c_str()),(int*)a3,PASSCHAR(a4.c_str()),&v5);
#else
  hbnam(&v1,PASSCHAR(a2.c_str()),(int*)a3,PASSCHAR(a4.c_str()),&v5,a2.size(),a4.size());
#endif
}

void CHGNPAR(int a1,STR a2) {
  int v1 = a1;
#ifdef WIN32
  hgnpar(&v1,PASSCHAR(a2.c_str()));
#else
  hgnpar(&v1,PASSCHAR(a2.c_str()),a2.size());
#endif
}

void CHFNT(int a1) {
  int v1 = a1;
  hfnt(&v1);
}

void CHNTVAR2(
 int a1,int a2
,std::string& a3,std::string& a4,std::string& a5
,int& a6,int& a7,int& a8,int& a9) {

  int v1 = a1;
  int v2 = a2;

  char name[32];
  ::memset(name,' ',sizeof(name));
  name[sizeof(name)-1] = 0;

  char block[32];
  ::memset(block,' ',sizeof(block));
  block[sizeof(block)-1] = 0;

  char fullname[64];
  ::memset(fullname,' ',sizeof(fullname));
  fullname[sizeof(fullname)-1]=0;

  int v6,v7,v8,v9;
#ifdef WIN32
  hntvar2(&v1,&v2,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),&v6,&v7,&v8,&v9);
#else
  hntvar2(&v1,&v2,PASSCHAR(name),PASSCHAR(fullname),PASSCHAR(block),&v6,&v7,&v8,&v9,32,64,32);
#endif

  int j;
  for (j=30;j>0;j--) {
    if (name[j] == ' ') name[j] = 0;
  }
  for (j=62;j>0;j--) {
    if (fullname[j] == ' ') fullname[j] = 0;
  }
  for (j=30;j>0;j--) {
    if (block[j] == ' ') block[j] = 0;
    else break;
  }

  a3 = name;
  a4 = fullname;
  a5 = block;

  a6 = v6;
  a7 = v7;
  a8 = v8;
  a9 = v9;
}

int CHGIVEN(
 int aID
,std::string& aTitle
,std::vector<std::string>& aColumns
){
  int id = aID;

  // Get number of columns :
  char chtitl[128];
  int nvar = 0;
  float rmin[1000], rmax[1000];
#ifdef WIN32
  hgiven(&id,chtitl,80,&nvar,PASSCHAR(""),rmin,rmax);
#else
  hgiven(&id,chtitl,&nvar,PASSCHAR(""),rmin,rmax,80,0);
#endif

  // Get title and columns name :
  const int Nchar = 9;
  char* chtag_out = new char[nvar*Nchar+1];
  chtag_out[nvar*Nchar]=0;
  int i;
  for (i=0;i<=80;i++)chtitl[i]=0;
#ifdef WIN32
  hgiven(&id,chtitl,80,&nvar,chtag_out,Nchar,rmin,rmax);
#else
  hgiven(&id,chtitl,&nvar,chtag_out,rmin,rmax,80,Nchar);
#endif
  for (i=80;i>0;i--) {if (chtitl[i] == ' ') chtitl[i] = 0; }

  aTitle = chtitl;

  aColumns.clear();
  char* name = chtag_out;
  for(i=0; i<nvar;i++) {
    name[Nchar-1] = 0;
    int first = 0;
    int last = 0;
    // suppress trailing blanks
    int j;
    for (j=Nchar-2;j>0;j--) {
      if (name[j] == ' ' && last == 0) name[j] = 0;
      else last = j;
    }
    // suppress heading blanks
    for (j=0;j<Nchar;j++) {
      if (name[j] != ' ') break;
      first = j+1;
    }
    aColumns.push_back(std::string(name+first));
    name += Nchar;
  }

  return nvar;
}

int CHGNT(int aID,int aRow){
  int id = aID;
  int row = aRow;
  int ier = 0;
  hgnt(&id,&row,&ier);
  return ier;
}

int CHGNF(int aID,int aRow,float* aBuffer){
  int id = aID;
  int row = aRow;
  int ier = 0;
  hgnf(&id,&row,aBuffer,&ier);
  return ier;
}

void CHOPERA(int aID1,STR aOpts,int aID2,int aID3,float aC1,float aC2) {
  int id1 = aID1;
  int id2 = aID2;
  int id3 = aID3;
  float c1 = aC1;
  float c2 = aC2;
#ifdef WIN32
  hopera(&id1,PASSCHAR(aOpts.c_str()),&id2,&id3,&c1,&c2);
#else
  hopera(&id1,PASSCHAR(aOpts.c_str()),&id2,&id3,&c1,&c2,aOpts.size());
#endif
}

//////////////////////////////////////////////////////////////////////////////
bool CHEDIR(
 const std::string& aDir
,bool aPAWC
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> drs = CHDIRS(aPAWC);
  std::vector<std::string>::iterator it;
  for(it=drs.begin();it!=drs.end();++it){
    if(aDir==(*it)) return true;
  }
  return false;
}
bool CHISID(int aID) {
  int id = aID;
  return (hisid(&id)==1?true:false);
}
/*
std::vector<int> CHIDALL() {
  // We should use first a function getting the number of ids only.
  // See hids.f in this directory and hidall.F code.
  int ids[100000];
  int n;
  hidall(ids,&n);
  std::vector<int> r;
  r.resize(n,0);
  for(int i=0;i<n;i++) r[i] = ids[i];
  return r;
}
std::vector<int> CHID1() {
  // We should use first a function getting the number of 1D ids only.
  // See hids.f in this directory and hid1.F code.
  int ids[100000];
  int n;
  hid1(ids,&n);
  std::vector<int> r;
  r.resize(n,0);
  for(int i=0;i<n;i++) r[i] = ids[i];
  return r;
}
std::vector<int> CHID2() {
  // We should use first a function getting the number of 2D ids only.
  // See hids.f in this directory and hid1.F code.
  int ids[100000];
  int n;
  hid2(ids,&n);
  std::vector<int> r;
  r.resize(n,0);
  for(int i=0;i<n;i++) r[i] = ids[i];
  return r;
}
bool CHEXIST(int aID) {
  int id = aID;
  // What is the mapping of F77 LOGICAL to C ?
  logical l = hexist(&id);
}
//////////////////////////////////////////////////////////////////////////////
void CHCLR(
)
//////////////////////////////////////////////////////////////////////////////
// Clear //PAWC structure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //CHCDIR("//PAWC"," ");
  std::vector<std::string> drs = CHDIRS(true);
 {std::vector<std::string>::iterator it;
  for(it=drs.begin();it!=drs.end();++it){
    std::string& dir = *it;
    CHDDIR(dir);
  }}
  CHDELET(0);
}
*/

#ifdef WIN32
#define pawc PAWC
#define quest QUEST
//#define hcbits HCBITS
#define hcbook HCBOOK
#define rzcl RZCL
#else
#define pawc pawc_
#define quest quest_
//#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
#endif

extern "C" int pawc[1];
extern "C" int quest[100];
//extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];

//////////////////////////////////////////////////////////////////////////////
std::vector<std::string> CHDIRS(
 bool aPAWC
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<std::string> dirs;

  if(aPAWC) {

    int* iq = &(pawc[17]);

    int* lq = &(pawc[9]);
    int lcdir = hcbook[6];
    int lf = lq[lcdir-1];
    while(lf!=0) {

      int ncw = 4;
      //int ihdir[4];
      //CZITOH(iq+lf+1,ihdir,&ncw);
      int* ihdir = iq+lf+1;

      char chdir[17];
     {for(int i=0;i<17;i++) chdir[i] = 0;}
      int nch=16;
#ifdef WIN32
      uhtoc(ihdir,&ncw,chdir,16,&nch);
#else
      uhtoc(ihdir,&ncw,chdir,&nch,16);
#endif
     {for(int i=17-1;i>0;i--) {
        if(chdir[i]==0) continue;
        if(chdir[i]!=' ') break;
        chdir[i] = 0;
     }}

      dirs.push_back(std::string(chdir));

      lf = lq[lf];
    }

  } else { //On a UNIT.

    int* iq = &(pawc[17]);

    const int KLS = 26;
    const int KNSD = 23;
    int lcdir = rzcl[2];
    int ls = iq[lcdir+KLS];
    int ndir = iq[lcdir+KNSD];
    for (int k=0;k<ndir;k++) {
      int lcdir = rzcl[2];
      int ncw = 4;
      int ihdir[4];
      CZITOH(iq+(lcdir+ls+7*k),ihdir,&ncw);

      char chdir[17];
     {for(int i=0;i<17;i++) chdir[i] = 0;}
      int nch=16;
#ifdef WIN32
      uhtoc(ihdir,&ncw,chdir,16,&nch);
#else
      uhtoc(ihdir,&ncw,chdir,&nch,16);
#endif
     {for(int i=17-1;i>0;i--) {
        if(chdir[i]==0) continue;
        if(chdir[i]!=' ') break;
        chdir[i] = 0;
      }}

      dirs.push_back(std::string(chdir));
    }
  }

  return dirs;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<int> CHKEYS(
)
//////////////////////////////////////////////////////////////////////////////
// Apply on a //LUN<i> directory.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<int> keys;
  for (int key=1;key<1000000;key++) {
    CRZINK(key,0,"S");
    if (quest[0]) break;
    if (quest[13] & 8) {
      continue;
    }
    int id = quest[20];
    keys.push_back(id);
  }
  return keys;
}

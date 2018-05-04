/*
   For doing RZ write : 
   - A //LUN<i> has to be open.
   - We create a //PAWC/LUN<i> under //PAWC.
   - The hierarchy is done in //PAWC/LUN<i> and histos 
     are booked here. (A HBOOK1 books anyway in a directory under //PAWC).
   - The //PAWC/LUN<i> structure has to be duplicated under //LUN<i>.
   - Then a global CHROUT(0,0,"T") can be done after doing :
        CHCDIR("//PAWC/LUN<i>"," ")
        CHCDIR("//LUN<i>"," ")

*/

#include <BatchLab/Zebra/Tree.h>

#include <Slash/Core/ISession.h>
#include <Slash/Store/IObject.h>

#include <AIDA/IManagedObject.h>

#include <inlib/sprintf>
#include <inlib/args>
#include <inlib/cast>

#include <Lib/Debug.h>

#include <BatchLab/Zebra/Manager.h>

#include <BatchLab/Zebra/CHBOOK.h>

#include <cstring>
#include <cstdlib> //rand

#ifdef WIN32
# define uhtoc UHTOC
# define oclose OCLOSE
extern "C" void __stdcall uhtoc(int*,int*,const char*,const int,int*);
extern "C" void __stdcall oclose(int*);
#else
# define uhtoc uhtoc_
# define oclose oclose_
extern "C" void uhtoc(int*,int*,const char*,int*,int);
extern "C" void oclose(int*);
#endif

//static int sUnit = 200;
static int sUnit = 1;

//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Tree::Tree(
 Slash::Core::ISession& aSession
,const std::string& aStoreName
,bool aReadOnly
,bool aCreateNew
,const std::string& aOptions
,Zebra::Manager& aManager
,int aVerboseLevel
)
:MemoryTree(aSession,aStoreName) //FIXME : have a name # storeName
,fStoreName(aStoreName)
,fManager(aManager)
,fIsValid(false)
,fUnit(0)
,fWriting(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //aVerboseLevel = 1;

  setVerboseLevel(aVerboseLevel);

  inlib::args args(aOptions,";",true);

  int unit = 0;
 {std::string value;
  if(args.find("unit",value)) {
    if(!inlib::to<int>(value,unit) || (unit<0)) {
      out() << "BatchLab::Zebra::Tree::Tree :"
          << " bad syntax in option string " << inlib::sout(aOptions)
          << std::endl;
      return;
    }
  } else {
    unit = sUnit;
    sUnit++; 
  }}

  ///////////////////////////////////
  // Open the HBOOK file :
  ///////////////////////////////////
  int record_size = 0;

  std::string opts = " ";
  if(aCreateNew) {
    // Create new file :

    opts = "NQ";
    // maximum number of records
    fManager.get_quest()[9] = 65000;
    record_size = 1024;

    fWriting = true;
  } else { 
    // Open an existing file.
    if(!aReadOnly) {
      out() << "BatchLab::Zebra::Tree::Tree :"
          << " update mode not supported."
          << std::endl;
      return;
    }

  }

  std::string path,name,suffix;
  if(!inlib::path_name_suffix(fStoreName,path,name,suffix)) return;

  //fDirectory = inlib::nosuffix(name);
  inlib::sprintf(fDirectory,32,"LUN%d",unit);
  
  int ier = CHROPEN(unit,fDirectory,fStoreName,opts,record_size);
  if(ier) {
    out() << "BatchLab::Zebra::Tree::Tree :"
        << " error on hropen, code " << ier 
        << std::endl;
    return;
  }

  if(fManager.get_quest()[0]) {
    out() << "BatchLab::Zebra::Tree::Tree :"
        << " error, cannot open input file " << inlib::sout(aStoreName)
        << std::endl;
    return;
  } 

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::Tree :"
        << " input file " << inlib::sout(aStoreName) << " opened "
        << (fWriting?" for writing.":" for reading.")
        << std::endl;
  }

  CHCDIR("//PAWC"," ");
  if(CHEDIR(fDirectory,true)) CHDDIR(fDirectory);
  CHMDIR(fDirectory," ");

  if(fWriting) {
  } else { //reading.
    // Clone the //LUN<i> directory tree structure in //PAWC/LUN<i>
    // (the objects are not copied).
    clone_tree("//"+fDirectory);
  }

  CHCDIR("//PAWC/"+fDirectory," ");

  fUnit = unit;
  fIsValid = true;
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Zebra::Tree::~Tree(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::~Zebra::Tree : cleanup..." << std::endl;
  }

  rootFolder().clear();
  
  if(fIsValid) {

    //CHDELET(0);
    if(verboseLevel()) {
      out() << "BatchLab::Zebra::Tree::~Zebra::Tree : hrend..." << std::endl;
    }
    CHREND(fDirectory);
    oclose(&fUnit);
    fUnit = 0;

    CHCDIR("//PAWC"," ");
    if(CHEDIR(fDirectory,true)) CHDDIR(fDirectory);

  }  

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::~Zebra::Tree : end." << std::endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void* BatchLab::Zebra::Tree::cast(
 const std::string& a_class
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  INLIB_IF_CAST(BatchLab::Zebra::Tree)
  else return MemoryTree::cast(a_class);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Tree::storeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStoreName;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Tree::storeType(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return "BatchLab::Zebra::Tree";
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::isValid(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIsValid;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::commit(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fIsValid) return false;
  if(!fWriting) return true;

  std::string pwd = CHPWD();

  CHCDIR("//PAWC/"+fDirectory," ");
  CHCDIR("//"+fDirectory," ");
  CHROUT(0,0,"T");

  //CHLDIR("//PAWC/"+fDirectory,"T");
  //CHLDIR("//"+fDirectory,"T");

  CHCDIR(pwd," ");

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::close(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fIsValid) return true; //done or not opened.
  CHREND(fDirectory);
  oclose(&fUnit);
  fUnit = 0;
  fIsValid = false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::mkdir(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// mkdir in MemoryTree, //PAWC/LUN<i> and //LUN<i>
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fIsValid) return false;

  //FIXME : handle .. in path.

  if(aPath.find("..")!=std::string::npos) {
    out() << "BatchLab::Zebra::Tree::mkdir :"
        << " path containing .. not yet supported."
        << std::endl;
    return false;
  }

  if(!MemoryTree::mkdir(aPath)) return false;
 
  std::string pwd = CHPWD(); //Under //PAWC/LUN<i>

  std::string path,name,suffix;
  inlib::path_name_suffix(aPath,path,name,suffix);

  // mkdir under //PAWC/LUN<i>
  if(path.size()) {
    if(path.size() && (path[0]=='/')) {
      path = "//PAWC/"+fDirectory+path;
    }
    CHCDIR(path," ");
  }

  if(CHEDIR(name,true)) {
    CHCDIR(pwd," ");
    return false;
  }

  CHMDIR(name," ");

  // mkdir under //LUN<i> :
  toLUN();

  //In principle, no need to check if already exists.
  CHMDIR(name," ");

  CHCDIR(pwd," ");

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::mkdirs(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tree::mkdirs :"
      << " not yet implemented."
      << std::endl;
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::rmdir(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  out() << "BatchLab::Zebra::Tree::rmdir :"
      << " not yet implemented."
      << std::endl;
  return false;
}

bool BatchLab::Zebra::Tree::hcd(const std::string& aPath){
  // cd under //PAWC/LUN<i>
  if(!fIsValid) return false;

  //FIXME : handle .. in path.

  if((aPath!="..") && (aPath.find("..")!=std::string::npos)) {
    out() << "BatchLab::Zebra::Tree::hcd :"
        << " path containing .. not yet supported."
        << std::endl;
    return false;
  }

  if(aPath.empty()) return true;

  if(aPath=="..") {
    CHCDIR("\\"," ");
    return true;
  }

  if(aPath=="/") {
    CHCDIR("//"+fDirectory," ");
    return true;
  }

  if(aPath.size() && (aPath[0]=='/')) {
    CHCDIR("//PAWC/"+fDirectory+aPath," ");
  } else {
    CHCDIR(aPath," ");
  }


  return true;
}

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::cd(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// cd in MemoryTree and //PAWC/LUN<i>
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fIsValid) return false;

  //FIXME : handle .. in path.

  if((aPath!="..") && (aPath.find("..")!=std::string::npos)) {
    out() << "BatchLab::Zebra::Tree::cd :"
        << " path containing .. not yet supported."
        << std::endl;
    return false;
  }

  if(!MemoryTree::cd(aPath)) return false;

  return hcd(aPath);
}
//////////////////////////////////////////////////////////////////////////////
AIDA::IManagedObject* BatchLab::Zebra::Tree::find(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath = /dir1/dir2/.../SID
//       = SID   (where SID = storage identifier)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aPath.empty()) return 0;

  if((aPath!="..") && (aPath.find("..")!=std::string::npos)) {
    out() << "BatchLab::Zebra::Tree::find :"
        << " path containing .. not yet supported."
        << std::endl;
    return 0;
  }

  // Look first in memory :
 {AIDA::IManagedObject* object = MemoryTree::find(aPath);
  if(object) {
    if(verboseLevel()) {
      out() << "BatchLab::Zebra::Tree::find :" 
          << " find " << inlib::sout(aPath) << " in memory."
          << std::endl;
    } 
    return object;
  }}

  // Not found in memory, get in file :
  if(!fIsValid) return 0;

  std::string pwd = CHPWD(); //Under //PAWC/LUN<i>

  std::string path,SID,suffix;
  inlib::path_name_suffix(aPath,path,SID,suffix);
 
  // go under //PAWC/LUN<i> :
  if(path.size()) {
    if(path.size() && (path[0]=='/')) {
      path = "//PAWC/"+fDirectory+path;
    }
    CHCDIR(path," ");
  }

  // go under //LUN<i> :
  std::string from_pwd = CHPWD(); //Under //PAWC/LUN<i>
  std::string to_pwd = from_pwd;
  inlib::replace(to_pwd,"//PAWC/"+fDirectory,"//"+fDirectory);
  CHCDIR(to_pwd," ");

  // Action :
  Slash::Store::IObject* object = retreiveObject(SID);
  if(!object) {
    out() << "BatchLab::Zebra::Tree::find :"
        << " can't retreive object " << inlib::sout(SID) 
        << " in store " << inlib::sout(storeName()) << "." 
        << std::endl;
    CHCDIR(pwd," ");
    return 0;
  } 

  AIDA::IManagedObject* aobject = INLIB_CAST(*object,AIDA::IManagedObject);
  if(!aobject) {
    out() << "BatchLab::Zebra::Tree::find :"
        << " for " << inlib::sout(aPath) 
        << ",Slash::Store::IObject not a AIDA::IManagedObject."
        << " in store " << inlib::sout(storeName()) << "." 
        << std::endl;
    delete object;
    CHCDIR(pwd," ");
    return 0;
  }

  if(!MemoryTree::mkdirs(path)) {
    delete object;
    CHCDIR(pwd," ");
    return 0;
  }
  if(!manageObject(object,path)) {
    delete object;
    CHCDIR(pwd," ");
    return 0;
  }

  CHCDIR(pwd," ");

  return aobject;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Zebra::Tree::hierarchy(
 const std::string& aWhat
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aWhat=="memory") return MemoryTree::hierarchy();

  if(!fIsValid) return "";

  std::string pwd = CHPWD(); //Under //PAWC/LUN<i>

  std::string sout;
  sout += "<tree>";

  CHCDIR("//"+fDirectory," ");

  visitTree(sout);

  sout += "</tree>";

  CHCDIR(pwd," ");

  return sout;
}

//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Zebra::Tree::ls(
 const std::string& aPath
,bool aRecursive
,std::ostream&
) const
//////////////////////////////////////////////////////////////////////////////
// PAW /ZEBRA/RZ/LDIR output style.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME : ls what ? It seems more usefull to dump store keys.
  //  But could be interesting to see objects in MemoryTree 
  // ready to be committed.

  if(!fIsValid) return false;

  if(!MemoryTree::isADirectory(aPath)) return false;

  if(aPath.find("..")!=std::string::npos) {
    out() << "BatchLab::Zebra::Tree::ls :"
        << " path containing .. not yet supported."
        << std::endl;
    return false;
  }

  std::string pwd = CHPWD(); //Should be under //PAWC/LUN<i>.

  // go under //PAWC/LUN<i> :
  if(aPath.size()) {
    if(aPath.size() && (aPath[0]=='/')) {
      CHCDIR("//PAWC/"+fDirectory+aPath," ");
    } else {
      CHCDIR(aPath," ");
    }
  }

  // go under //LUN<i> :
  std::string from_pwd = CHPWD(); //Under //PAWC/LUN<i>
  std::string to_pwd = from_pwd;
  inlib::replace(to_pwd,"//PAWC/"+fDirectory,"//"+fDirectory);
  CHCDIR(to_pwd," ");

  // Action :
  ls_dir(aRecursive);

  CHCDIR(pwd," ");

  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Zebra::Tree::ls_dir(
 bool aRecursive
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
/* Dump example from CERN/PAW LDIR command :
 ************** Directory ===> //LUN22 <===

                  Created 070528/0832  Modified 070528/0832


 ===> List of subdirectories 
 HISTOS           Created 070528/0832 at record          4
 TUPLES           Created 070528/0832 at record         16

 ===> List of objects 
     HBOOK-ID  VARIABLE                             CYCLE    DATE/TIME   NDATA 
*/



  out() << " ************** Directory ===> "
      << CHPWD()
      << " <===" 
      << std::endl;

  std::vector<std::string> drs = CHDIRS(false);

  if(drs.size()) {
    out() << " ===> List of subdirectories" << std::endl;
    std::vector<std::string>::iterator it;
    for(it=drs.begin();it!=drs.end();++it){
      std::string& dir = *it;
      // Exa : 
      // HISTOS           Created 070528/0832 at record          4
      out() << " " << dir << std::endl;
    }
    out() << std::endl;
  }

  out() << " ===> List of objects" << std::endl;
  //HBOOK-ID  VARIABLE                             CYCLE    DATE/TIME   NDATA 
  //    10         0                                   1   070528/0832    151 

  out() << "     HBOOK-ID" << std::endl;

 {std::vector<int> keys = CHKEYS();
  std::vector<int>::iterator it;
  for(it=keys.begin();it!=keys.end();++it){
    out() << "         " << (*it) << std::endl;
  }}

  out() << std::endl;

  if(aRecursive) {
    std::vector<std::string>::iterator it;
    for(it=drs.begin();it!=drs.end();++it){
      std::string& dir = *it;
      CHCDIR(dir," ");
      ls_dir(aRecursive);
      CHCDIR("\\"," ");
    }
  }

}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Zebra::Tree::clone_tree(
 const std::string& aDirectory
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  CHCDIR(aDirectory," ");

  std::string from_pwd = CHPWD();

  std::vector<std::string> drs = CHDIRS(false);

 {std::string to_pwd = from_pwd;
  inlib::replace(to_pwd,"//"+fDirectory,"//PAWC/"+fDirectory);
  CHCDIR(to_pwd," ");
  std::vector<std::string>::iterator it;
  for(it=drs.begin();it!=drs.end();++it){
    std::string& dir = *it;
    CHMDIR(dir," ");

    MemoryTree::mkdir(dir);

  }}

  CHCDIR(from_pwd," ");

 {std::vector<std::string>::iterator it;
  for(it=drs.begin();it!=drs.end();++it){
    std::string& dir = *it;
    clone_tree(from_pwd+"/"+dir);
  }}
}

//////////////////////////////////////////////////////////////////////////////
void BatchLab::Zebra::Tree::visitTree(
 std::string& aOut
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {std::vector<int> keys = CHKEYS();
  std::vector<int>::iterator it;
  for(it=keys.begin();it!=keys.end();++it){
    int key = (*it);
    std::string s;
    s += inlib::to<int>(key);
/* FIXME : Can we know the type of object without doing a HRIN ?
    s += ";";
    std::string cs;
    inlib::sprintf(cs,32,"%d",key->cycle());
    s += cs;
    if(aAll) {
      s += " class=";
      s += key->className();
      s += " title=";
      s += "\""+inlib::to_xml(key->title())+"\"";
    }
*/
    aOut += "<treeItem><label>" + s + "</label></treeItem>";
  }}

 {std::vector<std::string> dirs = CHDIRS(false);
  std::vector<std::string>::iterator it;
  for(it=dirs.begin();it!=dirs.end();++it){
    aOut += "<treeItem><label>" + inlib::to_xml(*it) + "</label>";
    CHCDIR(*it," ");
    visitTree(aOut);
    CHCDIR("\\"," ");
    aOut += "</treeItem>";
  }}

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Data specific area :

//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Tree::retreiveObject(
 const std::string& aSID
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int id; 
  if(!inlib::to<int>(aSID,id)) {
    out() << "BatchLab::Zebra::Tree::retreiveObject :" 
        << " found " << inlib::sout(aSID)
        //        << " in store " << inlib::sout(storeName()) 
        << "."
        << std::endl;
    return 0;
  }
  
  //  The retreived HBOOK object is transient in //PAWC/LUN<i>, 
  // it is deleted after the creation of the BatchLab::Histogram.

  Slash::Store::IObject* object = 0;

  CHRIN(id,999,0);
  if(fManager.get_quest()[0]) {
    out() << "BatchLab::Zebra::Tree::retreiveObject :" 
        << " error cannot read ID = " << id
        << std::endl;
    return 0;
  }
  CHDCOFL();
  int* iq = &(fManager.get_pawc()[17]);
  int lcid  = fManager.get_hcbook()[10];
  //int lcont = lq[lcid-1];
  if(fManager.get_hcbits()[3]) {
    if (iq[lcid-2] == 2) {
      object = convertRowWiseTuple(id);
    } else {
      object = convertColumnWiseTuple(id);
    }
    CHDELET(id);
    return object;
  }
  if(fManager.get_hcbits()[0] && fManager.get_hcbits()[7]) {
    object = convertProfile(id);
    CHDELET(id);
    return object;
  }
  if(fManager.get_hcbits()[0]) {
    object = convertHistogram1D(id);
    CHDELET(id);
    return object;
  }
  if(fManager.get_hcbits()[1] || fManager.get_hcbits()[2]) {
    object = convertHistogram2D(id);
    CHDELET(id);
    return object;
  }

  out() << "BatchLab::Zebra::Tree::retreiveObject :" 
      << " don't know how to convert ID = " << id
      << std::endl;

  return 0;
}

#include <BatchLab/Core/Histogram.h>
#include <BatchLab/Core/MemoryTuple.h>

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <inlib/histo/h3d>
#include <inlib/histo/p1d>
#include <inlib/histo/p2d>

typedef inlib::histo::histo_data<double,unsigned int,double> hd_data;
typedef inlib::histo::profile_data<double,unsigned int,double,double> pd_data;

Slash::Store::IObject* BatchLab::Zebra::Tree::convertHistogram1D(int id) {
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertHistogram1D :"
        << " " << id << std::endl;
  }

  std::string idname;
  //if (id > 0) inlib::sprintf(idname,128,"h%d",id);
  //else        inlib::sprintf(idname,128,"h_%d",-id);
  inlib::sprintf(idname,128,"%d",id);

  int nentries = CHNOENT(id);

  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  CHGIVE(id,title,ncx,xmin,xmax,ncy,ymin,ymax);

  inlib::histo::h1d* hcl = 0;

  if (fManager.get_hcbits()[5]) {
    // FIXME : how to get edges ?     
    std::vector<double> edges(ncx+1);
    for(int index=0;index<ncx;index++) {
      edges[index] = CHIX(id,index+1); //HBOOK counts in [1,NX]
    }
    edges[ncx] = xmax;
    hcl = new inlib::histo::h1d(title,edges);
  } else {
    hcl = new inlib::histo::h1d(title,ncx,xmin,xmax);
  }

  // have to overrite some fields :
  hd_data dhisto = hcl->get_histo_data();
  delete hcl;

  int nx = ncx+2;
  float sw = 0;
  int i;
  for (i=0;i<nx;i++) sw += CHI(id,i);

  for (i=0;i<nx;i++) {
    float height = CHI(id,i);
    int offset = i;
    dhisto.m_bin_Sw[offset] = height;
    if(fManager.get_hcbits()[8]) {
      float error = CHIE(id,i);
      dhisto.m_bin_Sw2[offset] = error * error;
    }
    if(sw) {
      dhisto.m_bin_entries[offset] = (int)((nentries * height) / sw);
    }
  }

  if(ncx) {
    float dx = (xmax-xmin)/ncx;
    for (i=0;i<ncx;i++) {
      float x = xmin + dx * (0.5F+i);
      int offset = i+1;
      double height = dhisto.m_bin_Sw[offset];
      dhisto.m_bin_Sxw[offset][0] = x * height;
      dhisto.m_bin_Sx2w[offset][0] = x * x * height;
    }
  }

  int allEntries = dhisto.get_all_entries();
  if(allEntries!=nentries) { //FIXME
    //Correct some bins randomly (beurk, do you have a better idea ?) :
    int diff = nentries-allEntries;
    unsigned int nx = dhisto.m_axes[0].m_number_of_bins;
    for(int i=0;i<diff;i++) {
      int ri = ::rand();
      int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
      int offset = ibin;
      dhisto.m_bin_entries[offset]++;
    }
  }


  hcl = new inlib::histo::h1d("",10,0,1);
  hcl->copy_from_data(dhisto);
  hcl->update_fast_getters();

  return new BatchLab::Histogram1D(out(),idname,*hcl);
}
Slash::Store::IObject* BatchLab::Zebra::Tree::convertHistogram2D(int id) {
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertHistogram2D :"
        << " " << id << std::endl;
  }

  std::string idname;
  //if (id > 0) inlib::sprintf(idname,128,"h%d",id);
  //else        inlib::sprintf(idname,128,"h_%d",-id);
  inlib::sprintf(idname,128,"%d",id);

  int nentries = CHNOENT(id);

  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  CHGIVE(id,title,ncx,xmin,xmax,ncy,ymin,ymax);

  inlib::histo::h2d* hcl = 
    new inlib::histo::h2d(title,ncx,xmin,xmax,ncy,ymin,ymax);
  //have to overrite some fields :
  hd_data dhisto = hcl->get_histo_data();
  delete hcl;

  int nx = ncx+2;
  int ny = ncy+2;

  int i,j;

  float sw = 0;
 {for (j=0;j<ny;j++) for (i=0;i<nx;i++) sw += CHIJ(id,i,j);}

  for (j=0;j<ny;j++) {
    for (i=0;i<nx;i++) {
      float height = CHIJ(id,i,j);
      int offset = i + nx * j;
      dhisto.m_bin_Sw[offset] = height;
      //if(height!=0) printf("debug : %d %d %g\n",i,j,height);
      if(fManager.get_hcbits()[8]) {
        float error = CHIJE(id,i,j);
        dhisto.m_bin_Sw2[offset] = error * error;
      }
      if(sw) {
        dhisto.m_bin_entries[offset] = (int)((nentries * height) / sw);
      }
    }
  }


  if(ncx && ncy) {
    float dx = (xmax-xmin)/ncx;
    float dy = (ymax-ymin)/ncy;
    for (j=0;j<ncy;j++) {
      float y = ymin + dy * (0.5F+j);
      for (i=0;i<ncx;i++) {
        int offset = (i+1) + nx * (j+1);
        double height = dhisto.m_bin_Sw[offset];
        float x = xmin + dx * (0.5F+i);
        dhisto.m_bin_Sxw[offset][0] = x * height;
        dhisto.m_bin_Sx2w[offset][0] = x * x * height;
        dhisto.m_bin_Sxw[offset][1] = y * height;
        dhisto.m_bin_Sx2w[offset][1] = y * y * height;
      }
    }
  }

  int allEntries = dhisto.get_all_entries();
  if(allEntries!=nentries) { //FIXME
    //Correct some bins randomly (beurk, do you have a better idea ?) :
    int diff = nentries-allEntries;
    unsigned int nx = dhisto.m_axes[0].m_number_of_bins;
    unsigned int ny = dhisto.m_axes[1].m_number_of_bins;
    for(int i=0;i<diff;i++) {
      int ri = ::rand();
      int ibin =  int((nx-1) * ((double)ri/(double)RAND_MAX)) + 1;
      int rj = ::rand();
      int jbin =  int((ny-1) * ((double)rj/(double)RAND_MAX)) + 1;
      int offset = ibin + jbin * dhisto.m_axes[1].m_offset;
      dhisto.m_bin_entries[offset]++;
    }
  }

  hcl = new inlib::histo::h2d("",10,0,1,10,0,1);
  hcl->copy_from_data(dhisto);
  hcl->update_fast_getters();

  return new BatchLab::Histogram2D(out(),idname,*hcl);
}

Slash::Store::IObject* BatchLab::Zebra::Tree::convertProfile(int id){
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertProfile :"
        << " " << id << std::endl;
  }

// the following structure is used in Hbook
//    lcid points to the profile in array iq
//    lcont = lq(lcid-1)
//    lw    = lq(lcont)
//    ln    = lq(lw)
//      if option S jbyt(iq(lw),1,2) = 1
//      if option I jbyt(iq(lw),1,2) = 2

  std::string idname;

  //if (id > 0) inlib::sprintf(idname,128,"h%d",id);
  //else        inlib::sprintf(idname,128,"h_%d",-id);
  inlib::sprintf(idname,128,"%d",id);

  //int nentries = CHNOENT(id);

  std::string title;
  int ncx,ncy;
  float xmin,xmax,ymin,ymax;
  CHGIVE(id,title,ncx,xmin,xmax,ncy,ymin,ymax);

  int* lq = &(fManager.get_pawc()[9]);
  int* iq = &(fManager.get_pawc()[17]);
  void* qq = iq;
  float* q = (float*)qq;

  int lcid  = fManager.get_hcbook()[10];
  int lcont = lq[lcid-1];

  int lw = lq[lcont];
  int ln = lq[lw];

  //FIXME : handle option string
  //std::string option= " ";
  //if (iq[lw] == 1) option = "S";
  //if (iq[lw] == 2) option = "I";

  inlib::histo::p1d* hcl = new inlib::histo::p1d(title,ncx,xmin,xmax);
  //have to overrite some fields :
  pd_data dhisto = hcl->get_histo_data();
  delete hcl;

  //FXIME ymin,ymax);

  const int kCON1 = 9;
  int i;
  for (i=1;i<=ncx;i++) {
    int n = (int)q[ln+i];

    float content = q[lcont+kCON1+i];
    //float error   = q[lw+i];
    //printf("debug : +++ : %d : %d %g %g\n",i,n,content,error);

    int offset = i;

    //FIXME : Assumed weight = 1 at filling ???
    dhisto.m_bin_entries[offset] = n;
    dhisto.m_bin_Sw[offset] = (double)n;
    dhisto.m_bin_Sw2[offset] = (double)n;

    //FIXME dhisto.m_bin_Sxw[offset][0] = 
    //FIXME dhisto.m_bin_Sx2w[offset][0] = 

    dhisto.m_bin_Svw[offset] = (double)(content * n);
    //FIXME : dhisto.m_bin_Sv2w[offset] = (double)(error * error);
    dhisto.m_bin_Sv2w[offset] = 0;
  }

  //GB p->SetEntries(nentries);

  hcl = new inlib::histo::p1d("",10,0,1);
  hcl->copy_from_data(dhisto);
  hcl->update_fast_getters();

  return new BatchLab::Profile1D(out(),idname,*hcl);
}

Slash::Store::IObject* BatchLab::Zebra::Tree::convertRowWiseTuple(int id) {
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertRowWiseTuple :"
        << " " << id << std::endl;
  }

  std::string idname;
  //if (id > 0) inlib::sprintf(idname,128,"h%d",id);
  //else        inlib::sprintf(idname,128,"h_%d",-id);
  inlib::sprintf(idname,128,"%d",id);

  int nentries = CHNOENT(id);

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertRowWiseTuple :"
        << " converting RWN with ID= " << id 
        << ", nentries = " << nentries
        << std::endl;
  }

  std::string title;
  std::vector<std::string> cols;
  int nvar = CHGIVEN(id,title,cols);
  if(nvar<=0) {
    out() << "BatchLab::Zebra::Tree::convertRowWiseTuple :"
        << " RWN with ID= " << id 
        << " as no columns."
        << std::endl;
    return 0;
  }

  CHGNPAR(id,"?");

  std::string columns;
  int i;
  for(i=0; i<nvar;i++) {
    if(i) columns += ",";
    columns += "float " + cols[i];
  }

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertRowWiseTuple :"
        << " columns are " << inlib::sout(columns)
        << std::endl;
  }

  BatchLab::MemoryTuple* tuple = new BatchLab::MemoryTuple(out(),
                                                 idname,title,
                                                 columns);

  float* x = new float[nvar];
  for(i=1;i<=nentries;i++) {
    int ier = CHGNF(id,i,x);
    if(ier) {
      out() << "BatchLab::Zebra::Tree::convertRowWiseTuple :"
          << " error on hgnf, code " << ier 
          << std::endl;
      break;
    }

    int j;
    for(j=0; j<nvar;j++) {
      tuple->fill(j,x[j]);
    }

    tuple->addRow();
  }

  delete [] x;

  return tuple;
}
//////////////////////////////////////////////////////////////////////////////
Slash::Store::IObject* BatchLab::Zebra::Tree::convertColumnWiseTuple(
 int id
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
        << " " << id << std::endl;
  }

  std::string idname;
  //if (id > 0) inlib::sprintf(idname,128,"h%d",id);
  //else        inlib::sprintf(idname,128,"h_%d",-id);
  inlib::sprintf(idname,128,"%d",id);

  int nentries = CHNOENT(id);

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
        << " converting CWN with ID= " << id 
        << ", nentries = " << nentries
        << std::endl;
  }

  std::string title;
  std::vector<std::string> cols;
  int nvar = CHGIVEN(id,title,cols);
  if(nvar<=0) {
    out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
        << " CWN with ID= " << id 
        << " as no columns."
        << std::endl;
    return 0;
  }

  int* charflag = new int[nvar];
  int* lenchar  = new int[nvar];
  int maxbuf = 500000;
  char* bigbuf = new char[maxbuf];

  int i;
  for (i=0;i<nvar;i++) charflag[i] = 0; 

  typedef void* pointer;

  int ischar = 0;
  pointer add = bigbuf;
  CHBNAM(id," ",&add,"$CLEAR",ischar);

  int bufpos = 0;
  int isachar = 0;
  int oldischar = -1;

  std::string oldblock = "OLDBLOCK";

  std::string columns;
  int* colts = new int[nvar];
  pointer* colas = new pointer[nvar];

  for(i=0; i<nvar;i++) {

     int ivar = i+1;
     int nsub,itype,isize,ielem;
     std::string name,fullname,block;
     CHNTVAR2(id,ivar,name,fullname,block,nsub,itype,isize,ielem);

     if(i) columns += ",";
     if (itype == 1 && isize == 4) {
       columns += "float ";
       columns += fullname;
       colts[i] = 1;
     }
     if (itype == 1 && isize == 8) {
       columns += "double ";
       columns += fullname;
       colts[i] = 2;
     }
     if (itype == 2) {
       columns += "int ";
       columns += fullname;
       colts[i] = 3;
     }
     if (itype == 3) {
       //FIXME : unsigned int
       columns += "int ";
       columns += fullname;
       colts[i] = 3;
     }
     if (itype == 4) {
       //FIXME : unsigned int
       columns += "int ";
       columns += fullname;
       colts[i] = 3;
     }
     if (itype == 5) {
       columns += "string ";
       columns += fullname;
       colts[i] = 4;
     }

     int ischar;
     if (itype == 5) ischar = 1;
     else            ischar = 0;

     if( (ischar != oldischar) || (oldblock!=block) ){
        oldblock = block;
        oldischar = ischar;
        pointer add = bigbuf+bufpos;
        CHBNAM(id,block,&add,"$SET",ischar);
     }

     colas[i] = bigbuf+bufpos;

     bufpos += isize*ielem;
     if (ischar) {
       isachar++; 
       charflag[i] = bufpos-1; 
       lenchar[i] = isize*ielem;
     }

  }

  if(verboseLevel()) {
    out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
        << " columns are " << inlib::sout(columns)
        << std::endl;
  }

  BatchLab::MemoryTuple* tuple = new BatchLab::MemoryTuple(out(),
                                                 idname,title,
                                                 columns);
  for(i=1;i<=nentries;i++) {
    int ier = CHGNT(id,i);
    if(ier) {
      out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
          << " error on hgnt, code " << ier 
          << std::endl;
      break;
    }
    if (isachar) { // if column is character, set terminator
      for (int j=0;j<nvar;j++) {
        int cf = charflag[j];
        if (cf) {
          if(cf>=maxbuf) {
            out() << "BatchLab::Zebra::Tree::convertColumnWiseTuple :"
                << " for entry " << i << " (" << nentries << ")"
                << " and for column " << j << " (" << nvar << ")"
                << " number of char (" << cf 
                << ") too large (" << maxbuf << ")." 
                << std::endl;
          } else {
            bigbuf[cf] = '\0';
            if (bigbuf[cf-1] != ' ') continue;
            bigbuf[cf-1] = '\0';
            if (bigbuf[cf-2] != ' ') continue;
            bigbuf[cf-2] = '\0';
          }
        }
      }
    }
    for(int j=0; j<nvar;j++) {
      if(colts[j]==1) { //float
        tuple->fill(j,*((float*)colas[j]));
      } else if(colts[j]==2) { //double
        tuple->fill(j,*((double*)colas[j]));
      } else if(colts[j]==3) { //int
        tuple->fill(j,*((int*)colas[j]));
      } else if(colts[j]==4) { //string
        tuple->fill(j,std::string((char*)colas[j]));
      }
    }
    tuple->addRow();
  }

  delete [] colts;
  delete [] colas;

  delete [] bigbuf;
  delete [] charflag;
  delete [] lenchar;

  return tuple;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Zebra::Tree::toLUN(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::string s = CHPWD(); //Under //PAWC/LUN<i>
  inlib::replace(s,"//PAWC/"+fDirectory,"//"+fDirectory);
  CHCDIR(s," ");
}

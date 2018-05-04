// this :
#include <BatchLab/Rio/Store.h>

#include <Slash/Core/ISession.h>

#include <inlib/srep>
#include <inlib/path>
#include <inlib/sprintf>
#include <inlib/system>

#include <Lib/Debug.h>
#include <inlib/cast>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IKey.h>
#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/Printer.h>
#include <Rio/Core/File.h> //FIXME : have IFile ?

// BatchLab :
#include <BatchLab/Rio/Manager.h>

//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Store::Store(
 std::ostream& a_out
,Rio::Manager& aManager
,const std::string& aFile
,const std::string& aMode
,int aVerboseLevel
)
:f_out(a_out)
,fRioFile(0)
,fDirectory(0)
,fFile(aFile)
,fVerboseLevel(aVerboseLevel)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Lib::Debug::increment("BatchLab::Rio::Store");
  if(aFile.empty()) return;

  inlib::file_name(aFile,fFile); //to expand environment variables.

  if(aMode=="RECREATE") {
    fRioFile = new ::Rio::File(aManager.dictionary(),aManager.out(),
                             fFile,::Rio::File::RECREATE,fVerboseLevel);
  } else if(aMode=="UPDATE") {
    fRioFile = new ::Rio::File(aManager.dictionary(),aManager.out(),
                             fFile,::Rio::File::UPDATE,fVerboseLevel);
  } else if(aMode=="READ") {
    fRioFile = new ::Rio::File(aManager.dictionary(),aManager.out(),
                             fFile,::Rio::File::READ,fVerboseLevel);
  } else {
    f_out << "BatchLab::Rio::Store::Rio::Store : bad mode " << aMode 
        << ". Open in READ." << std::endl;
    fRioFile = new ::Rio::File(aManager.dictionary(),aManager.out(),
                             fFile,::Rio::File::READ,fVerboseLevel);
  }
  if( !fRioFile || (!fRioFile->isOpen()) ) {
    f_out << "BatchLab::Rio::Store::Rio::Store :"
        << " Can't open " << fFile 
        << std::endl;
    delete fRioFile;
    fRioFile = 0;
    return;
  } 

  fDirectory = &(fRioFile->directory());
}
//////////////////////////////////////////////////////////////////////////////
BatchLab::Rio::Store::~Store(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fRioFile) {
    fRioFile->close();
    delete fRioFile;
  }
  Lib::Debug::decrement("BatchLab::Rio::Store");
}
//////////////////////////////////////////////////////////////////////////////
::Rio::IFile* BatchLab::Rio::Store::rioFile(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fRioFile;
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Store::storeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return "";
  return fRioFile->name();
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Store::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fVerboseLevel = aVerboseLevel;
  if(!fRioFile) return;
  fRioFile->setVerboseLevel(aVerboseLevel);  
}
//////////////////////////////////////////////////////////////////////////////
int BatchLab::Rio::Store::verboseLevel(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Store::commit(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return false;
  int n;
  return fRioFile->write(n);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Store::mkdir(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath should NOT start with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aPath.empty()) return false;
  std::string path,name,suffix;
  inlib::path_name_suffix(aPath,path,name,suffix);
  // name is the last folder to create.
  ::Rio::IDirectory* directory = findDirectory(path);
  if(!directory) return false;
  return (directory->mkdir(name)?true:false);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Store::mkdirs( 
 const std::string& aPath
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (makeDirectories(aPath) ? true : false);
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Store::cd(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath = /dir1/....
// aPath = dir1/dir2
// aPath should NOT start with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return false;
  if(aPath.empty()) return true;
  if(aPath=="..") {
    ::Rio::IDirectory* directory = fDirectory->parent();
    if(!directory) return false;
    fDirectory = directory;
    return true;
  } else if((aPath.size()>=3)&&(aPath.substr(0,3)=="../")) {
    //aPath = ../dir0/dir1/.../dirn/name
    ::Rio::IDirectory* directory = fDirectory->parent();
    if(!directory) return false;
    fDirectory = directory;
    std::string path = aPath.substr(3,aPath.size()-3);
    directory = findDirectory(path);
    if(!directory) return false;
    fDirectory = directory;
    return true;
  } else {
    ::Rio::IDirectory* directory = findDirectory(aPath);
    if(!directory) return false;
    fDirectory = directory;
    return true;
  }
}

//stativ 
std::string BatchLab::Rio::Store::path(const ::Rio::IDirectory& aDirectory){
  // WARNING : with Rio, some directory->path()
  //           returns a path that starts with /<file name> :
  //           we remove this header directory.

  const std::string& header = aDirectory.file().directory().name();
  std::string::size_type l = header.size();

  //    /file/ab
  //    01234567
  // rm /file

  std::string path = aDirectory.path();
  path = path.substr(l+1,path.size()-(l+1));
  if(path.empty()) path = "/";

  return path;
}

std::string BatchLab::Rio::Store::path(){
  if(!fDirectory) return "";
  return path(*fDirectory);
}

//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Store::ls(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fDirectory) return;
  ::Rio::Printer printer(fDirectory->out());
  fDirectory->print(printer);
}
//////////////////////////////////////////////////////////////////////////////
std::string BatchLab::Rio::Store::hierarchy(
 const std::string& aFormat
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return "";
  std::string sout;
  sout += "<tree>";
  visitTree(fRioFile->directory(),aFormat.empty()?false:true,sout);
  sout += "</tree>";
  return sout;
}
//////////////////////////////////////////////////////////////////////////////
void BatchLab::Rio::Store::visitTree(
 ::Rio::IDirectory& aDirectory
,bool aAll
,std::string& aOut
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector< ::Rio::IDirectory*> dirs;
 {std::vector< ::Rio::IKey*>::const_iterator it;
  std::vector< ::Rio::IKey*> keys = aDirectory.keys();
  for(it=keys.begin();it!=keys.end();++it) {
    ::Rio::IKey* key = *it;
    if(key->className()=="TDirectory") { //Do not read object to get its class.
      ::Rio::IObject* obj = aDirectory.find(key->name());
      if(obj) {
        ::Rio::IDirectory* dir = ::Rio::cast< ::Rio::IDirectory>(*obj);
        if(dir) {
          dirs.push_back(dir);
        }
      }
    } else {
      std::string s;
      s += inlib::to_xml(key->name());
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
      aOut += "<treeItem><label>" + s + "</label></treeItem>";
    }
  }}

 {std::vector< ::Rio::IDirectory* >::iterator it;
  for(it=dirs.begin();it!=dirs.end();++it){
    ::Rio::IDirectory* dir = *it;

    aOut += 
      "<treeItem><label>" + inlib::to_xml(dir->name()) + "</label>";
    visitTree(*dir,aAll,aOut);

    aOut += "</treeItem>";
  }}
}
//////////////////////////////////////////////////////////////////////////////
::Rio::IDirectory* BatchLab::Rio::Store::directory(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDirectory;
}
//////////////////////////////////////////////////////////////////////////////
::Rio::IDirectory* BatchLab::Rio::Store::findDirectory(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// Same logic than MemoryTree::findFolder.
// Import in memory if needed.
// aPath should NOT start with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return 0;
  ::Rio::IDirectory* start = 0;
  std::string path;
  if(aPath.size() && (aPath[0]=='/')) {
    start = &(fRioFile->directory());
    path = aPath.substr(1,aPath.size()-1);
  } else {
    start = fDirectory;
    path = aPath;
  }
  std::vector<std::string> words;
  inlib::words(path,"/",false,words);
  ::Rio::IDirectory* folder = start;
  for(unsigned int index=0;index<words.size();index++) {
    if(words[index]==".") {
    } else if(words[index]=="..") {
      folder = folder->parent();
      if(!folder) return 0;
    } else {
      folder = folder->findDirectory(words[index]);
      if(!folder) return 0;
    }
  }
  return folder;
}
//////////////////////////////////////////////////////////////////////////////
bool BatchLab::Rio::Store::isADirectory(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// aPath should NOT start with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return false;
  if(aPath.empty()) { //Current directory.
    return fDirectory?true:false;
  } else {
    // Find the directory :
    // aPath[0]=='/' : absolute path : /dir1/dir2
    //                else relative path : dir1/dir2
    std::vector<std::string> words;
    inlib::words(aPath,"/",false,words);
    ::Rio::IDirectory* directory = 
      (aPath.size()&&(aPath[0]=='/')) ? &(fRioFile->directory()) : fDirectory;
    for(unsigned int index=0;index<words.size();index++) {
      if(!directory->isADirectory(words[index])) return false;
      ::Rio::IDirectory* d = directory->findDirectory(words[index]);
      if(!d) return false;
      directory = d;
    }
    return true;
  }
}
//////////////////////////////////////////////////////////////////////////////
::Rio::IDirectory* BatchLab::Rio::Store::makeDirectories( 
 const std::string& aPath
) 
//////////////////////////////////////////////////////////////////////////////
// Leaf in the path is returned.
// Same logic as MemoryTree::makeFolders.
// aPath should NOT start with /<file name>.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fRioFile) return 0;
  ::Rio::IDirectory* start = 0;
  std::string path;
  if(aPath.size() && (aPath[0]=='/')) {
    start = &(fRioFile->directory());
    path = aPath.substr(1,aPath.size()-1);
  } else {
    start = fDirectory;
    path = aPath;
  }
  std::vector<std::string> words;
  inlib::words(path,"/",false,words);
  ::Rio::IDirectory* folder = start;
  for(unsigned int index=0;index<words.size();index++) {
    if(words[index]==".") {
    } else if(words[index]=="..") {
      folder = folder->parent();
      if(!folder) return 0;
    } else {
      ::Rio::IDirectory* f = 0;
      ::Rio::IObject* obj = folder->findInMemory(words[index]);
      if(obj) {
        f = ::Rio::cast< ::Rio::IDirectory>(*obj);
        if(!f) return 0; // An object exists which is not a directory.
        // We have a directory.
      }
      if(f) {
        folder = f;
      } else {
        folder = folder->mkdir(words[index]);
        if(!folder) return 0;
      }
    }
  }
  return folder;
}

// this :
#include <Rio/Tree/Tree.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Debug.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Array_int.h>
#include <Rio/Core/Array_double.h>

// Graf :
#include <Rio/Graf/AttLine.h>
#include <Rio/Graf/AttFill.h>
#include <Rio/Graf/AttMarker.h>

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BranchObject.h>
#include <Rio/Tree/BranchElement.h>
#include <Rio/Tree/BaseLeaf.h>
#include <Rio/Tree/LeafString.h>

#define MINIMUM(a,b) ((a)<(b)?a:b) 

RIO_META_SOURCE(Rio::Tree)

//////////////////////////////////////////////////////////////////////////////
Rio::Tree::Tree(
 const IDictionary& aDictionary
,IDirectory& aDirectory
)
:Named(aDictionary,"","")
,fClass(aDictionary.opt_Tree_Class())
,fDirectory(aDirectory)
,fBranches(aDictionary,aDirectory.out())
,fLeaves(aDictionary,aDirectory.out())
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fSavedBytes(0)
,fMaxVirtualSize(0)
,fAutoSave(100000000)
,fReadEntry(-1)
,fTotalBuffers(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Tree");
  if(fDirectory.file().verboseLevel()) {
    out() << "Rio::Tree::Tree1 : called for \"" << fName << "\" dir \"" 
          << fDirectory.name() << "\"." << std::endl;
  }

  fDirectory.appendObject(this);
}

#define aDico aDirectory.file().dictionary()

//////////////////////////////////////////////////////////////////////////////
Rio::Tree::Tree(
 IDirectory& aDirectory
,const std::string& aName
,const std::string& aTitle
)
:Named(aDico,aName,aTitle)
,fClass(aDico.opt_Tree_Class())
,fDirectory(aDirectory)
,fBranches(aDico,aDirectory.out())
,fLeaves(aDico,aDirectory.out())
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fSavedBytes(0)
,fAutoSave(100000000)
,fReadEntry(-1)
,fTotalBuffers(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Tree");
  if(fDirectory.file().verboseLevel()) {
    out() << "Rio::Tree::Tree2 : called for \"" << fName << "\" dir \"" 
          << fDirectory.name() << "\"." << std::endl;
  }

  fDirectory.appendObject(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree::Tree(
 const Tree& aFrom
)
:Named(aFrom)
,fClass(aFrom.fClass)
,fDirectory(aFrom.fDirectory)
,fBranches(aFrom.fDirectory.file().dictionary(),aFrom.fDirectory.out())
,fLeaves(aFrom.fDirectory.file().dictionary(),aFrom.fDirectory.out())
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fSavedBytes(0)
,fAutoSave(100000000)
,fReadEntry(-1)
,fTotalBuffers(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree::~Tree(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fDirectory.file().verboseLevel()) {
    out() << "Rio::Tree::~Tree : called for \"" << fName << "\"." << std::endl;
  }

  if(!fDirectory.isClosing()) fDirectory.removeObject(this);
    
 {unsigned int sz = fBranches.size();
  for(unsigned int index=0;index<sz;index++) delete fBranches[index];
  fBranches.clear();}

  fLeaves.clear(); // Leaves deleted by the Branches.

  Debug::decrement("Rio::Tree");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree& Rio::Tree::operator=(
 const Tree&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory& Rio::Tree::directory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDirectory;
}

std::ostream& Rio::Tree::out() const {return fDirectory.out();}

//////////////////////////////////////////////////////////////////////////////
void* Rio::Tree::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(Tree);
  else 
    return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::BaseLeaf*>& Rio::Tree::leaves(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLeaves;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::Branch*>& Rio::Tree::branches(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBranches;
}
//////////////////////////////////////////////////////////////////////////////
double Rio::Tree::entries(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntries;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::Tree::leaf(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    if((*it)->name()==aName) return (*it);
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::incrementTotalBuffers(
 int aNumberOfBytes
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTotalBuffers += aNumberOfBytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::addTotBytes(
 int aTotal
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fTotBytes += aTotal;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::addZipBytes(
 int aZIP
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fZipBytes += aZIP;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::createBranch(
 const std::string& aName
,void* aAddress
,const std::string& aLeafList
,int aBufferSize
)
//////////////////////////////////////////////////////////////////////////////
// Create a new TTree Branch :
//     This Branch constructor is provided to support non-objects in
//     a Tree. The variables described in leaflist may be simple variables
//     or structures.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch* branch = new Branch(dictionary(),
                              *this,
                              aName,
                              fName, // Branch title is tree name.
                              aAddress,aLeafList,aBufferSize,
                              fDirectory.file().compressionLevel());
  /*FIXME
    if (branch->IsZombie()) {
    delete branch;
    return 0;
    }
  */
  fBranches.push_back(branch);
  return branch;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::createBranch(
 const std::string& aName
,const IClass& aClass
,IObject** aAddObj
,int aBufSize
)
//////////////////////////////////////////////////////////////////////////////
// Create a new TTree BranchObject
//
//    Build a TBranchObject for an object of class classname.
//    addobj is the address of a pointer to an object of class classname.
//    IMPORTANT: classname must derive from TObject.
//    The class dictionary must be available (ClassDef in class header).
//
//    This option requires access to the library where the corresponding class
//    is defined. Accessing one single data member in the object implies
//    reading the full object.
//    See the next Branch constructor for a more efficient storage
//    in case the entry consists of arrays of identical objects.
//
//    By default the branch buffers are stored in the same file as the Tree.
//    use TBranch::SetFile to specify a different file
//
//      IMPORTANT NOTE about branch names
//    In case two or more master branches contain subbranches with
//    identical names, one must add a "." (dot) character at the end
//    of the master branch name. This will force the name of the subbranch
//    to be master.subbranch instead of simply subbranch.
//    This situation happens when the top level object (say event)
//    has two or more members referencing the same class.
//    For example, if a Tree has two branches B1 and B2 corresponding
//    to objects of the same class MyClass, one can do:
//       tree.Branch("B1.","MyClass",&b1,8000,1);
//       tree.Branch("B2.","MyClass",&b2,8000,1);
//    if MyClass has 3 members a,b,c, the two instructions above will generate
//    subbranches called B1.a, B1.b ,B1.c, B2.a, B2.b, B2.c
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch* branch = new BranchObject(dictionary(),
                                    *this,
                                    aName,
                                    fName, // Branch title is tree name.
                                    aClass,aAddObj,
                                    aBufSize,
                                    fDirectory.file().compressionLevel());
  fBranches.push_back(branch);

  /* FIXME splitting not yet supported by Rio.
  if (!aSplitLevel) return branch;
  TObjArray* blist = branch->GetListOfBranches();
  const char *rdname;
  const char *dname;
  char branchname[128];
  char **apointer = (char**)(addobj);
  TObject *obj = (TObject*)(*apointer);
  Bool_t delobj = kFALSE;
  if (!obj) {
    obj = (TObject*)cl->New();
    delobj = kTRUE;
  }
  //build the StreamerInfo if first time for the class
  BuildStreamerInfo(cl,obj);
   
  // Loop on all public data members of the class and its base classes
  Int_t lenName = strlen(name);
  Int_t isDot = 0;
  if (name[lenName-1] == '.') isDot = 1;
  TBranch *branch1 = 0;
  TRealData *rd, *rdi;
  TIter      nexti(cl->GetListOfRealData());
  TIter      next(cl->GetListOfRealData());
  while ((rd = (TRealData *) next())) {
    TDataMember *dm = rd->GetDataMember();
    if (!dm->IsPersistent()) continue; //do not process members with a ! as the first
    // character in the comment field
    if (rd->IsObject()) {
      TClass *clm = gROOT->GetClass(dm->GetFullTypeName());
      if (clm) BuildStreamerInfo(clm,(char*)obj+rd->GetThisOffset());
      continue;
    }
    rdname = rd->GetName();
    dname  = dm->GetName();
    if (cl->CanIgnoreTObjectStreamer()) {
      if (strcmp(dname,"fBits") == 0) continue;
      if (strcmp(dname,"fUniqueID") == 0) continue;
    }
    
    TDataType *dtype = dm->GetDataType();
    Int_t code = 0;
    if (dtype) code = dm->GetDataType()->GetType();
    
    // Encode branch name. Use real data member name
    sprintf(branchname,"%s",rdname);
    if (isDot) {
      if (dm->IsaPointer()) sprintf(branchname,"%s%s",name,&rdname[1]);
      else                  sprintf(branchname,"%s%s",name,&rdname[0]);
    }
    char leaflist[128];
    Int_t offset    = rd->GetThisOffset();
    char *pointer   = (char*)obj + offset;
    if (dm->IsaPointer()) {
      TClass *clobj = 0;
      if (!dm->IsBasic()) clobj = gROOT->GetClass(dm->GetTypeName());
      if (clobj && clobj->InheritsFrom("TClonesArray")) {
        char *cpointer  =(char*)pointer;
        char **ppointer =(char**)cpointer;
        TClonesArray *list = (TClonesArray*)(*ppointer);
        if (splitlevel != 2) {
          if (isDot) branch1 = new TBranchClones(&branchname[0],pointer,bufsize);
          else       branch1 = new TBranchClones(&branchname[1],pointer,bufsize);
          blist->Add(branch1);
        } else {
          if (isDot) branch1 = new TBranchObject(&branchname[0],list->ClassName(),pointer,bufsize);
          else       branch1 = new TBranchObject(&branchname[1],list->ClassName(),pointer,bufsize);
          blist->Add(branch1);
        }
      } else {
        if (!clobj) {
          const char * index = dm->GetArrayIndex();
          if (strlen(index)!=0) {
            //check that index is a valid data member name
            //if member is part of an object (eg fA and index=fN)
            //index must be changed from fN to fA.fN
            char aindex[128];
            strcpy(aindex,rd->GetName());
            char *rdot = strrchr(aindex,'.');
            if (rdot) strcpy(rdot+1,index);
            nexti.Reset();
            while ((rdi = (TRealData *) nexti())) {
              if (strcmp(rdi->GetName(),index) == 0) break;
              if (strcmp(rdi->GetName(),aindex) == 0) {index = rdi->GetName(); break;}
            }
            if      (code ==  1) 
              // Note that we differentiate between strings and
              // char array by the fact that there is NO specified
              // size for a string (see next if (code == 1)
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"B");
            else if (code == 11) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"b");
            else if (code ==  2) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"S");
            else if (code == 12) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"s");
            else if (code ==  3) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"I");
            else if (code == 13) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"i");
            else if (code ==  5) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"F");
            else if (code ==  8) 
              sprintf(leaflist,"%s[%s]/%s",&rdname[0],index,"D");
            else {
              printf("Cannot create branch for rdname=%s, code=%d\n",branchname, code);
              leaflist[0] = 0;
            }
          } else {
            if (code == 1) {
              sprintf(leaflist,"%s/%s",dname,"C");
            } else {
              continue;
            }
          }
          
          // there are '*' in both the branchname and rdname
          char bname[128];
          UInt_t cursor,pos;
          for(cursor = 0, pos = 0; 
              cursor< strlen(branchname); 
              cursor++ ) {
            if (branchname[cursor]!='*') {
              bname[pos++]=branchname[cursor];
            }
          }
          bname[pos] = '\0';
          for(cursor = 0, pos = 0; cursor< strlen(leaflist); cursor++ ) {
            if (leaflist[cursor]!='*') {
              leaflist[pos++]=leaflist[cursor];
            }
          }
          leaflist[pos] = '\0';
          
          // Add the branch to the tree and indicate that the address
          // is that of a pointer to be dereferenced before using.
          branch1 = new TBranch(bname,*(void**)pointer,leaflist,bufsize);
          TLeaf *leaf = (TLeaf*) branch1->GetListOfLeaves()->At(0);
          leaf->SetBit(TLeaf::kIndirectAddress);
          leaf->SetAddress((void**)pointer);
          blist->Add(branch1);
        } else {
          if (!clobj->InheritsFrom(TObject::Class())) continue;
          branch1 = new TBranchObject(dname,clobj->GetName(),pointer,bufsize,0);
          if (isDot) branch1->SetName(&branchname[0]);
          else       branch1->SetName(&branchname[1]);  //do not use the first character (*)
          blist->Add(branch1);
        }
      }
    } else {
      //-------------Data Member is a basic data type----------
      if (dm->IsBasic()) {
        if      (code ==  1) sprintf(leaflist,"%s/%s",rdname,"B");
        else if (code == 11) sprintf(leaflist,"%s/%s",rdname,"b");
        else if (code ==  2) sprintf(leaflist,"%s/%s",rdname,"S");
        else if (code == 12) sprintf(leaflist,"%s/%s",rdname,"s");
        else if (code ==  3) sprintf(leaflist,"%s/%s",rdname,"I");
        else if (code == 13) sprintf(leaflist,"%s/%s",rdname,"i");
        else if (code ==  5) sprintf(leaflist,"%s/%s",rdname,"F");
        else if (code ==  8) sprintf(leaflist,"%s/%s",rdname,"D");
        else {
          printf("Cannot create branch for rdname=%s, code=%d\n",branchname, code);
          leaflist[0] = 0;
        }
        branch1 = new TBranch(branchname,pointer,leaflist,bufsize);
        branch1->SetTitle(rdname);
        blist->Add(branch1);
      }
    }
    if (branch1) branch1->SetOffset(offset);
    else Warning("Branch","Cannot process member:%s",rdname);
    
  }
  if (delobj) delete obj;
  */

  return branch;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::createBranchElement(
 const std::string& aName
,const IClass& aClass
,void* aPointer
,int aBufSize
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch* branch = new BranchElement(dictionary(),
                                     *this,
                                     aName,
                                     fName, // Branch title is tree name.
                                     aClass,aPointer,
                                     aBufSize,
                                     fDirectory.file().compressionLevel());
  fBranches.push_back(branch);
  return branch;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    //printf("debug : Tree::stream : %ld : version %d\n",this,v);
    if (v > 4) {
      /*
      TTree::Class()->ReadBuffer(b, this, v, s, c);
      if (fEstimate <= 10000) fEstimate = 1000000;
      fSavedBytes = fTotBytes;
      fDirectory = gDirectory;
      gDirectory->Append(this);
      return;
      */
    }
    //====process old versions before automatic schema evolution

    double dummy_double;
    int dummy_int;

    if(!Named::stream(aBuffer)) return false;

    {AttLine dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {AttFill dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {AttMarker dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    if(v<=4) {
      if(!aBuffer.read(dummy_int)) return false; //fScanField
      if(!aBuffer.read(dummy_int)) return false; //fMaxEntryLoop
      if(!aBuffer.read(fMaxVirtualSize)) return false;
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
      if(!aBuffer.read(fAutoSave)) return false;
      if(!aBuffer.read(dummy_int)) return false; //fEstimate
    } else if(v<=9) {
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
      if(!aBuffer.read(dummy_double)) return false; //fSavedBytes
      if(!aBuffer.read(dummy_int)) return false; //fTimerInterval
      if(!aBuffer.read(dummy_int)) return false; //fScanField (25)
      if(!aBuffer.read(dummy_int)) return false; //fUpdate
      if(!aBuffer.read(dummy_int)) return false; //fMaxEntryLoop(1 000 000 000)
      if(!aBuffer.read(fMaxVirtualSize)) return false;
      if(!aBuffer.read(fAutoSave)) return false;  // (100 000 000))
      if(!aBuffer.read(dummy_int)) return false; //fEstimate (1 000 000)
    } else {
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
      if(!aBuffer.read(dummy_double)) return false; //fSavedBytes
      if(!aBuffer.read(dummy_double)) return false; //fWeight
      if(!aBuffer.read(dummy_int)) return false; //fTimerInterval
      if(!aBuffer.read(dummy_int)) return false; //fScanField (25)
      if(!aBuffer.read(dummy_int)) return false; //fUpdate
      if(!aBuffer.read(dummy_int)) return false; //fMaxEntryLoop(1 000 000 000)
      if(!aBuffer.read(fMaxVirtualSize)) return false;
      if(!aBuffer.read(fAutoSave)) return false;  // (100 000 000))
      if(!aBuffer.read(dummy_int)) return false; //fEstimate (1 000 000)
    }
    //FIXME if (fEstimate <= 10000) fEstimate = 1000000;

    Arguments args;
    Args_init(args);
    Args_insertDirectory(args,&fDirectory);
    Args_insertTree(args,this);

   {unsigned int sz = fBranches.size();
    for(unsigned int index=0;index<sz;index++) delete fBranches[index];
    fBranches.clear();}
    fLeaves.clear();
    //TObjArray
    //The below fBranches.read will create leaves.
    if(!fBranches.read(aBuffer,args)) {
      out() << "Rio::Tree::stream : can't read branches." << std::endl;
      return false;
    }

    //TObjArray
    // We read leaves in order to keep streaming synchronisation.
    // In fact fLeaves are references to existing leaves read by 
    // the branches in the upper line of code.
    ObjArrayT<BaseLeaf> leaves(dictionary(),out());
    if(!leaves.read(aBuffer,args)) {
      aBuffer.out() << "Rio::Tree::stream : can't read leaves." << std::endl;
      return false;
    }

    if(v>=10) { //TList* fAliases
      Arguments args;
      Args_init(args);
      IObject* obj;
      if(!aBuffer.readObject(args,obj)) {
        out() << "Rio::Tree::stream : can't read fAliases." << std::endl;
        return false;
      }
    } 

    fSavedBytes = fTotBytes;
    /*
      fDirectory = gDirectory;
      gDirectory->Append(this);
    */
    {Array_double dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;} //fIndexValues TArrayD

    {Array_int dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}    // fIndex (TArrayI).
    
    if(v>=6) { //TList* fFriends
      Arguments args;
      Args_init(args);
      IObject* obj;
      if(!aBuffer.readObject(args,obj)) {
        out() << "Rio::Tree::stream : can't read fFriends." << std::endl;
        return false;
      }
    } 
    
    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;
    //====end of old versions
    
  } else {
    //TTree::Class()->WriteBuffer(b,this);
    //====process old versions before automatic schema evolution
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!Named::stream(aBuffer)) return false;

    // Beurk.
    {AttLine dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {AttFill dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}
    {AttMarker dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}

    if(fClass.version()<5) {
      if(!aBuffer.write((int)25)) return false;         //fScanField
      if(!aBuffer.write((int)1000000000)) return false; //fMaxEntryLoop
      if(!aBuffer.write(fMaxVirtualSize)) return false;
      if(!aBuffer.write(fEntries)) return false;
      if(!aBuffer.write(fTotBytes)) return false;
      if(!aBuffer.write(fZipBytes)) return false;
      if(!aBuffer.write(fAutoSave)) return false;
      if(!aBuffer.write((int)1000000)) return false;    //fEstimate;
    } else {
      if(!aBuffer.write(fEntries)) return false;
      if(!aBuffer.write(fTotBytes)) return false;
      if(!aBuffer.write(fZipBytes)) return false;
      if(!aBuffer.write((double)0)) return false; //fSavedBytes
      if(!aBuffer.write((int)0)) return false;    //fTimerInterval
      if(!aBuffer.write((int)25)) return false;   //fScanField (25)
      if(!aBuffer.write((int)0)) return false;    //fUpdate
      if(!aBuffer.write((int)1000000000)) return false; //fMaxEntryLoop
      if(!aBuffer.write(fMaxVirtualSize)) return false;
      if(!aBuffer.write(fAutoSave)) return false;
      if(!aBuffer.write((int)1000000)) return false;    //fEstimate;
    }
    if(!fBranches.write(aBuffer)) return false;
    if(!fLeaves.write(aBuffer)) return false;

    {Array_double dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}    // fIndexValues (TArrayD).

    {Array_int dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;}    // fIndex (TArrayI).

    if(!aBuffer.setByteCount(c,true)) return false;
    //====end of old versions
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::reset(
)
//////////////////////////////////////////////////////////////////////////////
// Reset buffers and entries count in all branches/leaves
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fEntries        = 0;
  fTotBytes       = 0;
  fZipBytes       = 0;
  fSavedBytes     = 0;
  fTotalBuffers   = 0;
  //fChainOffset    = 0;
  fReadEntry      = -1;

  std::vector<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    (*it)->reset();
  }
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::fill(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Fill all branches of a Tree :
//   This function loops on all the branches of this tree.
//   For each branch, it copies to the branch buffer (basket) the current
//   values of the leaves data types.
//   If a leaf is a simple data type, a simple conversion to a machine
//   independent format has to be done.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  /*FIXME
  //case of one single super branch. Automatically update
  // all the branch addresses if a new object was created
  if (nb == 1) {
    branch = (TBranch*)fBranches.UncheckedAt(0);
    branch->UpdateAddress();
    }*/
  
  int nbytes = 0;
  std::vector<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    //FIXME if ((*it)->testBit(kDoNotProcess)) continue;
    int n;
    if(!(*it)->fill(n)) return false;;
    nbytes += n;
  }

  fEntries++;
  if (fTotBytes - fSavedBytes > fAutoSave) {
    if(!autoSave()) return false;
  }

  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::setAutoSave(
 int aAutoSave
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fAutoSave = aAutoSave;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::autoSave(
)
//////////////////////////////////////////////////////////////////////////////
// AutoSave tree header every fAutoSave bytes :
//
//   When large Trees are produced, it is safe to activate the AutoSave
//   procedure. Some branches may have buffers holding many entries.
//   AutoSave is automatically called by TTree::Fill when the number of bytes
//   generated since the previous AutoSave is greater than fAutoSave bytes.
//   This function may also be invoked by the user, for example every
//   N entries.
//   Each AutoSave generates a new key on the file.
//   Once the key with the tree header has been written, the previous cycle
//   (if any) is deleted.
//
//   Note that calling TTree::AutoSave too frequently (or similarly calling
//   TTree::SetAutoSave with a small value) is an expensive operation.
//   You should make tests for your own application to find a compromize
//   between speed and the quantity of information you may loose in case of
//   a job crash.
//
//   In case your program crashes before closing the file holding this tree,
//   the file will be automatically recovered when you will connect the file
//   in UPDATE mode.
//   The Tree will be recovered at the status corresponding to the 
//   last AutoSave.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fDirectory.file().verboseLevel()) {
    out() << "Rio::Tree::autosave :"
          << " autosave tree : " << name() 
          << " after " << fTotBytes << " bytes written."
	  << std::endl;
  }
  fSavedBytes = fTotBytes;
  if(!fDirectory.saveObject(*this)) return false;
  // save StreamerInfo
  //FIXME TFile *file = fDirectory->GetFile();
  //FIXME if (file) file->WriteStreamerInfo();
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::findEntry(
 int aEntry
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Read all branches of entry and return total number of bytes :
//     getall = 0 : get only active branches
//     getall = 1 : get all branches
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns 0.
//  if entry is the same as the previous call, the function returns 1.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if (aEntry < 0 || aEntry >= fEntries) return false;
  int nbytes = 0;
  fReadEntry = aEntry;
  std::vector<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    int n;
    if(!(*it)->findEntry(aEntry,n)) return false;
    nbytes += n;
  }
  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::show(
 int aEntry
)
//////////////////////////////////////////////////////////////////////////////
// Print values of all active leaves for entry :
// if entry==-1, print current entry (default)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aEntry != -1) {
    int n;
    if(!findEntry(aEntry,n)) return false;
  }
  out() << "======> EVENT:" << fReadEntry << std::endl;
  std::vector<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    BaseLeaf* leaf = *it;
    //Branch& branch = leaf->branch();
    //FIXME if (branch->TestBit(kDoNotProcess)) continue;
    //FIXME if (branch.GetListOfBranches()->GetEntriesFast() > 0) continue;
    int len = leaf->length();
    if (len <= 0) continue;
    //FIXME if (leaf->IsA() == TLeafF::Class()) len = TMath::Min(len,5);
    //FIXME if (leaf->IsA() == TLeafD::Class()) len = TMath::Min(len,5);
    len = MINIMUM(len,10);
    std::string s;
    smanip::printf(s,32," %-15s = ",leaf->name().c_str());
    out() << s;
    if(LeafString* ls = Rio::cast<LeafString>(*leaf)) {
      ls->printValue(out());      
      out() << std::endl;
    } else { 
      for (int i=0;i<len;i++) {
        leaf->printValue(out(),i);
        if (i == len-1) out() << std::endl;
        else            out() << ", ";
      }
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch* Rio::Tree::findBranch(
 const std::string& aName
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<Branch*>::const_iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    Branch* branch = *it;
    if (branch->name()==aName) return branch;
    /*FIXME
    TObjArray *lb = branch->GetListOfBranches();
    nb1 = lb->GetEntriesFast();
    for (j=0;j<nb1;j++) {
      b1 = (TBranch*)lb->UncheckedAt(j);
      if (!strcmp(b1->GetName(),name)) return b1;
      lb1 = b1->GetListOfBranches();
      nb2 = lb1->GetEntriesFast();
      for (k=0;k<nb2;k++) {
        b2 = (TBranch*)lb1->UncheckedAt(k);
        if (!strcmp(b2->GetName(),name)) return b2;
      }
    }
    */
  }

  std::vector<BaseLeaf*>::const_iterator itl;
  for(itl=fLeaves.begin();itl!=fLeaves.end();++itl) {
    if((*itl)->branch().name()==aName) return &((*itl)->branch());
  }

  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::loadTree(
 int aEntry
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // this function is overloaded in TChain
  fReadEntry = aEntry;
  return fReadEntry;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::readEntry(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fReadEntry;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Tree::memoryFull(
 int aNumber
) const
//////////////////////////////////////////////////////////////////////////////
// Check if adding aNumber bytes to memory we are still below MaxVirtualsize.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if ((fTotalBuffers + aNumber) < fMaxVirtualSize) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Tree::maxVirtualSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 return fMaxVirtualSize;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Tree::setMaxVirtualSize(
 int aSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMaxVirtualSize = aSize;
}

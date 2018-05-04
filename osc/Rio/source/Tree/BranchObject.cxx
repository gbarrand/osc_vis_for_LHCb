// this :
#include <Rio/Tree/BranchObject.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Arguments.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/LeafObject.h>
#include <Rio/Tree/Basket.h>

RIO_META_SOURCE(Rio::BranchObject)
//////////////////////////////////////////////////////////////////////////////
Rio::BranchObject::BranchObject(
 const IDictionary& aDictionary
,Tree& aTree
)
:Branch(aDictionary,aTree)
,fClass(aDictionary.opt_BranchObject_Class())
,fOldObject(0)
//,fWarn(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNleaves = 1;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BranchObject::BranchObject(
 const IDictionary& aDictionary
,Tree& aTree
,const std::string& aName
,const std::string& aTitle
,const IClass& aClass
,IObject** aAddObj
,int aBasketSize
//FIXME ,int aSplitLevel
,int aCompress
)
:Branch(aDictionary,aTree)
,fClass(aDictionary.opt_BranchObject_Class())
,fOldObject(0)
//,fWarn(false)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IObject* obj = (IObject*)(*aAddObj);
  /*FIXME
  bool delobj = false;
  if (!obj) {
    obj = (TObject*)cl->New();
    delobj = true;
  }
  gTree->BuildStreamerInfo(cl,obj);
  if (delobj) delete obj;
  */

  if(!obj) {
    out() << "Rio::BranchObject::BranchObject2 :"
          << " need the address of an IObject of class \"" 
          << aClass.name() << "\"." << std::endl;
    return;
  }
  if(&(obj->isA())!=&aClass) {
    out() << "Rio::BranchObject::BranchObject2 :"
          << " given object not of  class \"" << aClass.name() << "\"." 
          << std::endl;
    return;
  }

  setName(aName);
  setTitle(aTitle);
  fCompress = aCompress == -1 ? 
    fTree.directory().file().compressionLevel() : aCompress;
  fBasketSize     = aBasketSize < 100 ? 100 : aBasketSize;
  fAddress        = (char*)aAddObj;
  fClassName      = aClass.inStoreName();
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  
  fBasketEntry[0] = fEntryNumber;
  fBasketBytes[0] = 0;
  
  BaseLeaf* leaf = new LeafObject(dictionary(),*this,aName,aClass);
  if(!leaf->setAddress(aAddObj)) {
    out() << "Rio::BranchObject::BranchObject2 :"
          << " leaf setAddress failed. (Class \"" << aClass.name() << "\")." 
          << std::endl;
  }
  fNleaves = 1;
  fLeaves.push_back(leaf);
  fTree.leaves().push_back(leaf);

  // Set the bit kAutoDelete to specify that when reading
  // in TLeafObject::ReadBasket, the object should be deleted
  // before calling Streamer.
  // It is foreseen to not set this bit in a future version.
  setAutoDelete(true);
  
  //FIXME fFileName   = "";
  
  // Create the first basket
  //FIXME if (splitlevel) return;
  fBaskets[0] = new Basket(dictionary(),fTree.directory(),*this,aName,aTitle);
}
//////////////////////////////////////////////////////////////////////////////
Rio::BranchObject::~BranchObject(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int sz = fBranches.size();
  for(unsigned int index=0;index<sz;index++) delete fBranches[index];
  fBranches.clear();
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::BranchObject::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(BranchObject);
  else 
    return Branch::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchObject::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s, c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!Branch::stream(aBuffer)) return false;
    if(!aBuffer.read(fClassName)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!Branch::stream(aBuffer)) return false;
    if(!aBuffer.write(fClassName)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchObject::fill(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Loop on all leaves of this branch to fill Basket buffer
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  int nbytes = 0;
  if (fBranches.size()) {
    fEntries++;
    if(!updateAddress()) return false;
    std::vector<Branch*>::iterator it;
    for(it=fBranches.begin();it!=fBranches.end();++it) {
      //FIXME if (!branch->TestBit(kDoNotProcess)) 
      int n;
      if(!(*it)->fill(n)) return false;
      nbytes += n;
    } 
  } else {
    //FIXME if (!TestBit(kDoNotProcess)) 
    int n;
    if(!Branch::fill(n)) return false;
    nbytes += n;
  }
  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchObject::findEntry(
 int aEntry
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Read all branches of a BranchObject and return total number of bytes
//   If entry = 0 take current entry number + 1
//   If entry < 0 reset entry number to 0
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns 0.
//  if entry is the same as the previous call, the function returns 1.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  //FIXME if (TestBit(kDoNotProcess) && !getall) return 0;
  int nbytes;
  if (fBranches.size()) {
    if (fAddress == 0) { // try to create object
      //if (!fWarn) {
        IClass* cl = dictionary().findInStoreClass(fClassName);
        if (cl) {
          out() << "Rio::BranchObject::findEntry :" 
                << " debug : who delete ?"
                << std::endl;
          Arguments args;
          Args_init(args);
          IObject* obj = cl->create(args);
          if(!obj) {
            out() << "Rio::BranchObject::findEntry :" 
                  << "Cannot create object of class: \"" 
                  << fClassName << "\"." 
                  << std::endl;
            //fWarn = true;
            return false;
          } else {
            //FIXME : who delete the Pointer and the object ?
            typedef IObject* Pointer;
            Pointer* voidobj = new Pointer;
            *voidobj = (Pointer)obj;
            if(!setAddress(voidobj)) {
              out() << "Rio::BranchObject::findEntry :" 
                    << "setAddress failed (class: \"" << fClassName << "\")." 
                    << std::endl;
              //fWarn = true;
              return false;
            }
          }
        } else {
          out() << "Rio::BranchObject::findEntry :" 
                << "Cannot get class: \"" << fClassName << "\"." 
                << std::endl;
          //fWarn = true;
          return false;
        }
      //}
    }
    nbytes = 0;
    std::vector<Branch*>::iterator it;
    for(it=fBranches.begin();it!=fBranches.end();++it) {
      int n;
      if(!(*it)->findEntry(aEntry,n)) return false;
      nbytes += n;
    } 
  } else {
    if(!Branch::findEntry(aEntry,nbytes)) return false;
  }
  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BranchObject::reset(
)
//////////////////////////////////////////////////////////////////////////////
//    Existing buffers are deleted
//    Entries, max and min are reset
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch::reset();
  std::vector<Branch*>::iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    (*it)->reset();
  } 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchObject::updateAddress(
)
//////////////////////////////////////////////////////////////////////////////
// Update branch addresses if a new object was created
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  IObject** ppointer = (IObject**)fAddress;
  if (ppointer == 0) return true;
  IObject* obj = *ppointer;
  if (obj != fOldObject) {
    fOldObject = obj;
    if(!setAddress(fAddress)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BranchObject::setAutoDelete(
 bool aAutoDelete
)
//////////////////////////////////////////////////////////////////////////////
// Set the AutoDelete bit
//  This function can be used to instruct Root in TBranchObject::ReadBasket
//  to not delete the object referenced by a branchobject before reading a
//  new entry. By default, the object is deleted.
//  If autodel is kTRUE, this existing object will be deleted, a new object
//    created by the default constructor, then object->Streamer called.
//  If autodel is kFALSE, the existing object is not deleted. Root assumes
//    that the user is taking care of deleting any internal object or array
//    This can be done in Streamer itself.
//  If this branch has sub-branches, the function sets autodel for these
//  branches as well.
//  We STRONGLY suggest to activate this option by default when you create
//  the top level branch. This will make the read phase more efficient
//  because it minimizes the numbers of new/delete operations.
//  Once this option has been set and the Tree is written to a file, it is
//  not necessary to specify the option again when reading, unless you
//  want to set the opposite mode.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch::setAutoDelete(aAutoDelete);
  std::vector<Branch*>::iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    (*it)->setAutoDelete(aAutoDelete);
  } 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchObject::setAddress(
 void* aAddress  //IObject**
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //special case when called from code generated by TTree::MakeClass
  //if (long(aAddress) == -1) {
  //  //fWarn = true;
  //  return false;
  //}
  fReadEntry = -1; 
  BaseLeaf* leaf = fLeaves.size() ? (BaseLeaf*)fLeaves[0] : 0;
  if (leaf) {
    if(!leaf->setAddress(aAddress)) return false;
  } else {
    out() << "Rio::BranchObject::setAddress : no leaf !"  << std::endl;
  }
  fAddress = (char*)aAddress;
  IObject** ppointer = (IObject**)aAddress;
  IObject* obj = 0;
  if (aAddress) obj = *ppointer;
  IClass* cl = dictionary().findInStoreClass(fClassName);
  if (!obj && cl) {
    out() << "Rio::BranchObject::setAddress : who delete ? " << std::endl;
    //FIXME : who delete ?
    Arguments args;
    Args_init(args);
    obj = cl->create(args);
    *ppointer = obj;
  }
  fOldObject = obj;
  if (!cl) {
    out() << "Rio::BranchObject::setAddress : no class." << std::endl;
    std::vector<Branch*>::iterator it;
    for(it=fBranches.begin();it!=fBranches.end();++it) {
      char* pointer = (char*)obj;
      if(!(*it)->setAddress(pointer)) return false;
    } 
    return true;
  }
  /*
  int i, offset;
  if (!cl->GetListOfRealData())  cl->BuildRealData(obj);
  char* fullname = new char[200];
  const char *bname = GetName();
  int lenName = strlen(bname);
  int isDot = 0;
  if (bname[lenName-1] == '.') isDot = 1;
  const char *rdname;
  TRealData *rd;
  TIter      next(cl->GetListOfRealData());
  while ((rd = (TRealData *) next())) {
    TDataMember *dm = rd->GetDataMember();
    if (!dm->IsPersistent()) continue;
    rdname = rd->GetName();
    TDataType *dtype = dm->GetDataType();
    int code = 0;
    if (dtype) code = dm->GetDataType()->GetType();
    int offset  = rd->GetThisOffset();
    char *pointer = (char*)obj + offset;
    TBranch *branch = 0;
    if (dm->IsaPointer()) {
      TClass *clobj = 0;
      if (!dm->IsBasic()) clobj = gROOT->GetClass(dm->GetTypeName());
      if (clobj && clobj->InheritsFrom("TClonesArray")) {
        if (isDot) sprintf(fullname,"%s%s",bname,&rdname[1]);
        else       sprintf(fullname,"%s",&rdname[1]);
        branch = (TBranch*)fBranches.FindObject(fullname);
      } else {
        if (!clobj) {
          // this is a basic type we can handle only if
          // he has a dimension:
          const char * index = dm->GetArrayIndex();
          if (strlen(index)==0) {
            if (code==1) {
              // Case of a string ... we do not need the size
              if (isDot) sprintf(fullname,"%s%s",bname,&rdname[0]);
              else       sprintf(fullname,"%s",&rdname[0]);
            } else {
              continue;
            }
          }
          if (isDot) sprintf(fullname,"%s%s",bname,&rdname[0]);
          else       sprintf(fullname,"%s",&rdname[0]);
          // let's remove the stars!
          Uint cursor,pos;
          for( cursor = 0, pos = 0;
               cursor < strlen(fullname);
               cursor ++ ) {
            if (fullname[cursor]!='*') {
              fullname[pos++] = fullname[cursor];
            };
          };
          fullname[pos] = '\0';
          branch = (TBranch*)fBranches.FindObject(fullname);                 
        } else {
          if (!clobj->InheritsFrom(TObject::Class())) continue;
          if (isDot) sprintf(fullname,"%s%s",bname,&rdname[1]);
          else       sprintf(fullname,"%s",&rdname[1]);
          branch = (TBranch*)fBranches.FindObject(fullname);
        }
      }
    } else {
      if (dm->IsBasic()) {
        if (isDot) sprintf(fullname,"%s%s",bname,&rdname[0]);
        else       sprintf(fullname,"%s",&rdname[0]);
        branch = (TBranch*)fBranches.FindObject(fullname);
      }
    }
    if(branch) branch->SetAddress(pointer);
  }
  delete [] fullname;
  */
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BranchObject::setBasketSize(
 int aBufferSize
)
//////////////////////////////////////////////////////////////////////////////
// Reset basket size for all subbranches of this branchobject
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBasketSize = aBufferSize;
  std::vector<Branch*>::iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    (*it)->setBasketSize(aBufferSize);
  } 
}
/*
//______________________________________________________________________________
void TBranchObject::Print(Option_t *option) const
{
  int i;
  int nbranches = fBranches.GetEntriesFast();
  if (nbranches) {
    Printf("*Branch  :%-9s : %-54s *",GetName(),GetTitle());
    Printf("*Entries : %8d : BranchObject (see below)                               *",int(fEntries));
    Printf("*............................................................................*");
    for (i=0;i<nbranches;i++)  {
      TBranch *branch = (TBranch*)fBranches.At(i);
      branch->Print(option);
    }
  } else {
    TBranch::Print(option);
  }
}

//______________________________________________________________________________
Bool_t TBranchObject::IsFolder() const
{
  // Return TRUE if more than one leaf, FALSE otherwise

   int nbranches = fBranches.GetEntriesFast();
   if (nbranches >= 1) return kTRUE;
   else                return kFALSE;
}

*/

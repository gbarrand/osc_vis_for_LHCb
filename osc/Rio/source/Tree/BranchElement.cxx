// this :
#include <Rio/Tree/BranchElement.h>

// Rio :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Arguments.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/LeafElement.h>
#include <Rio/Tree/Basket.h>

RIO_META_SOURCE(Rio::BranchElement)
//////////////////////////////////////////////////////////////////////////////
Rio::BranchElement::BranchElement(
 const IDictionary& aDictionary
,Tree& aTree
)
:Branch(aDictionary,aTree)
,fClass(aDictionary.opt_BranchElement_Class())
,fClassVersion(0)
,fID(0)
,fType(0)
,fStreamerType(-1)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNleaves = 1;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BranchElement::BranchElement(
 const IDictionary& aDictionary
,Tree& aTree
,const std::string& aName
,const std::string& aTitle
,const IClass& aObjectClass
,void* aPointer  //Address of an Rio::Array_double, std::vector 
,int aBasketSize
//FIXME ,int aSplitLevel
,int aCompress
)
:Branch(aDictionary,aTree)
,fClass(aDictionary.opt_BranchElement_Class())
,fClassVersion(0)
,fID(0)
,fType(0)
,fStreamerType(-1)
//////////////////////////////////////////////////////////////////////////////
// To clone the cstore : TBranchElement::TBranchElement(const char* bname, TVirtualCollectionProxy* cont, Int_t basketsize, Int_t splitlevel, Int_t compress)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  setName(aName);
  setTitle(aTitle);
  fCompress = aCompress == -1 ? 
    fTree.directory().file().compressionLevel() : aCompress;
  fBasketSize     = aBasketSize < 100 ? 100 : aBasketSize;
  //FIXME setAddress(aPointer);
  fAddress        = (char*)aPointer; //FIXME
  fClassName      = aObjectClass.inStoreName();
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  
  fBasketEntry[0] = fEntryNumber;
  fBasketBytes[0] = 0;
  
  //std::vector for the moment. See fillLeaves.
  fID = -1;
  fType = 0;
  fStreamerType  = -1; // TStreamerInfo::kSTLp;

  BaseLeaf* leaf = new LeafElement(dictionary(),*this,aName,fID,fStreamerType);
  fNleaves = 1;
  fLeaves.push_back(leaf);
  fTree.leaves().push_back(leaf);

  // Set the bit kAutoDelete to specify that when reading
  // in TLeafObject::ReadBasket, the object should be deleted
  // before calling Streamer.
  // It is foreseen to not set this bit in a future version.
  setAutoDelete(true);
  
  // Create the first basket
  //FIXME if (splitlevel) return;
  fBaskets[0] = new Basket(dictionary(),fTree.directory(),*this,aName,aTitle);
}
//////////////////////////////////////////////////////////////////////////////
Rio::BranchElement::~BranchElement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int sz = fBranches.size();
  for(unsigned int index=0;index<sz;index++) delete fBranches[index];
  fBranches.clear();
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::BranchElement::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(BranchElement);
  else 
    return Branch::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchElement::stream(
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
    if(!aBuffer.read(fClassVersion)) return false;
    if(!aBuffer.read(fID)) return false;
    if(!aBuffer.read(fType)) return false;
    if(!aBuffer.read(fStreamerType)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;

  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!Branch::stream(aBuffer)) return false;
    if(!aBuffer.write(fClassName)) return false;
    if(!aBuffer.write(fClassVersion)) return false;
    if(!aBuffer.write(fID)) return false;
    if(!aBuffer.write(fType)) return false;
    if(!aBuffer.write(fStreamerType)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchElement::fill(
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
    //FIXME updateAddress();
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
bool Rio::BranchElement::fillLeaves(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fType == 4) {           // STL vector/list of objects
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 41) {   // sub branch of an STL class
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 3) {   //top level branch of a TClonesArray
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 31) {   // sub branch of a TClonesArray
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 0) {
     if (fID >= 0) {           // branch in split mode
       out() << "Rio::BranchElement::fillLeaves :"
             << " type 0 with ID " << fID << " not treated."
             << std::endl;
       return false;
     } else if (fID == -1) {   // top level branch in non split mode
       if(!fAddress) {
         out() << "Rio::BranchElement::fillLeaves :"
               << " fAddress is null."
               << std::endl;
         return false;
       }
       //FIXME : optimize : have a IClass* fObjectClass field.
       IClass* cl = dictionary().findInStoreClass(fClassName);
       if(!cl) {
         out() << "Rio::BranchElement::fillLeaves :"
               << " class " << fClassName << " not found."
               << std::endl;
         return false;
       }
       return cl->writeBuffer(aBuffer,fAddress);

     } else { //G.Barrand : added in osc-17.
       out() << "Rio::BranchElement::fillLeaves :"
             << " type 0 with ID " << fID << " not treated."
             << std::endl;
       return false;
     }

  } else { //G.Barrand : added in osc-17.
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  }   
  
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchElement::readLeaves(
IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fType == 4) {           // STL vector/list of objects
    out() << "Rio::BranchElement::readLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 41) {    // sub branch of an STL class
    out() << "Rio::BranchElement::readLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 3) {    //top level branch of a TClonesArray
    out() << "Rio::BranchElement::readLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 31) {    // sub branch of a TClonesArray
    out() << "Rio::BranchElement::readLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  } else if (fType == 0) {
     if (fID >= 0) {           // branch in split mode
       out() << "Rio::BranchElement::readLeaves :"
             << " type 0 with ID " << fID << " not treated."
             << std::endl;
       return false;
     } else if (fID == -1) {   // top level branch in non split mode
       if(!fAddress) {
         out() << "Rio::BranchElement::readLeaves :"
               << " fAddress is null."
               << std::endl;
         return false;
       }
       //FIXME : optimize : have a IClass* fObjectClass field.
       IClass* cl = dictionary().findInStoreClass(fClassName);
       if(!cl) {
         out() << "Rio::BranchElement::readLeaves :"
               << " class " << fClassName << " not found."
               << std::endl;
         return false;
       }
       if(!cl->readBuffer(aBuffer,fAddress)) {
         out() << "Rio::BranchElement::readLeaves :"
               << " can't read buffer for class " << fClassName
               << std::endl;
         return false;
       }
       return true;       

     } else { //G.Barrand : added in osc-17.
       out() << "Rio::BranchElement::fillLeaves :"
             << " type 0 with ID " << fID << " not treated."
             << std::endl;
       return false;
     }

  } else { //G.Barrand : added in osc-17.
    out() << "Rio::BranchElement::fillLeaves :"
          << " type " << fType << " not treated."
          << std::endl;
    return false;
  }   
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchElement::findEntry(
 int aEntry
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Read all branches of a BranchElement and return total number of bytes
//   If entry = 0 take current entry number + 1
//   If entry < 0 reset entry number to 0
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns 0.
//  if entry is the same as the previous call, the function returns 1.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //FIXME if (TestBit(kDoNotProcess) && !getall) return 0;
   
  if(!Branch::findEntry(aEntry,aNbytes)) {
    out() << "Rio::BranchElement::findEntry :"
          << " Branch::findEntry failed."
          << std::endl;
    return false;
  }

  if(fBranches.size()) {
    if(fAddress==0) { // try to create object
      IClass* cl = dictionary().findInStoreClass(fClassName);
      if(cl) {
        out() << "Rio::BranchElement::findEntry :"
              << " debug : who delete ?" 
              << std::endl;
        Arguments args;
        Args_init(args);
        void* obj = cl->createOpaque(args);
        if(!obj) {
          out() << "Rio::BranchElement::findEntry :" 
                << "Cannot create object of class: \"" << fClassName << "\"." 
                << std::endl;
          return false;
        } else {
          typedef IObject* Pointer;
          //FIXME : who delete the Pointer and the object ?
          Pointer* voidobj = new Pointer;
          *voidobj = (Pointer)obj;
          if(!setAddress(voidobj)) {
            out() << "Rio::BranchElement::findEntry :" 
                  << "Cannot setAddress (class: \"" << fClassName << "\")." 
                  << std::endl;
            return false;
          }
        }
      } else {
        out() << "Rio::BranchElement::findEntry " 
              << "Cannot get class: \"" << fClassName << "\"." << std::endl;
        return false;
      }
    }
    std::vector<Branch*>::iterator it;
    for(it=fBranches.begin();it!=fBranches.end();++it) {
      int n;
      if(!(*it)->findEntry(aEntry,n)) return false;
      aNbytes += n;
    } 
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BranchElement::reset(
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
void Rio::BranchElement::setAutoDelete(
 bool aAutoDelete
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Branch::setAutoDelete(aAutoDelete);
  std::vector<Branch*>::iterator it;
  for(it=fBranches.begin();it!=fBranches.end();++it) {
    (*it)->setAutoDelete(aAutoDelete);
  } 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BranchElement::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //special case when called from code generated by TTree::MakeClass
  //if (long(aAddress) == -1) {
  //  return false;
  //}
  fReadEntry = -1; 
  //BaseLeaf* leaf = fLeaves.size() ? (BaseLeaf*)fLeaves[0] : 0;
  //if (leaf) {
  //  leaf->setAddress(aAddress);
  //} else {
  //  Out out(printer());
  //  out << "Rio::BranchElement::setAddress : no leaf !"  << endl;
  //}
  fAddress = (char*)aAddress;
/*
  IObject** ppointer = (IObject**)aAddress;
  IObject* obj = 0;
  if (aAddress) obj = (IObject*)(*ppointer);
  IClass* cl = dictionary().findInStoreClass(fClassName);
  if (!obj && cl) {
    Out out(printer());
    out << "Rio::BranchElement::setAddress : who delete ? " << endl;
    //FIXME : who delete ?
    Arguments args;
    Args_init(args);
    obj = cl->create(args);
    *ppointer = obj;
  }
  fOldObject = obj;
  if (!cl) {
    Out out(printer());
    out << "Rio::BranchElement::setAddress : no class." << endl;
    std::vector<Branch*>::iterator it;
    for(it=fBranches.begin();it!=fBranches.end();++it) {
      char* pointer = (char*)obj;
      (*it)->setAddress(pointer);
    } 
    return;
  }
*/
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BranchElement::setBasketSize(
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
//////////////////////////////////////////////////////////////////////////////
std::string Rio::BranchElement::className(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fClassName;
}

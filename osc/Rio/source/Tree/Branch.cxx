// this :
#include <Rio/Tree/Branch.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Debug.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Lib.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/realloc.h>

// Tree :
#include <Rio/Tree/Basket.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/BaseLeaf.h>
#include <Rio/Tree/Leaf.h>
#include <Rio/Tree/LeafString.h>

// Graf :
#include <Rio/Graf/AttFill.h> //Beurk

#define MAXIMUM(a,b) ((a)>(b)?a:b) 

const unsigned int Rio_kNewClassTag   = 0xFFFFFFFF;
// OR the byte count with this
const unsigned int Rio_kByteCountMask = 0x40000000;  
const int Rio_kMaxRAM = 10;
const int Rio_kInitCapacity = 16;

RIO_META_SOURCE(Rio::Branch)

//////////////////////////////////////////////////////////////////////////////
Rio::Branch::Branch(
 const IDictionary& aDictionary
,Tree& aTree
)
:Named(aDictionary,"","")
,fClass(aDictionary.opt_Branch_Class())
,fTree(aTree)
,fAutoDelete(false)
,fBranches(aDictionary,fTree.out())
,fLeaves(aDictionary,fTree.out())
,fBaskets(aDictionary,fTree.out(),Rio_kInitCapacity,0)
,fCompress(0)
,fBasketSize(32000)
,fEntryOffsetLen(1000)
,fWriteBasket(0)
,fEntryNumber(0)
,fOffset(0)
,fMaxBaskets(10) //1000 in 3.0.06. 10 in 4.00.08. Optimize stream write.
,fNleaves(0)
,fReadBasket(0)
,fReadEntry(-1)
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fNBasketRAM(0)
,fBasketRAM(0)
,fBasketBytes(0)
,fBasketEntry(0)
,fBasketSeek(0)
,fAddress(0)
,fEntryBuffer(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Branch");
  fNBasketRAM = Rio_kMaxRAM+1;
  fBasketRAM = new int[Rio_kMaxRAM]; 
  for (int i=0;i<Rio_kMaxRAM;i++) fBasketRAM[i] = -1;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch::Branch(
 const IDictionary& aDictionary
,Tree& aTree
,const std::string& aName
,const std::string& aTitle
,void* aAddress
,const std::string& aLeafList
,int aBasketSize
,int aCompress
)
:Named(aDictionary,aName,aLeafList)
,fClass(aDictionary.opt_Branch_Class())
,fTree(aTree)
,fAutoDelete(false)
,fBranches(aDictionary,fTree.out())
,fLeaves(aDictionary,fTree.out())
,fBaskets(aDictionary,fTree.out(),Rio_kInitCapacity,0)
,fCompress(aCompress)
,fBasketSize(0)
,fEntryOffsetLen(0)
,fWriteBasket(0)
,fEntryNumber(0)
,fOffset(0)
,fMaxBaskets(10) //1000 in 3.0.06. 10 in 4.00.08. Optimize stream write.
,fSplitLevel(0)
,fNleaves(0)
,fReadBasket(0)
,fReadEntry(-1)
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fNBasketRAM(0)
,fBasketRAM(0)
,fBasketBytes(0)
,fBasketEntry(0)
,fBasketSeek(0)
,fAddress(0)
,fEntryBuffer(0)
//////////////////////////////////////////////////////////////////////////////
// Create a Branch :
//       * address is the address of the first item of a structure
//         or the address of a pointer to an object (see example).
//       * leaflist is the concatenation of all the variable names and types
//         separated by a colon character :
//         The variable name and the variable type are separated by 
//         a slash (/).
//         The variable type may be 0,1 or 2 characters. If no type is given,
//         the type of the variable is assumed to be the same as the previous
//         variable. If the first variable does not have a type, it is assumed
//         of type F by default. The list of currently supported 
//         types is given below:
//            - C : a character string terminated by the 0 character
//            - B : an 8 bit signed integer (Char_t)
//            - b : an 8 bit unsigned integer (UChar_t)
//            - S : a 16 bit signed integer (Short_t)
//            - s : a 16 bit unsigned integer (UShort_t)
//            - I : a 32 bit signed integer (Int_t)
//            - i : a 32 bit unsigned integer (UInt_t)
//            - F : a 32 bit floating point (Float_t)
//            - D : a 64 bit floating point (Double_t)
//
//         By default, a variable will be copied to the buffer with 
//         the number of
//         bytes specified in the type descriptor character. However, 
//         if the type
//         consists of 2 characters, the second character is an integer that
//         specifies the number of bytes to be used when copying the variable
//         to the output buffer. Example:
//             X    : variable X, type Float_t
//             Y/I  : variable Y, type Int_t
//             Y/I2 : variable Y, type Int_t converted to a 16 bits integer
//
//   See an example of a Branch definition in the TTree constructor.
//
//   Note that in case the data type is an object, this branch can contain
//   only this object.
//
//    Note that this function is invoked by TTree::Branch
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::Branch");
  if (aBasketSize < 100) aBasketSize = 100;
  fBasketSize     = aBasketSize;
  fAddress        = (char*)aAddress;
  fNBasketRAM     = Rio_kMaxRAM+1;
  fBasketRAM      = new int[Rio_kMaxRAM]; 
  int i;
  for (i=0;i<Rio_kMaxRAM;i++) fBasketRAM[i] = -1;
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  fBasketEntry[0] = fEntryNumber; //GB ??? the below override !
  for (i=0;i<fMaxBaskets;i++) {
    fBasketBytes[i] = 0;
    fBasketEntry[i] = 0;
    fBasketSeek[i]  = 0;
  }
  
  // Decode the leaflist (search for : as separator)
  std::vector<std::string> leaves;
  smanip::words(aLeafList,":",leaves);
  unsigned int leafn = leaves.size();
  int offset = 0;
  for(unsigned int index=0;index<leafn;index++) {
    std::vector<std::string> words;
    smanip::words(leaves[index],"/",words);
    if(words.size()==2) {
      std::string& leafName = words[0];
      std::string& leafType = words[1];
      BaseLeaf* leaf = 0;
      if(leafType == "F") {
        leaf = new LeafFloat(dictionary(),*this,leafName);
      } else if(leafType == "D") {
        leaf = new LeafDouble(dictionary(),*this,leafName);
      } else if( (leafType == "I") || (leafType == "i") ) {
        leaf = new LeafInteger(dictionary(),*this,leafName);
      } else if( (leafType == "S") || (leafType == "s") ) {
        leaf = new LeafShort(dictionary(),*this,leafName);
      } else if( (leafType == "B") || (leafType == "b") ) {
        leaf = new LeafCharacter(dictionary(),*this,leafName);
      } else if(leafType == "C") {
        leaf = new LeafString(dictionary(),*this,leafName);
      } else {
	std::ostream& out = fTree.out();
        out << "Rio::Branch::Branch : Illegal data type \"" 
            << leafType << "\"." << std::endl;
        break;
      }
      /*FIXME 
        if (leaf->IsZombie()) {
        delete leaf;
        Error("TBranch","Illegal leaf:%s/%s",name,leaflist);
        MakeZombie();
        break;
        }*/
      
      if(!leaf->setAddress((char*)(fAddress+offset))) {
	std::ostream& out = fTree.out();
        out << "Rio::Branch::Branch : setAddress failed." << std::endl;
      }
      leaf->setOffset(offset);
      if (leaf->leafCount()) fEntryOffsetLen = 1000;
      if(Rio::cast<LeafString>(*leaf)) fEntryOffsetLen = 1000;
      fNleaves++;
      fLeaves.push_back(leaf);
      fTree.leaves().push_back(leaf);
      offset += leaf->lengthType() * leaf->length();
    } else {
      std::ostream& out = fTree.out();
      out << "Rio::Branch::Branch : Bad format for \"" 
          << leaves[index] << "\". Should be \"<name>/[F,f]\"." << std::endl;
      break;
    }
  }


  // Have at least one valid basket (should be done after the upper loop) :
  fBaskets[0] = new Basket(aDictionary,fTree.directory(),*this,aName,aTitle);

}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch::Branch(
 const Branch& aFrom
)
:Named(aFrom)
,fClass(aFrom.fClass)
,fTree(aFrom.fTree)
,fAutoDelete(false)
,fBranches(aFrom.dictionary(),aFrom.fTree.out())
,fLeaves(aFrom.dictionary(),aFrom.fTree.out())
,fBaskets(aFrom.dictionary(),aFrom.fTree.out(),Rio_kInitCapacity,0)
,fCompress(0)
,fBasketSize(32000)
,fEntryOffsetLen(1000)
,fWriteBasket(0)
,fEntryNumber(0)
,fOffset(0)
,fMaxBaskets(10)
,fNleaves(0)
,fReadBasket(0)
,fReadEntry(-1)
,fEntries(0)
,fTotBytes(0)
,fZipBytes(0)
,fNBasketRAM(0)
,fBasketRAM(0)
,fBasketBytes(0)
,fBasketEntry(0)
,fBasketSeek(0)
,fAddress(0)
,fEntryBuffer(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch::~Branch(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fBasketRAM)   delete [] fBasketRAM;
  if (fBasketEntry) delete [] fBasketEntry;
  if (fBasketBytes) delete [] fBasketBytes;
  if (fBasketSeek)  delete [] fBasketSeek;
  fBasketRAM   = 0;
  fBasketEntry = 0;
  fBasketBytes = 0;
  fBasketSeek  = 0;
  
  /*FIXME
 {unsigned int sz = fBranches.size();
  for(unsigned int index=0;index<sz;index++) delete fBranches[index];
  fBranches.clear();}*/

 {unsigned int sz = fLeaves.size();
  for(unsigned int index=0;index<sz;index++) delete fLeaves[index];
  fLeaves.clear();}

 {unsigned int sz = fBaskets.size();
  for(unsigned int index=0;index<sz;index++) delete fBaskets[index];
  fBaskets.clear();}

 /*
   // Warning. Must use FindObject by name instead of fDirectory->GetFile()
   // because two branches<may point to the same file and the file
   // already deleted in the previous branch
   if (fDirectory && fDirectory != fTree->GetDirectory()) {
   TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject(GetFileName());
   if (file ) delete file;
   }
   fTree        = 0;
   fDirectory   = 0;
 */

  Debug::decrement("Rio::Branch");
}
//////////////////////////////////////////////////////////////////////////////
Rio::Branch& Rio::Branch::operator=(
 const Branch&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
double Rio::Branch::entries(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntries;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Tree& Rio::Branch::tree(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fTree;
}
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::BaseLeaf*>& Rio::Branch::leaves(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLeaves;
}
/*
//////////////////////////////////////////////////////////////////////////////
std::vector<Rio::Branch*>& Rio::Branch::branches(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBranches;
}
*/

std::ostream& Rio::Branch::out() const {return fTree.directory().out();}

//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::compressionLevel(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCompress;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::basketSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBasketSize;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::writeBasket(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fWriteBasket;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Branch::entryOffsetLength(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntryOffsetLen;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setEntryOffsetLength(
 int aEntryOffsetLen
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fEntryOffsetLen = aEntryOffsetLen;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::Branch::leaf(
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
bool Rio::Branch::fill(
 int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Loop on all leaves of this branch to fill Basket buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  //FIXME if (TestBit(kDoNotProcess)) return 0;

  Basket* basket = getBasket(fWriteBasket);
  if (!basket) {
    fTree.out() << "Rio::Branch::fill : getBasket failed." << std::endl;
    return false;
  }
  Buffer* buf = basket->bufferRef();
  
  // Fill basket buffer
  if (buf->inline_isReading()) {
    basket->setWriteMode();
  }

  buf->resetMap();
  int lold = buf->inline_length();
  int objectStart = 0;
  if ( fEntryBuffer ) {
    /*FIXME
    if ( fEntryBuffer->IsA() == TMessage::Class() ) {
      objectStart = 8;
    }
    */
    // We are required to copy starting at the version number (so not
    // including the class name.
    // See if byte count is here, if not its class still be a newClass
    unsigned int startpos = fEntryBuffer->inline_length();
    fEntryBuffer->inline_setBufferOffset(objectStart);

    unsigned int tag;
    fEntryBuffer->read(tag);

    if ( tag & Rio_kByteCountMask ) {
      fEntryBuffer->read(tag);
    }
    if ( tag == Rio_kNewClassTag ) {
      char s[80];
      fEntryBuffer->readString(s, 80);
    } else {
      fEntryBuffer->inline_setBufferOffset(objectStart);
    }
    objectStart = fEntryBuffer->inline_length();
    fEntryBuffer->inline_setBufferOffset(startpos);
    
    basket->update(lold, objectStart - fEntryBuffer->displacement());
  } else {
    basket->update(lold,lold);
  }

  fEntries++;
  fEntryNumber++;

  if (fEntryBuffer) {
    
    unsigned int len;
    unsigned int startpos = fEntryBuffer->inline_length();
    if (startpos > (unsigned int)(objectStart)) {
      // We assume this buffer have just been directly filled
      // the current position in the buffer indicates the end of the object!
      len = fEntryBuffer->inline_length() - objectStart;
    } else {
      // The buffer have been acquired either via TSocket or via
      // TBuffer::SetBuffer(newloc,newsize)
      // Only the actual size of the memory buffer gives us an hint about where
      // the object ends.
      len = fEntryBuffer->bufferSize() - objectStart;
    }
    if(!buf->writeBuffer(fEntryBuffer->buffer()+objectStart,len)) {
      fTree.out() << "Rio::Branch::fill : " 
                  << " Basket.bufferRef().writeBuffer() failed."
                  << std::endl;
      return false;
    }
  } else {
    if(!fillLeaves(*buf)) return false;
  }

  int lnew   = buf->inline_length();
  int nbytes = lnew - lold;
  int nsize  = 0;
  if (fEntryOffsetLen) {
    int nevbuf = basket->nevBuf();
    nsize = nevbuf * sizeof(int); //total size in bytes of EntryOffset table
  } else {
    if (!basket->nevBufSize()) basket->setNevBufSize(nbytes);
  }
  
  // Should we create a new basket?
  if ( (lnew +2*nsize +nbytes) >= fBasketSize ) { 
    int nout;
    if(!basket->writeBuffer(nout)) { //GB : does a Key::writeFile.
      fTree.out() << "Rio::Branch::fill : " 
                  << " Basket.writeBuffer() failed."
                  << std::endl;
      return false;
    }
    fBasketBytes[fWriteBasket]  = basket->numberOfBytes();
    fBasketSeek[fWriteBasket]   = basket->seekKey();
    int addbytes = basket->objectSize() + basket->keyLength() ;
    if (fTree.directory().isWritable()) {
      delete basket;
      fBaskets[fWriteBasket] = 0;
    }
    fZipBytes += nout;
    fTotBytes += addbytes;
    fTree.addTotBytes(addbytes);
    fTree.addZipBytes(nout);
    basket = 
      new Basket(dictionary(),fTree.directory(),*this,fName,fTree.name());
    fWriteBasket++;
    //FIXME fBaskets.AddAtAndExpand(basket,fWriteBasket);
    if(fWriteBasket>=(int)fBaskets.size())  { //FIXME : the same than upper ?
      fBaskets.resize(2*fWriteBasket,0);
    }
    fBaskets[fWriteBasket] = basket;

    if (fWriteBasket >= fMaxBaskets) {
      //Increase BasketEntry buffer of a minimum of 10 locations
      // and a maximum of 50 per cent of current size
      int newsize = MAXIMUM(10,int(1.5*fMaxBaskets));
      fBasketEntry  = Rio::realloc<int>(fBasketEntry, 
                                        newsize * sizeof(int), 
                                        fMaxBaskets * sizeof(int));
      fBasketBytes  = Rio::realloc<int>(fBasketBytes, 
                                        newsize * sizeof(int), 
                                        fMaxBaskets * sizeof(int));
      fBasketSeek = Rio::realloc<Seek>(fBasketSeek, 
                                       newsize * sizeof(Seek), 
                                       fMaxBaskets * sizeof(Seek));
      fMaxBaskets   = newsize;
    }
    fBasketEntry[fWriteBasket] = fEntryNumber;
    fBasketBytes[fWriteBasket] = 0;
    fBasketSeek[fWriteBasket]  = 0;
  }
  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket* Rio::Branch::getBasket(
 int aBasketIndex
)
//////////////////////////////////////////////////////////////////////////////
// Return pointer to basket aBasketIndex in this Branch.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  static int nerrors = 0;

  // reference to an existing basket in memory ?
  if (aBasketIndex <0 || aBasketIndex > fWriteBasket) {
    fTree.out() << "Rio::Branch::getBasket :" 
                << " bad index."
                << std::endl;
    return 0;
  }

  Basket* basket = fBaskets[aBasketIndex];
  if (basket) return basket;

  // create/decode basket parameters from buffer
  basket = new Basket(dictionary(),fTree.directory(),*this);
  if (fBasketBytes[aBasketIndex] == 0) {
    int n;
    if(!basket->readBasketBytes(fBasketSeek[aBasketIndex],n)) {
      fTree.out() << "Rio::Branch::getBasket :" 
                  << " readBasketBytes failed."
                  << std::endl;
      delete basket;
      return 0;
    }
    fBasketBytes[aBasketIndex] = n;
  }

  bool status = basket->readBasketBuffers(fBasketSeek[aBasketIndex],
                                          fBasketBytes[aBasketIndex]);
  if (!status || 
      (basket->seekKey() != fBasketSeek[aBasketIndex]) ||
      (basket->numberOfBytes() != fBasketBytes[aBasketIndex])
      ){
    std::ostream& out = fTree.out();

    if(basket->seekKey()!=fBasketSeek[aBasketIndex]) { //G.Barrand
      out << "Rio::Branch::getBasket :" 
          << " seeKey problem."
          << " basket seekKey is " << basket->seekKey()
          << ", whilst fBasketSeek[aBasketIndex] is "
          << fBasketSeek[aBasketIndex] << "."
          << std::endl;
    }
    if(basket->numberOfBytes()!=fBasketBytes[aBasketIndex]) { //G.Barrand
      out << "Rio::Branch::getBasket :" 
          << " length problem."
          << " basket numberOfBytes is " << basket->numberOfBytes()
          << ", whilst fBasketBytes[aBasketIndex] is "
          << fBasketBytes[aBasketIndex] << "."
          << std::endl;
    }

    nerrors++;
    if (nerrors > 10) {
      out << "Rio::Branch::getBasket : " 
          << "too much error." << std::endl;
      delete basket;
      return 0;
    }
    if (nerrors == 10) {
      out << "Rio::Branch::getBasket : " 
          << "file probably overwritten: "
          << "stopping reporting error messages." << std::endl;
      if (fBasketSeek[aBasketIndex] > 2000000000) {
        out << "Rio::Branch::getBasket : " 
            << "===>File is more than 2 Gigabytes." << std::endl;
        delete basket;
        return 0;
      }
      if (fBasketSeek[aBasketIndex] > 1000000000) {
        out << "Rio::Branch::getBasket : " 
            << "===>Your file is may be bigger than the "
            << "maximum file size allowed on your system." << std::endl;
        out << "Rio::Branch::getBasket :     " 
            << "Check your AFS maximum file size limit for example." 
            << std::endl;
        delete basket;
        return 0;
      }
    }
    out << "Rio::Branch::getBasket : File: \"" 
        << fTree.directory().file().name() 
        << "\" at byte:" << basket->seekKey() 
        << ", branch:" << fName << ", entry:" << fReadEntry << std::endl;
    delete basket;
    return 0;
  }
  
  fBaskets[aBasketIndex] = basket;
  if (fNBasketRAM < Rio_kMaxRAM) fBasketRAM[fNBasketRAM] = aBasketIndex;
  fNBasketRAM++;
  return basket;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::fillLeaves(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<BaseLeaf*>::iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    if(!(*it)->fillBasket(aBuffer)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::readLeaves(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::vector<BaseLeaf*>::iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    if(!(*it)->readBasket(aBuffer)) {
      std::ostream& out = fTree.out();
      out << "Rio::Branch::readLeaves : failed." << std::endl;
      return false;
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::findEntry( 
 int aEntry
,int& aNbytes
)
//////////////////////////////////////////////////////////////////////////////
// Read all leaves of entry and return total number of bytes :
// The input argument entry is the entry serial number in the current tree.
// In case of a TChain, the entry number in the current Tree must be found
//  before calling this function. example with TChain *chain;
//  Int_t localEntry = chain->LoadTree(entry);
//  branch->GetEntry(localEntry);
//
//  The function returns the number of bytes read from the input buffer.
//  If entry does not exist or an I/O error occurs, the function returns false.
//  (not supported : if entry is the same as the previous call, 
//   the function returns 1)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aNbytes = 0;
  if (fReadEntry == aEntry) return true;
  if (aEntry < 0 || aEntry >= fEntryNumber) {
    std::ostream& out = fTree.out();
    out << "Rio::Branch::findEntry :"
        << " aEntry not within [0," << fEntryNumber << "[."
        << std::endl;
    return false;
  }
  int nbytes;
  int first = fBasketEntry[fReadBasket];
  int last;
  if (fReadBasket == fWriteBasket) last = fEntryNumber - 1;
  else                             last = fBasketEntry[fReadBasket+1] - 1;

  //
  //      Are we still in the same ReadBasket?
  if (aEntry < first || aEntry > last) {
    fReadBasket = binarySearch(fWriteBasket+1, fBasketEntry, aEntry);
    first       = fBasketEntry[fReadBasket];
  }

  //     We have found the basket containing this entry.
  //     make sure basket buffers are in memory.
  Basket* basket = fBaskets[fReadBasket];
  if (!basket) {
    basket = getBasket(fReadBasket);
    if (!basket) {
      std::ostream& out = fTree.out();
      out << "Rio::Branch::findEntry :"
          << " can't get basket."
          << std::endl;
      return false;
    }
  }

  Buffer* buf = basket->bufferRef();
  //     This test necessary to read very old Root files (NvE)
  if (!buf) {
    //File* file = GetFile(0);
    //FIXME : should check status
    if(!basket->readBasketBuffers(fBasketSeek[fReadBasket],
                              fBasketBytes[fReadBasket])) {
      std::ostream& out = fTree.out();
      out << "Rio::Branch::findEntry :"
          << " can't read basket buffers."
          << std::endl;
      return false;
    }
    buf = basket->bufferRef();
  }
  //     Set entry offset in buffer and read data from all leaves
  buf->resetMap();
  if (!buf->inline_isReading()) {
    basket->setReadMode();
  }
  
  int bufbegin;
 {int* entryOffset = basket->entryOffset();
  if (entryOffset) {
    bufbegin = entryOffset[aEntry-first];
    //::printf("debug : entryOffset : uuuu %d\n",bufbegin);
  } else {
    bufbegin = basket->keyLength() + (aEntry-first) * basket->nevBufSize();
  }}

  buf->inline_setBufferOffset(bufbegin);
  int* displacement = basket->displacement();
  if (displacement) buf->setDisplacement(displacement[aEntry-first]);
  else buf->setDisplacement();

  if(!readLeaves(*buf)) {
    std::ostream& out = fTree.out();
    out << "Rio::Branch::findEntry :"
        << " can't read leaves."
        << std::endl;
    return false;
  }

  nbytes = buf->inline_length() - bufbegin;
  fReadEntry = aEntry;

  aNbytes = nbytes;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::reset(
)
//////////////////////////////////////////////////////////////////////////////
// Reset a Branch :
//    Existing buffers are deleted
//    Entries, max and min are reset
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {for(unsigned int index=0;index<fBaskets.size();index++) 
    delete fBaskets[index];
  fBaskets.clear();}

  if (fBasketEntry) delete [] fBasketEntry;
  if (fBasketBytes) delete [] fBasketBytes;
  if (fBasketSeek)  delete [] fBasketSeek;
  fBasketEntry    = 0;
  fReadBasket     = 0;
  fReadEntry      = -1;
  fWriteBasket    = 0;
  fEntries        = 0;
  fTotBytes       = 0;
  fZipBytes       = 0;
  fEntryNumber    = 0;
  fBasketEntry    = new int[fMaxBaskets];
  fBasketBytes    = new int[fMaxBaskets];
  fBasketSeek     = new Seek[fMaxBaskets];
  fBasketEntry[0] = fEntryNumber;
  for (int i=0;i<fMaxBaskets;i++) {
    fBasketBytes[i] = 0;
    fBasketEntry[i] = 0;
    fBasketSeek[i]  = 0;
  }
  fBaskets.resize(Rio_kInitCapacity,0);
  fBaskets[0] = new Basket(dictionary(),fTree.directory(),*this,fName,fTitle);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fReadEntry = -1;
  fAddress = (char*)aAddress;
  std::vector<BaseLeaf*>::const_iterator it;
  for(it=fLeaves.begin();it!=fLeaves.end();++it) {
    int offset = (*it)->offset();
    //FIXME if (TestBit(kIsClone)) offset = 0;
    if(!(*it)->setAddress(fAddress+offset)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void* Rio::Branch::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(Branch);
  else 
    return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s, c;
    fAddress = 0;
    //FIXME gROOT->SetReadingObject(kTRUE);
    short v;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if (v > 5) {
      /*FIXME 
      TBranch::Class()->ReadBuffer(b, this, v, s, c);
      gBranch = branchSave;
      fDirectory = gDirectory;
      fNleaves = fLeaves.GetEntriesFast();
      if (fFileName.Length() != 0) fDirectory = 0;
      gROOT->SetReadingObject(kFALSE);
      return;
        */
    }
    //====process old versions before automatic schema evolution

    if(!Named::stream(aBuffer)) return false;

    if (v <=5 ) {
      if(!aBuffer.read(fCompress)) return false;
      if(!aBuffer.read(fBasketSize)) return false;
      if(!aBuffer.read(fEntryOffsetLen)) return false;
      if(!aBuffer.read(fMaxBaskets)) return false;
      if(!aBuffer.read(fWriteBasket)) return false;
      if(!aBuffer.read(fEntryNumber)) return false;
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
      if(!aBuffer.read(fOffset)) return false;
    } else if(v<=6) {
      if(!aBuffer.read(fCompress)) return false;
      if(!aBuffer.read(fBasketSize)) return false;
      if(!aBuffer.read(fEntryOffsetLen)) return false;
      if(!aBuffer.read(fWriteBasket)) return false;
      if(!aBuffer.read(fEntryNumber)) return false;
      if(!aBuffer.read(fOffset)) return false;
      if(!aBuffer.read(fMaxBaskets)) return false;
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
    } else if(v<=7) {
      if(!aBuffer.read(fCompress)) return false;
      if(!aBuffer.read(fBasketSize)) return false;
      if(!aBuffer.read(fEntryOffsetLen)) return false;
      if(!aBuffer.read(fWriteBasket)) return false;
      if(!aBuffer.read(fEntryNumber)) return false;
      if(!aBuffer.read(fOffset)) return false;
      if(!aBuffer.read(fMaxBaskets)) return false;
      if(!aBuffer.read(fSplitLevel)) return false;
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
    } else { //v 8
      AttFill dummy(dictionary());
      if(!dummy.stream(aBuffer)) return false;
      if(!aBuffer.read(fCompress)) return false;
      if(!aBuffer.read(fBasketSize)) return false;
      if(!aBuffer.read(fEntryOffsetLen)) return false;
      if(!aBuffer.read(fWriteBasket)) return false;
      if(!aBuffer.read(fEntryNumber)) return false;
      if(!aBuffer.read(fOffset)) return false;
      if(!aBuffer.read(fMaxBaskets)) return false;
      if(!aBuffer.read(fSplitLevel)) return false;
      if(!aBuffer.read(fEntries)) return false;
      if(!aBuffer.read(fTotBytes)) return false;
      if(!aBuffer.read(fZipBytes)) return false;
    }

    Arguments args;
    Args_init(args);
    Args_insertDirectory(args,&fTree.directory());
    Args_insertTree(args,&fTree);
    Args_insertBranch(args,this);

   {unsigned int sz = fBranches.size();
    for(unsigned int index=0;index<sz;index++) delete fBranches[index];
    fBranches.clear();}
   {unsigned int sz = fLeaves.size();
    for(unsigned int index=0;index<sz;index++) delete fLeaves[index];
    fLeaves.clear();}
   {unsigned int sz = fBaskets.size();
    for(unsigned int index=0;index<sz;index++) delete fBaskets[index];
    fBaskets.clear();}
    //TObjArray
    if(!fBranches.read(aBuffer,args)) {
      std::ostream& out = aBuffer.out();
      out << "Rio::Branch::stream : can't read branches." << std::endl;
      return false;
    }
    //TObjArray
    if(!fLeaves.read(aBuffer,args)) {
      std::ostream& out = aBuffer.out();
      out << "Rio::Branch::stream : can't read leaves." << std::endl;
      return false;
    } 
    //TObjArray
    if(!fBaskets.read(aBuffer,args,false)) {
      std::ostream& out = aBuffer.out();
      out << "Rio::Branch::stream : can't read baskets." << std::endl;
      return false;
    }

    if(fBaskets.size()<=0) {
      std::ostream& out = aBuffer.out();
      out << "Rio::Branch::stream : error : fBaskets now empty." << std::endl;
    } else {
      /* debug
         std::ostream& out = aBuffer.out();
         out << "Rio::Branch::stream : read : baskets " 
             << fBaskets.size() << " :" << std::endl;
         std::vector<Basket*>::iterator it;
         int index = 0;
         for(it=fBaskets.begin();it!=fBaskets.end();++it,++index) {
	   if(*it) {
             out << "Rio::Branch::stream : read :"
                 << " index " << index
                 << " obj " << *it
                 << " seek " << (*it)->seekKey()
                 << " nbytes " << (*it)->numberOfBytes()
                 << " objectSize " << (*it)->objectSize()
                 << std::endl;
           } 
         } 
      */
    }
    
    fNleaves = fLeaves.size();
    
    fBasketEntry = new int[fMaxBaskets];
    fBasketBytes = new int[fMaxBaskets];
    fBasketSeek = new Seek[fMaxBaskets];
    
    if (v <6 ) {
      int n;
      if(!aBuffer.readArray(fBasketEntry,n)) return false;
      if (v <= 4) {
        for (int n=0;n<fMaxBaskets;n++) fBasketBytes[n] = 0;
      } else {
        if(!aBuffer.readArray(fBasketBytes,n)) return false;
      }
      if (v < 2) {
        for (int n=0;n<fWriteBasket;n++) {
          fBasketSeek[n] = getBasket(n)->seekKey();
        }
      } else {
#ifdef RIO_LARGE_FILE
        if(!aBuffer.read(n)) return false;
        Seek32 s;
        for(int i=0;i<n;i++) {
          if(!aBuffer.read(s)) return false;
          fBasketSeek[i] = s;
        }
#else
        if(!aBuffer.readArray(fBasketSeek,n)) return false;
#endif
      }
    } else {
      // See TStreamerInfo::ReadBuffer::ReadBasicPointer
      //Int_t[fMaxBaskets]
      char isArray;
      if(!aBuffer.read(isArray)) return false;
      if(isArray!=0) {
        if(!aBuffer.readFastArray(fBasketBytes,fMaxBaskets)) return false;
      }
      //Int_t[fMaxBaskets]
      if(!aBuffer.read(isArray)) return false;
      if(isArray!=0) {
        if(!aBuffer.readFastArray(fBasketEntry,fMaxBaskets)) return false;
      }
      //Seek_t[fMaxBaskets]
#ifdef RIO_LARGE_FILE
      char isBigFile;
      if(!aBuffer.read(isBigFile)) return false;
      if(isBigFile==2) {
        if(!aBuffer.readFastArray(fBasketSeek,fMaxBaskets)) return false;
      } else {
        Seek32 s;
        for(int i=0;i<fMaxBaskets;i++) {
          if(!aBuffer.read(s)) return false;
          fBasketSeek[i] = s;
        }
      }
#else
      if(!aBuffer.read(isArray)) return false;
      if(!aBuffer.readFastArray(fBasketSeek,fMaxBaskets)) return false;
#endif
    }
    
    if (v > 2) {
      //TString
      std::string fileName;
      if(!aBuffer.read(fileName)) return false;
    }
    
    //FIXME if (v < 4) SetAutoDelete(kTRUE);
    //FIXME gROOT->SetReadingObject(kFALSE);
    if(!aBuffer.checkByteCount(s, c,fClass.inStoreName())) return false;
    
    //====end of old versions
  } else {
    /*FIXME
    TBranch::Class()->WriteBuffer(b,this);
    // if branch is in a separate file save this branch
    // as an independent key
    if (fDirectory && fDirectory != fTree->GetDirectory()) {
    TDirectory *cursav = gDirectory;
    fDirectory->cd();
    fDirectory = 0;  // to avoid recursive calls
    Write();
    fDirectory = gDirectory;
    cursav->cd();
    }
    */
    //====process old versions before automatic schema evolution
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;

    if(!Named::stream(aBuffer)) return false;

    AttFill dummy(dictionary());
    if(!dummy.stream(aBuffer)) return false;
    if(!aBuffer.write(fCompress)) return false;
    if(!aBuffer.write(fBasketSize)) return false;
    if(!aBuffer.write(fEntryOffsetLen)) return false;
    if(!aBuffer.write(fWriteBasket)) return false;
    if(!aBuffer.write(fEntryNumber)) return false;
    if(!aBuffer.write(fOffset)) return false;
    if(!aBuffer.write(fMaxBaskets)) return false;
    if(!aBuffer.write(fSplitLevel)) return false;
    if(!aBuffer.write(fEntries)) return false;
    if(!aBuffer.write(fTotBytes)) return false;
    if(!aBuffer.write(fZipBytes)) return false;

    if(!fBranches.write(aBuffer)) return false;
    if(!fLeaves.write(aBuffer)) return false;
    if(!fBaskets.write(aBuffer)) return false; //does Basket::stream(write)

    // See TStreamerInfo::ReadBuffer::WriteBasicPointer
    if(!aBuffer.write((char)1)) return false;
    if(!aBuffer.writeFastArray(fBasketBytes,fMaxBaskets)) return false;
    if(!aBuffer.write((char)1)) return false;
    if(!aBuffer.writeFastArray(fBasketEntry,fMaxBaskets)) return false;
#ifdef RIO_LARGE_FILE
    char isBigFile = 1;

    //GB : begin
    //if(fTree.directory().file().end()>RIO_START_BIG_FILE) isBigFile = 2;
   {for(int i=0;i<fMaxBaskets;i++) {
      if(fBasketSeek[i]>RIO_START_BIG_FILE) {
        isBigFile = 2;
        break;
      }
    }}
    //GB : end
   
    if(!aBuffer.write(isBigFile)) return false;
    if(isBigFile==2) {
      if(!aBuffer.writeFastArray(fBasketSeek,fMaxBaskets)) return false;
    } else {
      for(int i=0;i<fMaxBaskets;i++) {
        if(fBasketSeek[i]>RIO_START_BIG_FILE) { //G.Barrand : add this test.
          aBuffer.out() << "Rio::Branch::stream :"
                        << " attempt to write big Seek "
                        << fBasketSeek[i] << " on 32 bits."
                        << std::endl;
          return false;
        }

        if(!aBuffer.write((Seek32)fBasketSeek[i])) return false;
      }
    }
#else
    if(!aBuffer.write((char)1)) return false;
    if(!aBuffer.writeFastArray(fBasketSeek,fMaxBaskets)) return false;
#endif

    // fFileName
    if(!aBuffer.write(std::string(""))) return false;

    /*
    // if branch is in a separate file save this branch
    // as an independent key
    if (fDirectory && fDirectory != fTree->GetDirectory()) {
      TDirectory *cursav = gDirectory;
      fDirectory->cd();
      fDirectory = 0;  // to avoid recusive calls
      Write();
      fDirectory = gDirectory;
      cursav->cd();
    }
    */
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Branch::isAutoDelete(
) const
//////////////////////////////////////////////////////////////////////////////
// Return TRUE if an existing object in a TBranchObject must be deleted.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAutoDelete;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setAutoDelete(
 bool aAutoDelete
)
//////////////////////////////////////////////////////////////////////////////
// Set the AutoDelete bit
// this bit is used by TBranchObject::ReadBasket to decide if an object
// referenced by a TBranchObject must be deleted or not before reading
// a new entry
// if autodel is kTRUE, this existing object will be deleted, a new object
//    created by the default constructor, then object->Streamer
// if autodel is kFALSE, the existing object is not deleted. Root assumes
//    that the user is taking care of deleting any internal object or array
//    This can be done in Streamer itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   fAutoDelete = aAutoDelete;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::setBasketSize(
 int aBufferSize
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBasketSize = aBufferSize;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Branch::dropBaskets(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //   Loop on all branch baskets.  Drop all except readbasket

  // fast algorithm in case of only a few baskets in memory
  int i,j;
  if (fNBasketRAM < Rio_kMaxRAM) {
    for (i=0;i<Rio_kMaxRAM;i++) {
      j = fBasketRAM[i];
      if (j < 0) continue;
      if (j == fReadBasket || j == fWriteBasket) continue;
      Basket* basket = fBaskets[j];
      if (!basket) continue;
      basket->dropBuffers();
      fBaskets[j] = 0;
      delete basket;
      fBasketRAM[i] = -1;
      fNBasketRAM--;
    }
    if (fNBasketRAM < 0) {
      std::ostream& out = fTree.out();
      out << "Rio::Branch::dropBaskets :"
          << " ERROR, fNBasketRAM = " << fNBasketRAM << " ." << std::endl;
      fNBasketRAM = 0;
    }
    i = 0;
    for (j=0;j<Rio_kMaxRAM;j++) {
      if (fBasketRAM[j] < 0) continue;
      fBasketRAM[i] = fBasketRAM[j];
      i++;
    }
    return;
  }

  //general algorithm looping on the full baskets table.
  int nbaskets = fBaskets.size();
  fNBasketRAM = 0;
  for (j=0;j<nbaskets-1;j++)  {
    Basket* basket = fBaskets[j];
    if (!basket) continue;
    if (fNBasketRAM < Rio_kMaxRAM) fBasketRAM[fNBasketRAM] = j;
    fNBasketRAM++;
    if (j == fReadBasket || j == fWriteBasket) continue;
    basket->dropBuffers();
    fBaskets[j] = 0;
    delete basket;
    fNBasketRAM--;
    fBasketRAM[fNBasketRAM] = -1;
    if (!fTree.memoryFull(0)) break;
  }
}
//////////////////////////////////////////////////////////////////////////////
char* Rio::Branch::address(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fAddress;
}

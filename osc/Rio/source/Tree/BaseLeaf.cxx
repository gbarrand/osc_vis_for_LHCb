// this :
#include <Rio/Tree/BaseLeaf.h>

// Core :
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Debug.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/Arguments.h>

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

RIO_META_SOURCE(Rio::BaseLeaf)

//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::BaseLeaf(
 const IDictionary& aDictionary
,Branch& aBranch
)
:Named(aDictionary,"","")
,fClass(aDictionary.opt_BaseLeaf_Class())
,fBranch(aBranch)
,fIndirectAddress(false)
,fNewValue(false)
,fNdata(0)
,fLength(0)
,fOffset(0)
,fIsRange(false)
,fIsUnsigned(false)
,fLeafCount(0)
//////////////////////////////////////////////////////////////////////////////
// Called when reading a branch. We add the leaf
// in the tree here.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::BaseLeaf");
  bool found = false;
  const std::vector<BaseLeaf*>& leaves = fBranch.tree().leaves();
  std::vector<BaseLeaf*>::const_iterator it;
  for(it=leaves.begin();it!=leaves.end();++it) {
    if((*it)==this) {
      found = true;
      break;
    }
  }
  if(!found) fBranch.tree().leaves().push_back(this);
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::BaseLeaf(
 const IDictionary& aDictionary
,Branch& aBranch
,const std::string& aName
)
:Named(aDictionary,aName,aName)
,fClass(aDictionary.opt_BaseLeaf_Class())
,fBranch(aBranch)
,fIndirectAddress(false)
,fNewValue(false)
,fNdata(0)
,fLength(0)
,fOffset(0)
,fIsRange(false)
,fIsUnsigned(false)
,fLeafCount(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::BaseLeaf");
  fLeafCount  = getLeafCounter(fLength);
  if (fLength == -1) {/*FIXME MakeZombie(); */return;}
  fLengthType = 4;
  std::string::size_type pos = 0;
  if( fLeafCount || 
      ((pos = aName.find("[")) != std::string::npos) ) {
    fName = aName.substr(0,pos);
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::BaseLeaf(
 const BaseLeaf& aFrom
)
:Named(aFrom)
,ILeaf()
,fClass(aFrom.fClass)
,fBranch(aFrom.fBranch)
,fIndirectAddress(false)
,fNewValue(false)
,fNdata(0)
,fLength(0)
,fOffset(0)
,fIsRange(0)
,fLeafCount(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment("Rio::BaseLeaf");
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf::~BaseLeaf(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::decrement("Rio::BaseLeaf");
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf& Rio::BaseLeaf::operator=(
 const BaseLeaf&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::BaseLeaf::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(BaseLeaf);
  else 
    return Named::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BaseLeaf::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    short v;
    unsigned int s,c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    //FIXME if (v > 1) {
    //TLeaf::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
    //FIXME } else {
    //====process old versions before automatic schema evolution
    if(!Named::stream(aBuffer)) return false;

    // Ok with v 1 & 2
    if(!aBuffer.read(fLength)) return false;
    if(!aBuffer.read(fLengthType)) return false;
    if(!aBuffer.read(fOffset)) return false;
    if(!aBuffer.read(fIsRange)) return false;
    if(!aBuffer.read(fIsUnsigned)) return false;

    Arguments args;
    Args_init(args);
    Args_insertDirectory(args,&fBranch.tree().directory());
    Args_insertTree(args,&fBranch.tree());
    Args_insertBranch(args,&fBranch);

    IObject* obj = 0;
    if(!aBuffer.readObject(args,obj)) {
      aBuffer.out() << "Rio::BaseLeaf::stream : can't read object." 
                    << std::endl;
      fLeafCount = 0;
      return false;
    }

    fLeafCount = Rio::cast<BaseLeaf>(*obj);

    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
    //====end of old versions
    //}
    if (fLength == 0) fLength = 1;
    fNewValue = false;
    if(!setAddress(0)) return false;
  } else {
    //FIXME TLeaf::Class()->WriteBuffer(b,this);
    //====process old versions before automatic schema evolution
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    //fBranch.tree().printer().println
    //("Rio::BaseLeaf::stream : debug : name \"%s\" title \"%s\" ",
    // fName.c_str(),fTitle.c_str());
    if(!Named::stream(aBuffer)) return false;
    if(!aBuffer.write(fLength)) return false;
    if(!aBuffer.write(fLengthType)) return false;
    if(!aBuffer.write(fOffset)) return false;
    if(!aBuffer.write(fIsRange)) return false;
    if(!aBuffer.write(fIsUnsigned)) return false;
    if(!aBuffer.writeObject(fLeafCount)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Rio::Branch& Rio::BaseLeaf::branch(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBranch;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BaseLeaf::isIndirectAddress(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIndirectAddress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setIndirectAddress(
 bool aIndirectAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fIndirectAddress = aIndirectAddress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setOffset(
 int aOffset
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fOffset = aOffset; 
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::offset(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fOffset; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::BaseLeaf::isRange(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fIsRange;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::setRange(
 bool aRange
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fIsRange = aRange;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::lengthType(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLengthType;
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::BaseLeaf::leafCount(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLeafCount;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::maximum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::minimum(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::BaseLeaf::length(
) const
//////////////////////////////////////////////////////////////////////////////
// Return the number of effective elements of this leaf.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int len;
  if (fLeafCount) {
    len = fLeafCount->number();
    if (len > fLeafCount->maximum()) {
      std::ostream& out = fBranch.tree().out();
      out << "Rio::BaseLeaf::length : ERROR leaf:" 
          << fName << ", len=" << len << " and max=" 
          << fLeafCount->maximum() << std::endl;
      len = fLeafCount->maximum();
    }
    return len * fLength;
  } else {
    return fLength;
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::BaseLeaf* Rio::BaseLeaf::getLeafCounter(
 int& aCountValue
) const
//////////////////////////////////////////////////////////////////////////////
// Return Pointer to counter of referenced Leaf :
//  If leaf name has the forme var[nelem], where nelem is alphanumeric, then
//     if nelem is a leaf name, return countval = 1 and the pointer to 
//     the leaf named nelem.
//  If leaf name has the forme var[nelem], where nelem is a digit, then
//     return countval = nelem and a null pointer.
//  Otherwise return countval=0 and a null pointer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aCountValue = 1;
  std::string::size_type pos1;
  if((pos1 = fTitle.find("[")) == std::string::npos) return 0;
  std::string::size_type pos2;
  if((pos2 = fTitle.find("]")) == std::string::npos) return 0;
  if(pos1>=pos2) return 0;
  std::string countname = fTitle.substr(pos1+1,pos2-pos1+1);
  
  // Now search a branch name with a leave name = countname
  // We search for the leaf in the list of leaves from the Tree. 

  const std::vector<BaseLeaf*>& leaves = fBranch.tree().leaves();

  BaseLeaf* leaf = 0;
  std::vector<BaseLeaf*>::const_iterator it;
  for(it=leaves.begin();it!=leaves.end();++it) {
    if((*it)->name()==countname) {
      leaf = *it;
      break;
    }
  }

  //int i;
  if (leaf) {
    aCountValue = 1;
    leaf->setRange();
    /*FIXME
    if (bleft2) {
      sscanf(bleft2,"[%d]",&i);
      aCountValue *= i;
    }
    bleft = bleft2;
    while(bleft) {
      bleft2++;
      bleft = (char*)strchr(bleft2,'[');
      if (!bleft) break;
      sscanf(bleft,"[%d]",&i);
      aCountValue *= i;
      bleft2 = bleft;
    }
    */
    return leaf;
  }
  // not found in a branch/leaf :
  int value;
  if(!smanip::toint(countname,value)) { // Is it a numerical value?
    aCountValue = -1;
    return 0;
  }
  aCountValue = value;
  /*
  if (bleft2) {
    sscanf(bleft2,"[%d]",&i);
    aCountValue *= i;
  }
  bleft = bleft2;
  while(bleft) {
    bleft2++;
    bleft = (char*)strchr(bleft2,'[');
    if (!bleft) break;
    sscanf(bleft,"[%d]",&i);
    aCountValue *= i;
    bleft2 = bleft;
  }
  */
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::BaseLeaf::updateNdata(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fLeafCount) fNdata = fLength * (fLeafCount->maximum() + 1);
  else            fNdata = fLength;
}

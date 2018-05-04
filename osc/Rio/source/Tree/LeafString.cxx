// this :
#include <Rio/Tree/LeafString.h>

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

#include <string.h>

RIO_META_SOURCE(Rio::LeafString)
//////////////////////////////////////////////////////////////////////////////
Rio::LeafString::LeafString(
 const IDictionary& aDictionary
,Branch& aBranch
)
:BaseLeaf(aDictionary,aBranch)
,fClass(aDictionary.opt_LeafString_Class())
,fMinimum(0)
,fMaximum(0)
,fValue(0)
//,fPointer(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafString::LeafString(
 const IDictionary& aDictionary
,Branch& aBranch
,const std::string& aName
)
:BaseLeaf(aDictionary,aBranch,aName)
,fClass(aDictionary.opt_LeafString_Class())
,fMinimum(0)
,fMaximum(0)
,fValue(0)
//,fPointer(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLengthType = 1;
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafString::~LeafString(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fNewValue) delete [] fValue;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::LeafString::number(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return (int) fValue[0];
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafString::valuePointer(
) const 
//////////////////////////////////////////////////////////////////////////////
// WARNING : the return is not a pointer to a std::string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fValue;
}
//////////////////////////////////////////////////////////////////////////////
std::string Rio::LeafString::value(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return std::string(fValue?fValue:"none");
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafString::fillBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //if(fPointer) fValue = *fPointer;
  int len = ::strlen(fValue);
  if (len >= fMaximum) fMaximum = len+1;
  if (len >= fLength) fLength = len+1;
  if (len < 255) {
    if(!aBuffer.write((unsigned char)len)) return false;
  } else {
    if(!aBuffer.write((unsigned char)255)) return false;
    if(!aBuffer.write(len)) return false;
  }
  if (len) if(!aBuffer.writeFastArray(fValue,len)) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafString::readBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned char lenchar;
  if(!aBuffer.read(lenchar)) {
    branch().out() << "Rio::LeafString::readBasket :" 
                   << " read(unsigned char) failed." 
                   << std::endl;
    return false;
  }
  int len = 0;
  if(lenchar < 255) {
    len = lenchar;
  } else {
    if(!aBuffer.read(len)) {
      branch().out() << "Rio::LeafString::readBasket :" 
                     << " read(int) failed." 
                     << std::endl;
      return false;
    }
  }
  if(len) {
    if (len >= fLength) len = fLength-1;
    // fValue should have been allocated through
    //  BaseLeaf::stream/setAddress(0)
    if(!aBuffer.readFastArray(fValue,len)) {
      branch().out() << "Rio::LeafString::readBasket :" 
                     << " readFastArray failed." 
                     << std::endl;
      return false;
    }
    fValue[len] = 0;
  } else {
    fValue[0] = 0;
  }
  return true;
}

void Rio::LeafString::printValue(std::ostream& aOut,int) const {
  aOut << value();
}

//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafString::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  updateNdata();
  if(fNewValue) {
    delete [] fValue;
    fValue = 0;
  }
  if (aAddress) {
    fNewValue = false;
  /*if (TestBit(kIndirectAddress)) {
      fPointer = (char**)aAddress;
      int ncountmax = fLength;
      if (fLeafCount) ncountmax = fLength * (fLeafCount->GetMaximum() + 1);
      if ((ncountmax > fNdata)|| (*fPointer == 0)) {
        if (*fPointer) delete [] *fPointer;
        if (ncountmax > fNdata) fNdata = ncountmax;
        *fPointer = new char[fNdata];
      }
      fValue = *fPointer;
    } else {*/
      fValue = (char*)aAddress;
    //}
  } else {
    fNewValue = true;
    fValue = new char[fNdata];
    fValue[0] = 0;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafString::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==&fClass) 
    return Rio_SCast(LeafString);
  else 
    return BaseLeaf::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafString::stream(
 IBuffer& aBuffer
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(aBuffer.isReading()) {
    short v;
    unsigned int s,c;
    if(!aBuffer.readVersion(v,s,c)) return false;
    if(!BaseLeaf::stream(aBuffer)) return false;
    if(!aBuffer.read(fMinimum)) return false;
    if(!aBuffer.read(fMaximum)) return false;
    if(!aBuffer.checkByteCount(s,c,fClass.inStoreName())) return false;
  } else {
    unsigned int c;
    if(!aBuffer.writeVersion(fClass.version(),c)) return false;
    if(!BaseLeaf::stream(aBuffer)) return false;
    if(!aBuffer.write(fMinimum)) return false;
    if(!aBuffer.write(fMaximum)) return false;
    if(!aBuffer.setByteCount(c,true)) return false;
  }
  return true;
}
/* FIXME : should be nice to be able to do that.
           It would be permit to template the :
             BatchLab/Rio/Rio_Tuple.cxx/RioColumn_string.
//////////////////////////////////////////////////////////////////////////////
bool Rio::LeafString::setAddress(
 std::string* aString
)
//////////////////////////////////////////////////////////////////////////////
// For BatchLab/Rio/Rio_Tuple.cxx/RioColumn_string.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!setAddress((void*)aString->c_str())) return false;
  fFrom_std_string = true;
  return true;
}
*/

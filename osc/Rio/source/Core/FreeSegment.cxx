//
// Each file has a linked list of free segments. Each free segment
// is described by its firts and last address.
// When an object is written to a file via TObject::Write, a new Key (see TKey)
// is created. The first free segment big enough to accomodate the object
// is used.
// If the object size has a length corresponding to the size of the free 
// segment,the free segment is deleted from the list of free segments.
// When an object is deleted from a file, a new TFree object is generated.
// If the deleted object is contiguous to an already deleted object, the free
// segments are merged in one single segment.
//


// this :
#include <Rio/Core/FreeSegment.h>

#include "tobuf.h"

#define Rio_FreeSegment_Class_Version 1

//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::FreeSegment(std::ostream& aOut)
:fOut(aOut)
,fFirst(0)
,fLast(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::FreeSegment(
 std::ostream& aOut
,Seek aFirst
,Seek aLast
)
:fOut(aOut)
,fFirst(aFirst)
,fLast(aLast)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::FreeSegment(
 const FreeSegment& aFrom
)
:fOut(aFrom.fOut)
,fFirst(aFrom.fFirst)
,fLast(aFrom.fLast)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment::~FreeSegment(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment& Rio::FreeSegment::operator=(
 const FreeSegment& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //fOut
  fFirst = aFrom.fFirst;
  fLast = aFrom.fLast;
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::FreeSegment::first(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFirst;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::FreeSegment::last(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fLast;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::setFirst(
 Seek aFirst
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fFirst = aFirst;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::FreeSegment::setLast(
 Seek aLast
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLast = aLast;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::FreeSegment::recordSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_LARGE_FILE
  //GB if(fLast>RIO_START_BIG_FILE) {
  if((fFirst>RIO_START_BIG_FILE)|| //GB
     (fLast>RIO_START_BIG_FILE) ){
    return sizeof(short) +  2 * sizeof(Seek);
  } else {
    return sizeof(short) +  2 * sizeof(Seek32);
  }
#else
  return sizeof(short) +  2 * sizeof(Seek);
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::FreeSegment::fillBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode FREE structure into output buffer :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version = Rio_FreeSegment_Class_Version;
#ifdef RIO_LARGE_FILE
  //GB if(fLast>RIO_START_BIG_FILE) version += 1000;
  if((fFirst>RIO_START_BIG_FILE)||
     (fLast>RIO_START_BIG_FILE)) version += 1000;
#endif
  if(!tobuf(fOut,aEOB,aBuffer,version)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000) {
    if(!tobuf(fOut,aEOB,aBuffer,fFirst)) return false;
    if(!tobuf(fOut,aEOB,aBuffer,fLast)) return false;
  } else {
    if(fFirst>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::FreeSegment::fillBuffer :"
           << " attempt to write big Seek "
           << fFirst << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,aEOB,aBuffer,(Seek32)fFirst)) return false;
    if(fLast>RIO_START_BIG_FILE) { //GB
      fOut << "Rio::FreeSegment::fillBuffer :"
           << " attempt to write big Seek "
           << fLast << " on 32 bits."
           << std::endl;
      return false;
    }
    if(!tobuf(fOut,aEOB,aBuffer,(Seek32)fLast)) return false;
  }
#else
  if(!tobuf(fOut,aEOB,aBuffer,fFirst)) return false;
  if(!tobuf(fOut,aEOB,aBuffer,fLast)) return false;
#endif
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::FreeSegment::readBuffer(
 const char* aEOB
,char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode one FREE structure from input buffer :
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version;
  if(!frombuf(fOut,aEOB,aBuffer, &version)) return false;
#ifdef RIO_LARGE_FILE
  if(version>1000) {
    if(!frombuf(fOut,aEOB,aBuffer,&fFirst)) return false;
    if(!frombuf(fOut,aEOB,aBuffer,&fLast)) return false;
  } else {
   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer,&i)) return false;
    fFirst = i;}
   {Seek32 i;
    if(!frombuf(fOut,aEOB,aBuffer,&i)) return false;
    fLast = i;}
  }
#else
  if(!frombuf(fOut,aEOB,aBuffer,&fFirst)) return false;
  if(!frombuf(fOut,aEOB,aBuffer,&fLast)) return false;
#endif
  return true;
}


/*
TFree::TFree()
{
}

TFree::TFree(TList *lfree, Seek_t first, Seek_t last)
{
  fFirst = first;
  fLast  = last;
  lfree->Add(this);
}

TFree *TFree::AddFree(TList *lfree, Seek_t first, Seek_t last)
{
// Add a new free segment to the list of free segments :
//  If last just preceedes an existing free segment, then first becomes
//     the new starting location of the free segment.
//  if first just follows an existing free segment, then last becomes
//     the new ending location of the free segment.
//  if first just follows an existing free segment AND last just preceedes
//     an existing free segment, these two segments are merged into
//     one single segment.
//
  TFree *idcur = this;
  while (idcur) {
    Seek_t curfirst = idcur->GetFirst();
    Seek_t curlast  = idcur->GetLast();
    if (curlast == first-1) {
      idcur->SetLast(last);
      TFree *idnext = (TFree*)lfree->After(idcur);
      if (idnext == 0) return idcur;
      if (idnext->GetFirst() > last+1) return idcur;
      idcur->SetLast( idnext->GetLast() );
      lfree->Remove(idnext);
      return idcur;
    }
    if (curfirst == last+1) {
      idcur->SetFirst(first);
      return idcur;
    }
    if (first < curfirst) {
      TFree * newfree = new TFree();
      newfree->SetFirst(first);
      newfree->SetLast(last);
      lfree->AddBefore(idcur, newfree);
      return newfree;
    }
    idcur = (TFree*)lfree->After(idcur);
  }
  return 0;
}

*/

#ifndef Rio_FreeSegment_h
#define Rio_FreeSegment_h

//////////////////////////////////////////////////////////////////////////
// Description of free segments on a file.                              //
//////////////////////////////////////////////////////////////////////////

#include <ostream>

#include <Rio/Interfaces/Seek.h>

namespace Rio {

class FreeSegment {
public:
  FreeSegment(std::ostream&);
  FreeSegment(std::ostream&,Seek,Seek);
  FreeSegment(const FreeSegment&);
  virtual ~FreeSegment();
  FreeSegment& operator=(const FreeSegment&);
  // Get/set :
  Seek first() const;
  Seek last() const;
  void setFirst(Seek);
  void setLast(Seek);
  // Else :
  virtual int recordSize() const;
  virtual bool fillBuffer(const char*,char*&);
  virtual bool readBuffer(const char*,char*&);
private:
  /*
  FreeSegment* AddFree(TList *lfree,Seek first,Seek last);
  virtual void FillBuffer(char *&buffer);
  FreeSegment* GetBestFree(TList *lfree, Int_t nbytes);
  virtual void     ReadBuffer(char *&buffer);
  */
private:
  std::ostream& fOut;
  Seek fFirst;            //First free word of segment
  Seek fLast;             //Last free word of segment
};

}

#endif

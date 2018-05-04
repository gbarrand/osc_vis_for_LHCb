#ifndef Rio_Map_h
#define Rio_Map_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// This class stores a (key,value) pair using an external hash.         //
// The (key,value) are Long_t's and therefore can contain object        //
// pointers or any longs. The map uses an open addressing hashing       //
// method (linear probing).                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <ostream>

namespace Rio {

class Assoc;

class Map {
public:
   Map(std::ostream&,int = 100);
   ~Map();
private:
   Map(const Map&);
   Map& operator=(const Map&);
public:
   bool add(unsigned long,long,long);
   bool add(long,long);
   long findValue(unsigned long,long);
   long findValue(long);
   void remove(unsigned long,long);
   void remove(long);
   int size() const;
   /*FIXME
   void delete(Option_t *opt = "");
   Int_t     Capacity() const { return fSize; }

   Long_t   &operator()(ULong_t hash, Long_t key);
   Long_t   &operator()(Long_t key) { return operator()(key, key); }
   */

   void dumpValues() const;
private:
   bool expand(int);
   int findElement(unsigned long,long);
   bool highWaterMark();
   void fixCollisions(int);
private:
   std::ostream& fOut;
   Assoc** fTable;
   int fSize;
   int fTally;
};

}

#endif





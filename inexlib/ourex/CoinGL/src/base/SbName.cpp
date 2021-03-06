/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SbName SbName.h Inventor/SbName.h
  \brief The SbName class stores strings by reference.
  \ingroup base

  The class is used by Coin for storing keywords, names and other
  strings. They are stored in a manner where identical strings are
  guaranteed to map to the same memory address (as returned by the
  SbName::getString() method).

  The main advantage of storing identical strings to the same memory
  address is that it simplifies comparison operations, and
  particularly when working with string data as keys in other data
  structures, like e.g. in hash (dictionary) tables.

  Apart from that, mapping identical strings to the same memory
  address can also save on memory resources, and provide run-time
  optimizations. String comparisons for SbName objects are very
  efficient, for instance.


  There is an aspect of using SbName instances that it is important to
  be aware of: since strings are stored \e permanently, using SbName
  instances in code where there is continually changing strings or the
  continual addition of new unique strings will in the end swamp
  memory resources. So where possible, use SbString instances instead.

  \sa SbString
*/

#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


// **** The private SbNameEntry class **************************************

static const int CHUNK_SIZE = 65536-32; // leave some room for other data
static const int NAME_TABLE_SIZE = 1999;

struct SbNameChunk {
  char mem[CHUNK_SIZE];
  char * curByte;
  int bytesLeft;
  SbNameChunk * next;
};

class SbNameEntry {
public:
  SbBool isEmpty(void) const
    { return (! *str); }
  SbBool isEqual(const char * const string) const
    { return (strcmp(str, string) == 0); }

  static void print_info();

  static void initClass(void);
  SbNameEntry(const char * const s, const unsigned long h,
               SbNameEntry * const n)
    { str = s; hashValue = h; next = n; };
  static const SbNameEntry * insert(const char * const s);

  static const char * findStringAddress(const char * s);

  const char * str;

  static void cleanup(void);

private:
  static int nameTableSize;
  static SbNameEntry ** nameTable;
  static struct SbNameChunk * chunk;
  unsigned long hashValue;
  SbNameEntry * next;

  static void * mutex;
};

int SbNameEntry::nameTableSize;
SbNameEntry * * SbNameEntry::nameTable;
SbNameChunk * SbNameEntry::chunk;
void * SbNameEntry::mutex = NULL;

void 
SbNameEntry::cleanup(void)
{
  SbNameChunk * thechunk = SbNameEntry::chunk;
  while (thechunk) {
    SbNameChunk * next = thechunk->next;
    delete thechunk;
    thechunk = next;
  }
  
  for (int i = 0; i < SbNameEntry::nameTableSize; i++) {
    SbNameEntry * entry = SbNameEntry::nameTable[i];
    while (entry) {
      SbNameEntry * next = entry->next;
      delete entry;
      entry = next;
    }
  }
  delete[] SbNameEntry::nameTable;
  CC_MUTEX_DESTRUCT(SbNameEntry::mutex);
}


// This static method initializes static data for the SbNameEntry
// class.
void
SbNameEntry::initClass(void)
{
  SbNameEntry::nameTableSize = NAME_TABLE_SIZE;
  SbNameEntry::nameTable = new SbNameEntry * [ SbNameEntry::nameTableSize ];
  for (int i = 0; i < SbNameEntry::nameTableSize; i++) { SbNameEntry::nameTable[i] = NULL; }
  SbNameEntry::chunk = NULL;

  coin_atexit((coin_atexit_f*) SbNameEntry::cleanup, CC_ATEXIT_SBNAME);
}

void
SbNameEntry::print_info(void)
{
  for (int i = 0; i < SbNameEntry::nameTableSize; i++) {
    SbNameEntry * entry = SbNameEntry::nameTable[ i ];
    int cnt = 0;
    while (entry != NULL) {
      entry = entry->next;
      cnt++;
    }
    printf("name entry: %d, cnt: %d\n", i, cnt);
  }
}

const char *
SbNameEntry::findStringAddress(const char * s)
{
  // FIXME: the stringlen should be stored in the chunk before the
  // actual string -- that could be an important optimization.
  // 20030606 mortene.
  int len = strlen(s) + 1;

    // names > CHUNK_SIZE characters are truncated.
  if (len >= CHUNK_SIZE) { len = CHUNK_SIZE; }

  if (chunk == NULL || chunk->bytesLeft < len) {
    SbNameChunk * newChunk = new SbNameChunk;

    newChunk->curByte = newChunk->mem;
    newChunk->bytesLeft = CHUNK_SIZE;
    newChunk->next = chunk;

    chunk = newChunk;
  }

  (void)strncpy(chunk->curByte, s, len);
  s = chunk->curByte;

  chunk->curByte += len;
  chunk->bytesLeft -= len;

  return s;
}

const SbNameEntry *
SbNameEntry::insert(const char * const str)
{
  if (SbNameEntry::mutex == NULL) {
    CC_MUTEX_CONSTRUCT(SbNameEntry::mutex);
  }
  CC_MUTEX_LOCK(SbNameEntry::mutex);
  if (nameTableSize == 0) { initClass(); }

  unsigned long h = SbString::hash(str);
  unsigned long i = h % nameTableSize;
  SbNameEntry * entry = nameTable[i];
  SbNameEntry * head = entry;

  while (entry != NULL) {
    if (entry->hashValue == h && entry->isEqual( str) )
      break;
    entry = entry->next;
  }

  if (entry == NULL) {
    entry = new SbNameEntry(findStringAddress(str), h, head );
    nameTable[i] = entry;
  }
  CC_MUTEX_UNLOCK(SbNameEntry::mutex);
  return entry;
}


// *************************************************************************

/*!
  This is the default constructor.
*/
SbName::SbName(void)
{
  this->entry = SbNameEntry::insert("");
}

/*!
  Constructor. Adds the \a nameString string to the name table.
*/
SbName::SbName(const char * nameString)
{
  assert(nameString);
  this->entry = SbNameEntry::insert(nameString);
}

/*!
  Constructor. Adds \a str to the name table.
*/
SbName::SbName(const SbString & str)
{
  this->entry = SbNameEntry::insert(str.getString());
}

/*!
  Copy constructor.
*/
SbName::SbName(const SbName & name)
  : entry(name.entry)
{
}

/*!
  The destructor.
*/
SbName::~SbName()
{
  // No unreferences of memory resources happens here, because strings
  // will be stored permanently for the remaining life of the process.
  //
  // This is how the string mapping feature of SbName is *supposed* to
  // work. The strings should be stored _permanently_, so the return
  // value from SbName::getString() will be valid even after all its
  // SbName-instances have been destructed.
}

/*!
  This method returns pointer to character array for the name.

  The returned memory pointer for the character string is guaranteed
  to be valid for the remaining life time of the process, even after
  all SbName instances referencing the string has been destructed.
*/
const char *
SbName::getString(void) const
{
  return this->entry->str;
}

/*!
  This method returns the number of characters in the name.
*/
int
SbName::getLength(void) const
{
  // FIXME: shouldn't we cache this value for subsequent faster
  // execution? 20010909 mortene.
  //
  // UPDATE 20030606 mortene: this can easily be done by storing an
  // extra value in the memory chunk right before the string itself.
  return (int)strlen(this->entry->str);
}

/*!
  This method checks if the \a c character is a valid identifier start
  character for a name.

  \sa SbBool SbName::isIdentChar(const char c)

*/
SbBool
SbName::isIdentStartChar(const char c)
{
  // There is an important reason why the cast below is necessary:
  // isdigit() et al takes an "int" as input argument. A _signed_ char
  // value for any character above the 127-value ASCII character set
  // will be promoted to a negative integer, which can cause the
  // function to make an array reference that's out of bounds.
  //
  // FIXME: this needs to be fixed other places isdigit() is used,
  // aswell as for other is*() function. 20021124 mortene.
  const unsigned char uc = (unsigned char)c;

  if (isdigit(uc)) return FALSE;
  return SbName::isIdentChar(c);
}

/*!
  This method checks if the \a c character is a valid character for a
  name.

  \sa SbBool SbName::isIdentStartChar(const char c)
*/
SbBool
SbName::isIdentChar(const char c)
{
  // FIXME: isalnum() takes the current locale into account. This can
  // lead to "interesting" artifacts. We very likely need to audit and
  // fix our isalnum() calls in the Coin sourcecode to behave in the
  // exact manner that we expect them to. 20020319 mortene.
  return (isalnum(c) || c == '_');
}

/*!
  Returns \c TRUE if the given character is valid for use as the first
  character of a name for an object derived from a class inheriting
  SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  Legal characters for the first character of an SoBase object name is
  underscore ("_") and any uppercase and lowercase alphabetic
  character from the ASCII character set (i.e. A-Z and a-z).

  This method is not part of the original Open Inventor API.

  \sa isBaseNameChar()
*/
SbBool
SbName::isBaseNameStartChar(const char c)
{
  // FIXME: it seems silly to have this function here, instead of in
  // SoBase. 20040611 mortene.

  // FIXME: isalpha() takes the current locale into account. This can
  // lead to "interesting" artifacts. We very likely need to audit and
  // fix our isalpha() calls in the Coin sourcecode to behave in the
  // exact manner that we expect them to. 20020319 mortene.
  if (c == '_' || (coin_isascii(c) && isalpha(c))) return TRUE;
  return FALSE;
}

/*!
  Returns \c TRUE if the given character is valid for use in naming
  object instances of classes derived from SoBase.

  SoBase derived objects needs to be named in a manner which will not
  clash with the special characters reserved as tokens in the syntax
  rules of Open Inventor and VRML files.

  Legal characters to use for an SoBase object name is any character
  from the ASCII character set from and including character 33 (hex
  0x21) to and including 126 (hex 0x7e), \e except single and double
  apostrophes, the plus sign and punctuation, backslash and the curly
  braces.

  This method is not part of the original Open Inventor API.

  \sa isBaseNameStartChar()
*/
SbBool
SbName::isBaseNameChar(const char c)
{
  // FIXME: it seems silly to have this function here, instead of in
  // SoBase. 20040611 mortene.

  static const char invalid[] = "\"\'+.\\{}";
  if (c <= 0x20 || c >= 0x7f || strchr(invalid, c)) return FALSE;
  return TRUE;
}

/*!
  This unary operator results in \c FALSE if the SbName object is
  non-empty and \c TRUE if the SbName object is empty.  An empty name
  contains a null-length string.
*/

int
SbName::operator ! (void) const
{
  return this->entry->isEmpty();
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const SbName & lhs, const char *rhs)
{
  return lhs.entry->isEqual(rhs);
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const char *lhs, const SbName & rhs)
{
  return rhs.entry->isEqual(lhs);
}

/*!
  This operator checks for equality and returns TRUE if so, and FALSE
  otherwise.
*/

int
operator == (const SbName & lhs, const SbName & rhs)
{
  return (lhs.entry == rhs.entry);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const SbName & lhs, const char *rhs)
{
  return ! lhs.entry->isEqual(rhs);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const char *lhs, const SbName & rhs)
{
  return ! rhs.entry->isEqual(lhs);
}

/*!
  This operator checks for inequality and returns TRUE if so, and FALSE
  if the names are equal.
*/

int
operator != (const SbName & lhs, const SbName & rhs)
{
  return lhs.entry != rhs.entry;
}

/*!
  This operator returns a pointer to the character array for the name string.
  It is intended for implicit use.  Use SbName::getString() explicitly instead
  of this operator - it might be removed later.

  \sa const char * SbName::getString(void)
*/

SbName::operator const char * (void) const
{
  return this->entry->str;
}

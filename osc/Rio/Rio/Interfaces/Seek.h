#ifndef Rio_Seek_h
#define Rio_Seek_h

//////////////////////////////////////////////////////////////////////////
// Definition of a file pointer .                                       //
//////////////////////////////////////////////////////////////////////////

#include <Rio/Interfaces/typedefs.h>

//
#define RIO_LARGE_FILE

//code in ifdef RIO_LARGE_FILE has the logic taken from ROOT/v4-00-01

namespace Rio {

#define RIO_START_BIG_FILE 2000000000

#ifdef RIO_LARGE_FILE
typedef int64 Seek;
typedef int Seek32;
#else
typedef int Seek;
#endif

}

#endif

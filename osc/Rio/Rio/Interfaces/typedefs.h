#ifndef Rio_typedefs_h
#define Rio_typedefs_h

// Similar to AIDA/v3r3p0/Types.h

namespace Rio {

#if defined(WIN32) && !defined(GNU_GCC)

// WIN32 and NOT GNU_GCC
typedef __int64 int64;
#define RIO_INT64_FORMAT "%ld"

typedef unsigned __int64 uint64;
#define RIO_UINT64_FORMAT "%lu"

#elif defined(_LP64) 

// 64 Bit Platforms
typedef long int64;
#define RIO_INT64_FORMAT "%ld"

typedef unsigned long uint64;
#define RIO_UINT64_FORMAT "%lu"

#define RIO_INT64_AS_LONG

#elif defined(__alpha) 

// 64 Bit Platforms
typedef long int64;
#define RIO_INT64_FORMAT "%ld"

typedef unsigned long uint64;
#define RIO_UINT64_FORMAT "%lu"

#define RIO_INT64_AS_LONG

#else 

// 32-Bit Platforms
typedef long long int64;
#define RIO_INT64_FORMAT "%lld"

typedef unsigned long long uint64;
#define RIO_UINT64_FORMAT "%llu"

#endif

//WARNING : heavily used in Tree, then have a simple type.
typedef void* Arguments[3];

}

#endif

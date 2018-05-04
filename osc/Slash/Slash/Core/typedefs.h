#ifndef Slash_Core_typedefs_h
#define Slash_Core_typedefs_h 1

// Similar to AIDA/v3r3p0/Types.h

namespace Slash {

#if defined(WIN32) && !defined(GNU_GCC)

// WIN32 and NOT GNU_GCC
typedef int int32;
typedef __int64 int64;
#define SLASH_INT32_FORMAT "%d"
#define SLASH_INT64_FORMAT "%ld"

typedef unsigned int uint32;
typedef unsigned __int64 uint64;
#define SLASH_UINT32_FORMAT "%u"
#define SLASH_UINT64_FORMAT "%lu"

#elif defined(_LP64) 

// 64 Bit Platforms
typedef int int32;
typedef long int64;
#define SLASH_INT32_FORMAT "%d"
#define SLASH_INT64_FORMAT "%ld"

typedef unsigned int uint32;
typedef unsigned long uint64;
#define SLASH_UINT32_FORMAT "%u"
#define SLASH_UINT64_FORMAT "%lu"

#define SLASH_INT64_AS_LONG

#else 

// 32-Bit Platforms
typedef int int32;
typedef long long int64;
#define SLASH_INT32_FORMAT "%ld"
#define SLASH_INT64_FORMAT "%lld"

typedef unsigned int uint32;
typedef unsigned long long uint64;
#define SLASH_UINT32_FORMAT "%lu"
#define SLASH_UINT64_FORMAT "%llu"

#endif

} //Slash

#endif

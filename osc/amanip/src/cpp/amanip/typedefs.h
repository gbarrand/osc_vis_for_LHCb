#ifndef amanip_typedefs_h
#define amanip_typedefs_h

// Similar to AIDA/v3r3p0/Types.h

namespace amanip {

#if defined(WIN32) && !defined(GNU_GCC)

// WIN32 and NOT GNU_GCC
typedef int int32;
typedef __int64 int64;
#define AMANIP_INT32_FORMAT "%d"
#define AMANIP_INT64_FORMAT "%ld"

typedef unsigned int uint32;
typedef unsigned __int64 uint64;
#define AMANIP_UINT32_FORMAT "%u"
#define AMANIP_UINT64_FORMAT "%lu"

#elif defined(_LP64) 

// 64 Bit Platforms
typedef int int32;
typedef long int64;
#define AMANIP_INT32_FORMAT "%d"
#define AMANIP_INT64_FORMAT "%ld"

typedef unsigned int uint32;
typedef unsigned long uint64;
#define AMANIP_UINT32_FORMAT "%u"
#define AMANIP_UINT64_FORMAT "%lu"

#define AMANIP_INT64_AS_LONG

#elif defined(__alpha) 

// 64 Bit Platforms
typedef int int32;
typedef long int64;
#define AMANIP_INT32_FORMAT "%d"
#define AMANIP_INT64_FORMAT "%ld"

typedef unsigned int uint32;
typedef unsigned long uint64;
#define AMANIP_UINT32_FORMAT "%u"
#define AMANIP_UINT64_FORMAT "%lu"

#define AMANIP_INT64_AS_LONG

#else 

// 32-Bit Platforms
typedef int int32;
typedef long long int64;
#define AMANIP_INT32_FORMAT "%ld"
#define AMANIP_INT64_FORMAT "%lld"

typedef unsigned int uint32;
typedef unsigned long long uint64;
#define AMANIP_UINT32_FORMAT "%lu"
#define AMANIP_UINT64_FORMAT "%llu"

#endif

}

#endif

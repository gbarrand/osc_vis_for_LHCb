// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <cstdio>

#include <inlib/typedefs>
#include <inlib/snpf>

#ifndef WIN32
#include <unistd.h>
#endif

int main() {
  ::printf("bool %d\n",(int)sizeof(bool));
  ::printf("char %d\n",(int)sizeof(char));
  ::printf("short %d\n",(int)sizeof(short));
  ::printf("int %d\n",(int)sizeof(int));
  ::printf("long %d\n",(int)sizeof(long));
  ::printf("unsigned char %d\n",(int)sizeof(unsigned char));
  ::printf("unsigned short %d\n",(int)sizeof(unsigned short));
  ::printf("unsigned int %d\n",(int)sizeof(unsigned int));
  ::printf("unsigned long %d\n",(int)sizeof(unsigned long));
  ::printf("------------\n");
  ::printf("bool %d\n",(int)sizeof(bool));
  ::printf("size_t %d\n",(int)sizeof(size_t));
#ifndef WIN32
  ::printf("off_t %d\n",(int)sizeof(off_t));
#endif
  ::printf("void* %d\n",(int)sizeof(void*));
  ::printf("long long %d\n",(int)sizeof(long long));
  ::printf("unsigned long long %d\n",(int)sizeof(unsigned long long));

  ::printf("inlib::uint32 %d\n",(int)sizeof(inlib::uint32));
  ::printf("inlib::uint64 %d\n",(int)sizeof(inlib::uint64));

  ::printf("------------\n");
  // max numbers :
 {unsigned char n = 0;
  for(unsigned int i=0;i<8;i++)  n += 1<<i;
  ::printf("max unsigned char : %u\n",n);}
 {unsigned int n = 0;
  for(unsigned int i=0;i<32;i++)  n += 1<<i;
  ::printf("max unsigned int : %u\n",n);}

 {inlib::uint32 n = inlib::uint32_mx();
  char s[32];
  inlib::snpf(s,sizeof(s),"max uint32 : %s\n",inlib::uint32_format());
  ::printf(s,n);}

 {inlib::uint64 n = inlib::uint64_mx();
  char s[32];
  inlib::snpf(s,sizeof(s),"max uint64 : %s\n",inlib::uint64_format());
  ::printf(s,n);}

#if defined(__GLIBC__)
  ::printf("__GLIBC__ : %d\n",__GLIBC__);
#endif
#if defined(__GLIBC_MINOR__)
  ::printf("__GLIBC_MINOR__ : %d\n",__GLIBC_MINOR__);
#endif

  return 0;
}



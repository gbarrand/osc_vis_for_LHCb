// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.
#include <inlib/platform>

#include <cstdlib>
#include <cstdio>

int main() {

  bool is_LE = inlib::is_little_endian();

  if(is_LE) 
    ::printf("This computer is LE (Little Endian).\n");
  else      
    ::printf("This computer is BE (Big Endian).\n");

  ::printf("\n");

  int i = 1;
  char* b = (char*)&i;
  ::printf("b[0] = %d\n",b[0]);
  ::printf("b[1] = %d\n",b[1]);
  ::printf("b[2] = %d\n",b[2]);
  ::printf("b[3] = %d\n",b[3]);

  // BE = Big Endian, LE = Little Endian.
  // The Intels x86 are LE.

  ::printf("\n");
  ::printf("Mac PPC b[3] is 1 (BE)\n");
  ::printf("Mac Intel b[0] is 1 (LE)\n");
  ::printf("Linux i386 b[0] is 1 (LE)\n");
  ::printf("Linux x86_64 b[0] is 1 (LE)\n");

  return EXIT_SUCCESS;
}


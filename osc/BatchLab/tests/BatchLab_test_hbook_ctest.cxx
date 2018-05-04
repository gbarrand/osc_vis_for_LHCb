
#include <BatchLab/Zebra/CHBOOK.h>

#define PAWC_SIZE 2000000
#ifdef WIN32

#define pawc PAWC
#define quest QUEST
#define hcbits HCBITS
#define hcbook HCBOOK
#define rzcl RZCL
extern "C" int pawc[PAWC_SIZE];
extern "C" int quest[100];
extern "C" int hcbits[37];
extern "C" int hcbook[51];
extern "C" int rzcl[11];

#else

#define pawc pawc_
#define quest quest_
#define hcbits hcbits_
#define hcbook hcbook_
#define rzcl rzcl_
int pawc[PAWC_SIZE];
int quest[100];
int hcbits[37];
int hcbook[51];
int rzcl[11];

#endif

#include <stdio.h>

#ifdef WIN32
#define setpawc SETPAWC
#else
#define setpawc setpawc_
#endif

extern "C" int setpawc();

int main () {

  printf("debug : main : begin\n");
  CHLIMIT(setpawc());
  printf("debug : main : end\n");

  int ier = CHROPEN(1,"xxx","pawdemo.hbook"," ",0);
  printf("debug : hropen : ier %d\n",ier);

  return 0;
}

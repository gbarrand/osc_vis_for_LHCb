#ifndef BatchLab_HBOOK_h
#define BatchLab_HBOOK_h

#ifdef WIN32
//#define hlimit HLIMIT
#define hrndm1 HRNDM1
#define hfill HFILL
#else
//#define hlimit hlimit_
#define hrndm1 hrndm1_
#define hfill hfill_
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
  float __stdcall hrndm1(int*);
  void __stdcall hfill(int*,float*,float*,float*);
#else
  float hrndm1(int*);
  void hfill(int*,float*,float*,float*);
#endif

#ifdef __cplusplus
}
#endif

#endif

#ifndef Rio_tobuf_h
#define Rio_tobuf_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A set of inline byte handling routines.                              //
//                                                                      //
// The set of Rio_tobuf() and frombuf() routines take care of           //
// packing a basic type value into a buffer in network byte order       //
// (i.e. they perform byte swapping when needed). The buffer does not   //
// have to start on a machine (long) word boundary.                     //
//                                                                      //
// For __GNUC__ on linux on i486 processors and up                      //
// use the `bswap' opcode provided by the GNU C Library.                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <Rio/Core/Platform.h>

#ifndef RIO_BYTESWAP
#include <string.h> //For memcpy.
#endif

#include <string>
#include <cstring> //For strlen.
#include <cstdio> //sprintf

#include <ostream>

#define RIO_TO_CHECK_EOB(n,cmt) \
  if((buf+n)>eob) {\
    a_out << "Rio_tobuf : " << cmt << " : "\
          << " try to access out of buffer " << long_tos(n) << " bytes "\
          << " (buf=" << char_p_tos(buf) \
          << ", eob=" << char_p_tos(eob) << ")." << std::endl;\
    return false;}

#define RIO_FROM_CHECK_EOB(n,v,cmt) \
  if((buf+n)>eob) {\
    *x = v;\
    a_out << "Rio_frombuf : " << cmt << " : "\
         << " try to access out of buffer " << long_tos(n) << " bytes"\
         << " (buf=" << char_p_tos(buf) \
         << ", eob=" << char_p_tos(eob) << ")." << std::endl;\
    return false;}

namespace Rio {

inline std::string char_p_tos(const char* aValue) {
  char s[32];
  ::sprintf(s,"%lu",(unsigned long)aValue);
  return std::string(s);
}

inline std::string long_tos(const long aValue) {
  char s[32];
  ::sprintf(s,"%ld",aValue);
  return std::string(s);
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, unsigned char x) {
  RIO_TO_CHECK_EOB(1,"unsigned char")
  *buf++ = x;
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, unsigned short x) {
  RIO_TO_CHECK_EOB(sizeof(unsigned short),"unsigned short")
#ifdef RIO_BYTESWAP
  char *sw = (char *)&x;
  buf[0] = sw[1];
  buf[1] = sw[0];
#else
  ::memcpy(buf, &x, sizeof(unsigned short));
#endif
  buf += sizeof(unsigned short);
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, unsigned int x) {
  RIO_TO_CHECK_EOB(sizeof(unsigned int),"unsigned int")
#ifdef RIO_BYTESWAP
  char *sw = (char *)&x;
  buf[0] = sw[3];
  buf[1] = sw[2];
  buf[2] = sw[1];
  buf[3] = sw[0];
#else
  ::memcpy(buf, &x, sizeof(unsigned int));
#endif
  buf += sizeof(unsigned int);
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, uint64 x){
  RIO_TO_CHECK_EOB(8,"uint64")
#ifdef RIO_BYTESWAP
  char *sw = (char *)&x;
  buf[0] = sw[7];
  buf[1] = sw[6];
  buf[2] = sw[5];
  buf[3] = sw[4];
  buf[4] = sw[3];
  buf[5] = sw[2];
  buf[6] = sw[1];
  buf[7] = sw[0];
#else
  ::memcpy(buf, &x, 8);
#endif
  buf += 8;
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, float x) {
  RIO_TO_CHECK_EOB(sizeof(float),"float")
#ifdef RIO_BYTESWAP
  char *sw = (char *)&x;
  buf[0] = sw[3];
  buf[1] = sw[2];
  buf[2] = sw[1];
  buf[3] = sw[0];
#else
  ::memcpy(buf, &x, sizeof(float));
#endif
  buf += sizeof(float);
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, double x) {
  RIO_TO_CHECK_EOB(sizeof(double),"double")
#ifdef RIO_BYTESWAP
  char *sw = (char *)&x;
  buf[0] = sw[7];
  buf[1] = sw[6];
  buf[2] = sw[5];
  buf[3] = sw[4];
  buf[4] = sw[3];
  buf[5] = sw[2];
  buf[6] = sw[1];
  buf[7] = sw[0];
#else
  ::memcpy(buf, &x, sizeof(double));
#endif
  buf += sizeof(double);
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, unsigned char* x) {
  RIO_FROM_CHECK_EOB(1,0,"unsigned char")
  *x = *buf++;
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, unsigned short* x) {
  RIO_FROM_CHECK_EOB(sizeof(unsigned short),0,"unsigned short")
#ifdef RIO_BYTESWAP
  char *sw = (char *)x;
  sw[0] = buf[1];
  sw[1] = buf[0];
#else
  ::memcpy(x, buf, sizeof(unsigned short));
#endif
  buf += sizeof(unsigned short);
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, unsigned int* x) {
  RIO_FROM_CHECK_EOB(sizeof(unsigned int),0,"unsigned int")
#ifdef RIO_BYTESWAP
  char *sw = (char *)x;
  sw[0] = buf[3];
  sw[1] = buf[2];
  sw[2] = buf[1];
  sw[3] = buf[0];
#else
  ::memcpy(x, buf, sizeof(unsigned int));
#endif
  buf += sizeof(unsigned int);
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, uint64 *x){
  RIO_FROM_CHECK_EOB(8,0,"uint64")
#ifdef RIO_BYTESWAP
  char *sw = (char *)x;
  sw[0] = buf[7];
  sw[1] = buf[6];
  sw[2] = buf[5];
  sw[3] = buf[4];
  sw[4] = buf[3];
  sw[5] = buf[2];
  sw[6] = buf[1];
  sw[7] = buf[0];
#else
  ::memcpy(x, buf, 8);
#endif
  buf += 8;
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, float* x) {
  RIO_FROM_CHECK_EOB(sizeof(float),0,"float")
#ifdef RIO_BYTESWAP
  char *sw = (char *)x;
  sw[0] = buf[3];
  sw[1] = buf[2];
  sw[2] = buf[1];
  sw[3] = buf[0];
#else
  ::memcpy(x, buf, sizeof(float));
#endif
  buf += sizeof(float);
  return true;
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, double* x) {
  RIO_FROM_CHECK_EOB(sizeof(double),0,"double")
#ifdef RIO_BYTESWAP
  char *sw = (char *)x;
  sw[0] = buf[7];
  sw[1] = buf[6];
  sw[2] = buf[5];
  sw[3] = buf[4];
  sw[4] = buf[3];
  sw[5] = buf[2];
  sw[6] = buf[1];
  sw[7] = buf[0];
#else
  ::memcpy(x, buf, sizeof(double));
#endif
  buf += sizeof(double);
  return true;
}

inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, char x)  { 
  return tobuf(a_out,eob,buf,(unsigned char) x); 
}
inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, short x) { 
  return tobuf(a_out,eob,buf,(unsigned short) x); 
}
inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, int x)   { 
  return tobuf(a_out,eob,buf,(unsigned int) x); 
}
inline bool tobuf(std::ostream& a_out,const char* eob,char *&buf, int64 x)   { 
  return tobuf(a_out,eob,buf,(uint64) x); 
}

inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, char* x)  { 
  return frombuf(a_out,eob,buf,(unsigned char*) x); 
}
inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, short* x) { 
  return frombuf(a_out,eob,buf,(unsigned short*) x); 
}
inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, int* x) { 
  return frombuf(a_out,eob,buf,(unsigned int*) x); 
}
inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf, int64* x) { 
  return frombuf(a_out,eob,buf,(uint64*) x); 
}

inline bool tobuf(std::ostream& a_out,const char* eob,char*& buf, const std::string& x) {
  unsigned char nwh;
  int nchars = x.size();
  if (nchars > 254) {
    nwh = 255;
    if(!tobuf(a_out,eob,buf,nwh)) return false;
    if(!tobuf(a_out,eob,buf,nchars)) return false;
  } else {
    nwh = (unsigned char)nchars;
    if(!tobuf(a_out,eob,buf,nwh)) return false;
  }
  RIO_TO_CHECK_EOB(nchars,"std::string")
  for (int i = 0; i < nchars; i++) buf[i] = x[i];
  buf += nchars;
  return true;
}
inline bool tobuf(std::ostream& a_out,const char* eob,char*& buf, const char* x) {
  unsigned char nwh;
  int nchars = ::strlen(x);
  if (nchars > 254) {
    nwh = 255;
    if(!tobuf(a_out,eob,buf,nwh)) return false;
    if(!tobuf(a_out,eob,buf,nchars)) return false;
  } else {
    nwh = (unsigned char)nchars;
    if(!tobuf(a_out,eob,buf,nwh)) return false;
  }
  RIO_TO_CHECK_EOB(nchars,"char*")
  for (int i = 0; i < nchars; i++) buf[i] = x[i];
  buf += nchars;
  return true;
}
inline bool frombuf(std::ostream& a_out,const char* eob,char *&buf,std::string& x) {
  unsigned char nwh;
  int nchars;
  if(!frombuf(a_out,eob,buf,&nwh)) return false;
  if (nwh == 255) {
    if(!frombuf(a_out,eob,buf,&nchars)) return false;
  } else {
    nchars = nwh;
  }
  if(nchars<0) {
    a_out << "frombuf(string) :"
          << " negative char number " << nchars << "." << std::endl;
    return false;
  }
  RIO_TO_CHECK_EOB(nchars,"from : std::string")
  x.resize(nchars);  
  ::memcpy((char*)x.c_str(),buf,nchars);
  buf += nchars;
  return true;
}
inline int Rio_std_string_recordSize(const std::string& x) {
  // Returns size string will occupy on I/O buffer.
  if (x.size() > 254)
    return x.size()+sizeof(unsigned char)+sizeof(int);
  else
    return x.size()+sizeof(unsigned char);
}

}

#undef RIO_TO_CHECK_EOB
#undef RIO_FROM_CHECK_EOB

#endif

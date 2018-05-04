
#include <string>
#include <string.h>
#include <stdio.h>

class A {
public:
  virtual void* cast(const std::string&) = 0;
//  virtual void* cast(const char*) = 0;
  virtual void* castID(int) = 0;
public:
  virtual ~A(){};
private:
  int fi;
};

#define ID_B 222

class B : public A {
public:
  virtual void* cast(const std::string& aTo) {
    //if(aTo=="0123456789*") return (B*)this;
    if(!strcmp(aTo.c_str(),"0123456789*")) return (B*)this;
    //if(aTo=="01234567890123456789*") return (B*)this;
    else return 0;
  }
/*
  virtual void* cast(const char* aTo) {
    if(!strcmp(aTo,"0123456789*")) return (B*)this;
    //if(aTo=="01234567890123456789*") return (B*)this;
    else return 0;
  }
*/
  virtual void* castID(int aID) {
    if(aID==ID_B) return (B*)this;
    else return 0;
  }
public:
  B():m_count(0){}
  virtual ~B(){};
  void dummy() {
    m_count++;
  }
private:
  int fi;
public:
  unsigned int m_count;
};

static double getTime();

#include <stdio.h>

/*
  With 100000000 
  On a Pentium III 600 Mhz (Dell Inspiron 5000) ; RedHat-6.1, g++ 2.95.2;
   - castID : 3.09017 sec
   - dynamic_cast : 19.8742
   - cast(string) compare 20 chars : 105.639
   - cast(string) compare 10 chars : 86.3942

  Then "class id" technique is faster...
 */

class IA {
public:
  virtual ~IA(){};
public:
  virtual void xxx() = 0;
};

class IB : public virtual IA {
public:
  virtual ~IB(){};
public:
  virtual void yyy() = 0;
};

class XB : public virtual IB {
public:
  virtual void xxx() {}
  virtual void yyy() {}
public:
  virtual ~XB(){};
};

class XBB : public IB {
public:
  virtual void xxx() {}
  virtual void yyy() {}
  unsigned char fCode;
public:
  virtual ~XBB(){};
};

static void checkCode(IB* aIB){
  XBB* xbb = (XBB*)aIB;
  printf("debug : code :%d\n",xbb->fCode);
}

int main(int a_argc,char** a_args) {
  // Tests :
  XB xb;
  IB* ib = (IB*)&xb;
  IA* ia = (IA*)ib;
  // Does not compile : IB* ibb = (IB*)ia;
  ia = 0;
  // Does not compile : XB* yb = (XB*)ib;
  XBB* ybb = (XBB*)ib; //Ok.
  ybb = 0;

  XBB xbb;
  xbb.fCode = 255;
  checkCode(&xbb);  

  // Timing :
  B* b = new B;
  A* a = (A*)b;
  int number = 100000000;
  if (a_argc>=2 && std::string(a_args[1])=="-string") {
    double begin = getTime();
    std::string sclass("0123456789*");
    for(int count=0;count<number;count++) {
      B* a_to_b = (B*)a->cast(sclass);
      a_to_b->dummy();
      //a->cast("01234567890123456789*");
    }
    double end = getTime();
    printf("debug : B::m_count %u\n",b->m_count);
    printf("debug : cast(string) :%g\n",end - begin);
  } else if (a_argc>=2 && std::string(a_args[1])=="-id") {
    double begin = getTime();
    for(int count=0;count<number;count++) {
      B* a_to_b = (B*)a->castID(ID_B);
      a_to_b->dummy();
    }
    double end = getTime();
    printf("debug : B::m_count %u\n",b->m_count);
    printf("debug : castID(int) :%g\n",end - begin);
  } else {
    double begin = getTime();
    for(int count=0;count<number;count++) {
      B* a_to_b = dynamic_cast<B*>(a);
      a_to_b->dummy();
    }
    double end = getTime();
    printf("debug : B::m_count %u\n",b->m_count);
    printf("debug : dynamic_cast :%g\n",end - begin);
  }

  return 0;
}
#ifdef WIN32
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <sys/types.h> // For time_t.
double getTime(){
#ifdef WIN32
  struct timeb tb;
  ftime(&tb);
  return tb.time + tb.millitm * 0.001;
#else
  struct timeval tval;
  gettimeofday(&tval,NULL);
  return tval.tv_sec + tval.tv_usec * 0.000001;
#endif
}

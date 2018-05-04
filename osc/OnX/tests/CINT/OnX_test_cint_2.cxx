
#include <stdio.h>

#include <string>

// Class to wrap :
class A {
public:
  A():fI(314){
    printf("debug : A::A : %lu\n",(unsigned long)this);
  }
  ~A(){
    printf("debug : A::~A : %lu\n",(unsigned long)this);
  }
  void dump(){
    printf("debug : A::dump : %lu : fI : %d\n",(unsigned long)this,fI);
  }
public:
  // For CINT wrapping of private data fields.
  // ROOT, for the IO, instruments (Rtypes.h/ClassDef) with the 
  // TXxx::ShowMembers object method exploited by a pure ROOT 
  // mechanism (not a CINT one).
  //static 
  long offset(const std::string& aField){
    A* p =(A*)0x1000;
    if(aField=="fI_private") {
      return ((long)(&(p->fI_private))-(long)(p));
    } else {
      return 0L;
    }
  }
public: 
  int fI;
  short fS;
  long fL;
  float fF;
  double fD;
  unsigned int fUI;
  unsigned short fUS;
  unsigned long fUL;
private:
  int fI_private;
};


#include <G__ci.h>

// CINT wrapping of class A :
#include <common.h>

// G__ci.h
//typedef struct {
//  const char *tagname;
//  char tagtype;
//  short tagnum;
//} G__linked_taginfo;

G__linked_taginfo Wrap_A = { "A" , 99 , -1 }; //FIXME : 99, -1 ?

// Data Member information setup
static void memvar_A(void) {

  //newlink.c ;
  //int G__memvar_setup
  //  void *p
  //  int type
  //  int reftype
  //  int constvar,tagnum,typenum,statictype,accessin
  //  char *expr
  //  int definemacro
  //  char *comment

  G__tag_memvar_setup(G__get_linked_tagnum(&Wrap_A));
  //A* p =(A*)0; //crashes
  A* p =(A*)0x1000;
  if (p) { 
    G__memvar_setup((void*)((long)(&p->fI)-(long)(p)),(int)G__TMPLT_INTARG,0,0,-1,-1,-1,G__PUBLIC,"fI=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fS)-(long)(p)),(int)G__TMPLT_SHORTARG,0,0,-1,-1,-1,G__PUBLIC,"fS=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fL)-(long)(p)),(int)G__TMPLT_LONGARG,0,0,-1,-1,-1,G__PUBLIC,"fL=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fF)-(long)(p)),(int)G__TMPLT_FLOATARG,0,0,-1,-1,-1,G__PUBLIC,"fF=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fD)-(long)(p)),(int)G__TMPLT_DOUBLEARG,0,0,-1,-1,-1,G__PUBLIC,"fD=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fUI)-(long)(p)),(int)G__TMPLT_UINTARG,0,0,-1,-1,-1,G__PUBLIC,"fUI=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fUS)-(long)(p)),(int)G__TMPLT_USHORTARG,0,0,-1,-1,-1,G__PUBLIC,"fUS=",0,(char*)NULL);
    G__memvar_setup((void*)((long)(&p->fUL)-(long)(p)),(int)G__TMPLT_ULONGARG,0,0,-1,-1,-1,G__PUBLIC,"fUL=",0,(char*)NULL);

    //cint wrapping (cint -w0) does not wrap private data members.
    //rootcint wraps with the below AND ALSO by producing
    // TXxx::ShowMembers (Rtypes.h/ClassDef) used by a 
    // pure ROOT logic (not CINT one).
    //G__memvar_setup((void*)NULL,(int)G__TMPLT_INTARG,0,0,-1,-1,-1,G__PRIVATE,"fI_private=",0,(char*)NULL);
    //  
    //long off = A::offset("fI_private");
    // The below works !!!
    long off = p->offset("fI_private");
    G__memvar_setup((void*)off,(int)G__TMPLT_INTARG,0,0,-1,-1,-1,G__PRIVATE,"fI_private=",0,(char*)NULL);
  } 
  G__tag_memvar_reset();
}

// Member function Interface Method

static int wrap_A_cstor(
 G__value *result
,G__CONST char* funcname
,struct G__param* libp
,int hash
) 
{
  printf("debug : wrap_A_cstor\n");
  A* p = NULL;
  if(G__getaryconstruct()) {
    p = new A[G__getaryconstruct()];
  } else {
    p = new A;
  }
  result->obj.i = (long)p;
  result->ref = (long)p;
  result->type = G__TMPLT_CLASSARG; //'u'
  result->tagnum = G__get_linked_tagnum(&Wrap_A);
  return(1 || funcname || hash || result || libp) ;
}

static int wrap_A_cp_cstor(
 G__value* result
,G__CONST char* funcname
,struct G__param* libp
,int hash
)
{
  printf("debug : wrap_A_cp_cstor\n");
  A* p = new A(*(A*)G__int(libp->para[0]));
  result->obj.i = (long)p;
  result->ref = (long)p;
  result->type = G__TMPLT_CLASSARG; //'u'
  result->tagnum = G__get_linked_tagnum(&Wrap_A);
  return(1 || funcname || hash || result || libp) ;
}

typedef A T_A;
static int wrap_A_dstor(
 G__value* result
,G__CONST char* funcname
,struct G__param* libp
,int hash
) 
{
  printf("debug : wrap_A_dstor\n");
  if(G__getaryconstruct()) {
    if(G__PVOID==G__getgvp()) {
      delete[] (A*)(G__getstructoffset());
    } else {
      for(int i=G__getaryconstruct()-1;i>=0;i--) {
        ((A*)((G__getstructoffset())+sizeof(A)*i))->~T_A();
      }
    }
  } else {
    delete (A*)(G__getstructoffset());
  }
  G__setnull(result);
  return(1 || funcname || hash || result || libp) ;
}

static int wrap_A_dump(
 G__value* result
,G__CONST char* funcname
,struct G__param* libp
,int hash
) {
  G__setnull(result);
  ((A*)(G__getstructoffset()))->dump();
  return(1 || funcname || hash || result || libp) ;
}

static void memfunc_A(void) {

  printf("debug : memfunc_A\n");

  //newlink.c : 
  //int G__memfunc_setup
  //  char *funcname
  //  int hash
  //  int (*funcp)()
  //  int type,tagnum
  //  int typenum,reftype,para_nu,ansi,accessin,isconst;
  //  char *paras
  //  char *comment
  //  void* truep2f
  //  int isvirtual

  G__tag_memfunc_setup(G__get_linked_tagnum(&Wrap_A));

  int i;
  int hash;

  G__hash("A",hash,i);
  G__memfunc_setup("A",hash,wrap_A_cstor,(int)('i'),G__get_linked_tagnum(&Wrap_A),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);

  // automatic copy constructor
  G__hash("A",hash,i);
  G__memfunc_setup("A",hash,wrap_A_cp_cstor,(int)('i'),G__get_linked_tagnum(&Wrap_A),-1,0,1,1,1,0,"u 'A' - 1 - -",(char*)NULL,(void*)NULL,0);

  // automatic destructor
  G__hash("~A",hash,i);
  G__memfunc_setup("~A",hash,wrap_A_dstor,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);

  G__hash("dump",hash,i);
  G__memfunc_setup("dump",hash,wrap_A_dump,(int)'y',-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);

  G__tag_memfunc_reset();
}


extern "C" void MyWrap(void) {
  G__check_setup_version(30051472,"MyWrap()");

  Wrap_A.tagnum = -1 ;

  /* Setting up class,struct,union tag entry */
  G__tagtable_setup(G__get_linked_tagnum(&Wrap_A),sizeof(A),-1,1024,(char*)NULL,memvar_A,memfunc_A);

  //G__cpp_setup_inheritanceOnXCintWrap();
  //G__cpp_setup_typetableOnXCintWrap();
  //G__cpp_setup_memvarOnXCintWrap();
  //G__cpp_setup_memfuncOnXCintWrap();
  //G__cpp_setup_globalOnXCintWrap();
  //G__cpp_setup_funcOnXCintWrap();

  if(0==G__getsizep2memfunc()) {
    class SZ {
    public:
      SZ() { p = &SZ::sz;}
      size_t sz() { return sizeof(p);}
    private:
      size_t (SZ::*p)();
    };
    SZ a;
    //printf("debug : sz %d\n",(int)a.sz());
    G__setsizep2memfunc((int)a.sz());
  }
  return;
}

#include <Api.h> //To query CINT about a class (G__ClassInfo,etc..)

int main() {
   printf("debug : main : begin\n");

   G__add_setup_func("MyWrap",(G__incsetup)(&MyWrap));

   G__init_cint("cint");

   //G__exec_tempfile("a.C");
   printf("debug : main : exec : begin\n");
   G__exec_text("\
{printf(\"debug : G__exec_text : hello\\n\");\n\
 A a;\n\
 a.dump();\n\
 a.fI = 315;\n\
 a.dump();\n\
 A* a2 = new A;\n\
 a2->fI = 316;\n\
 a2->dump();\n\
 delete a2;}\n\
");
   printf("debug : main : exec : end\n");
   //FIXME : a is not deleted at end of the upper 
   //  but at the below G__scratch_all.

  // See ROOT/TCint.cxx for an unfolding of a class (base classes, etc..)
  {G__ClassInfo ci("A");
   printf("debug : class  A : %d %d\n",
     ci.NMethods(),ci.NDataMembers());
   G__MethodInfo mi(ci);
   while (mi.Next()) {
      printf("debug :   method %s\n",mi.Name());
   }
   G__DataMemberInfo dmi(ci);
   while (dmi.Next()) {
      printf("debug :   data member %s offset %ld\n",dmi.Name(),dmi.Offset());
   }}

   G__remove_setup_func("MyWrap");

   printf("debug : main : scracth_all\n");

   G__scratch_all();

   printf("debug : main : end\n");

   return 0;
}

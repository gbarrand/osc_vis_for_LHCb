
#include <Lib/Parser.h>

#define Lib_CastHead(aObject,aClass) (aClass*)(aObject).cast_head()

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef Lib::IParser::Length L_t;

class H_Result : public virtual Lib::IResult {
public:
  H_Result():m_bool(false){}
  virtual ~H_Result(){}
  virtual void* cast_head() const { 
    return (void*)static_cast<const H_Result*>(this);
  }
public:
  bool m_bool;
};

class H_Parser : public Lib::BaseParser {
public:
  virtual ~H_Parser(){}
public:
  virtual Lib::IResult* create_result() const { return new H_Result;}
  virtual bool primary(const char* b,L_t l,
                       Lib::IResult& a_res) const {
    if(l==0) return false;
    H_Result* res = Lib_CastHead(a_res,H_Result);
    if(!res) return false;

    if(!tobool(b,l,res->m_bool)) return false;

    return true;
  }
  virtual bool unary(const char* o,L_t lo,
                     const Lib::IResult& r,
                     Lib::IResult& a_res) const {
    H_Result* sr = Lib_CastHead(r,H_Result);
    if(!sr) return false;
    H_Result* res = Lib_CastHead(a_res,H_Result);
    if(!res) return false;

    if(eq(o,lo,"not",3)||eq(o,lo,"!",1)) {
      res->m_bool = (sr->m_bool?false:true);
    } else {
      return false;
    }

    return true;
  }
  virtual bool binary(const Lib::IResult& l,
                      const char* o,L_t lo,
                      const Lib::IResult& r,
                      Lib::IResult& a_res) const {
    H_Result* sl = Lib_CastHead(l,H_Result);
    if(!sl) return false;
    H_Result* sr = Lib_CastHead(r,H_Result);
    if(!sr) return false;
    H_Result* res = Lib_CastHead(a_res,H_Result);
    if(!res) return false;

    if(eq(o,lo,"and",3)||eq(o,lo,"&&",2)) {
      res->m_bool = sl->m_bool && sr->m_bool;
    } else if(eq(o,lo,"or",2)||eq(o,lo,"||",2)) {
      res->m_bool = sl->m_bool || sr->m_bool;
    } else {
      return false;
    }

    return true;
  }
private:
  static bool eq(const char* b1,L_t l1,
                 const char* b2,L_t l2) {
    if(l1!=l2) return false;
    for(L_t i=0;i<l1;i++,b1++,b2++) {
      if(*b1!=*b2) return false;
    }    
    return true;
  }
  static bool tobool(const char* b,L_t l,bool& v) {
    if(eq(b,l,"true",4) ){
      v = true;
      return true;
    } else if(eq(b,l,"false",5)) {
      v = false;
      return true;
    } else {
      v = false;
      return false;
    }
  }
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <Lib/fmanip.h>

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ::printf("Lib_test_parse_h : 0\n");

  if(argc<=1) {
    ::fprintf(stderr,"Give a file name.\n");
    return EXIT_FAILURE;
  }

  long l;
  char* b = Lib::fmanip::readBytes(argv[1],l);
  if(!b) {
    ::fprintf(stderr,"Cant't read \"%s\".\n",argv[1]);
    return EXIT_FAILURE;
  }

  char* p_b = b;
  char* p_beg = b;
  for(Lib::IParser::Length pos=0;pos<l;pos++,p_b++) {
    if((*p_b)==';') {
      unsigned int lstmt = p_b-p_beg+1;
      std::string stmt(lstmt,' ');
      ::memcpy((void*)stmt.c_str(),p_beg,lstmt);      
      //::printf("debug : stmt \"%s\"\n",stmt.c_str());

      p_beg = p_b+1;
    }
  }


/*
  H_Result res;
  H_Parser parser;
  if(!parser.evaluate(b,l,res)) {
    ::fprintf(stderr,"parse error.\n");
    delete [] b;
    return EXIT_FAILURE;
  }
*/

  delete [] b;

  return EXIT_SUCCESS;
}

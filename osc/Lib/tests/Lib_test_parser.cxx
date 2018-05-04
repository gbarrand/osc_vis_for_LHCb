
#include <Lib/Parser.h>

#define Lib_CastHead(aObject,aClass) (aClass*)(aObject).cast_head()

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

typedef Lib::IParser::Length L_t;

class BoolResult : public virtual Lib::IResult {
public:
  BoolResult():m_bool(false){}
  virtual ~BoolResult(){}
  virtual void* cast_head() const { 
    return (void*)static_cast<const BoolResult*>(this);
  }
public:
  bool m_bool;
};

class BoolParser : public Lib::BaseParser {
public:
  virtual ~BoolParser(){}
public:
  virtual Lib::IResult* create_result() const { return new BoolResult;}
  virtual bool primary(const char* b,L_t l,
                       Lib::IResult& a_res) const {
    if(l==0) return false;
    BoolResult* res = Lib_CastHead(a_res,BoolResult);
    if(!res) return false;

    if(!tobool(b,l,res->m_bool)) return false;

    return true;
  }
  virtual bool unary(const char* o,L_t lo,
                     const Lib::IResult& r,
                     Lib::IResult& a_res) const {
    BoolResult* sr = Lib_CastHead(r,BoolResult);
    if(!sr) return false;
    BoolResult* res = Lib_CastHead(a_res,BoolResult);
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
    BoolResult* sl = Lib_CastHead(l,BoolResult);
    if(!sl) return false;
    BoolResult* sr = Lib_CastHead(r,BoolResult);
    if(!sr) return false;
    BoolResult* res = Lib_CastHead(a_res,BoolResult);
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
#include <string>
#include <string.h> //memcpy

class StringResult : public virtual Lib::IResult {
public:
  virtual ~StringResult(){}
  virtual void* cast_head() const { 
    return (void*)static_cast<const StringResult*>(this);
  }
public:
  std::string m_string;
};

#define ADD(a_l,a_b) \
   {std::string s(a_l,' ');\
    ::memcpy((char*)s.c_str(),a_b,a_l);\
    res->m_string += s;}

class StringParser : public Lib::BaseParser {
public:
  virtual ~StringParser(){}
public:
  virtual Lib::IResult* create_result() const { return new StringResult;}
  virtual bool primary(const char* b,L_t l,
                       Lib::IResult& a_res) const {
    if(l==0) return false;
    StringResult* res = Lib_CastHead(a_res,StringResult);
    if(!res) return false;

   {std::string s(l,' ');
    ::memcpy((char*)s.c_str(),b,l);
    res->m_string += s;}

    return true;
  }
  virtual bool unary(const char* o,L_t lo,
                     const Lib::IResult& r,
                     Lib::IResult& a_res) const {
    StringResult* sr = Lib_CastHead(r,StringResult);
    if(!sr) return false;
    StringResult* res = Lib_CastHead(a_res,StringResult);
    if(!res) return false;

    res->m_string += "{";
    ADD(lo,o)
    res->m_string += "}";
    res->m_string += "[";
    res->m_string += sr->m_string;
    res->m_string += "]";
    return true;
  }
  virtual bool binary(const Lib::IResult& l,
                      const char* o,L_t lo,
                      const Lib::IResult& r,
                      Lib::IResult& a_res) const {
    StringResult* sl = Lib_CastHead(l,StringResult);
    if(!sl) return false;
    StringResult* sr = Lib_CastHead(r,StringResult);
    if(!sr) return false;
    StringResult* res = Lib_CastHead(a_res,StringResult);
    if(!res) return false;

    res->m_string += "[";
    res->m_string += sl->m_string;
    res->m_string += "]";
    ADD(lo,o)
    res->m_string += "[";
    res->m_string += sr->m_string;
    res->m_string += "]";
    return true;
  }
};


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define B_CHECK_ERROR(aString) \
 {std::string s(aString);\
  BoolResult res;\
  BoolParser parser;\
  if(parser.evaluate(s.c_str(),s.size(),res)) {\
    ::printf("should stop with error in \"%s\"\n",s.c_str());\
  }}

#define B_CHECK_OK(aString,aRes) \
 {std::string s(aString);\
  BoolResult res;\
  BoolParser parser;\
  if(!parser.evaluate(s.c_str(),s.size(),res)) {\
    ::printf("error in \"%s\"\n",s.c_str());\
  } else if(res.m_bool!=aRes) {\
    ::printf("for \"%s\", unexpected result \"%d\"\n",s.c_str(),res.m_bool);\
  }}
  
#define S_CHECK_ERROR(aString) \
 {std::string s(aString);\
  StringResult res;\
  StringParser parser;\
  if(parser.evaluate(s.c_str(),s.size(),res)) {\
    ::printf("should stop with error in \"%s\"\n",s.c_str());\
  }}
  

#define S_CHECK_OK(aString,aRes) \
 {std::string s(aString);\
  StringResult res;\
  StringParser parser;\
  if(!parser.evaluate(s.c_str(),s.size(),res)) {\
    ::printf("error in \"%s\"\n",s.c_str());\
  } else if(res.m_string!=aRes) {\
    ::printf("for \"%s\", unexpected result \"%s\"\n",\
        s.c_str(),res.m_string.c_str());\
  }}
  

//////////////////////////////////////////////////////////////////////////////
int main(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ::printf("Lib_test_parser : 29\n");

  for(unsigned int loop=0;loop<1;loop++) {
  //for(unsigned int loop=0;loop<10000;loop++) {

  S_CHECK_ERROR("xxx(yyy")
  S_CHECK_ERROR("xxx)yyy)")
  S_CHECK_ERROR("()")
  S_CHECK_ERROR("(xxx)(yyy)")
  S_CHECK_ERROR("(xxx)(yyy(")
  S_CHECK_ERROR("(xxx)yyy(")
  S_CHECK_ERROR("(xxx)y")
  S_CHECK_ERROR(" (xxx)y")
    
  S_CHECK_OK("xxx","xxx")
  S_CHECK_OK("(xxx)","xxx")
  S_CHECK_OK("((xxx))","xxx")
  S_CHECK_OK("((xxx)and(yyyy))or(zzz)","[[xxx]and[yyyy]]or[zzz]")
  S_CHECK_OK("((xxx)and(yyy))or((zzz)or(ttt))","[[xxx]and[yyy]]or[[zzz]or[ttt]]")

  S_CHECK_OK("xxx(yyy)","{xxx}[yyy]")
  S_CHECK_OK("((xxx)and(uuu(yyy)))or(zzz)","[[xxx]and[{uuu}[yyy]]]or[zzz]")
  
  //////////////////////////////////////////
  //////////////////////////////////////////
  //////////////////////////////////////////

  B_CHECK_ERROR("(truexx)")
  B_CHECK_ERROR("n(true)")

  B_CHECK_OK("(true)",true)
  B_CHECK_OK("(true)and(false)",false)
  B_CHECK_OK("((true)and(false))or(true)",true)
  B_CHECK_OK("!((true)&&(false))||(false)",true)

  } //end loop

  return EXIT_SUCCESS;
}

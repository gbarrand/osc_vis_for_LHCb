#ifndef Lib_Columns_no_yacc_h
#define Lib_Columns_no_yacc_h 

#include <inlib/columns>

#include <inlib/words>
#include <list>
#include <Lib/Value.h>

namespace Lib {
class Columns : public inlib::columns::parser {
public:
  Columns(const std::string& aString,std::ostream& a_out,
          int aVerboseLevel = 0) 
  :fVerboseLevel(aVerboseLevel)
  ,fString(aString)
  ,f_out(a_out)
  //,fSuccess(false)
  ,fCurrentType(Slash::Core::IValue::NONE)
  {}
  virtual ~Columns() {clear();}
private:
  Columns(const Columns& a_from):f_out(a_from.f_out){}
  Columns& operator=(const Columns&){return *this;}
public:
  //void setScript(const std::string& aString) { fString = aString;}
  bool findVariables() {
    clear();
    if(fString.empty()) return false; //keep old version logic.
    if(!parse(fString)) return false;    
    //dump(f_out);
    //analyse m_top :
    if(!analyse(m_top,fStack)) {clear();return false;}
    return true;
  }

  std::list<Lib::Value> result() const {
    std::list<Lib::Value> vars;
    copyColumns(fStack,vars);
    return vars;
  }
  void clear() {
    m_top.clear();
    deleteColumns(fStack);
    fCurrentType = Slash::Core::IValue::NONE;
  }
  const std::string& script() const { return fString;}
  std::string script() { return fString;}
  //bool isSuccess() const { return fSuccess;}
  std::ostream& out() const {return f_out;}

public:
  inline static void deleteColumns(std::list<Lib::Value>& aVars) {
    std::list<Lib::Value>::iterator it;
    for(it=aVars.begin();it!=aVars.end();++it) {
      if((*it).type()==Slash::Core::IValue::VOID_STAR) {
        std::list<Lib::Value>* vars = 
          (std::list<Lib::Value>*)(*it).get_void_star();
        deleteColumns(*vars);
        delete vars;
        //(*it).set((void*)0);
      }
    }
    aVars.clear();
  }

  inline static void copyColumns(const std::list<Lib::Value>& aFrom,
                                 std::list<Lib::Value>& aTo) {
    std::list<Lib::Value>::const_iterator it;
    for(it=aFrom.begin();it!=aFrom.end();++it) {
      if((*it).type()==Slash::Core::IValue::VOID_STAR) {
        std::list<Lib::Value>* vars = new std::list<Lib::Value>();
        Lib::Value v((void*)vars);
        v.setLabel((*it).label());
        aTo.push_back(v);
        std::list<Lib::Value>* p = 
          (std::list<Lib::Value>*)(*it).get_void_star(); 
        copyColumns(*p,*vars);
      } else {
        aTo.push_back(*it);
      }
    }
  }

  static void dumpColumns(const std::list<Lib::Value>& aVars,
                          std::ostream& a_out,
                          const std::string& a_margin = "") {
    std::list<Lib::Value>::const_iterator it;
    for(it=aVars.begin();it!=aVars.end();++it) {
      if((*it).type()==Lib::Value::VOID_STAR) {
        a_out << a_margin
              << " ---- ITuple : " << (*it).label() << " : begin "
              << std::endl;
        std::list<Lib::Value>* vars = 
          (std::list<Lib::Value>*)(*it).get_void_star();
        dumpColumns(*vars,a_out,a_margin+" ");
        a_out << a_margin
              << " ---- ITuple : " << (*it).label() << " : end "
              << std::endl;
      } else {
        //(*it).print(a_out);
        a_out << a_margin
              << (*it).stype() << " : " 
              << (*it).label() << " : " 
              << Lib::Value::toString(*it)
              << std::endl;
      }
    }
  }
private:
  bool analyse(inlib::columns::tree& a_tree,std::list<Lib::Value>& a_stack) {
    if(a_tree.m_dcl.empty()) { //top
      //std::cout << "debug : dcl empty" << std::endl;
      std::vector<inlib::columns::tree>::iterator it;
      for(it=a_tree.m_sub.begin();it!=a_tree.m_sub.end();++it) {
        if(!analyse(*it,a_stack)) return false;
      }
    } else {
      //std::cout << "debug : dcl not empty" << std::endl;
      Lib::Value* v = analyse_dcl(a_tree.m_dcl);
      if(!v) return false;
      //std::cout << "debug : dcl label " << v->label() << std::endl;
      if(a_tree.m_sub.size()) {
        if(v->type()!=Slash::Core::IValue::VOID_STAR) {
          f_out << "Lib::Columns::analyse :"
                << " Expect a VOID_STAR."
                << std::endl;
          delete v;
          return false;
        }
        fCurrentType = Slash::Core::IValue::NONE;
        std::list<Lib::Value>* stk = new std::list<Lib::Value>();
        std::vector<inlib::columns::tree>::iterator it;
        for(it=a_tree.m_sub.begin();it!=a_tree.m_sub.end();++it) {
          if(!analyse(*it,*stk)) {
            delete v;
            return false;
          }
        }
        v->set((void*)stk);
      } else {
        fCurrentType = v->type();
      }
      a_stack.push_back(*v);
      delete v;
    }
    return true;
  }
  Lib::Value* analyse_dcl(const std::string& a_s) {
    std::vector<std::string> words;
    inlib::words(a_s,"=",false,words);
    if(words.size()==2) { //<type> <name>=<value>
      std::vector<std::string> swords;
      inlib::words(words[0]," ",false,swords);
      if(swords.size()==2) {

        inlib::strip(swords[0]);
        inlib::strip(swords[1]);

        if(swords[0]=="ITuple") {
  
          //create a Slash::Core::IValue::VOID_STAR :
          Lib::Value* v = new Lib::Value((void*)0);
          v->setLabel(swords[1]);
          return v;
  
        } else {
  
          Lib::Value* v = new_Value(swords[0]);
          if(!v) {
            f_out << "Lib::Columns::analyse_dcl :"
                  << " syntax error in " << inlib::sout(a_s) << "."
                  << " Unknown type " << inlib::sout(swords[0]) << "."
                  << std::endl;
            return 0;
          }
          v->setLabel(swords[1]);
    
          inlib::strip(words[1]);
          if(!set_Value(*v,words[1])) {
            f_out << "Lib::Columns::analyse_dcl :"
                  << " syntax error in " << inlib::sout(a_s) << "."
                  << " Bad value " << inlib::sout(words[1]) << "."
                  << std::endl;
            delete v;
            return 0;
          }
          return v;

        }

      } else if(swords.size()==1) {
        if(fCurrentType==Slash::Core::IValue::NONE) {
          f_out << "Lib::Columns::analyse_dcl :"
                << " (1) current type is NONE."
                << std::endl;
          return 0;
        }

        Lib::Value* v = new Lib::Value();
        v->setType(fCurrentType);       
        v->setLabel(swords[0]);
    
        inlib::strip(words[1]);
        if(!set_Value(*v,words[1])) {
          f_out << "Lib::Columns::analyse_dcl :"
                << " syntax error in " << inlib::sout(a_s) << "."
                << " Bad value " << inlib::sout(words[1]) << "."
                << std::endl;
          delete v;
          return 0;
        }
        return v;
      }

    } else if(words.size()==1) {
      //<type> <name>
      //<type> <name>={
      std::vector<std::string> swords;
      inlib::words(words[0]," ",false,swords);
      if(swords.size()==2) {
        inlib::strip(swords[0]);
        inlib::strip(swords[1]);

        if(swords[0]=="ITuple") {
  
          //create a Slash::Core::IValue::VOID_STAR :
          Lib::Value* v = new Lib::Value((void*)0);
          v->setLabel(swords[1]);
          return v;

	} else {
          Lib::Value* v = new_Value(swords[0]);
          if(!v) {
            f_out << "Lib::Columns::analyse_dcl :"
                  << " (2) syntax error in " << inlib::sout(words[0]) << "."
                  << " Unknown type " << inlib::sout(swords[0]) << "."
                  << std::endl;
            return 0;
          }
          v->setLabel(swords[1]);
          return v;
        }

      } else if(swords.size()==1) {

        if(fCurrentType==Slash::Core::IValue::NONE) {
          f_out << "Lib::Columns::analyse_dcl :"
                << " (1) current type is NONE."
                << std::endl;
          return 0;
        }

        Lib::Value* v = new Lib::Value();
        v->setType(fCurrentType);       
        v->setLabel(swords[0]);
        return v;
      }

    } else {
      f_out << "Lib::Columns::analyse_dcl :"
            << " syntax error in " << inlib::sout(a_s) << "."
            << " two or four words expected."
            << std::endl;
      return 0;
    }
  }
private:
  inline Lib::Value* new_Value(const std::string& a_type) {
    Lib::Value* v = new Lib::Value();    
           if(a_type=="float") {
      v->setType(Slash::Core::IValue::FLOAT);
    } else if(a_type=="double") {
      v->setType(Slash::Core::IValue::DOUBLE);
    } else if(a_type=="char") {
      v->setType(Slash::Core::IValue::CHAR);
    } else if(a_type=="byte") {
      v->setType(Slash::Core::IValue::UNSIGNED_CHAR);
    } else if(a_type=="short") {
      v->setType(Slash::Core::IValue::SHORT);
    } else if(a_type=="int") {
      v->setType(Slash::Core::IValue::INT);
    } else if(a_type=="long") {
      v->setType(Slash::Core::IValue::INT64);
    } else if(a_type=="boolean") {
      v->setType(Slash::Core::IValue::BOOL);
    } else if(a_type=="bool") {
      v->setType(Slash::Core::IValue::BOOL);
    } else if(a_type=="string") {
      v->setType(Slash::Core::IValue::STRING);
    } else if(a_type=="java.lang.String") {
      v->setType(Slash::Core::IValue::STRING);

    } else if(a_type=="float[]") {
      v->setType(Slash::Core::IValue::ARRAY_FLOAT);
    } else if(a_type=="double[]") {
      v->setType(Slash::Core::IValue::ARRAY_DOUBLE);
    } else if(a_type=="char[]") {
      v->setType(Slash::Core::IValue::ARRAY_CHAR);
    } else if(a_type=="byte[]") {
      v->setType(Slash::Core::IValue::ARRAY_UNSIGNED_CHAR);
    } else if(a_type=="short[]") {
      v->setType(Slash::Core::IValue::ARRAY_SHORT);
    } else if(a_type=="int[]") {
      v->setType(Slash::Core::IValue::ARRAY_INT);
    } else if(a_type=="long[]") {
      v->setType(Slash::Core::IValue::ARRAY_INT64);
    } else if(a_type=="boolean[]") {
      v->setType(Slash::Core::IValue::ARRAY_BOOL);
    } else if(a_type=="bool[]") {
      v->setType(Slash::Core::IValue::ARRAY_BOOL);
    } else if(a_type=="string[]") {
      v->setType(Slash::Core::IValue::ARRAY_STRING);
    } else if(a_type=="java.lang.String[]") {
      v->setType(Slash::Core::IValue::ARRAY_STRING);

    } else {
      delete v;
      return 0;
    }
    return v;
  }
  inline bool set_Value(Lib::Value& a_v,const std::string& a_s) {
    if(a_v.type()==Slash::Core::IValue::FLOAT) {
      float v;
      if(!inlib::to<float>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::DOUBLE) {
      double v;
      if(!inlib::to<double>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::CHAR) {
      char v;
      if(!inlib::to<char>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::UNSIGNED_CHAR) {
      unsigned short v;
      if(!inlib::to<unsigned short>(a_s,v)) return false;
      a_v.set((unsigned char)v);
    } else if(a_v.type()==Slash::Core::IValue::SHORT) {
      short v;
      if(!inlib::to<short>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::INT) {
      int v;
      if(!inlib::to<int>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::INT64) {
      inlib::int64 v;
      if(!inlib::to<inlib::int64>(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::BOOL) {
      bool v;
      if(!inlib::to(a_s,v)) return false;
      a_v.set(v);
    } else if(a_v.type()==Slash::Core::IValue::STRING) {
      std::string v = a_s;
      if( (v.size()>=2) && (v[0]=='"') && (v[v.size()-1]=='"') ){
        v = v.substr(1,v.size()-2);
      }
      a_v.set(v);
    } else {
      return false;
    }
    return true;
  }
public:
  int fVerboseLevel;
  std::string fString;
  std::ostream& f_out;
  std::list<Lib::Value> fStack;
  Slash::Core::IValue::Type fCurrentType;
  //bool fSuccess;
};

}

#endif

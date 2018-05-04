#ifndef Lib_FunctionManager_h
#define Lib_FunctionManager_h

// Inheritance :
#include <Slash/Data/IFunction.h>
#include <Slash/Core/IManager.h>
#include <Slash/Data/IFunctionManager.h>

#include <Slash/Core/ISession.h>
#include <Slash/Core/ILibraryManager.h>
#include <Slash/Core/ILibrary.h>

#include <Slash/Tools/Manager.h>

#include <inlib/file>
#include <inlib/system>
#include <inlib/parse>
#include <inlib/touplow>
#include <inlib/srep>

#include <list>

#include <Lib/Value.h>
#include <Lib/Functions.h>

namespace Lib {

class FunctionManager 
:public virtual Slash::Core::IManager 
,public virtual Slash::Data::IFunctionManager 
{
public: //Slash::Core::IManager
  virtual std::string name() const {return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Slash::Data::IFunctionManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public: //IFunctionManager
  virtual Slash::Data::IFunction* findFunction(const std::string& aModel){
    std::list<Slash::Data::IFunction*>::iterator it;
    for(it=fList.begin();it!=fList.end();++it) {
      if(aModel==(*it)->model()) return (*it);
    }
    return 0;
  }

  virtual bool load(const std::string& aFile,const std::string& aBuilder = ""){
    std::ostream& out = fSession.cout();
    std::string _aFile;
    inlib::file_name(aFile,_aFile);
    if(!inlib::file::exists(_aFile)) {
      out << "Lib::FunctionManager::load :" 
          << " file " << inlib::sout(aFile) << " not found."
          << std::endl;
      return false;
    }
    if(inlib::is_f77(aFile)) return loadF77(aFile,aBuilder);  
    if(inlib::is_cpp(aFile)) return loadC(aFile,aBuilder);  
    return false;
  }
public:
  FunctionManager(Slash::Core::ISession& aSession)
  :fSession(aSession),fName("FunctionManager")
  {}
  virtual ~FunctionManager(){
    // Delete in reverse order :
    std::list<Slash::Data::IFunction*>::iterator it;
    while(!fList.empty()) {
      it = fList.end();
      it--;
      Slash::Data::IFunction* entry  = *it;
      fList.erase(it);
      delete entry;
    }
  }
public:
  typedef std::vector<Lib::Value> FunctionArgs;

  static bool find_f77_functions(const std::string& aFile,
                                 std::vector<std::string>& aNames,
                                 std::vector<FunctionArgs>& aArgs,
                                 std::ostream& a_out){
    aNames.clear();
    aArgs.clear();
    std::vector<std::string> lines;
    std::string _aFile;
    inlib::file_name(aFile,_aFile);
    if(!inlib::file::read(_aFile,lines)) {
      a_out << "Lib::FunctionManager::find_f77_functions :" 
          << " can't read file " << inlib::sout(aFile) << "."
          << std::endl;
      return false;
    }
    unsigned int linen = lines.size();
    bool inFunction = false;
    for(unsigned int linei = 0;linei<linen;linei++) {
      std::string line = lines[linei];
      inlib::touppercase(line);
      std::vector<std::string> words;
      inlib::words(line," ",false,words);
      unsigned int index;
      if( inlib::item_index<std::string>(words,"FUNCTION",index)   ||
          inlib::item_index<std::string>(words,"SUBROUTINE",index) ){
        if(inFunction) { //Syntax error.
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " [FUNCTION,SUBROUTINE] / END mismatch."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        } 
        // For example : REAL FUNCTION FUNC(I)
        // For example : FUNCTION FUNC(I)
        if((index!=0)&&(index!=1)) { //Syntax error. 
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " [FUNCTION,SUBROUTINE] is not the first or second word."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        }
        if((index+1)>=words.size()) { //Syntax error.
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " No space expected between func name and arguments."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        }
        std::string name;
        std::vector<std::string> sargs;
        if(!inlib::name_args(words[index+1],name,sargs)) {
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " syntax error in arguments."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        }
        unsigned int sargn = sargs.size();
        std::vector<Lib::Value> args(sargn);
        for(unsigned int argi=0;argi<sargn;argi++) {
  	std::string s = sargs[argi];
          inlib::strip(s);
          if(!inlib::is_variable(s)) {
            a_out << "Lib::FunctionManager::find_f77_functions :" 
                << " In " << inlib::sout(line) << ","
                << " " << inlib::sout(s)
                << " has not the syntax of a variable."
                << std::endl;
            aNames.clear();
            aArgs.clear();
            return false;
          }
          Lib::Value& v = args[argi];
          v.setLabel(s);
          v.setDimension(1);
          char c = s[0];
          if( (c>='I')&&(c<='N') ) {
            v.setType(Slash::Core::IValue::INT_STAR);
          } else {
            v.setType(Slash::Core::IValue::FLOAT_STAR);
          }
          //WARNING : can't have a DOUBLE_STAR here, else
          //          semantic clash with the VECTOR logic.
        }
        aNames.push_back(name);
        aArgs.push_back(args);
        inFunction = true;
  
      } else if(inlib::item_index<std::string>(words,"END",index)) {
        if(!inFunction) { //Syntax error.
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " [FUNCTION,SUBROUTINE] / END mismatch."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        }
        if(index!=0) { //Syntax error.
          a_out << "Lib::FunctionManager::find_f77_functions :" 
              << " In " << inlib::sout(line) << ","
              << " END is not the first word."
              << std::endl;
          aNames.clear();
          aArgs.clear();
          return false;
        }
        inFunction = false;
      } else if(inFunction) {
        // Search argument declaration :
        if( inlib::item_index<std::string>(words,"DIMENSION",index) ||
            inlib::item_index<std::string>(words,"VECTOR",index) ){
          if((index+1)>=words.size()) { //Syntax error.
            a_out << "Lib::FunctionManager::find_f77_functions :" 
                << " In " << inlib::sout(line) << ","
                << " [DIMENSION,VECTOR] is not the previous last word."
                << std::endl;
            aNames.clear();
            aArgs.clear();
            return false;
          }
          if(index!=0) { //Syntax error.
            a_out << "Lib::FunctionManager::find_f77_functions :" 
                << " In " << inlib::sout(line) << ","
                << " [DIMENSION,VECTOR] is not the first word."
                << std::endl;
            aNames.clear();
            aArgs.clear();
            return false;
          }
          bool isVector = (words[index]=="VECTOR");
          std::string s = words[index+1];
          std::vector<std::string> words2;
          inlib::words(s,",",false,words2);
          std::vector<Lib::Value>& lastArgs = aArgs[aArgs.size()-1];
          unsigned int wordn2 = words2.size();
          for(unsigned int vari=0;vari<wordn2;vari++) {
            const std::string& VNAME = words2[vari];
     	  std::string NAME;
            int dim;
            if(!inlib::parse_array(VNAME,NAME,dim)) {
              a_out << "Lib::FunctionManager::find_f77_functions :" 
                  << " In " << inlib::sout(line) << ","
                  << " array syntax error in " << inlib::sout(VNAME) << "."
                  << std::endl;
              aNames.clear();
              aArgs.clear();
              return false;
            }
            inlib::strip(NAME);
            if(!inlib::is_variable(NAME)) {
              a_out << "Lib::FunctionManager::find_f77_functions :" 
                  << " In " << inlib::sout(line) << ","
                  << " " << inlib::sout(NAME) 
                  << " has not the syntax of a variable."
                  << std::endl;
              aNames.clear();
              aArgs.clear();
              return false;
            }
            if(isVector) {
    	    Lib::Value v;
              v.setType(Slash::Core::IValue::DOUBLE_STAR);
              v.setLabel(NAME);
              if(dim!=NotFound) v.setDimension(dim); //Else 0.Used in pafort.f.
              lastArgs.push_back(v);
            } else {
              if(dim==NotFound) {
                a_out << "Lib::FunctionManager::find_f77_functions :" 
                    << " In " << inlib::sout(line) << ","
                    << " variable " << inlib::sout(VNAME) 
                    << " is not an array."
                    << std::endl;
                aNames.clear();
                aArgs.clear();
                return false;
              }
  
              //printf("debug : %s %s\n",NAME.c_str(),DIM.c_str());
              bool found = false;
              unsigned int argn = lastArgs.size();
              for(unsigned int argi=0;argi<argn;argi++) {
                if(lastArgs[argi].label()==NAME) {
       	        Lib::Value& v = lastArgs[argi];
                  v.setDimension((int)dim);
                  found = true;
                  break;
                }
              }
              if(!found) {
                a_out << "Lib::FunctionManager::find_f77_functions :" 
                    << " In " << inlib::sout(line) << ","
                    << " " << inlib::sout(NAME) 
                    << " not found in argument list."
                    << std::endl;
                aNames.clear();
                aArgs.clear();
                return false;
              }
            }
          }
        }
      }
    }
  
    return true;
  }

  static bool find_cpp_functions(const std::string& aFile,
                                 std::vector<std::string>& aNames,
                                 std::vector<FunctionArgs>& aArgs,
                                 std::vector<unsigned int>& aParns){
    aNames.clear();
    aArgs.clear();
    aParns.clear();
    std::vector<std::string> lines;
    std::string _aFile;
    inlib::file_name(aFile,_aFile);
    inlib::file::read(_aFile,lines);
    unsigned int linen = lines.size();
    for(unsigned int linei = 0;linei<linen;linei++) {
      std::string& line = lines[linei];
      std::string sfunc;
  
      unsigned int parn = 0;
  
     {std::string::size_type pos_func = line.find("/*function(");
      if(pos_func!=std::string::npos) {
        // header of the form /*function(<unsigned int>)*/
        std::string::size_type end_pos_func = line.find(")*/");
        if( (end_pos_func==std::string::npos) || (pos_func>=end_pos_func) ) {
          aNames.clear();
          aArgs.clear();
          aParns.clear();
          return false;
        }
        sfunc = line.substr(end_pos_func+3,line.size()-(end_pos_func+3));
        std::string sparn = 
          line.substr(pos_func+11,(end_pos_func-(pos_func+11)));
        typedef unsigned int uint_t;
        if(!inlib::to<uint_t>(sparn,parn)) { //Syntax error.
          aNames.clear();
          aArgs.clear();
          aParns.clear();
          return false;
        }
      }}
  
     {std::string::size_type pos_func = line.find("/*function*/");
      if(pos_func!=std::string::npos) {
        sfunc = line.substr(pos_func+12,line.size()-(pos_func+12));
      }}
  
      if(!sfunc.size()) continue; //not a header.
  
      std::string head;
      std::vector<std::string> sargs;
      if(!inlib::name_args(sfunc,head,sargs)) {
        aNames.clear();
        aArgs.clear();
        aParns.clear();
        return false;
      }
      inlib::strip(head);
  
      // head is of the form <type> <name>.
     {std::vector<std::string> words;
       inlib::words(head," ",false,words);
      if(words.size()!=2) {
        aNames.clear();
        aArgs.clear();
        aParns.clear();
        return false;
      }
      std::string name = words[1];
      inlib::strip(name);
      if(!inlib::is_variable(name)) {
        aNames.clear();
        aArgs.clear();
        aParns.clear();
        return false;
      }
      aNames.push_back(name);}
  
      unsigned int argn = sargs.size();
  
      // Arguments :
      std::vector<Lib::Value> vargs;
      for(unsigned int argi=0;argi<argn;argi++) {
        std::vector<std::string> words;
        inlib::words(sargs[argi]," ",false,words);
        if(words.size()!=2) { //Syntax error
          aNames.clear();
          aArgs.clear();
          aParns.clear();
          return false;
        }
        Lib::Value v;
  
        std::string type = words[0];
        std::string s = words[1];
        inlib::strip(s);
        // Look for <atring>[<number>]
       {std::string::size_type lbrace = s.find('[');
        std::string::size_type rbrace = s.find(']');
        if((lbrace==std::string::npos)&&(rbrace==std::string::npos)){
          if(!inlib::is_variable(s)) {
            aNames.clear();
            aArgs.clear();
            aParns.clear();
            return false;
          }
          v.setLabel(s);
          v.setDimension(0);
          if(type=="int") {
            v.setType(Slash::Core::IValue::INT);
          } else if(type=="float") {
            v.setType(Slash::Core::IValue::FLOAT);
          } else if(type=="double") {
            v.setType(Slash::Core::IValue::DOUBLE);
          } else { //Type not supported.
            aNames.clear();
            aArgs.clear();
            aParns.clear();
            return false;
          }
        } else if( (lbrace!=std::string::npos) &&
                   (rbrace!=std::string::npos) && 
                   (lbrace<rbrace)             ){
          std::string NAME = s.substr(0,lbrace);
          if(!inlib::is_variable(NAME)) {
            aNames.clear();
            aArgs.clear();
            aParns.clear();
            return false;
          }
          std::string DIM = s.substr(lbrace+1,rbrace-(lbrace+1));
          unsigned int dim = 0;
          if(DIM.empty()) { //xxx[] syntax
            dim = 0; 
          } else {
            typedef unsigned int uint_t;
            if(!inlib::to<uint_t>(DIM,dim)) { //Syntax error.
              aNames.clear();
              aArgs.clear();
              aParns.clear();
              return false;
            }
          }
          v.setLabel(NAME);
          v.setDimension(dim);
          if(type=="int") {
            v.setType(Slash::Core::IValue::INT_STAR);
          } else if(type=="float") {
            v.setType(Slash::Core::IValue::FLOAT_STAR);
          } else if(type=="double") { //VECTOR for OpenPAW.
            v.setType(Slash::Core::IValue::DOUBLE_STAR);
            //dim could be 0 to indicate OpenPAW/pafort.f 
            //to search for an existing VECTOR
          } else { //Type not supported.
            aNames.clear();
            aArgs.clear();
            aParns.clear();
            return false;
          }
        } else {  //Syntax error.
          aNames.clear();
          aArgs.clear();
          aParns.clear();
          return false;
        }}          
        vargs.push_back(v);
  
      } 
      aArgs.push_back(vargs);
      aParns.push_back(parn);
    }
    return true;
  }

  static bool edit_f77_file(const std::string& aFile,
                            const std::string& aNewFile,
                            const std::vector<std::string>& aNames,
                            const std::vector<FunctionArgs>& aArgs){
    // Create  modified FORTRAN file :
    //   REAL FUNCTION FUNC(X)
    //   VECTOR A,B(1) 
    // changed to :
    //   REAL FUNCTION FUNC(X,A,B)
    //   REAL A,B(1)
    std::vector<std::string> lines;
    std::string _aFile;
    inlib::file_name(aFile,_aFile);
    if(!inlib::file::read(_aFile,lines)) return false;
  
    FILE* out_file = ::fopen(aNewFile.c_str(),"wb");
    if(!out_file) return false;
  
    unsigned funci = 0;
  
    unsigned int linen = lines.size();
    for(unsigned int linei = 0;linei<linen;linei++) {
      std::string line = lines[linei];
  
      inlib::touppercase(line);
  
      std::vector<std::string> words;
      inlib::words(line," ",false,words);
  
      unsigned int index;
      if( inlib::item_index<std::string>(words,"FUNCTION",index)   ||
          inlib::item_index<std::string>(words,"SUBROUTINE",index) ){
  
        // Assume that the line syntax is correct :
        //    <REAL,INTEGER> FUNCTION <name>(<args>)
        // Add extra arguments (coming from VECTOR) at end
        // of arguments.
  
        std::string new_line;
  
        std::string::size_type lb = line.find('(');
        if(lb==std::string::npos) {
          new_line = lines[linei];
        } else {
          new_line = lines[linei].substr(0,lb);
        }
        new_line += "(";
  
       {const std::vector<Lib::Value>& args = aArgs[funci];
        unsigned int argn = args.size();
        for(unsigned int argi=0;argi<argn;argi++) {
          if(argi) new_line += ",";
          new_line += args[argi].label();
        }}
   
        new_line += ")";
  
        ::fprintf(out_file,"%s\n",new_line.c_str());
        funci++;
  
      } else if(inlib::item_index<std::string>(words,"VECTOR",index)) {
  
        if((index+1)>=words.size()) return false;
  
                             //01234567
        std::string new_line = "      REAL*8 ";
  
        std::string s = words[index+1];
        std::vector<std::string> words2;
        inlib::words(s,",",false,words2);
        unsigned int wordn2 = words2.size();
        for(unsigned int vari=0;vari<wordn2;vari++) {
          const std::string& VNAME = words2[vari];
          std::string NAME;
          int dim;
          if(!inlib::parse_array(VNAME,NAME,dim)) return false;
          inlib::strip(NAME);
          if(vari) new_line += ",";
          new_line += NAME;
          new_line += "(*)";
        }
  
        ::fprintf(out_file,"%s\n",new_line.c_str());
  
      } else {
  
        ::fprintf(out_file,"%s\n",lines[linei].c_str());
  
      }
    }
  
    ::fclose(out_file);
    return true;
  }

private:
  bool loadF77(const std::string& aFile,const std::string& aBuilder = ""){
    std::ostream& out = fSession.cout();
    std::vector<std::string> funcs;
    std::vector<FunctionArgs> args;
    if(!find_f77_functions(aFile,funcs,args,out)) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " syntax problem in file " << inlib::sout(aFile) << "."
          << std::endl;
      return false;
    }
    if(!funcs.size()) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " file " << inlib::sout(aFile) << " has not function."
          << std::endl;
      return false;
    }
  
    std::string new_file = "Lib_new_"+inlib::nosuffix(aFile)+".f";
  
    if(!edit_f77_file(aFile,new_file,funcs,args)) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " can't produce new file " << inlib::sout(new_file)
          << " from file " << inlib::sout(aFile) << "."
          << std::endl;
      return false;
    }
  
    if(fSession.verboseLevel()) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " functions in file " << inlib::sout(new_file) 
          << std::endl;
      unsigned int funcn = funcs.size();
      for(unsigned int funci=0;funci<funcn;funci++) {
        std::string sargs;
        unsigned int argn = args[funci].size();
        for(unsigned int argi=0;argi<argn;argi++) {
          if(argi) sargs += ",";
          sargs += args[funci][argi].label();
          sargs += "(";
          sargs += inlib::to<int>((int)args[funci][argi].dimension());
          sargs += ")";
        }
        out << "Lib::FunctionManager::loadF77 :" 
            << " name : " << inlib::sout(funcs[funci]) 
  	  << " args : " << inlib::sout(sargs) 
            << std::endl;
      }
    }
  
    std::string path,name,suffix;
    inlib::path_name_suffix(new_file,path,name,suffix);
    std::string dll = inlib::nosuffix(name);
   
    std::string fullname;
    inlib::file_name(new_file,fullname);
    inlib::file::cp(fullname,name);
  
    std::vector<Slash::Core::ILibrary::Procedure> procs;
    if(!get_procs(fSession,dll,suffix,"Lib_wrap_",funcs,args,procs,aBuilder,
                  build_f77_dll)) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " unable to get some f77 function in " << inlib::sout(dll)
          << std::endl;
      return false;
    }
    if(fSession.verboseLevel()) {
      out << "Lib::FunctionManager::loadF77 :" 
          << " found f77 function in " << inlib::sout(dll)
          << std::endl;
    }
  
    unsigned int funcn = funcs.size();
    for(unsigned int funci=0;funci<funcn;funci++) {
      addFunction(new_file,funcs[funci],(void*)procs[funci],args[funci],0);
    }
  
    return true;
  }

  bool loadC(const std::string& aFile,const std::string& aBuilder = ""){
    std::ostream& out = fSession.cout();
    std::vector<std::string> funcs;
    std::vector<FunctionArgs> args;
    std::vector<unsigned int> parns;
    if(!find_cpp_functions(aFile,funcs,args,parns)) {
      out << "Lib::FunctionManager::loadC :" 
          << " syntax problem in file " << inlib::sout(aFile) << "."
          << std::endl;
      return false;
    }
    if(!funcs.size()) {
      out << "Lib::FunctionManager::loadC :" 
          << " file " << inlib::sout(aFile) << " has not function."
          << std::endl;
      return false;
    }
  
    if(fSession.verboseLevel()) {
      out << "Lib::FunctionManager::loadC :" 
          << " functions in file " << inlib::sout(aFile) 
          << std::endl;
      unsigned int funcn = funcs.size();
      for(unsigned int funci=0;funci<funcn;funci++) {
        std::string sargs;
        unsigned int argn = args[funci].size();
        for(unsigned int argi=0;argi<argn;argi++) {
          if(argi) sargs += ",";
          sargs += args[funci][argi].stype();
          sargs += " ";
          sargs += args[funci][argi].label();
          sargs += "(";
          sargs += inlib::to<int>((int)args[funci][argi].dimension());
          sargs += ")";
        }
        out << "Lib::FunctionManager::loadC :" 
            << "   name : " << inlib::sout(funcs[funci]) 
  	  << " args : " << inlib::sout(sargs) 
            << std::endl;
      }
    }
  
    std::string path,name,suffix;
    inlib::path_name_suffix(aFile,path,name,suffix);
    std::string dll = inlib::nosuffix(name);
   
    std::string fullname;
    inlib::file_name(aFile,fullname);
    inlib::file::cp(fullname,name); //FIXME : avoid this copy.
  
    std::vector<Slash::Core::ILibrary::Procedure> procs;
    if(!get_procs(fSession,dll,suffix,"Lib_wrap_",funcs,args,procs,aBuilder,
                  build_C_plugin)) {
      out << "Lib::FunctionManager::loadC :" 
          << " unable to get some C function in " << inlib::sout(dll)
          << std::endl;
      return false;
    }
    if(fSession.verboseLevel()) {
      out << "Lib::FunctionManager::loadC :" 
          << " found C function in " << inlib::sout(dll)
          << std::endl;
    }
  
    unsigned int funcn = funcs.size();
    for(unsigned int funci=0;funci<funcn;funci++) {
      addFunction
        (aFile,funcs[funci],(void*)procs[funci],args[funci],parns[funci]);    
    }
  
    return true;
  }


  Slash::Data::IFunction* addFunction(const std::string& aFile,
                                      const std::string& aModel,
                                      void* aAddress,
                                      const std::vector<Lib::Value>& aArgs,
                                      unsigned int aParn){
    if(aModel.empty()) return 0;
    Slash::Data::IFunction* entry = findFunction(aModel);
    if(entry) return entry;
    entry = 
      new Lib::Function::Compiled
        (fSession.cout(),aFile,aModel,aAddress,aArgs,aParn);
    fList.push_back(entry);
    return entry;
  }


  typedef bool (*plugin_builder_func)(const std::string&,const std::string&,const std::vector<std::string>&,const std::vector<Lib::FunctionManager::FunctionArgs>&,const std::string&,std::ostream&);

  static bool get_procs(Slash::Core::ISession& aSession,
                        const std::string& aPlugin,
                        const std::string& aSuffix,
                        const std::string& aPrefix,
                        const std::vector<std::string>& aFuncs,
                 const std::vector<Lib::FunctionManager::FunctionArgs>& aArgs,
                 std::vector<Slash::Core::ILibrary::Procedure>& aProcs,
                 const std::string& aBuilder,
                 plugin_builder_func a_build_func){
    aProcs.clear();

    std::ostream& out = aSession.cout();

    Slash::Core::ILibraryManager* libraryManager = 
      Slash_findManager
        (aSession,"LibraryManager",Slash::Core::ILibraryManager);
    if(!libraryManager) {
      out << "get_procs :"
          << " can't find manager " << inlib::sout("LibraryManager") 
          << std::endl;
      return false;
    }
  
    Slash::Core::ILibrary* library = libraryManager->findLibrary(aPlugin);
    if(!library) {
      library = 
        libraryManager->addLibrary(aPlugin,aPlugin,"",true); //true = quiet.
      if(!library) { //DLL not found.
        if(!a_build_func(aPlugin,aSuffix,aFuncs,aArgs,aBuilder,out)) 
          return false;
        library = libraryManager->addLibrary(aPlugin,aPlugin,"");
      }
    }
    if(!library) return false;
  
    unsigned int funcn = aFuncs.size();
    for(unsigned int funci=0;funci<funcn;funci++) {
      const std::string& funcName = aFuncs[funci];
      std::string CNAME = aPrefix + funcName;
      Slash::Core::ILibrary::Procedure proc = library->findProcedure(CNAME);
      if(proc) {
        aProcs.push_back(proc);
        if(aSession.verboseLevel())
          out << "get_C_function :"
              << " function " << inlib::sout(funcName) 
              << " found in " << inlib::sout(aPlugin) 
              << std::endl;
      } else {
        if(aSession.verboseLevel())
          out << "get_C_function :"
              << " function " << inlib::sout(funcName) 
              << " not found in " << inlib::sout(aPlugin) 
              << std::endl;
        aProcs.clear();
        return false;
      }    
    }
  
    return true;
  }

  static bool build_f77_dll(const std::string& aName,
                            const std::string& aSuffix,
                            const std::vector<std::string>& aFuncs,
                  const std::vector<Lib::FunctionManager::FunctionArgs>& aArgs,
                  const std::string& aBuilder,
                  std::ostream& a_out){
    //printf("debug : build_f77_dll : \"%s\"\n",aName.c_str());

    std::string CNAME = aName;
    CNAME = std::string("Lib_wrap_") + CNAME;
    
    // Produce C frontend file :
    std::string fileName = CNAME+".cpp";
    FILE* file = ::fopen(fileName.c_str(),"wb");
    if(!file) {
      a_out << "build_f77_dll :" 
          << " can't create file " << inlib::sout(fileName)
          << std::endl;
      return false;
    }

    ::fprintf(file,"#include <Slash/Core/IValue.h>\nextern \"C\" {\n");

#ifdef WIN32
    std::string s_stdcall = "__stdcall ";              //space at end.
    std::string s_under = "";
    std::string s_declspec = "_declspec(dllexport) ";  //idem
#else 
    std::string s_stdcall = "";
    std::string s_under = "_";
    std::string s_declspec = "";
#endif

    std::string sh = "    ";
  
    typedef unsigned int uint_t;
  
    unsigned int funcn = aFuncs.size();
    for(unsigned int funci=0;funci<funcn;funci++) {
      std::string funcName = aFuncs[funci];
      unsigned int argn = aArgs[funci].size();
  
      std::string sargs;
      std::string sargs2;
      std::string svals;
      svals = "if(aArgs.size()!="+
        inlib::to<uint_t>(argn)+") return false;\n";
  
      for(unsigned int argi=0;argi<argn;argi++) {
        if(argi) {
          sargs  += ",";
          sargs2 += ",";
          svals  += "\n";
        }
        const Lib::Value& v = aArgs[funci][argi]; //FIXME : use Value.
        std::string scxx;
        if(Lib::Value::cxx_type(v,scxx)) {
          //FIXME : put code checking received type.
          sargs += scxx+" ";
          std::string what = scxx;
          inlib::replace(what,"*","_star");
          svals += sh+scxx+" "+v.label()+
            " = aArgs["+inlib::to<uint_t>(argi)+"]->get_"+what+"();";
        } else {
          a_out << "build_f77_dll :" 
              << " for file " << inlib::sout(aName) 
              << " can't handle variable type " << inlib::sout(v.stype()) 
              << "."
              << std::endl;
          ::fclose(file);
          ::remove(fileName.c_str());
          return false;
        }
        sargs  += v.label();
        sargs2 += v.label();
      }
  
#ifdef WIN32
      inlib::touppercase(funcName);
#else
      inlib::tolowercase(funcName);
#endif

      ::fprintf(file,"\
  float %s%s%s(%s);\n\
  %sbool Lib_wrap_%s(const std::vector<Slash::Core::IValue*>& aArgs,Slash::Core::IValue& aReturn) {\n\
    %s\n\
    aReturn.set(%s%s(%s));\n\
    return true;\n\
  }\n",s_stdcall.c_str(),funcName.c_str(),s_under.c_str(),sargs.c_str(),
       s_declspec.c_str(),aFuncs[funci].c_str(),
       svals.c_str(),
       funcName.c_str(),s_under.c_str(),sargs2.c_str());

    }

    ::fprintf(file,"}\n");

    ::fclose(file);

    std::string builder;
    if(aBuilder.size()) {
      builder = aBuilder;
    } else {
#ifdef WIN32
      builder = "$LIBROOT/scripts/DOS/makf77dll.bat";
#else
      builder = "$LIBROOT/scripts/sh/makf77dll.sh";
#endif
    }
    std::string _builder;
    inlib::file_name(builder,_builder);
    if(!inlib::file::exists(_builder)) {
      a_out << "build_f77_dll :" 
          << " file " << inlib::sout(builder) << " not found."
          << std::endl;
      return false;
    }
  
    std::string sys_build_proc;
    inlib::file_name(builder,sys_build_proc);
    sys_build_proc = inlib::file::quote(sys_build_proc);
  
    //FIXME : check ret status.
    inlib::execute(sys_build_proc+" "+aName+" "+aSuffix+" "+CNAME);
  
    return true;
  }

  static bool build_C_plugin(const std::string& aName,
                             const std::string& aSuffix,
                             const std::vector<std::string>& aFuncs,
                 const std::vector<Lib::FunctionManager::FunctionArgs>& aArgs,
                 const std::string& aBuilder,
                 std::ostream& a_out){
    std::string CNAME = aName;
    CNAME = std::string("Lib_wrap_") + CNAME;
    
    // Produce C frontend file :
    std::string fileName = CNAME+".cpp";
    FILE* file = ::fopen(fileName.c_str(),"wb");
    if(!file) {
      a_out << "build_C_plugin :" 
          << " can't create file " << inlib::sout(fileName)
          << std::endl;
      return false;
    }

    ::fprintf(file,"#include <Slash/Core/IValue.h>\nextern \"C\" {\n");

#ifdef WIN32
    std::string s_declspec = "_declspec(dllexport) ";
#else 
    std::string s_declspec = "";
#endif

    std::string sh = "    ";
  
    typedef unsigned int uint_t;
  
    unsigned int funcn = aFuncs.size();
    for(unsigned int funci=0;funci<funcn;funci++) {
      std::string funcName = aFuncs[funci];
      unsigned int argn = aArgs[funci].size();
  
      std::string sargs;
      std::string sargs2;
      std::string svals;
      svals = "if(aArgs.size()!="+
        inlib::to<uint_t>(argn)+") return false;\n";
  
      for(unsigned int argi=0;argi<argn;argi++) {
        if(argi) {
          sargs  += ",";
          sargs2 += ",";
          svals  += "\n";
        }
        const Lib::Value& v = aArgs[funci][argi]; //FIXME : use Value.
        std::string scxx;
        if(Lib::Value::cxx_type(v,scxx)) {
          //FIXME : put code checking received type.
          sargs += scxx+" ";
          std::string what = scxx;
          inlib::replace(what,"*","_star");
          svals += sh+scxx+" "+v.label()+
            " = aArgs["+inlib::to<uint_t>(argi)+"]->get_"+what+"();";
        } else {
          a_out << "build_c_dll :" 
              << " for file " << inlib::sout(aName) 
              << " can't handle variable type " << inlib::sout(v.stype()) 
              << "."
              << std::endl;
          ::fclose(file);
          ::remove(fileName.c_str());
          return false;
        }
        sargs  += v.label();
        sargs2 += v.label();
      }
  
      ::fprintf(file,"\
  double %s(%s);\n\
  %sbool Lib_wrap_%s(const std::vector<Slash::Core::IValue*>& aArgs,Slash::Core::IValue& aReturn) {\n\
    %s\n\
    aReturn.set(%s(%s));\n\
    return true;\n\
  }\n",funcName.c_str(),sargs.c_str(),
       s_declspec.c_str(),aFuncs[funci].c_str(),
       svals.c_str(),
       funcName.c_str(),sargs2.c_str());

    }
  
    ::fprintf(file,"}\n");

    ::fclose(file);

    std::string builder;
    if(aBuilder.size()) {
      builder = aBuilder;
    } else {
#ifdef WIN32
      builder = "$LIBROOT/scripts/DOS/makcdll.bat";
#else
      builder = "$LIBROOT/scripts/sh/makcdll.sh";
#endif
    }
    std::string _builder;
    inlib::file_name(builder,_builder);
    if(!inlib::file::exists(_builder)) {
      a_out << "build_c_dll :" 
          << " file " << inlib::sout(builder) << " not found."
          << std::endl;
      return false;
    }
  
    std::string sys_build_proc;
    inlib::file_name(builder,sys_build_proc);
    sys_build_proc = inlib::file::quote(sys_build_proc);
  
    //FIXME : check ret status.
    inlib::execute(sys_build_proc+" "+aName+" "+aSuffix+" "+CNAME);
  
    return true;
  }

private:
  static const int NotFound = -1;
private:
  Slash::Core::ISession& fSession;
  std::string fName;
  std::list<Slash::Data::IFunction*> fList;
};

}

#endif





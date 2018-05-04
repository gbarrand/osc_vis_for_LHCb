#ifndef BatchLab_FFactory_h
#define BatchLab_FFactory_h

#include <Slash/Tools/Managers.h>

#include <BatchLab/Core/Function.h>
#include <BatchLab/Core/FunctionCatalog.h>

#include <inlib/poldeg>

namespace BatchLab {

class FFactory {
public:
  inline static BaseFunction* create(Slash::Core::ISession& aSession,
                                       const std::string& aName,
                                       const std::string& aTitle,
                                       const std::string& aWhat) {
    // aWhat could be :
    // - Aliases :
    //   G, GAUSSIAN, E, EXPONENTIAL, EHBOOK, BW, BREITWIGNER, P<number>
    //   INSIDEPOLYGON, INSIDEELLIPSE
    // - Class:<one of the upper>
    // - Compiled:<model>;<file>
    //   WARNING : here, someone must not confuse "model" and
    //   function name. (The "model" is like a class name, which
    //   is not the same that the "name" that someone may put
    //   on an instance of the class).
    // - Composite:<script>
    // - Scripted:<script>
    // - if not an alias and not of the form : <kind>:<rest>, then
    //   aWhat is search in the loaded functions catalog.
    std::ostream& out = aSession.cout();
    
    std::string KIND;
    std::string rest;
   {std::string::size_type pos = aWhat.find(':');
    if(pos==std::string::npos) {
      rest = aWhat.substr(0,pos);
    } else {
      KIND = aWhat.substr(0,pos);
      rest = aWhat.substr(pos+1,aWhat.size()-(pos+1));
    }
    inlib::touppercase(KIND);}
  
    std::string REST = rest;
    inlib::touppercase(REST);
  
    std::string WHAT = aWhat;
    inlib::touppercase(WHAT);
  
    int degree;

    ///////////////
    /// CLASS /////
    ///////////////
    if( ((KIND=="CLASS") && (REST=="GAUSSIAN")) || //class
        (WHAT=="GAUSSIAN") ||  //Alias
        (WHAT=="G") ) {        //Alias
      return new BatchLab::Function::Gaussian(out,aName,aTitle);
    } else if( ((KIND=="CLASS") && (REST=="EXPAIDA")) ||
               (WHAT=="EXPAIDA") ||
               (WHAT=="E") || (WHAT=="EXPONENTIAL") ) {
      return new BatchLab::Function::ExpAIDA(out,aName,aTitle);
    } else if( ((KIND=="CLASS") && (REST=="EXPHBOOK")) ||
               (WHAT=="EXPHBOOK") || 
               (WHAT=="EHBOOK") ) {
      return new BatchLab::Function::ExpHBOOK(out,aName,aTitle);
    } else if( ((KIND=="CLASS") && (REST=="BREITWIGNER")) ||
               (WHAT=="BREITWIGNER") || 
               (WHAT=="BW") ) {
      return new BatchLab::Function::BreitWigner(out,aName,aTitle);
    } else if( ((KIND=="CLASS") && 
                inlib::polynomial_degree(REST,degree)) ||
               inlib::polynomial_degree(WHAT,degree)  ){
      std::vector<double> params;
      params.resize(degree+1,0);
      return  new BatchLab::Function::Polynomial(out,aName,aTitle,params);
  
    } else if( ((KIND=="CLASS") && (REST=="INSIDEPOLYGON")) ||
               (WHAT=="INSIDEPOLYGON") ){
      return new BatchLab::Function::InsidePolygon(out,aName,aTitle);
  
    } else if( ((KIND=="CLASS") && (REST=="INSIDEELLIPSE")) ||
               (WHAT=="INSIDEELLIPSE") ){
      return new BatchLab::Function::InsideEllipse(out,aName,aTitle);
  
    } else if(KIND=="CLASS") { 
  
      out << "BatchLab::FFactory::create :"
          << " unknown class function " << inlib::sout(aWhat) << "."
          << std::endl;
      return 0;
      
    //////////////////
    /// COMPILED /////
    //////////////////
    } else if(KIND=="COMPILED") {
  
      // rest has the form <model>;<file> 
      // <model> is of the form <string>
      //                   of   <string>(<int>,<int>)
      // For this last case, see for example :
      //   OnXLab/OnXLab_exa_func_rmason.cpp 
      //   (from OnXLab_Examples_functions)
  
      std::string model;
      std::string file;
      std::string::size_type pos = rest.find(';');
      if(pos==std::string::npos) {
        std::string suffix = inlib::suffix(rest);
        if(suffix.size()) {
          file = rest; //No model specified.
          model = inlib::nosuffix(rest); //Assume model = name of the file.
        } else {
          model = rest; //No file name specified.
        }
      } else {
        model = rest.substr(0,pos);
        file = rest.substr(pos+1,rest.size()-(pos+1));
      }
  
      BatchLab::FunctionCatalog catalog(aSession);
      Slash::Data::IFunction* func = catalog.find(file,model);
      if(!func) {
        out << "BatchLab::FFactory::create :"
            << " Problem creating " << inlib::sout(aWhat)
            << std::endl;
        return 0;
      }
  
      //WARNING : func will be referenced in the BatchLab::Function::Compiled
      // but will be managed by the Lib::FunctionManager.
      return 
        new BatchLab::Function::Compiled(out,aName,aTitle,func);
  
    //////////////////
    /// COMPOSITE ////
    //////////////////
    } else if(KIND=="COMPOSITE") {
  
      return new BatchLab::Function::Composite(out,aName,aTitle,rest);
  
    //////////////////
    /// SCRIPTED /////
    //////////////////
    } else if(KIND=="SCRIPTED") {
  
      BatchLab::Function::Scripted* object = 
        new BatchLab::Function::Scripted(out,aName,aTitle,rest);
      if(!object->isValid()) {
        delete object;
        return 0;
      }
      return object;
  
    //////////////////
    /// Else /////////
    //////////////////
    } else { 
  
      // Does not start with a "kind" prefix and is not
      // a known alias. Look in the compiled catalog :
  
     {Slash::Data::IFunctionManager* functionManager = 
        Slash::functionManager(aSession);
      if(!functionManager) {
        out << "BatchLab::FFactory::create :"
            << " FunctionManager not found."
            << std::endl;
        return 0;
      }
  
      std::string model = aWhat;
  
      Slash::Data::IFunction* func = functionManager->findFunction(model);
      if(!func) {
        std::string s = model;
        inlib::touppercase(s);
        func = functionManager->findFunction(s);
      }
  
      if(func) {
  
        //WARNING : func will be referenced in the BatchLab::Function::Compiled
        // but will be managed by the Lib::FunctionManager.
        return new BatchLab::Function::Compiled(out,aName,aTitle,func);
  
      }}

      // Not in the catalog, try to pass aWhat as a script to a Scripted :
      BatchLab::Function::Scripted* object = 
        new BatchLab::Function::Scripted(out,aName,aTitle,aWhat);
      if(object->isValid()) return object;
      delete object;
  
    }
  
    out << "BatchLab::FFactory::create :"
        << " can't create " << inlib::sout(aWhat) << "."
        << std::endl;
  
    return 0;
  }

};

}

#endif

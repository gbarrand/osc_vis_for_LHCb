
// this :
#include "HistogramAction.h"

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/IValue.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Data/IHandlersIterator.h>
#include <Slash/Data/IProperty.h>
#include <Slash/Data/IHandler.h>

#include <Slash/Tools/Manager.h>
#include <Slash/Tools/names.h>

#include <inlib/cast>
#include <inlib/args>

#include <Lib/Processor.h>

OnXLab::HistogramAction::HistogramAction(
 Slash::Data::IProcessor& aAccessorManager
,Slash::Core::ISession& aSession
)
:fAccessorManager(aAccessorManager)
,fName("histogram")
,fSession(aSession)
,fHistogram1D(0)
,fHistogram2D(0)
{}

OnXLab::HistogramAction::~HistogramAction(){}

void* OnXLab::HistogramAction::cast(const std::string&) const {return 0;}

std::string OnXLab::HistogramAction::name() const {return fName;}

Slash::Core::ISession& OnXLab::HistogramAction::session() {return fSession;}

AIDA::IHistogram1D* OnXLab::HistogramAction::histogram1D() const {
  return fHistogram1D;
}

AIDA::IHistogram2D* OnXLab::HistogramAction::histogram2D() const {
  return fHistogram2D;
}

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>

std::string OnXLab::HistogramAction::execute(const std::vector<std::string>& aArgs) {
  // aArgs :
  //  tree : tree name.
  //  path : histogram name.
  //  dimension : histogram dimension
  //  create : ask to create the histo.
  //  x : expression to fill X
  //  y : expression to fill Y
  //
  // In case of creation :
  //  xn : x number of bins
  //  xmin : x min
  //  xmax : x max
  //  yn : y number of bins
  //  ymin : y min
  //  ymax : y max

  std::ostream& out = fSession.cout();
  fHistogram1D = 0;
  fHistogram2D = 0;

  // Default args :
  std::string arg_tree = "MemoryTree";  
  std::string arg_path; //Will take "histogram_<lib_type_name>"
  std::string arg_title; //Will take arg_path
  int arg_dimension = 1;
  std::string arg_x;
  std::string arg_y;
  //std::string arg_z;

  bool arg_create = true;
  int arg_xn = 100;
  double arg_xmin = 0;
  double arg_xmax = 1;
  int arg_yn = 100;
  double arg_ymin = 0;
  double arg_ymax = 1;

  if(aArgs.size()) {
    inlib::args args(aArgs);
    std::string svalue;
    int ivalue;
    double dvalue;
    bool bvalue;

    if(args.find("tree",svalue)) arg_tree = svalue;
    if(args.find("path",svalue)) arg_path = svalue;
    if(args.find("title",svalue)) arg_title = svalue;
    if(args.find("dimension",ivalue)) arg_dimension = ivalue;

    if(args.find("x",svalue)) arg_x = svalue;
    if(args.find("y",svalue)) arg_y = svalue;
    //if(args.find("z",svalue)) arg_z = svalue;

    if(args.find("create",bvalue)) arg_create = bvalue;

    if(args.find("xn",ivalue)) arg_xn = ivalue;
    if(args.find("xmin",dvalue)) arg_xmin = dvalue;
    if(args.find("xmax",dvalue)) arg_xmax = dvalue;

    if(args.find("yn",ivalue)) arg_yn = ivalue;
    if(args.find("ymin",dvalue)) arg_ymin = dvalue;
    if(args.find("ymax",dvalue)) arg_ymax = dvalue;
  }

  if((arg_dimension>=1)&&(arg_x.empty())) {
    Slash::Data::IAccessor* lib_type = firstType();
    if(!lib_type) {
      out << "OnXLab::HistogramAction::execute :" 
          << " Nothing in the selection."
          << std::endl;
      return "";
    }
    unsigned int propn = lib_type->numberOfProperties();
    if(propn==0) {
      out << "OnXLab::TupleAction::execute :" 
          << " first object type in the selection as no properties." 
          << std::endl;
      return "";
    }
    arg_x = lib_type->property(0)->name();
  }
  if((arg_dimension>=2)&&(arg_y.empty())) {
    Slash::Data::IAccessor* lib_type = firstType();
    if(!lib_type) {
      out << "OnXLab::HistogramAction::execute :" 
          << " Nothing in the selection."
          << std::endl;
      return "";
    }
    unsigned int propn = lib_type->numberOfProperties();
    if(propn<1) {
      out << "OnXLab::TupleAction::execute :" 
          << " first object type in the selection don't have two properties." 
          << std::endl;
    }
    arg_y = lib_type->property(1)->name();
  }

  AIDA::IAnalysisFactory* aida = 
    Slash_findManager(fSession,Slash_AnalysisFactory,AIDA::IAnalysisFactory);
  if(!aida) {
    out << "OnXLab::HistogramAction::execute :" 
        << " Can't find AIDA."
        << std::endl;
    return "";
  }
  AIDA::ITree* tree = Slash_findManager(fSession,arg_tree,AIDA::ITree);
  if(!tree) {
    out << "OnXLab::HistogramAction::execute :" 
        << " Can't find tree " << inlib::sout(arg_tree) << "." 
        << std::endl;
    return "";
  }

  if(arg_create) {

    Slash::Data::IAccessor* lib_type = firstType();
    if(!lib_type) {
      out << "OnXLab::HistogramAction::execute :" 
          << " Nothing in the selection."
          << std::endl;
      return "";
    }

    if(arg_path.empty()) arg_path = "histo_"+lib_type->name();
    if(arg_title.empty()) arg_title = arg_path;

    if(tree->find(arg_path)) tree->rm(arg_path);

    AIDA::IManagedObject* object = tree->find(arg_path);
    if(object) {
      out << "OnXLab::HistogramAction::execute :" 
          << " object " <<  inlib::sout(arg_path) 
          << " already exists in tree " << inlib::sout(arg_tree) << "."
          << std::endl;
      return "";
    }
    AIDA::IHistogramFactory* factory = aida->createHistogramFactory(*tree);
    if(!factory) {
      out << "OnXLab::HistogramAction::execute :" 
          << " Can't get an histogram factory for tree " 
          << inlib::sout(arg_tree) << "." 
          << std::endl;
      return "";
    }
    if(arg_dimension==1) {
      AIDA::IHistogram1D* histo = 
        factory->createHistogram1D(arg_path,arg_title,
                                   arg_xn,arg_xmin,arg_xmax);
      delete factory;
      if(!histo) {
        out << "OnXLab::HistogramAction::execute :" 
            << " Can't create histogram " << inlib::sout(arg_path) 
            << " in tree " << inlib::sout(arg_tree) << "." 
            << std::endl;
        return "";
      }
      fHistogram1D = histo;
    } else if(arg_dimension==2) {
      AIDA::IHistogram2D* histo = 
        factory->createHistogram2D(arg_path,arg_title,
                                   arg_xn,arg_xmin,arg_xmax,
                                   arg_yn,arg_ymin,arg_ymax);
      delete factory;
      if(!histo) {
        out << "OnXLab::HistogramAction::execute :" 
            << " Can't create histogram " << inlib::sout(arg_path) 
            << " in tree " << inlib::sout(arg_tree) << "." 
            << std::endl;
        return "";
      }
      fHistogram2D = histo;
    } else {
      delete factory;
      out << "OnXLab::HistogramAction::execute :" 
          << " Today only 1D or 2D histograms handled." 
          << std::endl;
      return "";
    }
  } else {
    AIDA::IManagedObject* object = tree->find(arg_path);
    if(!object) {
      out << "OnXLab::HistogramAction::execute :" 
          << " Can't find object " << inlib::sout(arg_path) 
          << " in tree " << inlib::sout(arg_tree) << "." 
          << std::endl;
      return "";
    }
    if(arg_dimension==1) {
      AIDA::IHistogram1D* histo = INLIB_CAST(*object,AIDA::IHistogram1D);
      if(!histo) {
        out << "OnXLab::HistogramAction::execute :" 
            << " Object " << inlib::sout(arg_path) << " not an IHistogram1D." 
            << std::endl;
        return "";
      }
      fHistogram1D = histo;
    } else if(arg_dimension==2) {
      AIDA::IHistogram2D* histo = INLIB_CAST(*object,AIDA::IHistogram2D);
      if(!histo) {
        out << "OnXLab::HistogramAction::execute :" 
            << " Object " << inlib::sout(arg_path) << " not an IHistogram2D." 
            << std::endl;
        return "";
      }
      fHistogram2D = histo;
    } else {
      out << "OnXLab::HistogramAction::execute :" 
          << " Today only 1D or 2D histograms handled." 
          << std::endl;
      return "";
    }
  }

  if(!fHistogram1D && !fHistogram2D) return "";

  // Fill :
  std::vector<std::string> exps(2);
  exps[0] = arg_x;
  exps[1] = arg_y;

  Lib::Processor** processors = new Lib::Processor*[arg_dimension];
  std::vector<std::string>* variables = 
    new std::vector<std::string>[arg_dimension];

  bool status = true;
  int count;
  for(count=0;count<arg_dimension;count++) {
    processors[count] = new Lib::Processor(exps[count],fSession.cout());
    if(!processors[count]->findVariables(variables[count])) {
      out << "OnXLab::HistogramAction::execute :" 
          << " can't find variables in " << inlib::sout(exps[count]) 
          << std::endl;
      status = false;
    }
    if(variables[count].size()==0) {
      out << "OnXLab::HistogramAction::execute :" 
          << " No variables found in " << exps[count] 
          << std::endl;
      status = false;
    } else {
      if(!processors[count]->compile(variables[count])) {
        out << "OnXLab::HistogramAction::execute :" 
            << " compilation of " << inlib::sout(exps[count]) 
            << " failed." 
            << std::endl;
        status = false;
      }
    }
  }

  if(status) {
    
    Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
    for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
      std::vector<Lib::Value> values;
      bool stop = false;
      for(int dim=0;dim<arg_dimension;dim++) {
        unsigned int xn = variables[dim].size();
        std::vector<Lib::Value> vals;
        for(unsigned int count=0;count<xn;count++) {
          Slash::Core::IValue* value = 
            h->type()->findValue(h->object(),variables[dim][count],h->tag());
          if(!value) {
            out << "OnXLab::HistogramAction::execute :" 
                << " Property not found : " 
                << variables[dim][count]
                << std::endl;
            stop = true;
            break;
          }
          //FIXME : work directly on the vals[]
          Lib::Value var;
          if(!var.copy(*value)) {
            out << "OnXLab::HistogramAction::execute :" 
                << " convertion of IValue " << inlib::sout(value->stype()) 
                << " to a Lib::Value failed."
                << std::endl;
            stop = true;
            delete value;
            break;
          }
          delete value;
          switch(var.type()) {
          case Slash::Core::IValue::INT:
            vals.push_back(var);
            break;
          case Slash::Core::IValue::DOUBLE:
            vals.push_back(var);
            break;
          case Slash::Core::IValue::ARRAY_DOUBLE:
            vals.push_back(var);
            break;
          case Slash::Core::IValue::STRING:
          case Slash::Core::IValue::VOID_STAR:
          case Slash::Core::IValue::BOOL:
          default:     
            out << "OnXLab::HistogramAction::execute :" 
                << " Can't work on a non mumerical property." 
                << std::endl;
            stop = true;
            break;
          }
          if(stop) break;
        }
        if(stop) break;

        Lib::Value var; //FIXME : use values.
        std::string serror;
        if(!processors[dim]->execute(vals,var,serror)) {
          out << "OnXLab::HistogramAction::execute :" 
              << " execution failed"
              << std::endl;
          out << serror << std::endl;
          stop = true;
          break;
        }
        values.push_back(var);
      }
      if(!stop) {
        if(fHistogram1D && (values.size()>=1)) {
	  Lib::Value& vx = values[0];
          if(vx.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
            std::vector<unsigned int> is;
            const std::vector<double>& stdvx = vx.get_array_double(is);
            unsigned int nx = stdvx.size();
            for(unsigned int ix=0;ix<nx;ix++) {
              fHistogram1D->fill(stdvx[ix],1);
            }
	  } else {
            double dvx;
            if(!vx.to_double(dvx)) {
              out << "OnXLab::HistogramAction::execute :" 
                  << " can't convert Variable to a double."
                  << std::endl;
            } else {
              fHistogram1D->fill(dvx,1);
            }
          }
        } else if(fHistogram2D && (values.size()>=2)) {
	  Lib::Value& vx = values[0];
	  Lib::Value& vy = values[1];
          if( (vx.type()==Slash::Core::IValue::ARRAY_DOUBLE) && 
              (vy.type()==Slash::Core::IValue::ARRAY_DOUBLE) ) {
            std::vector<unsigned int> isx;
            const std::vector<double>& stdvx = vx.get_array_double(isx);
            unsigned int nx = stdvx.size();
            std::vector<unsigned int> isy;
            const std::vector<double>& stdvy = vy.get_array_double(isy);
            unsigned int ny = stdvy.size();
            for(unsigned int ix=0;ix<nx;ix++) {
              for(unsigned int iy=0;iy<ny;iy++) {
                fHistogram2D->fill(stdvx[ix],stdvy[iy],1);
              }
            }
          } else if(vx.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
            double dvy;
            if(!vy.to_double(dvy)) {
              out << "OnXLab::HistogramAction::execute :" 
                  << " can't convert Variable to a double."
                  << std::endl;
            } else {
              std::vector<unsigned int> isx;
              const std::vector<double>& stdvx = vx.get_array_double(isx);
              unsigned int nx = stdvx.size();
              for(unsigned int ix=0;ix<nx;ix++) {
                fHistogram2D->fill(stdvx[ix],dvy,1);
              }
            }
          } else if(vy.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
            double dvx;
            if(!vx.to_double(dvx)) {
              out << "OnXLab::HistogramAction::execute :" 
                  << " can't convert Variable to a double."
                  << std::endl;
            } else {
              std::vector<unsigned int> isy;
              const std::vector<double>& stdvy = vy.get_array_double(isy);
              unsigned int ny = stdvy.size();
              for(unsigned int iy=0;iy<ny;iy++) {
                fHistogram2D->fill(dvx,stdvy[iy],1);
              }
            }
	  } else {
            double dvx;
            if(!vx.to_double(dvx)) {
              out << "OnXLab::HistogramAction::execute :" 
                  << " can't convert Variable to a double."
                  << std::endl;
            } else {
              double dvy;
              if(!vy.to_double(dvy)) {
                out << "OnXLab::HistogramAction::execute :" 
                    << " can't convert Variable to a double."
                    << std::endl;
              } else {
                fHistogram2D->fill(dvx,dvy,1);
              }
            }
          }
        }
      }
    }
    delete hit;
  }

  for(count=0;count<arg_dimension;count++) delete processors[count];
  delete [] processors;
  delete [] variables;

  return "";
}

Slash::Data::IAccessor* OnXLab::HistogramAction::firstType() const {
  Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
  if(!hit) return 0;
  const Slash::Data::IHandler* h = hit->handler();
  Slash::Data::IAccessor* type = h?h->type():0;
  delete hit;
  return type;
}

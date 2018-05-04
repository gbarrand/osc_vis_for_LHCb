
// this :
#include "TupleAction.h"

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Core/IValue.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Data/IHandlersIterator.h>
#include <Slash/Data/IProperty.h>
#include <Slash/Data/IHandler.h>

#include <Slash/Tools/Manager.h>
#include <Slash/Tools/Value.h>
#include <Slash/Tools/names.h>

#include <inlib/cast>
#include <inlib/args>


#include <AIDA/IConstants.h> //For AIDA_VERSION

OnXLab::TupleAction::TupleAction(
 Slash::Data::IProcessor& aAccessorManager
,Slash::Core::ISession& aSession
)
:fAccessorManager(aAccessorManager)
,fName("tuple")
,fSession(aSession)
,fTuple(0)
{}

OnXLab::TupleAction::~TupleAction(){}

void* OnXLab::TupleAction::cast(const std::string&) const {return 0;}

std::string OnXLab::TupleAction::name() const {return fName;}

Slash::Core::ISession& OnXLab::TupleAction::session() {return fSession;}

AIDA::ITuple* OnXLab::TupleAction::tuple() const {return fTuple;}

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/ITuple.h>

std::string OnXLab::TupleAction::execute(const std::vector<std::string>& aArgs){
  // aArgs :
  //   tree : tree name in which we create the tuple.
  //   path : tuple creation path.
  //   title : tuple creation title.
  //   options : tuple creation options.

  std::ostream& out = fSession.cout();
  fTuple = 0;

  // Default args :
  std::string arg_tree = "MemoryTree";  
  std::string arg_path; //Will take "tuple_<lib_type_name>"
  std::string arg_title; //Will take arg_path
  std::string arg_opts;

  if(aArgs.size()) {
    inlib::args args(aArgs);
    std::string svalue;
    if(args.find("tree",svalue)) arg_tree = svalue;
    if(args.find("path",svalue)) arg_path = svalue;
    if(args.find("title",svalue)) arg_title = svalue;
    if(args.find("options",svalue)) arg_opts = svalue;
  }

  AIDA::IAnalysisFactory* aida = 
    Slash_findManager(fSession,Slash_AnalysisFactory,AIDA::IAnalysisFactory);
  if(!aida) {
    out << "OnXLab::TupleAction::execute :" 
        << " Can't find AIDA."
        << std::endl;
    return "";
  }
  AIDA::ITree* tree = Slash_findManager(fSession,arg_tree,AIDA::ITree);
  if(!tree) {
    out << "OnXLab::TupleAction::execute :" 
        << " Can't find tree " << inlib::sout(arg_tree) << "." 
        << std::endl;
    return "";
  }

  Slash::Data::IAccessor* lib_type = firstType();
  if(!lib_type) {
    out << "OnXLab::TupleAction::execute :" 
        << " Nothing in the selection."
        << std::endl;
    return "";
  }

  std::string booking;
  unsigned int propn = lib_type->numberOfProperties();
  for(unsigned int propi=0;propi<propn;propi++) {
    Slash::Data::IProperty::Type ptype = lib_type->property(propi)->type();
    if(ptype==Slash::Data::IProperty::INTEGER) {
      if(booking.size()) booking += ",";
      booking += "int ";
      booking += lib_type->property(propi)->name();
    } else if(ptype==Slash::Data::IProperty::DOUBLE) {
      if(booking.size()) booking += ",";
      booking += "double ";
      booking += lib_type->property(propi)->name();
    } else if(ptype==Slash::Data::IProperty::BOOLEAN) {
      if(booking.size()) booking += ",";
      booking += "boolean ";
      booking += lib_type->property(propi)->name();
    } else if(ptype==Slash::Data::IProperty::STRING) {
      if(booking.size()) booking += ",";
      booking += "string ";
      booking += lib_type->property(propi)->name();

    } else if(ptype==Slash::Data::IProperty::POINTER) {
      if(booking.size()) booking += ",";
      //NOTE : the booking "long" is understood as a int64.
      //       See BatchLab::BaseTuple::convertType
      //       and Lib::Columns[.l,.y].
      booking += "long ";
      booking += lib_type->property(propi)->name();

    } else if(ptype==Slash::Data::IProperty::VECTOR_DOUBLE) {
      if(booking.size()) booking += ",";
      booking += "ITuple ";
      booking += lib_type->property(propi)->name();
      booking += " = {double value}";
    } else {
      out << "OnXLab::TupleAction::execute :" 
          << " for property " 
          << inlib::sout(lib_type->property(propi)->name()) 
          << ", property type " 
          << inlib::sout(lib_type->property(propi)->typeString()) 
          << " not supported as a AIDA::ITuple column. Continue anyway." 
          << std::endl;
      //return "";
    }
  }
  if(booking.empty()) {
    out << "OnXLab::TupleAction::execute :" 
        << " first object type in the selection as no properties." 
        << std::endl;
    return "";
  }

  if(arg_path.empty()) arg_path = "tuple_" + lib_type->name();
  if(arg_title.empty()) arg_title = arg_path;

  if(tree->find(arg_path)) tree->rm(arg_path);

  AIDA::IManagedObject* object = tree->find(arg_path);
  if(object) {
    out << "OnXLab::TupleAction::execute :" 
        << " object " <<  inlib::sout(arg_path) 
        << " already exists in tree " << inlib::sout(arg_tree) << "."
        << std::endl;
    return "";
  }

  AIDA::ITupleFactory* factory = aida->createTupleFactory(*tree);
  if(!factory) {
    out << "OnXLab::TupleAction::execute :" 
        << " Can't get a tuple factory for tree " 
        << inlib::sout(arg_tree) << "." 
        << std::endl;
    return "";
  }

 {AIDA::ITuple* tuple = 
    factory->create(arg_path,arg_title,booking,arg_opts);
  delete factory;
  if(!tuple) {
    out << "OnXLab::TupleAction::execute :" 
        << " Can't create tuple " << inlib::sout(arg_path) 
        << " in tree " << inlib::sout(arg_tree) << "." 
        << std::endl;
    return "";
  }
  fTuple = tuple;}
  if(!fTuple->columns()) {
    out << "OnXLab::TupleAction::execute :" 
        << " Tuple " << inlib::sout(arg_path) 
        << " in tree " << inlib::sout(arg_tree)
        << " has no columns. Delete it and stop here."
        << std::endl;
    delete fTuple;
    fTuple = 0;
    return "";
  }

  Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
  for(;const Slash::Data::IHandler* h = hit->handler();hit->next()) {
    if(h->type()!=lib_type) continue;    

    for(unsigned int propi=0;propi<propn;propi++) {
      Slash::Data::IProperty::Type ptype = lib_type->property(propi)->type();

      Slash::Core::IValue* value = 
        h->type()->findValue
          (h->object(),lib_type->property(propi)->name(),h->tag());
      if(!value) {
        out << "OnXLab::TupleAction::execute :" 
            << " Property not found : " 
            << lib_type->property(propi)->name()
            << std::endl;
        delete hit;
        return "";
      }
      Slash::Value var;
      if(!var.copy(*value)) {
        out << "OnXLab::TupleAction::execute :" 
            << " convertion of IValue " << inlib::sout(value->stype()) 
            << " to a Slash::Value failed."
            << std::endl;
        delete hit;
        delete value;
        return "";
      }
      delete value;

      if(ptype==Slash::Data::IProperty::INTEGER) {
        if(var.type()==Slash::Core::IValue::INT) {
          fTuple->fill(propi,var.get_int());
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }
      } else if(ptype==Slash::Data::IProperty::DOUBLE) {
        if(var.type()==Slash::Core::IValue::DOUBLE) {
          fTuple->fill(propi,var.get_double());
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }
      } else if(ptype==Slash::Data::IProperty::BOOLEAN) {
        if(var.type()==Slash::Core::IValue::BOOL) {
          fTuple->fill(propi,var.get_bool());
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }
      } else if(ptype==Slash::Data::IProperty::STRING) {
        if(var.type()==Slash::Core::IValue::STRING) {
          fTuple->fill(propi,var.get_string());
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }

      } else if(ptype==Slash::Data::IProperty::POINTER) {
        if(var.type()==Slash::Core::IValue::VOID_STAR) {
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
          fTuple->fill(propi,(long)var.get_void_star());
#else
          fTuple->fill(propi,(AIDA::int64)var.get_void_star());
#endif
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }

      } else if(ptype==Slash::Data::IProperty::VECTOR_DOUBLE) {
        if(var.type()==Slash::Core::IValue::ARRAY_DOUBLE) {
	  AIDA::ITuple* subTuple = fTuple->getTuple(propi);
          std::vector<unsigned int> is;
          const std::vector<double>& v = var.get_array_double(is);
          unsigned int vn = v.size();
          for(unsigned int vi=0;vi<vn;vi++) {
            subTuple->fill(0,v[vi]);
          }
        } else {
          out << "OnXLab::TupleAction::execute :" 
              << " Property type / value type mismatch."
              << std::endl;
          delete hit;
          return "";
        }
      } else {
        // Should never happen.
      }
    }
    fTuple->addRow();
  }
  delete hit;

  return "";
}

Slash::Data::IAccessor* OnXLab::TupleAction::firstType() const {
  Slash::Data::IHandlersIterator* hit = fAccessorManager.handlersIterator();
  if(!hit) return 0;
  const Slash::Data::IHandler* h = hit->handler();
  Slash::Data::IAccessor* type = h?h->type():0;
  delete hit;
  return type;
}

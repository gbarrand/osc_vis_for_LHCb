// this :
#include "Accessors.h"

// AIDA :
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>
#include <AIDA/IBaseHistogram.h>
#include <AIDA/IConstants.h>

// Slash :
#include <Slash/Core/ISession.h>
#include <Slash/Data/IProcessor.h>
#include <Slash/Data/IIterator.h>
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/Manager.h>
#include <Slash/Tools/Value.h>

// amanip :
#include <amanip/create.h>

#include <inlib/svalues>
#include <inlib/cast>
#include <inlib/args>
#include <inlib/sprintf>

// HEPVis :
#include <HEPVis/SbPlottableThings.h> //Interfaces

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::SbObject_Accessor::SbObject_Accessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("SbObject")
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("title",Slash::Data::IProperty::STRING);
}
OnXLab::SbObject_Accessor::~SbObject_Accessor(){}
std::string OnXLab::SbObject_Accessor::name() const{return fName;}

Slash::Data::IIterator* OnXLab::SbObject_Accessor::iterator(){return 0;}

Slash::Core::IValue* OnXLab::SbObject_Accessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void*){
  SbPlottableObject* object = (SbPlottableObject*)aData;

  if(aName=="name") {
    return new Slash::Value(object->getName());
  } else if(aName=="title") {
    return new Slash::Value(object->getLabel());
  } else {
    return new Slash::Value();
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::SbFunction1D_Accessor::SbFunction1D_Accessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("SbFunction1D")
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("title",Slash::Data::IProperty::STRING);
  addProperty("dimension",Slash::Data::IProperty::INTEGER);
  addProperty("#params",Slash::Data::IProperty::INTEGER);
  addProperty("codelet",Slash::Data::IProperty::STRING);
  addProperty("address",Slash::Data::IProperty::POINTER);
  addProperty("params",Slash::Data::IProperty::STRING);
}

OnXLab::SbFunction1D_Accessor::~SbFunction1D_Accessor(){}
std::string OnXLab::SbFunction1D_Accessor::name() const{return fName;}
Slash::Data::IIterator* OnXLab::SbFunction1D_Accessor::iterator(){return 0;}

Slash::Core::IValue* OnXLab::SbFunction1D_Accessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void*){
  SbPlottableFunction1D* object = (SbPlottableFunction1D*)aData;

  if(!object->isValid()) return new Slash::Value();

  //NOTE : special usage of cast() :
  std::string scast = std::string(Sb_data)+"::AIDA::IFunction";
  AIDA::IFunction* data = (AIDA::IFunction*)object->cast(scast.c_str());
  if(!data) {
    fSession.cout() << "OnXLab::SbFunction1D_Accessor::findValue :"
                    << " cast(\"data::AIDA::IFunction\") failed."
                    << std::endl;
    return new Slash::Value();
  }

  if(aName=="name") {
    return new Slash::Value(object->getName());
  } else if(aName=="title") {
    return new Slash::Value(data->title());
  } else if(aName=="dimension") {
    return new Slash::Value(data->dimension());
  } else if(aName=="#params") {
    return new Slash::Value(data->numberOfParameters());
  } else if(aName=="codelet") {
    return new Slash::Value(data->codeletString());
  } else if(aName=="address") {
    return new Slash::Value((void*)data);
  } else if(aName=="params") {
    std::vector<std::string> names = data->parameterNames();
    std::vector<double> vals = data->parameters();
    std::string value;
    unsigned int number = names.size();
    for(unsigned int index=0;index<number;index++){
      std::string s;
      inlib::sprintf(s,32,"%g",vals[index]);
      value += names[index] + " = " + s + ";";
    }
    return new Slash::Value(value);
  } else {
    return new Slash::Value();
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::SbFunction2D_Accessor::SbFunction2D_Accessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("SbFunction2D")
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("title",Slash::Data::IProperty::STRING);
  addProperty("dimension",Slash::Data::IProperty::INTEGER);
  addProperty("#params",Slash::Data::IProperty::INTEGER);
  addProperty("codelet",Slash::Data::IProperty::STRING);
  addProperty("address",Slash::Data::IProperty::POINTER);
  addProperty("params",Slash::Data::IProperty::STRING);
}

OnXLab::SbFunction2D_Accessor::~SbFunction2D_Accessor(){}
std::string OnXLab::SbFunction2D_Accessor::name() const{return fName;}
Slash::Data::IIterator* OnXLab::SbFunction2D_Accessor::iterator(){return 0;}

Slash::Core::IValue* OnXLab::SbFunction2D_Accessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void*){
  SbPlottableFunction2D* object = (SbPlottableFunction2D*)aData;

  if(!object->isValid()) return new Slash::Value();

  //NOTE : special usage of cast() :
  std::string scast = std::string(Sb_data)+"::AIDA::IFunction";
  AIDA::IFunction* data = (AIDA::IFunction*)object->cast(scast.c_str());
  if(!data) {
    fSession.cout() << "OnXLab::SbFunction2D_Accessor::findValue :"
                    << " cast(\"data::AIDA::IFunction\") failed."
                    << std::endl;
    return new Slash::Value();
  }

  if(aName=="name") {
    return new Slash::Value(object->getName());
  } else if(aName=="title") {
    return new Slash::Value(data->title());
  } else if(aName=="dimension") {
    return new Slash::Value(data->dimension());
  } else if(aName=="#params") {
    return new Slash::Value(data->numberOfParameters());
  } else if(aName=="codelet") {
    return new Slash::Value(data->codeletString());
  } else if(aName=="address") {
    return new Slash::Value((void*)data);
  } else if(aName=="params") {
    std::vector<std::string> names = data->parameterNames();
    std::vector<double> vals = data->parameters();
    std::string value;
    unsigned int number = names.size();
    for(unsigned int index=0;index<number;index++){
      std::string s;
      inlib::sprintf(s,32,"%g",vals[index]);
      value += names[index] + " = " + s + ";";
    }
    return new Slash::Value(value);
  } else {
    return new Slash::Value();
  }
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::SbBin1D_Accessor::SbBin1D_Accessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("SbBin1D")
{
  addProperty("index",Slash::Data::IProperty::INTEGER);
  addProperty("height",Slash::Data::IProperty::DOUBLE);
  addProperty("error",Slash::Data::IProperty::DOUBLE);
  addProperty("entries",Slash::Data::IProperty::INTEGER);
  addProperty("lowerEdge",Slash::Data::IProperty::DOUBLE);
  addProperty("upperEdge",Slash::Data::IProperty::DOUBLE);
  addProperty("name",Slash::Data::IProperty::STRING);
}
OnXLab::SbBin1D_Accessor::~SbBin1D_Accessor(){}
std::string OnXLab::SbBin1D_Accessor::name() const{return fName;}
Slash::Data::IIterator* OnXLab::SbBin1D_Accessor::iterator(){return 0;}

Slash::Core::IValue* OnXLab::SbBin1D_Accessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void* aTag){
  SbPlottableBins1D* object = (SbPlottableBins1D*)aData;
  if(!object->isValid()) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::SbBin1D_Accessor::value :"
        << " data object associated to plotted representation " 
        << "is no more valid." << std::endl;
    return new Slash::Value(); 
  }
  // aTag is a char* (see SoPlotter).
  std::string infos((char*)aTag);
  std::vector<int> vals;
  if(!inlib::values<int>(infos," ",false,vals)) return new Slash::Value();
  if(vals.size()!=1) return new Slash::Value();
  int i = vals[0];
  if(aName=="index") {
    return new Slash::Value(i);
  } else if(aName=="height") {
    return new Slash::Value((double)object->getBinSumOfWeights(i));
  } else if(aName=="error") {
    return new Slash::Value((double)object->getBinBar(i));
  } else if(aName=="entries") {
    return new Slash::Value(object->getBinNumberOfEntries(i));
  } else if(aName=="lowerEdge") {
    return new Slash::Value((double)object->getBinLowerEdge(i));
  } else if(aName=="upperEdge") {
    return new Slash::Value((double)object->getBinUpperEdge(i));
  } else if(aName=="name") {
    return new Slash::Value(std::string(object->getName()));
  } 
  return new Slash::Value();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::SbBin2D_Accessor::SbBin2D_Accessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("SbBin2D")
{
  addProperty("indexX",Slash::Data::IProperty::INTEGER);
  addProperty("indexY",Slash::Data::IProperty::INTEGER);
  addProperty("height",Slash::Data::IProperty::DOUBLE);
  addProperty("error",Slash::Data::IProperty::DOUBLE);
  addProperty("entries",Slash::Data::IProperty::INTEGER);
  addProperty("lowerEdgeX",Slash::Data::IProperty::DOUBLE);
  addProperty("upperEdgeX",Slash::Data::IProperty::DOUBLE);
  addProperty("lowerEdgeY",Slash::Data::IProperty::DOUBLE);
  addProperty("upperEdgeY",Slash::Data::IProperty::DOUBLE);
  addProperty("name",Slash::Data::IProperty::STRING);
}

OnXLab::SbBin2D_Accessor::~SbBin2D_Accessor(){}
std::string OnXLab::SbBin2D_Accessor::name() const{return fName;}
Slash::Data::IIterator* OnXLab::SbBin2D_Accessor::iterator(){return 0;}

Slash::Core::IValue* OnXLab::SbBin2D_Accessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void* aTag){
  SbPlottableBins2D* object = (SbPlottableBins2D*)aData;
  if(!object->isValid()) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::SbBin2D_Accessor::value :"
        << " data object associated to plotted representation " 
        << "is no more valid." << std::endl;
    return new Slash::Value(); 
  }
  // aTag is a char* (see SoPlotter).
  std::string infos((char*)aTag);
  std::vector<int> vals;
  if(!inlib::values<int>(infos," ",false,vals)) return new Slash::Value();
  if(vals.size()!=2) return new Slash::Value();
  int i = vals[0];
  int j = vals[1];
  if(aName=="indexX") {
    return new Slash::Value(i);
  } else if(aName=="indexY") {
    return new Slash::Value(j);
  } else if(aName=="height") {
    return new Slash::Value((double)object->getBinSumOfWeights(i,j));
  } else if(aName=="error") {
    return new Slash::Value((double)object->getBinBar(i,j));
  } else if(aName=="entries") {
    return new Slash::Value(object->getBinNumberOfEntries(i,j));
  } else if(aName=="lowerEdgeX") {
    return new Slash::Value((double)object->getBinLowerEdgeX(i));
  } else if(aName=="upperEdgeX") {
    return new Slash::Value((double)object->getBinUpperEdgeX(i));
  } else if(aName=="lowerEdgeY") {
    return new Slash::Value((double)object->getBinLowerEdgeY(j));
  } else if(aName=="upperEdgeY") {
    return new Slash::Value((double)object->getBinUpperEdgeY(j));
  } else if(aName=="name") {
    return new Slash::Value(std::string(object->getName()));
  } 
  return new Slash::Value();
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// AIDA :
#include <AIDA/ITree.h>
#include <AIDA/IManagedObject.h>

// BatchLab :
#include <BatchLab/Core/MemoryTree.h>

OnXLab::ManagedObjectAccessor::ManagedObjectAccessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("ManagedObject")
{
  addProperty("name",Slash::Data::IProperty::STRING);
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  addProperty("type",Slash::Data::IProperty::STRING,20);
#endif
  addProperty("address",Slash::Data::IProperty::POINTER);
}
OnXLab::ManagedObjectAccessor::~ManagedObjectAccessor(){}
std::string OnXLab::ManagedObjectAccessor::name() const{return fName;}

namespace OnXLab {
  class ManagedObjectIterator : public Slash::Data::IIterator {
  public: //Slash::Data::IIterator
    virtual Slash::Data::IAccessor::Data object() {
      if(fIterator==fList.end()) return 0;
      return *fIterator;
    }
    virtual void next() { ++fIterator;}
    virtual void* tag() { return 0;}
  public:
    ManagedObjectIterator(const std::vector<AIDA::IManagedObject*>& aList):fList(aList){
      fIterator = fList.begin();
    }
  private:
    std::vector<AIDA::IManagedObject*> fList; // Local copy.
    std::vector<AIDA::IManagedObject*>::iterator fIterator;
  };
}

Slash::Data::IIterator* OnXLab::ManagedObjectAccessor::iterator(){
  // Returned object should be deleted by the receiver.
  BatchLab::MemoryTree* memoryTree = 
    Slash_findManager(fSession,"MemoryTree",BatchLab::MemoryTree);
  if(!memoryTree) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::ManagedObjectAccessor::iterator :"
        << " MemoryTree not found." << std::endl;
    return 0;
  }
  std::vector<AIDA::IManagedObject*> vec = memoryTree->allObjects();
  return new ManagedObjectIterator(vec);
}

Slash::Core::IValue* OnXLab::ManagedObjectAccessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void*){
  AIDA::IManagedObject* object = (AIDA::IManagedObject*)aData;
  if(aName=="name") {
    return new Slash::Value(object->name());
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
  } else if(aName=="type") {
    return new Slash::Value(object->type());
#endif
  } else if(aName=="address") {
    return new Slash::Value((void*)object);
  } else {
    return new Slash::Value();
  }
}
void OnXLab::ManagedObjectAccessor::beginVisualize(const Slash::UI::IStyle&) {}
void OnXLab::ManagedObjectAccessor::beginVisualize() {}
void OnXLab::ManagedObjectAccessor::endVisualize() {}
void OnXLab::ManagedObjectAccessor::visualize(Slash::Data::IAccessor::Data aData,void*){
  std::string where;
 {const std::vector<std::string>& eargs = executeArguments();
  if(eargs.size()) {
    inlib::args args(eargs);
    args.find("where",where);
  }}

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(fSession);
  if(!aida) return;
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,where);        
  if(!plotter) return;
  AIDA::IPlotterRegion& region = plotter->currentRegion();
  AIDA::IManagedObject* object = (AIDA::IManagedObject*)aData;
  if(AIDA::IBaseHistogram* h = INLIB_CAST(*object,AIDA::IBaseHistogram)) {
    region.plot(*h);
  } else if(AIDA::IFunction* f = INLIB_CAST(*object,AIDA::IFunction)) {
    region.plot(*f);
  }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::FunctionAccessor::FunctionAccessor(Slash::Core::ISession& aSession)
: Slash::BaseAccessor(aSession.cout())
,fSession(aSession)
,fName("Function")
{
  addProperty("name",Slash::Data::IProperty::STRING);
  addProperty("title",Slash::Data::IProperty::STRING);
  addProperty("dimension",Slash::Data::IProperty::INTEGER);
  addProperty("#params",Slash::Data::IProperty::INTEGER);
  addProperty("codelet",Slash::Data::IProperty::STRING);
  addProperty("address",Slash::Data::IProperty::POINTER);
  addProperty("params",Slash::Data::IProperty::STRING);
}
OnXLab::FunctionAccessor::~FunctionAccessor(){}
std::string OnXLab::FunctionAccessor::name() const{return fName;}

namespace OnXLab {
  class FunctionIterator : public Slash::Data::IIterator {
  public: //Slash::Data::IIterator
    virtual Slash::Data::IAccessor::Data object() {
      if(fIterator==fList.end()) return 0;
      return *fIterator;
    }
    virtual void next() { ++fIterator;}
    virtual void* tag() { return 0;}
  public:
    FunctionIterator(const std::vector<AIDA::IFunction*>& aList):fList(aList){
      fIterator = fList.begin();
    }
  private:
    std::vector<AIDA::IFunction*> fList; // Local copy.
    std::vector<AIDA::IFunction*>::iterator fIterator;
  };
}

Slash::Data::IIterator* OnXLab::FunctionAccessor::iterator(){
  BatchLab::MemoryTree* memoryTree = 
    Slash_findManager(fSession,"MemoryTree",BatchLab::MemoryTree);
  if(!memoryTree) {
    std::ostream& out = fSession.cout();
    out << "OnXLab::FunctionAccessor::iterator :"
        << " MemoryTree not found." << std::endl;
    return 0;
  }
  std::vector<AIDA::IManagedObject*> vec = memoryTree->allObjects();
  std::vector<AIDA::IFunction*> funcs;
  for(unsigned int index=0;index<vec.size();index++) {
    AIDA::IFunction* obj = INLIB_CAST(*(vec[index]),AIDA::IFunction);
    if(obj) funcs.push_back(obj);
  }
  return new FunctionIterator(funcs);
}
Slash::Core::IValue* OnXLab::FunctionAccessor::findValue(Slash::Data::IAccessor::Data aData,const std::string& aName,void*){
  AIDA::IFunction* object = (AIDA::IFunction*)aData;
  if(aName=="name") {
    AIDA::IManagedObject* mobject = INLIB_CAST(*object,AIDA::IManagedObject);
    return new Slash::Value(mobject?mobject->name():"none");
  } else if(aName=="title") {
    return new Slash::Value(object->title());
  } else if(aName=="dimension") {
    return new Slash::Value(object->dimension());
  } else if(aName=="#params") {
    return new Slash::Value(object->numberOfParameters());
  } else if(aName=="codelet") {
    return new Slash::Value(object->codeletString());
  } else if(aName=="address") {
    return new Slash::Value((void*)object);
  } else if(aName=="params") {
    std::vector<std::string> names = object->parameterNames();
    std::vector<double> vals = object->parameters();
    std::string value;
    unsigned int number = names.size();
    for(unsigned int index=0;index<number;index++){
      std::string s;
      inlib::sprintf(s,32,"%g",vals[index]);
      value += names[index] + " = " + s + ";";
    }
    return new Slash::Value(value);
  } else {
    return new Slash::Value();
  }
}
void OnXLab::FunctionAccessor::beginVisualize(const Slash::UI::IStyle&) {}
void OnXLab::FunctionAccessor::beginVisualize() {}
void OnXLab::FunctionAccessor::endVisualize() {}
void OnXLab::FunctionAccessor::visualize(Slash::Data::IAccessor::Data aData,void*){
  std::string where;
 {const std::vector<std::string>& eargs = executeArguments();
  if(eargs.size()) {
    inlib::args args(eargs);
    args.find("where",where);
  }}

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(fSession);
  if(!aida) return;
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,where);        
  if(!plotter) return;
  AIDA::IPlotterRegion& region = plotter->currentRegion();
  AIDA::IFunction* object = (AIDA::IFunction*)aData;
  region.plot(*object);
}


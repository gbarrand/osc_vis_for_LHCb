
// Slash :
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/UI.h>
#include <inlib/cast>

// amanip :
#include <amanip/create.h>

#include "inlib"

namespace OnXLab {

class FitterManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::FitterManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  FitterManager(Slash::Core::ISession&)
  :fName("OnXLab_FitterManager")
  ,fFittableTree(0),fFittable(0)
  ,fFunctionTree(0),fFunction(0)
  {}
  virtual ~FitterManager(){}
  void setFittable(AIDA::ITree* aTree,AIDA::IBaseHistogram* aFittable) { 
    fFittableTree = aTree;
    fFittable = aFittable;
  }
  AIDA::ITree* fittableTree() const { return fFittableTree;}
  AIDA::IBaseHistogram* fittable() const { return fFittable;}
  void setFunction(AIDA::ITree* aTree,AIDA::IFunction* aFunction) { 
    fFunctionTree = aTree;
    fFunction = aFunction;
  }
  AIDA::ITree* functionTree() const { return fFunctionTree;}
  AIDA::IFunction* function() const { return fFunction;}
private:
  std::string fName;
  AIDA::ITree* fFittableTree;
  AIDA::IBaseHistogram* fFittable;
  AIDA::ITree* fFunctionTree;
  AIDA::IFunction* fFunction;
};
}

static OnXLab::FitterManager* find_fitter_manager(Slash::UI::IUI& aUI){
  return Slash_findManager
    (aUI.session(),"OnXLab_FitterManager",OnXLab::FitterManager);
}

static bool getInfos(Slash::UI::IUI& aUI,AIDA::ITree*& aFittableTree,AIDA::IBaseHistogram*& aFittable,AIDA::ITree*& aFunctionTree,AIDA::IFunction*& aFunction){
  OnXLab::FitterManager* mgr = find_fitter_manager(aUI);
  if(!mgr) {
    aFittableTree = 0;aFittable = 0;
    aFunctionTree = 0;aFunction = 0;
    return false;
  }
  aFittableTree = mgr->fittableTree();
  aFittable = mgr->fittable();
  aFunctionTree = mgr->functionTree();
  aFunction = mgr->function();
  return true;
}

extern "C" {

void OnXLab_Fitter_get_fittable(Slash::UI::IUI& aUI) {

  OnXLab::FitterManager* mgr = find_fitter_manager(aUI);
  if(!mgr) {
    mgr = new OnXLab::FitterManager(aUI.session());
    aUI.session().addManager(mgr);
  }

  std::string selection;
  std::string stree;
  if(!aUI.session().parameterValue("OnXLab.tree",stree)) return;
  if(!aUI.parameterValue(stree+".selection",selection)) return;  

  AIDA::ITree* tree = 0;
  AIDA::IBaseHistogram* fittable = 0;
  std::string manager,path;
 {std::string basename;
  AIDA::IManagedObject* object = 
    find_aida_object(selection,aUI.session(),tree,manager,path,basename);
  if(tree) {
    if(object) {
      fittable = INLIB_CAST(*object,AIDA::IBaseHistogram);
      if(!fittable) {
        aUI.echo(basename + " is not an AIDA::IBaseHistogram.");
      }
    }
  }}

  mgr->setFittable(tree,fittable);

  if(fittable) {
    aUI.setParameter("OnXLab_Fitter_fittable.value",path);
  } else {
    aUI.setParameter("OnXLab_Fitter_fittable.value","");
  }

}

void OnXLab_Fitter_get_function(Slash::UI::IUI& aUI) {
  OnXLab::FitterManager* mgr = find_fitter_manager(aUI);
  if(!mgr) {
    mgr = new OnXLab::FitterManager(aUI.session());
    aUI.session().addManager(mgr);
  }

  std::string selection;
  std::string stree;
  if(!aUI.session().parameterValue("OnXLab.tree",stree)) return;
  if(!aUI.parameterValue(stree+".selection",selection)) return;  

  AIDA::ITree* tree = 0;
  AIDA::IFunction* func = 0;
  std::string manager,path;
 {std::string basename;
  AIDA::IManagedObject* object = 
    find_aida_object(selection,aUI.session(),tree,manager,path,basename);
  if(tree) {
    if(object) {
      func = INLIB_CAST(*object,AIDA::IFunction);
      if(!func) {
        aUI.echo(basename + " is not a function.");
      }
    }
  }}

  mgr->setFunction(tree,func);

  if(func) {
    aUI.setParameter("OnXLab_Fitter_function.value",path);
  } else {
    aUI.setParameter("OnXLab_Fitter_function.value","");
  }

  aUI.destroyComponent("OnXLab_Fitter_params_container");

  if(!func) return;

  std::vector<std::string> params = func->parameterNames();

  std::string sxml;
  sxml += "<widget class=\"VContainer\" name=\"OnXLab_Fitter_params_container\">";

  sxml += "  <widget class=\"HBox\">";
  sxml += "    <expand>second</expand>";
  sxml += "    <sizeOfFixed>30</sizeOfFixed>";
  sxml += "    <widget class=\"Label\">";
  sxml += "      <label>Param</label>";
  sxml += "    </widget>";
  sxml += "    <widget class=\"HBox\">";
  sxml += "      <expand>both</expand>";
  sxml += "      <widget class=\"Label\">";
  sxml += "        <label>Start</label>";
  sxml += "      </widget>";
  sxml += "      <widget class=\"Label\">";
  sxml += "        <label>Step</label>";
  sxml += "      </widget>";
  sxml += "    </widget>";
  sxml += "  </widget>";

  for(unsigned int index=0;index<params.size();index++) {
    std::string sindex = inlib::to<int>((int)index);

  sxml += "  <widget class=\"HBox\">";
  sxml += "    <expand>second</expand>";
  sxml += "    <sizeOfFixed>30</sizeOfFixed>";
  sxml += "    <widget class=\"Label\">";
  sxml += "      <label>"+params[index]+"</label>";
  sxml += "    </widget>";
  sxml += "    <widget class=\"HBox\">";
  sxml += "      <expand>both</expand>";
  sxml += "      <widget class=\"Entry\" name=\"OnXLab_Fitter_param_"+sindex+"_start\">";
  sxml += "        <value>0</value>";
  sxml += "      </widget>";
  sxml += "      <widget class=\"Entry\" name=\"OnXLab_Fitter_param_"+sindex+"_step\">";
  sxml += "        <value>0.1</value>";
  sxml += "      </widget>";
  sxml += "    </widget>";
  sxml += "  </widget>";
  }

  sxml += "</widget>";
  aUI.load("OnXLab_Fitter_params",sxml,false);

  Slash::UI::IWidget* w = aUI.findWidget("OnXLab_Fitter_params_container");
  if(w) w->show();

}

void OnXLab_Fitter_fit(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {

  AIDA::ITree* fittableTree = 0;
  AIDA::IBaseHistogram* fittable = 0;
  AIDA::ITree* functionTree = 0;
  AIDA::IFunction* function = 0;

  if(!getInfos(aUI,fittableTree,fittable,functionTree,function)) return;

  if(!fittable) return;
  if(!function) return;

  //FIXME : check validity of fittable and function.

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
  if(!aida) return;

  std::string engine;
  if(!aUI.parameterValue("OnXLab_Fitter_engine.value",engine)) return;

  AIDA::IFitter* fitter = amanip::create_fitter(*aida,engine);
  if(!fitter) return;

  fitter->resetParameterSettings();

 {std::vector<std::string> params = function->parameterNames();
  std::vector<double> starts;
  std::vector<double> steps;
  for(unsigned int index=0;index<params.size();index++) {
    std::string sindex = inlib::to<int>((int)index);
    double start;
    if(!Slash::parameter<double>
       (aUI,"OnXLab_Fitter_param_"+sindex+"_start.value",start)) {
      delete fitter;
      return;
    }
    starts.push_back(start);
    double step;
    if(!Slash::parameter<double>
       (aUI,"OnXLab_Fitter_param_"+sindex+"_step.value",step)) {
      delete fitter;
      return;
    }
    steps.push_back(step);
    //printf("debug : index %d start %g step %g\n",index,start,step);

    AIDA::IFitParameterSettings& ps = 
      fitter->fitParameterSettings(params[index]);
    ps.setStepSize(step);
  }
  function->setParameters(starts);}

  AIDA::IFitResult* fitResult = fitter->fit(*fittable,*function);

  if(!fitResult) {
    std::ostream& out = aUI.session().cout();
    out << "OnXLab_Fitter_fit : "
        << " fit failed."
        << std::endl;
    delete fitter;
    return;
  }

  AIDA::IFunction& func = fitResult->fittedFunction();
 {std::vector<double> params = func.parameters();
  for(unsigned int index=0;index<params.size();index++) {
    std::string sindex = inlib::to<int>((int)index);
    Slash::set_parameter<double>
      (aUI,"OnXLab_Fitter_param_"+sindex+"_start.value",params[index]);
  }}

  bool plot;
  if(!Slash::parameter(aUI,"OnXLab_Fitter_plot.value",plot)) return;  

  if(plot) {
    AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
    if(plotter) {
      AIDA::IPlotterRegion& region = plotter->currentRegion();
      region.plot(func);
      delete plotter;
    }
  }

  delete fitResult;

  delete fitter;
}

} // extern "C"


extern "C" {
  void onxlab_kuip_def_();
  void onxlab_kuip(void*);
}

#include <AIDA/IAnalysisFactory.h>
#include <AIDA/ITreeFactory.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ITupleFactory.h>
#include <AIDA/IFunctionFactory.h>
#include <AIDA/IFunction.h>
#include <AIDA/IManagedObject.h>
#include <AIDA/ITree.h>
#include <AIDA/IAxis.h>
#include <AIDA/IHistogram1D.h>
#include <AIDA/IHistogram2D.h>
#include <AIDA/ICloud1D.h>
#include <AIDA/ICloud2D.h>
#include <AIDA/ICloud3D.h>
#include <AIDA/ITuple.h>
#include <AIDA/IEvaluator.h>
#include <AIDA/IFilter.h>
#include <AIDA/IFitter.h>
#include <AIDA/IPlotter.h>
#include <AIDA/IPlotterRegion.h>
#include <AIDA/IPlotterFactory.h>
#include <AIDA/IFitFactory.h>
#include <AIDA/IFitter.h>
#include <AIDA/IFitResult.h>

#include <amanip/find.h>

#include <Slash/Core/ISession.h>
#include <Slash/Core/IManager.h>
#include <Slash/Core/IOpaqueManager.h>
#include <Slash/UI/IUI.h>
#include <Slash/Tools/Managers.h>
#include <Slash/Tools/UI.h>
#include <Slash/Tools/AIDA.h>
#include <inlib/cast>

#include <kuip/ksys.h>
#include <kuip/kfor.h>
#include <kuip/kuser.h>
#include <kuip/klink.h>

extern "C" {
  void OnXLabKuipInitialize(Slash::Core::ISession&);
  void OnXLabKuipFinalize(Slash::Core::ISession&);
}

//////////////////////////////////////////////////////////////////////////////
template <class aClass> aClass* findObject(
 Slash::Core::IOpaqueManager& aManager
,const std::string& aNAME
,std::ostream& aOut
,const std::string& aCmd
,const std::string& aClassName
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  AIDA::IManagedObject* aidaObject = 
    (AIDA::IManagedObject*)aManager.find(aNAME);
  if(!aidaObject) {
    aOut << "onxlab_kuip_ : for command :" << std::endl;
    aOut << " " << aCmd << std::endl;
    aOut << " Object " << inlib::sout(aNAME) << " not found." << std::endl;
    return 0;
  }

  //FIXME : have to check the validity of the aidaObject !!!!

  //aClass* object = (aClass*)aidaObject->cast(aClassName);
  aClass* object = dynamic_cast<aClass*>(aidaObject);
  if(!object) {
    aOut << "onxlab_kuip_ : for command :" << std::endl;
    aOut << " " << aCmd << std::endl;
    aOut << " Object " << inlib::sout(aNAME) 
        << " not an " << aClassName << "." 
        << std::endl;
    return 0;
  }
  return object;
}

void alreadyExists(std::ostream& aOut,const std::string& aCommand,const std::string& aName){
  aOut << "onxlab_kuip_ : for command :" << std::endl;
  aOut << " " << aCommand << std::endl;
  aOut << " Object " << inlib::sout(aName) << " already exists." << std::endl;
}

void notFound(std::ostream& aOut,const std::string& aCommand,const std::string& aName){
  aOut << "onxlab_kuip_ : for command :" << std::endl;
  aOut << " " << aCommand << std::endl;
  aOut << " Object " << inlib::sout(aName) << " not found." << std::endl;
}

void notA(std::ostream& aOut,const std::string& aCommand,const std::string& aName,const std::string& aClass) {
  aOut << "onxlab_kuip_ : for command :" << std::endl;
  aOut << " " << aCommand << std::endl;
  aOut << " Object " << inlib::sout(aName) 
       << " not an " << inlib::sout(aClass) << "."
       << std::endl;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLabKuipInitialize(
 Slash::Core::ISession&
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  onxlab_kuip_def_();
}
//////////////////////////////////////////////////////////////////////////////
void OnXLabKuipFinalize(
 Slash::Core::ISession&
) 
//////////////////////////////////////////////////////////////////////////////
// We should remove here OnXLab objects created through KUIP.
// Managers are deleted by OnX/~Session.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}

//////////////////////////////////////////////////////////////////////////////
void onxlab_kuip(
 void* aSession
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Slash::Core::ISession* session = (Slash::Core::ISession*)aSession;

  std::ostream& out = session->cout();

  AIDA::IAnalysisFactory* af = Slash::find_AIDA(*session);
  if(!af) {
    out << "onxlab_kuip :"
        << "AIDA not found."
        << std::endl;
    return;
  }

  AIDA::ITree* memTree = amanip::find_mem(*af);
  if(!memTree) {
    out << "onxlab_kuip :"
        << "MemoryTree not found."
        << std::endl;
    return;
  }

  Slash::UI::IUI* ui = Slash::find_UI(*session);
  if(!ui) {
    out << "onxlab_kuip :"
        << "UI not found."
        << std::endl;
    return;
  }

  Slash::Core::IOpaqueManager* kuipManager = 
    Slash_findManager(*session,"KuipManager",Slash::Core::IOpaqueManager);
  if(!kuipManager) {
    out << "onxlab_kuip :"
        << "KuipManager not found."
        << std::endl;
    return;
  }

  std::string cmd_path = ku_path() ? ku_path() : "";
  //std::string cmd_name = ku_name();
  //printf("debug : onxlab_kuip : %s\n",cmd_path.c_str());

  ///////////////////////////////////////////////////////
  /// TREE //////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/AIDA/TREE/RIO/NEW") {
    std::string NAME = ku_gets();
    std::string FILENAME = ku_gets();
    if(session->findManager(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::ITreeFactory* fac = af->createTreeFactory();
    AIDA::ITree* tree = fac->create(FILENAME,"ROOT",1,0);
    delete fac;
    if(!tree) {
      out << "onxlab_kuip_ : for command :" << std::endl;
      out << " " << cmd_path << std::endl;
      out << " Can't create tree " << inlib::sout(NAME) 
          << " for file " << inlib::sout(FILENAME)
          << std::endl;
      return;
    }
    kuipManager->add(tree,NAME); // KUIP is not the owner.
    return;
  }

  if(cmd_path=="/AIDA/TREE/DELETE") {
    std::string NAME = ku_gets();
    kuipManager->destroy(NAME);
    return;
  }

  if(cmd_path=="/AIDA/TREE/CD") {
    std::string NAME = ku_gets();
    std::string CHPATH = ku_gets();
    AIDA::ITree* tree = (AIDA::ITree*)kuipManager->find(NAME);
    if(!tree) {
      notFound(out,cmd_path,NAME);
      return;
    }
    if(CHPATH.size()) tree->cd(CHPATH);
    return;
  }

  if(cmd_path=="/AIDA/TREE/LS") {
    std::string NAME = ku_gets();
    AIDA::ITree* tree = (AIDA::ITree*)kuipManager->find(NAME);
    if(!tree) {
      notFound(out,cmd_path,NAME);
      return;
    }
    tree->ls();
    return;
  }

  ///////////////////////////////////////////////////////
  /// HISTOGRAM /////////////////////////////////////////
  ///////////////////////////////////////////////////////

  if(cmd_path=="/AIDA/HISTOGRAM/1D/GET") {
    std::string NAME = ku_gets();
    std::string TREENAME = ku_gets();
    if(kuipManager->find(NAME)) {
      notFound(out,cmd_path,NAME);
      return;
    }
    AIDA::ITree* tree = (AIDA::ITree*)kuipManager->find(TREENAME);
    if(!tree) {
      notFound(out,cmd_path,TREENAME);
      return;
    }
    AIDA::IManagedObject* mobject = tree->find(NAME);
    if(mobject) {
      if(!INLIB_CAST(*mobject,AIDA::IHistogram1D)) {
        notA(out,cmd_path,NAME,"AIDA::IHistogram1D");
        return;
      }
      kuipManager->add(mobject,NAME); // KUIP is not the owner.
    }
    return;
  }

  if(cmd_path=="/AIDA/HISTOGRAM/1D/NEW") {
    std::string NAME = ku_gets();
    std::string LABEL = ku_gets();
    int NCX = ku_geti();
    double XMIN = ku_getr();
    double XMAX = ku_getr();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::IHistogramFactory* fac = af->createHistogramFactory(*memTree);
    AIDA::IHistogram1D* object = 
      fac->createHistogram1D(NAME,LABEL,NCX,XMIN,XMAX);
    if(object) {
      AIDA::IManagedObject* mobject = INLIB_CAST(*object,AIDA::IManagedObject);
      if(!mobject) {
        notA(out,cmd_path,NAME,"AIDA::IHistogram1D");
      } else {
        kuipManager->add(mobject,NAME); // KUIP is not the owner.
      }
    }
    delete fac;
    return;
  }

  if(cmd_path=="/AIDA/HISTOGRAM/2D/GET") {
    std::string NAME = ku_gets();
    std::string TREENAME = ku_gets();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::ITree* tree = (AIDA::ITree*)kuipManager->find(TREENAME);
    if(!tree) {
      notFound(out,cmd_path,TREENAME);
      return;
    }
    AIDA::IManagedObject* mobject = tree->find(NAME);
    if(mobject) {
      if(!INLIB_CAST(*mobject,AIDA::IHistogram2D)) {
        notA(out,cmd_path,NAME,"AIDA::IHistogram2D");
        return;
      }
      kuipManager->add(mobject,NAME); // KUIP is not the owner.
    }
    return;
  }

  if(cmd_path=="/AIDA/HISTOGRAM/2D/NEW") {
    std::string NAME = ku_gets();
    std::string LABEL = ku_gets();
    int NCX = ku_geti();
    double XMIN = ku_getr();
    double XMAX = ku_getr();
    int NCY = ku_geti();
    double YMIN = ku_getr();
    double YMAX = ku_getr();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::IHistogramFactory* fac = af->createHistogramFactory(*memTree);
    AIDA::IHistogram2D* object = 
      fac->createHistogram2D(NAME,LABEL,NCX,XMIN,XMAX,NCY,YMIN,YMAX);
    if(object) {
      AIDA::IManagedObject* mobject = INLIB_CAST(*object,AIDA::IManagedObject);
      if(!mobject) {
        notA(out,cmd_path,NAME,"AIDA::IManagedObject");
      } else {
        kuipManager->add(mobject,NAME); // KUIP is not the owner.
      }
    }
    delete fac;
    return;
  }

  if(cmd_path=="/AIDA/HISTOGRAM/SHOW") {
    std::string NAME = ku_gets();
    AIDA::IHistogram* histo = 
      findObject<AIDA::IHistogram>(*kuipManager,
                                   NAME,out,cmd_path,"AIDA::IHistogram");
    if(!histo) return;
    AIDA::IPlotterFactory* fac = af->createPlotterFactory();
    if(fac) {
      AIDA::IPlotter* plotter = fac->create("");        
      if(plotter) {
        plotter->currentRegion().plot(*histo);
        delete plotter;
      }
      delete fac;
    }
    return;
  }

  ///////////////////////////////////////////////////////
  /// TUPLE /////////////////////////////////////////////
  ///////////////////////////////////////////////////////
  if(cmd_path=="/AIDA/TUPLE/GET") {
    std::string NAME = ku_gets();
    std::string TREENAME = ku_gets();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::ITree* tree = (AIDA::ITree*)kuipManager->find(TREENAME);
    if(!tree) {
      notFound(out,cmd_path,TREENAME);
      return;
    }
    AIDA::ITupleFactory* fac = af->createTupleFactory(*tree);
    AIDA::ITuple* object = fac->create(NAME,NAME,"");
    if(object) {
      AIDA::IManagedObject* mobject = INLIB_CAST(*object,AIDA::IManagedObject);
      if(!mobject) {
        notA(out,cmd_path,NAME,"AIDA::IManagedObject");
      } else {
        kuipManager->add(mobject,NAME);
      }
    }
    delete fac;
    return;
  }

  if(cmd_path=="/AIDA/TUPLE/SHOW") {
    std::string NAME = ku_gets();
    return;
  }

  if(cmd_path=="/AIDA/TUPLE/PROJECT/1D") {
    std::string NAMET = ku_gets();
    std::string NAMEH = ku_gets();
    std::string EXP = ku_gets();
    std::string CUT = ku_gets();
    AIDA::ITuple* tuple = 
      findObject<AIDA::ITuple>(*kuipManager,
                               NAMET,out,cmd_path,"AIDA::ITuple");
    if(!tuple) return;
    AIDA::IHistogram1D* h1D = 
      findObject<AIDA::IHistogram1D>(*kuipManager,
                                     NAMEH,out,cmd_path,"AIDA::IHistogram1D");
    if(!h1D) return;
    AIDA::ITupleFactory* fac = af->createTupleFactory(*memTree);
    if(fac) {
      AIDA::IEvaluator* evaluator = fac->createEvaluator(EXP);
      if(evaluator) {
        AIDA::IFilter* filter = fac->createFilter(CUT);
        if(filter) {
          tuple->project(*h1D,*evaluator,*filter);
          delete filter;        
        }
        delete evaluator;
      }
      delete fac;
    }
    return;
  }

  if(cmd_path=="/AIDA/TUPLE/PROJECT/2D") {
    std::string NAMET = ku_gets();
    std::string NAMEH = ku_gets();
    std::string XEXP = ku_gets();
    std::string YEXP = ku_gets();
    std::string CUT = ku_gets();
    AIDA::ITuple* tuple = 
      findObject<AIDA::ITuple>(*kuipManager,
                               NAMET,out,cmd_path,"AIDA::ITuple");
    if(!tuple) return;
    AIDA::IHistogram2D* h2D = 
      findObject<AIDA::IHistogram2D>(*kuipManager,
                                     NAMEH,out,cmd_path,"AIDA::IHistogram2D");
    if(!h2D) return;
    AIDA::ITupleFactory* fac = af->createTupleFactory(*memTree);
    if(fac) {
      AIDA::IEvaluator* evaluatorX = fac->createEvaluator(XEXP);
      if(evaluatorX) {
        AIDA::IEvaluator* evaluatorY = fac->createEvaluator(YEXP);
        if(evaluatorY) {
          AIDA::IFilter* filter = fac->createFilter(CUT);
          if(filter) {
            tuple->project(*h2D,*evaluatorX,*evaluatorY,*filter);
            delete filter;        
          }
          delete evaluatorY;
        }
        delete evaluatorX;
      }
      delete fac;
    }
    return;
  }

  ///////////////////////////////////////////////////////
  /// FUNCTION //////////////////////////////////////////
  ///////////////////////////////////////////////////////

  if(cmd_path=="/AIDA/FUNCTION/SHOW") {
    std::string NAME = ku_gets();
    AIDA::IFunction* function = 
      findObject<AIDA::IFunction>(*kuipManager,
                                  NAME,out,cmd_path,"AIDA::IFunction");
    if(!function) return;
    AIDA::IPlotterFactory* fac = af->createPlotterFactory();
    if(fac) {
      AIDA::IPlotter* plotter = fac->create("");        
      if(plotter) {
        plotter->currentRegion().plot(*function);
        delete plotter;
      }
      delete fac;
    }
    return;
  }

  ///////////////////////////////////////////////////////
  /// FITRESULT /////////////////////////////////////////
  ///////////////////////////////////////////////////////

  if(cmd_path=="/AIDA/FITRESULT/DELETE") {
    std::string NAME = ku_gets();
    if(kuipManager->isOwner(NAME)) {
      AIDA::IFitResult* fitResult = 
        (AIDA::IFitResult*)kuipManager->find(NAME);
      delete fitResult;
    }
    kuipManager->destroy(NAME);
    return;
  }

  ///////////////////////////////////////////////////////
  /// FITTER ////////////////////////////////////////////
  ///////////////////////////////////////////////////////

  if(cmd_path=="/AIDA/FITTER/NEW") {
    std::string NAME = ku_gets();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::IFitFactory* fac = af->createFitFactory();
    if(fac) {
      AIDA::IFitter* fitter = fac->createFitter("","","");        
      if(fitter) kuipManager->add(fitter,NAME,true);
      delete fac;
    }
    return;
  }

  if(cmd_path=="/AIDA/FITTER/DELETE") {
    std::string NAME = ku_gets();
    if(kuipManager->isOwner(NAME)) {
      AIDA::IFitter* fitter = (AIDA::IFitter*)kuipManager->find(NAME);
      delete fitter;
    }
    kuipManager->destroy(NAME);
    return;
  }

  if(cmd_path=="/AIDA/FITTER/FIT") {
    std::string NAME = ku_gets();
    std::string TYPE = ku_gets();
    std::string NAMEH = ku_gets();
    std::string NAMER = ku_gets();
    AIDA::IFitter* fitter = (AIDA::IFitter*)kuipManager->find(NAME);
    if(!fitter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    AIDA::IHistogram1D* h1D = 
      findObject<AIDA::IHistogram1D>(*kuipManager,
                                     NAMEH,out,cmd_path,
                                     "AIDA::IHistogram1D");
    if(!h1D) return;
    AIDA::IFitResult* fitResult = fitter->fit(*h1D,TYPE);
    if(!fitResult) return;
    kuipManager->add(fitResult,NAMER,true);
    return;
  }

  ///////////////////////////////////////////////////////
  /// PLOTTER ///////////////////////////////////////////
  ///////////////////////////////////////////////////////

  if(cmd_path=="/AIDA/PLOTTER/NEW") {
    std::string NAME = ku_gets();
    std::string WIDGET = ku_gets();
    if(kuipManager->find(NAME)) {
      alreadyExists(out,cmd_path,NAME);
      return;
    }
    AIDA::IPlotterFactory* fac = af->createPlotterFactory();
    if(fac) {
      AIDA::IPlotter* plotter = fac->create(WIDGET);        
      if(plotter) kuipManager->add(plotter,NAME,true);
      delete fac;
    }
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/DELETE") {
    std::string NAME = ku_gets();
    if(kuipManager->isOwner(NAME)) {
      AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
      delete plotter;
    }
    kuipManager->destroy(NAME);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/CLEAR") {
    std::string NAME = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->clearRegions();
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/NEXT") {
    std::string NAME = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->next();
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/SETPARAMETER") {
    std::string NAME = ku_gets();
    std::string PARAMETER = ku_gets();
    std::string VALUE = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->setParameter(PARAMETER,VALUE);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/REGION/GRID") {
    std::string NAME = ku_gets();
    int NC = ku_geti();
    int NR = ku_geti();
    int I = ku_geti();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->createRegions(NC,NR,I);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/REGION/CREATE") {
    std::string NAME = ku_gets();
    double X = ku_getr();
    double Y = ku_getr();
    double W = ku_getr();
    double H = ku_getr();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->createRegion(X,Y,W,H);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/REGION/SET") {
    std::string NAME = ku_gets();
    int I = ku_geti();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->setCurrentRegionNumber(I);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/REGION/SETPARAMETER") {
    std::string NAME = ku_gets();
    std::string PARAMETER = ku_gets();
    std::string VALUE = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->currentRegion().setParameter(PARAMETER,VALUE);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/REGION/CLEAR") {
    std::string NAME = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    plotter->currentRegion().clear();
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/PLOT/HISTOGRAM") {
    std::string NAME = ku_gets();
    std::string NAMEH = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    AIDA::IHistogram* histo = 
      findObject<AIDA::IHistogram>(*kuipManager,
                                   NAMEH,out,cmd_path,"AIDA::IHistogram");
    if(!histo) return;
    plotter->currentRegion().plot(*histo);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/PLOT/FUNCTION") {
    std::string NAME = ku_gets();
    std::string NAMEF = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    AIDA::IFunction* func = 
      findObject<AIDA::IFunction>(*kuipManager,
                                  NAMEF,out,cmd_path,"AIDA::IFunction");
    if(!func) return;
    plotter->currentRegion().plot(*func);
    return;
  }

  if(cmd_path=="/AIDA/PLOTTER/PLOT/FITRESULT") {
    std::string NAME = ku_gets();
    std::string NAMER = ku_gets();
    AIDA::IPlotter* plotter = (AIDA::IPlotter*)kuipManager->find(NAME);
    if(!plotter) {
      notFound(out,cmd_path,NAME);
      return;
    }
    AIDA::IFitResult* fitResult = (AIDA::IFitResult*)kuipManager->find(NAMER);
    if(!fitResult) {
      notFound(out,cmd_path,NAMER);
      return;
    }
    AIDA::IFunction& func = fitResult->fittedFunction(); 
    plotter->currentRegion().plot(func);
    return;
  }

}

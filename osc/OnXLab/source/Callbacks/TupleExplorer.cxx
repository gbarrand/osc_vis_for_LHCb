//
//  All functions here should be OnX callbacks, that is to say
// functions with signature :
//   extern "C" {
//     void callback_without_arguments(Slash::UI::IUI&);
//     void callback_with_arguments(Slash::UI::IUI&,
//                                  const std::vector<std::string>&);
//   }
//

// Slash :
#include <Slash/Tools/AIDA.h>
#include <Slash/Tools/Session.h>
#include <Slash/Tools/UI.h>

// amanip :
#include <amanip/find.h>
#include <amanip/create.h>
#include <amanip/filters.h>
#include <amanip/evaluators.h>
#include <amanip/project.h>
#include <amanip/tuple.h>

#include <iostream>

#include <BatchLab/Core/Filter.h>
#include <BatchLab/Core/Evaluator.h>
#include <BatchLab/Core/MemoryTuple.h>

#include "inlib"

#include <inlib/sys/atime>
#include <inlib/vmanip>
#include <inlib/args>

#define NotFound (-1)
#define NoLimit (-1)

namespace OnXLab {

class TupleExplorerManager : public virtual Slash::Core::IManager {
public: //Slash::Core::IManager
  virtual std::string name() const { return fName;}
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(OnXLab::TupleExplorerManager)
    else INLIB_IF_CAST(Slash::Core::IManager)
    else return 0;
  }
public:
  TupleExplorerManager(Slash::Core::ISession&)
  :fName("OnXLab_TupleExplorerManager"),fTree(0),fTuple(0),fRep(0){}
  virtual ~TupleExplorerManager(){
    delete fRep;
  }
  void setTreeTuple(AIDA::ITree* aTree,AIDA::ITuple* aTuple) { 
    fTree = aTree;
    fTuple = aTuple;
    delete fRep;
    fRep = 0;
  }
  AIDA::ITree* tree() const { return fTree;}
  AIDA::ITuple* tuple() const { return fTuple;}
  void setRep(AIDA::IBaseHistogram* aRep) {
    delete fRep;
    fRep = aRep;
  }
private:
  std::string fName;
  AIDA::ITree* fTree;
  AIDA::ITuple* fTuple;
  AIDA::IBaseHistogram* fRep;
};

class Bind {
public:
  Bind(std::ostream& a_out,AIDA::ITree& aTree,AIDA::ITuple& aTuple)
  :f_out(a_out),fTree(aTree),fTuple(aTuple),fEval(0){}
  virtual ~Bind(){delete fEval;}
public:
  bool minMax(const std::string& aFilter,double& aMin,double& aMax) const {
    if(!fEval) return false;
    BatchLab::Filter filter(f_out,aFilter,fTree);
    return amanip::min_max(f_out,fTuple,*fEval,filter,aMin,aMax);
  }
  AIDA::IEvaluator* eval() const { return fEval;}
  void setEval(AIDA::IEvaluator* aEval) { fEval = aEval;}
private:  
  std::ostream& f_out;
  AIDA::ITree& fTree;
  AIDA::ITuple& fTuple;
  AIDA::IEvaluator* fEval;
};

}

static OnXLab::TupleExplorerManager* find_explorer(Slash::UI::IUI& aUI){
  return Slash_findManager
    (aUI.session(),"OnXLab_TupleExplorerManager",OnXLab::TupleExplorerManager);
}

static AIDA::ITuple* getTreeTuple(Slash::UI::IUI& aUI,AIDA::ITree*& aTree){
  OnXLab::TupleExplorerManager* explorer = find_explorer(aUI);
  if(!explorer) {aTree = 0;return 0;}
  aTree = explorer->tree();
  return explorer->tuple();
}

static AIDA::IBaseHistogram* represent_tuple(
 Slash::Core::ISession& aSession
,AIDA::ITuple& aTuple
,const OnXLab::Bind& aBind
,int aXnumber
,double aXmin
,double aXmax
,const std::string& aFilter
){
  std::ostream& out = aSession.cout();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aSession);
  if(!aida) return 0;
  AIDA::ITree* tree = amanip::find_mem(*aida);
  if(!tree) return 0;

  std::string name = "tmp"; // it is ok for a not managed object.

  int xnumber = aXnumber<=0 ? 100 : aXnumber;
  double xmin = 0;
  double xmax = 0;
  if(aXmax<=aXmin) {
    // Guess the histogram booking parameters :
    inlib::atime begin = inlib::atime::now();
    if(!aBind.minMax(aFilter,xmin,xmax)) return 0;
    if(aSession.verboseLevel()) {
      inlib::atime end = inlib::atime::elapsed(begin);
      out << "OnXLab::represent_tuple :"
          << " get min, max column elapsed time : " 
          << end.seconds() << " seconds."
          << std::endl;
    }
    if(xmax==xmin) {
      xmax = xmin + 1;
    } else {
      // Tilt xmax in order that max value is in the histo :
      xmax = xmax + (xmax-xmin) * 0.01;
    }
  } else {
    xmin = aXmin;
    xmax = aXmax;
  }

  AIDA::IHistogramFactory* histogramFactory = 
    aida->createHistogramFactory(*tree);
  if(!histogramFactory) return 0;

  AIDA::IHistogram1D* histogram = 
    histogramFactory->createHistogram1D
      (amanip::path_not_managed()+"/"+name,name,xnumber,xmin,xmax);
  delete histogramFactory;
  if(!histogram) return 0;

  inlib::atime begin = inlib::atime::now();

  BatchLab::Filter filter(out,aFilter,*tree);
  amanip::project<AIDA::IHistogram1D>
    (out,aTuple,*histogram,*aBind.eval(),filter);
 
  if(aSession.verboseLevel()) {
    inlib::atime end = inlib::atime::elapsed(begin);
    out << "OnXLab::represent_tuple :"
        << " fill histogram elapsed time : " 
        << end.seconds() << " seconds."
        << std::endl;
  }
  
  return histogram;
}

static AIDA::IBaseHistogram* represent_tuple(
 Slash::Core::ISession& aSession
,AIDA::ITuple& aTuple
,const OnXLab::Bind& aBindX
,int aXnumber
,double aXmin
,double aXmax
,const OnXLab::Bind& aBindY
,int aYnumber
,double aYmin
,double aYmax
,const std::string& aFilter
,const std::string& aRep
,const std::string& aOnEvalFilter
){
  std::ostream& out = aSession.cout();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aSession);
  if(!aida) return 0;
  AIDA::ITree* tree = amanip::find_mem(*aida);
  if(!tree) return 0;

  std::string name = "tmp"; // it is ok for a not managed object.

  int xnumber = aXnumber<=0 ? 100 : aXnumber;
  double xmin = 0;
  double xmax = 0;
  if(aXmax<=aXmin) {
    if(!aBindX.minMax(aFilter,xmin,xmax)) return 0;
    if(xmax==xmin) {
      xmax = xmin + 1;
    } else {
      xmax = xmax + (xmax-xmin) * 0.01;
    }
  } else {
    xmin = aXmin;
    xmax = aXmax;
  }

  int ynumber = aYnumber<=0 ? 100 : aYnumber;
  double ymin = 0;
  double ymax = 0;
  if(aYmax<=aYmin) {
    if(!aBindY.minMax(aFilter,ymin,ymax)) return 0;
    if(ymax==ymin) {
      ymax = ymin + 1;
    } else {
      ymax = ymax + (ymax-ymin) * 0.01;
    }
  } else {
    ymin = aYmin;
    ymax = aYmax;
  }

  AIDA::IHistogramFactory* histogramFactory = 
    aida->createHistogramFactory(*tree);
  if(!histogramFactory) return 0;

  if(aRep=="Histogram2D") {
    AIDA::IHistogram2D* histogram = 
      histogramFactory->createHistogram2D(amanip::path_not_managed()+"/"+name,
                                          name,
                                          xnumber,xmin,xmax,
                                          ynumber,ymin,ymax);
    delete histogramFactory;
    if(!histogram) return 0;

    BatchLab::Filter filter(out,aFilter,*tree);

    if(aOnEvalFilter.size()&&(aOnEvalFilter!="none")) {
      //temporary not managed tuple.
      BatchLab::MemoryTuple tmp(out,"tmp","tmp","double x,double y");
      BatchLab::Filter onEvalFilter(out,aOnEvalFilter,*tree);
      amanip::project<AIDA::IHistogram2D>
        (out,aTuple,*histogram,*aBindX.eval(),*aBindY.eval(),
           filter,onEvalFilter,tmp);
    } else {
      amanip::project<AIDA::IHistogram2D>
        (out,aTuple,*histogram,*aBindX.eval(),*aBindY.eval(),filter);
    }

    return histogram;

  } else if(aRep=="Profile1D") {
    AIDA::IProfile1D* profile = 
      histogramFactory->createProfile1D(amanip::path_not_managed()+"/"+name,
                                        name,
                                        xnumber,xmin,xmax,
                                        ymin,ymax);
    delete histogramFactory;
    if(!profile) return 0;

    BatchLab::Filter filter(out,aFilter,*tree);

    if(aOnEvalFilter.size()&&(aOnEvalFilter!="none")) {
      //temporary not managed tuple.
      BatchLab::MemoryTuple tmp(out,"tmp","tmp","double x,double y");
      BatchLab::Filter onEvalFilter(out,aOnEvalFilter,*tree);
      amanip::project<AIDA::IProfile1D>
        (out,aTuple,*profile,*aBindX.eval(),*aBindY.eval(),
           filter,onEvalFilter,tmp);
    } else {
      amanip::project<AIDA::IProfile1D>
        (out,aTuple,*profile,*aBindX.eval(),*aBindY.eval(),filter);
    }
 
    return profile;

  } else if(aRep=="Cloud2D") {
    AIDA::ICloud2D* cloud = 
      histogramFactory->createCloud2D
        (amanip::path_not_managed()+"/"+name,name,NoLimit);
    delete histogramFactory;
    if(!cloud) return 0;

    amanip::filter_2D filter(*aBindX.eval(),xmin,xmax,
                             *aBindY.eval(),ymin,ymax);

    if(aOnEvalFilter.size()&&(aOnEvalFilter!="none")) {
      //temporary not managed tuple.
      BatchLab::MemoryTuple tmp(out,"tmp","tmp","double x,double y");
      BatchLab::Filter onEvalFilter(out,aOnEvalFilter,*tree);
      amanip::project<AIDA::ICloud2D>
        (out,aTuple,*cloud,*aBindX.eval(),*aBindY.eval(),
           filter,onEvalFilter,tmp);
    } else {
      amanip::project<AIDA::ICloud2D>
        (out,aTuple,*cloud,*aBindX.eval(),*aBindY.eval(),filter);
    }

    return cloud;
  }

  out << "represent_tuple :"
      << " unknown representation " << aRep
      << std::endl;

  delete histogramFactory;

  return 0;
}

static AIDA::IBaseHistogram* represent_tuple(
 Slash::Core::ISession& aSession
,AIDA::ITuple& aTuple
,const OnXLab::Bind& aBindX
,int aXnumber
,double aXmin
,double aXmax
,const OnXLab::Bind& aBindY
,int aYnumber
,double aYmin
,double aYmax
,const OnXLab::Bind& aBindZ
,int aZnumber
,double aZmin
,double aZmax
,const std::string& aFilter
,const std::string& aRep
){
  std::ostream& out = aSession.cout();

  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aSession);
  if(!aida) return 0;
  AIDA::ITree* tree = amanip::find_mem(*aida);
  if(!tree) return 0;

  std::string name = "tmp"; // it is ok for a not managed object.

  int xnumber = aXnumber<=0 ? 100 : aXnumber;
  double xmin = 0;
  double xmax = 0;
  if(aXmax<=aXmin) {
    if(!aBindX.minMax(aFilter,xmin,xmax)) return 0;
    if(xmax==xmin) {
      xmax = xmin + 1;
    } else {
      xmax = xmax + (xmax-xmin) * 0.01;
    }
  } else {
    xmin = aXmin;
    xmax = aXmax;
  }

  int ynumber = aYnumber<=0 ? 100 : aYnumber;
  double ymin = 0;
  double ymax = 0;
  if(aYmax<=aYmin) {
    if(!aBindY.minMax(aFilter,ymin,ymax)) return 0;
    if(ymax==ymin) {
      ymax = ymin + 1;
    } else {
      ymax = ymax + (ymax-ymin) * 0.01;
    }
  } else {
    ymin = aYmin;
    ymax = aYmax;
  }

  int znumber = aZnumber<=0 ? 100 : aZnumber;
  double zmin = 0;
  double zmax = 0;
  if(aZmax<=aZmin) {
    if(!aBindZ.minMax(aFilter,zmin,zmax)) return 0;
    if(zmax==zmin) {
      zmax = zmin + 1;
    } else {
      zmax = zmax + (zmax-zmin) * 0.01;
    }
  } else {
    zmin = aZmin;
    zmax = aZmax;
  }

  AIDA::IHistogramFactory* histogramFactory = 
    aida->createHistogramFactory(*tree);
  if(!histogramFactory) return 0;

  if(aRep=="Histogram3D") {
    AIDA::IHistogram3D* histogram = 
      histogramFactory->createHistogram3D(amanip::path_not_managed()+"/"+name,
                                          name,
                                          xnumber,xmin,xmax,
                                          ynumber,ymin,ymax,
                                          znumber,zmin,zmax);
    delete histogramFactory;
    if(!histogram) return 0;

    BatchLab::Filter filter(out,aFilter,*tree);
    amanip::project<AIDA::IHistogram3D>
      (out,aTuple,*histogram,
      *aBindX.eval(),*aBindY.eval(),*aBindZ.eval(),filter);

    return histogram;

  } else if(aRep=="Profile2D") {
    AIDA::IProfile2D* profile = 
      histogramFactory->createProfile2D(amanip::path_not_managed()+"/"+name,
                                        name,
                                        xnumber,xmin,xmax,
                                        ynumber,ymin,ymax,
                                        zmin,zmax);
    delete histogramFactory;
    if(!profile) return 0;

    BatchLab::Filter filter(out,aFilter,*tree);
    amanip::project<AIDA::IProfile2D>
      (out,aTuple,*profile,
       *aBindX.eval(),*aBindY.eval(),*aBindZ.eval(),filter);

    return profile;

  } else if(aRep=="Cloud3D") {
    AIDA::ICloud3D* cloud = 
      histogramFactory->createCloud3D
        (amanip::path_not_managed()+"/"+name,name,NoLimit);
    delete histogramFactory;
    if(!cloud) return 0;

    amanip::filter_3D filter(*aBindX.eval(),xmin,xmax,
                             *aBindY.eval(),ymin,ymax,
                             *aBindZ.eval(),zmin,zmax);
    amanip::project<AIDA::ICloud3D>
      (out,aTuple,*cloud,
       *aBindX.eval(),*aBindY.eval(),*aBindZ.eval(),filter);

    return cloud;
  }

  out << "represent_tuple :"
      << " unknown representation " << aRep
      << std::endl;

  delete histogramFactory;

  return 0;
}

static void tuple_represent(
 Slash::UI::IUI& aUI
,AIDA::ITuple& aTuple
,const OnXLab::Bind& aBindX
,const OnXLab::Bind& aBindY
,const OnXLab::Bind& aBindZ
,const std::string& aFilter
){
  OnXLab::TupleExplorerManager* explorer = find_explorer(aUI);
  if(!explorer) return;

  // Find AIDA :
  AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
  if(!aida) return;
  AIDA::IPlotter* plotter = amanip::create_plotter(*aida,"@current@");
  if(!plotter) return;
  AIDA::IPlotterRegion& region = plotter->currentRegion();

  region.clear();

  std::string srep;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_rep.value",srep)) return;

  std::string on_eval_filter;
  if(!aUI.parameterValue
    ("OnXLab_TupleExplorer_on_eval_filter.value",on_eval_filter)) return;

 {std::vector<std::string> words = inlib::words(srep," ");
  srep = words[0];}

  if(!aBindX.eval()) {
  } else {

    int xnumber;
    if(!Slash::parameter<int>
      (aUI,"OnXLab_TupleExplorer_xnumber.value",xnumber)) return;
    double xmin;
    if(!Slash::parameter<double>(aUI,"OnXLab_TupleExplorer_xmin.value",xmin)) 
      return;
    double xmax;
    if(!Slash::parameter<double>(aUI,"OnXLab_TupleExplorer_xmax.value",xmax))
      return;

    if(!aBindY.eval()) {

      AIDA::IBaseHistogram* rep = 
        represent_tuple(aUI.session(),aTuple,aBindX,xnumber,xmin,xmax,aFilter);
      if(rep) {
        explorer->setRep(rep);
        region.plot(*rep);
      }

    } else {

      int ynumber;
      if(!Slash::parameter<int>
        (aUI,"OnXLab_TupleExplorer_ynumber.value",ynumber)) return;
      double ymin;
      if(!Slash::parameter<double>
            (aUI,"OnXLab_TupleExplorer_ymin.value",ymin)) return;
      double ymax;
      if(!Slash::parameter<double>
            (aUI,"OnXLab_TupleExplorer_ymax.value",ymax)) return;

      if(!aBindZ.eval()) {

        AIDA::IBaseHistogram* rep = 
          represent_tuple(aUI.session(),aTuple,
                          aBindX,xnumber,xmin,xmax,
                          aBindY,ynumber,ymin,ymax,
                          aFilter,srep,on_eval_filter);
        if(rep) {
          explorer->setRep(rep);
          region.plot(*rep);
        }

      } else {

        int znumber;
        if(!Slash::parameter<int>
          (aUI,"OnXLab_TupleExplorer_znumber.value",znumber)) return;
        double zmin;
        if(!Slash::parameter<double>
              (aUI,"OnXLab_TupleExplorer_zmin.value",zmin)) return;
        double zmax;
        if(!Slash::parameter<double>
              (aUI,"OnXLab_TupleExplorer_zmax.value",zmax)) return;

        AIDA::IBaseHistogram* rep = 
          represent_tuple(aUI.session(),aTuple,
                          aBindX,xnumber,xmin,xmax,
                          aBindY,ynumber,ymin,ymax,
                          aBindZ,znumber,zmin,zmax,
                          aFilter,srep);
        if(rep) {
          explorer->setRep(rep);
          region.plot(*rep);
        }

      }
    }

  }

  delete plotter;
}

extern "C" {

void OnXLab_TupleExplorer_on_eval_filter_update(Slash::UI::IUI& aUI) {
  // get current items :
  std::string s;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_on_eval_filter.items",s)) return;
  std::vector<std::string> items = inlib::words(s,"\n");

  // append 2 dim IFunction found in the memory tree :
 {AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
  if(aida) {
  AIDA::ITree* tree = amanip::find_mem(*aida);
  if(tree) {
    //FIXME : optimize.
    //BatchLab::MemoryTree* memoryTree = 
    //std::vector<AIDA::IManagedObject*> vec = memoryTree->allObjects();
    //std::vector<AIDA::IFunction*> funcs;
    //for(unsigned int index=0;index<vec.size();index++) {
    //  AIDA::IFunction* obj = INLIB_CAST(*(vec[index]),AIDA::IFunction);
    //  if(obj) {}
    //}

    // detect 2 dim IFunctions :
    std::vector<std::string> nms = tree->listObjectNames();
    std::vector<std::string> tps = tree->listObjectTypes();
    std::vector<std::string>::const_iterator it1 = nms.begin();
    std::vector<std::string>::const_iterator it2 = tps.begin();
    for(;it1!=nms.end();++it1,++it2) {
      if((*it2)=="IFunction") {
        AIDA::IManagedObject* bo = tree->find(*it1);
        if(bo) {
          AIDA::IFunction* func = INLIB_CAST(*bo,AIDA::IFunction);
          if(func) {
            if(func->dimension()==2) { 
              items.push_back(bo->name()+"(x,y)>0");
            }
          }
        }
      }
    }
  }}}
  
  // append current value :
 {std::string s;
  if(!aUI.parameterValue
       ("OnXLab_TupleExplorer_on_eval_filter.value",s)) return;
  if(s.size()) items.push_back(s);}

  inlib::unique<std::string>(items);

  aUI.setParameter("OnXLab_TupleExplorer_on_eval_filter.items",
		   inlib::tos<std::string>(items,"\n"));

}

void OnXLab_TupleExplorer_update(Slash::UI::IUI& aUI) {
  OnXLab::TupleExplorerManager* explorer = find_explorer(aUI);
  if(!explorer) {
    explorer = new OnXLab::TupleExplorerManager(aUI.session());
    aUI.session().addManager(explorer);
  }

  std::string selection;
  std::string stree;
  if(!aUI.session().parameterValue("OnXLab.tree",stree)) return;
  if(!aUI.parameterValue(stree+".selection",selection)) return;  

  AIDA::ITree* tree = 0;
  AIDA::ITuple* tuple = 0;
  std::string mgr;
  std::string path;
 {std::string basename;
  AIDA::IManagedObject* object = 
    find_aida_object(selection,aUI.session(),tree,mgr,path,basename);
  if(tree) {
    if(object) {
      tuple = INLIB_CAST(*object,AIDA::ITuple);
      if(!tuple) {
        aUI.echo(basename + " is not a tuple.");
      }
    } else {
      // May be a tuple in a Rio tree ?
      AIDA::IAnalysisFactory* aida = Slash::find_AIDA(aUI.session());
      if(aida) {
        // Get the tuple from the rioTree :
        tuple = amanip::create_tuple(*aida,*tree,basename,"","");
        if(!tuple) {
          aUI.echo(basename + " not found.");
        }
      }
    }
  }}

  explorer->setTreeTuple(tree,tuple);
  if(tuple) {
    int n = tuple->columns();
    std::string s = "none";
    for(int index = 0;index<n;index++) {
      s += "\n"+tuple->columnName(index);
    }
    aUI.setParameter("OnXLab_TupleExplorer_bind_x.items",s);
    aUI.setParameter("OnXLab_TupleExplorer_bind_y.items",s);
    aUI.setParameter("OnXLab_TupleExplorer_bind_z.items",s);
    aUI.setParameter("OnXLab_TupleExplorer_tree.value",mgr);
    aUI.setParameter("OnXLab_TupleExplorer_tuple.value",path);
    aUI.setParameter("OnXLab_TupleExplorer_title.value",tuple->title());
    aUI.setParameter("OnXLab_TupleExplorer_rows.value",inlib::to<int>(tuple->rows()));
  } else {
    aUI.setParameter("OnXLab_TupleExplorer_bind_x.items","");
    aUI.setParameter("OnXLab_TupleExplorer_bind_y.items","");
    aUI.setParameter("OnXLab_TupleExplorer_bind_z.items","");
    aUI.setParameter("OnXLab_TupleExplorer_tree.value","");
    aUI.setParameter("OnXLab_TupleExplorer_tuple.value","");
    aUI.setParameter("OnXLab_TupleExplorer_title.value","");
    aUI.setParameter("OnXLab_TupleExplorer_rows.value","");
  }
  aUI.setParameter("OnXLab_TupleExplorer_xnumber.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_xmin.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_xmax.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_xmin_scale.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_xmax_scale.value","1000");

  aUI.setParameter("OnXLab_TupleExplorer_ynumber.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_ymin.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_ymax.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_ymin_scale.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_ymax_scale.value","1000");

  aUI.setParameter("OnXLab_TupleExplorer_znumber.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_zmin.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_zmax.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_zmin_scale.value","0");
  aUI.setParameter("OnXLab_TupleExplorer_zmax_scale.value","1000");

  OnXLab_TupleExplorer_on_eval_filter_update(aUI);

}

void OnXLab_TupleExplorer_infos(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs){
  std::ostream& out = aUI.session().cout();

  // aArgs[0] x or y
  if(aArgs.empty()) {
    out << "OnXLab_TupleExplorer_infos :" 
        << " one argument expected."
        << std::endl;
    return;
  }

  std::string scoord = aArgs[0];

  std::string column;
  if(!aUI.parameterValue
     ("OnXLab_TupleExplorer_bind_"+scoord+".value",column)) {
    out << "OnXLab_TupleExplorer_infos :" 
        << " IUI.parameterValue failed."
        << std::endl;
    return;
  }

  // Get the ITuple :
  AIDA::ITree* tree = 0;
  AIDA::ITuple* tuple = getTreeTuple(aUI,tree);
  if(!tuple) {
    out << "OnXLab_TupleExplorer_infos :" 
        << " getTreeTuple failed."
        << std::endl;
    return;
  }
  int i = tuple->findColumn(column);
  if(i==NotFound) {
    out << "OnXLab_TupleExplorer_infos :" 
        << " column " << column << " not found."
        << std::endl;
    return;
  }

  out << "/////////////////" << std::endl;
  out << "name : " << tuple->columnName(i) << std::endl;
  out << "rows : " << tuple->rows() << std::endl;
  out << "type : " << tuple->columnType(i) << std::endl;
  out << "min  : " << tuple->columnMin(i) << std::endl;
  out << "max  : " << tuple->columnMax(i) << std::endl;
  out << "mean : " << tuple->columnMean(i) << std::endl;
  out << "rms  : " << tuple->columnRms(i) << std::endl;
}

void OnXLab_TupleExplorer_bind(Slash::UI::IUI& aUI){
  // Get the ITuple :
  AIDA::ITree* tree = 0;
  AIDA::ITuple* tuple = getTreeTuple(aUI,tree);
  if(!tuple) return;

  std::ostream& out = aUI.session().cout();

  std::string sfilter;
  if(!aUI.parameterValue
       ("OnXLab_TupleExplorer_filter_entry.value",sfilter)) return;

  std::string column;
  std::string seval;
  double mn,mx;

  OnXLab::Bind bind_x(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_x.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_x_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
    aUI.setParameter("OnXLab_TupleExplorer_xnumber.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_xmin.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_xmax.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_xmin_scale.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_xmax_scale.value","1000");
  } else {
    if(seval.size()) { //priority to eval.
      bind_x.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_x.setEval(eval);
    } else {
      return;
    }
    bool vauto;
    if(!Slash::parameter
         (aUI,"OnXLab_TupleExplorer_xauto.set",vauto)) return;
    if(vauto) {
      if(!bind_x.minMax(sfilter,mn,mx)) return;
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_xmin.value",mn);
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_xmax.value",mx);
      // Save for the scale drag callback :
      Slash::Core::ISession& session = aUI.session();
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_xmin",mn);
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_xmax",mx);
      aUI.setParameter("OnXLab_TupleExplorer_xmin_scale.value","0");
      aUI.setParameter("OnXLab_TupleExplorer_xmax_scale.value","1000");
    }
  }

  OnXLab::Bind bind_y(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_y.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_y_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
    aUI.setParameter("OnXLab_TupleExplorer_ynumber.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_ymin.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_ymax.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_ymin_scale.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_ymax_scale.value","1000");
  } else {
    if(seval.size()) {
      bind_y.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_y.setEval(eval);
    } else {
      return;
    }
    bool vauto;
    if(!Slash::parameter
         (aUI,"OnXLab_TupleExplorer_yauto.set",vauto)) return;
    if(vauto) {
      if(!bind_y.minMax(sfilter,mn,mx)) return;
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_ymin.value",mn);
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_ymax.value",mx);
      // Save for the scale drag callback :
      Slash::Core::ISession& session = aUI.session();
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_ymin",mn);
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_ymax",mx);
      aUI.setParameter("OnXLab_TupleExplorer_ymin_scale.value","0");
      aUI.setParameter("OnXLab_TupleExplorer_ymax_scale.value","1000");
    }
  }

  OnXLab::Bind bind_z(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_z.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_z_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
    aUI.setParameter("OnXLab_TupleExplorer_znumber.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_zmin.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_zmax.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_zmin_scale.value","0");
    aUI.setParameter("OnXLab_TupleExplorer_zmax_scale.value","1000");
  } else {
    if(seval.size()) {
      bind_z.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_z.setEval(eval);
    } else {
      return;
    }
    bool vauto;
    if(!Slash::parameter
         (aUI,"OnXLab_TupleExplorer_zauto.set",vauto)) return;
    if(vauto) {
      if(!bind_z.minMax(sfilter,mn,mx)) return;
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_zmin.value",mn);
      Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_zmax.value",mx);
      // Save for the scale drag callback :
      Slash::Core::ISession& session = aUI.session();
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_zmin",mn);
      Slash::set_parameter<double>(session,"OnXLab_TupleExplorer_zmax",mx);
      aUI.setParameter("OnXLab_TupleExplorer_zmin_scale.value","0");
      aUI.setParameter("OnXLab_TupleExplorer_zmax_scale.value","1000");
    }
  }

  tuple_represent(aUI,*tuple,bind_x,bind_y,bind_z,sfilter);
}

void OnXLab_TupleExplorer_represent(Slash::UI::IUI& aUI){
  AIDA::ITree* tree = 0;
  AIDA::ITuple* tuple = getTreeTuple(aUI,tree);
  if(!tuple) return;

  std::ostream& out = aUI.session().cout();

  std::string sfilter;
  if(!aUI.parameterValue
       ("OnXLab_TupleExplorer_filter_entry.value",sfilter)) return;

  std::string column;
  std::string seval;

  OnXLab::Bind bind_x(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_x.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_x_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
  } else {
    if(seval.size()) {
      bind_x.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_x.setEval(eval);
    } else {
      return;
    }
  }

  OnXLab::Bind bind_y(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_y.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_y_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
  } else {
    if(seval.size()) {
      bind_y.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_y.setEval(eval);
    } else {
      return;
    }
  }

  OnXLab::Bind bind_z(out,*tree,*tuple);
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_z.value",column)) return;
  if(!aUI.parameterValue("OnXLab_TupleExplorer_bind_z_entry.value",seval)) return;
  if(seval.empty()&&(column=="none")) {
  } else {
    if(seval.size()) {
      bind_z.setEval(new BatchLab::Evaluator(out,seval,*tree));
    } else if(column!="none") {
      int index = tuple->findColumn(column);
      if(index==NotFound) return;
      amanip::evaluator_1D* eval = new amanip::evaluator_1D(out,*tuple,index);
      if(!eval->initialize(*tuple)) return;
      bind_z.setEval(eval);
    } else {
      return;
    }
  }

  tuple_represent(aUI,*tuple,bind_x,bind_y,bind_z,sfilter);
}

//REVISIT : because of the aUI.callbackValue()
void OnXLab_TupleExplorer_scale(Slash::UI::IUI& aUI,const std::vector<std::string>& aArgs) {
  // aArgs[0] x, y or z
  // aArgs[1] min or max
  if(aArgs.size()!=2) return;
  std::ostream& out = aUI.session().cout();

  double value;
  if(!inlib::to<double>(out,aUI.callbackValue(),value)) return; //In [0,1000]

  std::string scoord = aArgs[0];

  Slash::Core::ISession& session = aUI.session();
  double mn;
  if(!Slash::parameter<double>
    (session,"OnXLab_TupleExplorer_"+scoord+"min",mn)) return;
  double mx;
  if(!Slash::parameter<double>
    (session,"OnXLab_TupleExplorer_"+scoord+"max",mx)) return;
  if(mx<=mn) return;

  double dx = mx - mn;
  dx *= value/1000.;
  double v = mn + dx;
  Slash::set_parameter<double>(aUI,"OnXLab_TupleExplorer_"+scoord+aArgs[1]+".value",v);

  OnXLab_TupleExplorer_represent(aUI);
}

} // extern "C"


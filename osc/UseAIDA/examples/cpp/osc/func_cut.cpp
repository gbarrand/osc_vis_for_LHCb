
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Tuple projection with cuts done with 2D functions.

static void func_cut(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Cleanup mem :
  mem->rmdir("/");

  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/RootW.root";

  AIDA::ITree* rootTree = 
    amanip::create_tree(a_aida,fileName,"ROOT",true,false);
  if(!rootTree) return;

  rootTree->ls();

  AIDA::ITuple* tuple = 
    amanip::find_tuple(*rootTree,"myTree",true,a_out);
  if(!tuple) {
    a_out << "aida_exa_func_cut :"
          << " myTree not found."
          << std::endl;
    return;
  }

  AIDA::IEvaluator* eval_1 = 
    amanip::create_evaluator(a_aida,*rootTree,"xs1");
  AIDA::IEvaluator* eval_2 = 
    amanip::create_evaluator(a_aida,*rootTree,"xs2");
  AIDA::IFilter* filter = 
    amanip::create_filter(a_aida,*rootTree,"");

  //tuple->project(*h,*eval_1,*eval_2,*filter);

  if(!eval_1->initialize(*tuple)) return;
  if(!eval_2->initialize(*tuple)) return;
  if(!filter->initialize(*tuple)) return;

  AIDA::IFunction* funcut_1 = 
    amanip::create_function
      (a_aida,*mem,"funcut_1","Cut 1","InsidePolygon");
  if(!funcut_1) return;
 {std::vector<double> params;
  params.push_back(-1);params.push_back(0);
  params.push_back( 0);params.push_back(1);
  params.push_back(-1);params.push_back(2);
  funcut_1->setParameters(params);}

  AIDA::IFunction* funcut_2 = 
    amanip::create_function
      (a_aida,*mem,"funcut_2","Cut 2","InsidePolygon");
  if(!funcut_2) return;
 {std::vector<double> params;
  params.push_back( 0);params.push_back(0);
  params.push_back( 0);params.push_back(2);
  params.push_back(-1);params.push_back(1);
  funcut_2->setParameters(params);}

  // "on eval" tuple and filter :
  AIDA::ITuple* tmp_tuple = 
    amanip::create_tuple
      (a_aida,*mem,"tmp_tuple","tmp_tuple","double x,double y");
  if(!tmp_tuple) return;

  //std::string scut = "(funcut_1(x,y)>0)&&(funcut_2(x,y)>0)";
  //std::string scut = "(funcut_1(x,y)>0)||(funcut_2(x,y)>0)";
  std::string scut = "(funcut_1(x,y)>0)&&(funcut_2(x,y)<=0)";
  AIDA::IFilter* on_eval_filter = 
    amanip::create_filter(a_aida,*mem,scut);

  //WARNING : a ref to the AIDA::IFunction will be done by the below.
  if(!on_eval_filter->initialize(*tmp_tuple)) return;

  AIDA::IHistogram2D* h = 
    amanip::create_histogram_2D(a_aida,*mem,"xs1_xs2","xs2/xs1",
      100,-2,2,100,-2,2);
  if(!h) return;
  AIDA::IHistogram2D* h_cut = 
    amanip::create_histogram_2D
      (a_aida,*mem,"xs1_xs2_funcut","xs2/xs1 polygon cut",
      100,-2,2,100,-2,2);
  if(!h_cut) return;

  // Loop on entries :
  tuple->start();
  while(tuple->next()) {
    if(!filter->accept()) continue;
  
    double x = eval_1->evaluateDouble();
    double y = eval_2->evaluateDouble();
  
    h->fill(x,y,1);

    tmp_tuple->reset();
    tmp_tuple->fill(0,x);
    tmp_tuple->fill(1,y);
    tmp_tuple->addRow();
    tmp_tuple->start();
    tmp_tuple->next();
    if(!on_eval_filter->accept()) continue;    
  
    h_cut->fill(x,y,1);
  }
  delete on_eval_filter;
  delete tmp_tuple;

  delete eval_1;
  delete eval_2;
  delete filter;

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) return;

  plotter->createRegions(1,2,0);
 {AIDA::IPlotterRegion& region = plotter->currentRegion();
  region.plot(*h);}
 {AIDA::IPlotterRegion& region = plotter->next();
  region.setParameter("infosRegionVisible","TRUE");
  region.setParameter("plotter.infosStyle.options",
    "annotation.Name annotation.Entries annotation.Day annotation.OVFLW annotation.Signal annotation.RMS annotation.Empty");
  h_cut->annotation().addItem("Name"," Signal_123");
  h_cut->annotation().addItem("Entries"," 1024");
  h_cut->annotation().addItem("Day"," Sunday");
  h_cut->annotation().addItem("OVFLW"," 0");
  h_cut->annotation().addItem("Signal"," invalid");
  h_cut->annotation().addItem("RMS"," 172.211");
  h_cut->annotation().addItem("Empty","");
  region.plot(*h_cut);

  region.setParameter("plotter.wallStyle.visible","FALSE");
  region.setParameter("plotter.gridStyle.visible","FALSE");

  funcut_1->annotation().addItem("plotter.modeling","polygon");
  funcut_1->annotation().addItem("plotter.color","0 0.7 0.7");
  funcut_1->annotation().addItem("plotter.transparency","0.7");

  region.plot(*funcut_1);

  //region.setParameter("plotter.functionStyle[0].color","0 0.7 0.7");
  //NOTE : if doing the below [1] we have to do first the upper [0].
  //region.setParameter("plotter.functionStyle[1].automated","FALSE");
  //region.setParameter("plotter.functionStyle[1].modeling","polygon");
  //region.setParameter("plotter.functionStyle[1].color","0.8 0 0");

  funcut_2->annotation().addItem("plotter.modeling","polygon");
  funcut_2->annotation().addItem("plotter.color","0.8 0.8 0");
  funcut_2->annotation().addItem("plotter.transparency","0.7");
  funcut_2->annotation().addItem("plotter.editable","true");

  region.plot(*funcut_2);
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;

}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_func_cut :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  func_cut(*aida,std::cout,"",true);
  delete aida;
  return 0;
}

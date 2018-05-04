
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Rio file example.

static void rio(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Cleanup mem :
  if(!mem->rmdir("/")) return;

  // If already loaded close (not delete) the file :
  std::string fileName = 
    "$OSC_HOME_DIR/Resources/AIDA/examples/data/analysis.root";

  AIDA::ITree* tree = 
    amanip::create_tree(a_aida,fileName,"ROOT",true,false);
  if(!tree) return;

  tree->ls();

  // Create some factories to work with the tree :
  AIDA::ITupleFactory* tupleFactory = a_aida.createTupleFactory(*tree); 
  if(!tupleFactory) return;

  // Get a plotter in the GUI :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(!plotter) {
    delete tupleFactory;
    return;
  }

  plotter->setTitle("Rio example");
  //plotter->titleStyle().textStyle().setColor("blue");
  //plotter->titleStyle().textStyle().setFont("couri");

  plotter->createRegions(2,2,0);
    
  ///////////////////////////////////////////////////////////////////////////
  //First region :
  // A plot with two histograms.  
  ///////////////////////////////////////////////////////////////////////////
  {
    if(!tree->cd("histograms")) {
      a_out << "aida_exa_rio :" 
            << " can't cd to \"histograms\"." 
            << std::endl;
    } else {

      tree->ls();

      AIDA::IPlotterRegion& region = plotter->currentRegion();
    
      AIDA::IHistogram1D* BW = 
        amanip::find_histogram_1D(*tree,"BW",true,a_out);
      AIDA::IHistogram1D* gauss = 
        amanip::find_histogram_1D(*tree,"gauss",true,a_out);
    
      // Plot :
      region.style().titleStyle().textStyle().setColor("red");
      region.style().titleStyle().textStyle().setFont("couri");

      region.style().yAxisStyle().tickLabelStyle().setColor("red");
      region.style().yAxisStyle().tickLabelStyle().setFont("couri");

      region.style().yAxisStyle().labelStyle().setColor("red");
      region.style().yAxisStyle().labelStyle().setFont("couri");

      if(gauss) {
        gauss->annotation().addItem("plotter.modeling","boxes");
        gauss->annotation().addItem("plotter.color","0 1 0");
        region.plot(*gauss); //Behind (fat). Green. (main)
      }

      if(BW) {
        BW->annotation().addItem("plotter.modeling","wire_boxes");
        BW->annotation().addItem("plotter.color","1 0 0");
        region.plot(*BW);    //In front (slim). Red.
      }

    }
  }

  ///////////////////////////////////////////////////////////////////////////
  // Second region :
  //  A scatter plot.
  ///////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();
    AIDA::IHistogram2D* gauss_BW = 
      amanip::find_histogram_2D(*tree,"gauss_BW",true,a_out);
    region.setParameter("infosRegionVisible","TRUE");

    region.setParameter("plotter.wallStyle.visible","FALSE");
    if(gauss_BW) {
      gauss_BW->annotation().addItem("plotter.modeling","solid");
      //gauss_BW->annotation().addItem("plotter.painting","grey_scale");
      //gauss_BW->annotation().addItem("plotter.painting","violet_to_red");
      gauss_BW->annotation().addItem("plotter.painting","by_value");
      gauss_BW->annotation().addItem("plotter.colorMapping","black 1 cyan 2 green 4 orange");
      region.plot(*gauss_BW);
    } else {
      a_out << "aida_exa_rio :" 
          << " gauss_BW not found." << std::endl;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  // Third region :
  // Get a tuple, create an histo from one of its column, plot it. 
  /////////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    if(!tree->cd("..")) {
      a_out << "aida_exa_rio :" 
          << " can't cd to \"..\"." << std::endl;
    } else if(!tree->cd("tuples")) {
      a_out << "aida_exa_rio :" 
          << " can't cd to \"tuples\"." << std::endl;
    } else {

      tree->ls();
    
      AIDA::ITuple* tuple1 = tupleFactory->create("tuple1","","");
      AIDA::IHistogram1D* t1_v2 = 
        amanip::create_histogram_1D
          (a_aida,*mem,"t1_v2","tuple1/v2",10,0,10);
      AIDA::IEvaluator* evaluator = tupleFactory->createEvaluator("v2");
      AIDA::IFilter* filter = tupleFactory->createFilter("");
      tuple1->project(*t1_v2,*evaluator,*filter);
      delete evaluator;
      delete filter;

      region.plot(*t1_v2);
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  // Fourth region :
  // Get a tuple, create an histo from one of its column, plot it. 
  ////////////////////////////////////////////////////////////////////////////
  {
    AIDA::IPlotterRegion& region = plotter->next();

    AIDA::ITuple* tuple2 = tupleFactory->create("tuple2","","");
    //tuple2->prnt();

    AIDA::IHistogram1D* t2_gauss = 
      amanip::create_histogram_1D
      (a_aida,*mem,"t2_gauss","tuple2/rand_gauss",100,-5,5);
    AIDA::IEvaluator* evaluator = tupleFactory->createEvaluator("rand_gauss");
    std::string cuts("(fabs(rand_gauss)<=2.)&&(step>=1.)");
    AIDA::IFilter* filter = tupleFactory->createFilter(cuts);
    tuple2->project(*t2_gauss,*evaluator,*filter);
    delete evaluator;
    delete filter;

    region.plot(*t2_gauss);

    //////////////////////////////////////////////////////////////////////////
    // Fitting :
    //////////////////////////////////////////////////////////////////////////
    AIDA::IFitter* fitter = amanip::create_fitter(a_aida);
    if(fitter) {
      AIDA::IFitResult* fitResult = fitter->fit(*t2_gauss,"G");
      if(fitResult) {
        AIDA::IFunction& function = fitResult->fittedFunction();
        region.plot(function);
        delete fitResult;
      }
      delete fitter;
    }
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;

  delete tupleFactory;

}

#include <iostream>

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_rio :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  rio(*aida,std::cout,"",true);
  delete aida;
  return 0;
}

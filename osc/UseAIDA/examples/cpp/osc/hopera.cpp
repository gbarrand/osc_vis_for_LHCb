
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>
#include <inlib/random>

// Histogram operations example.

static void hopera(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Factories :
  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  // Get facilities :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {

  plotter->createRegions(2,2,0);
  plotter->setTitle("Histo arithmetic");

  AIDA::IHistogram1D* BW = 
    histogramFactory->createHistogram1D("BW","Rand BW",100,-4,4);

  inlib::random::bw r(0,1);
  for(int index = 0;index<10000;index++) BW->fill(r.shoot(),1);

  AIDA::IHistogram1D* h_add = histogramFactory->add("h_add",*BW,*BW);
  h_add->setTitle("BW+BW");
  {
    AIDA::IPlotterRegion& region = plotter->currentRegion();
    region.setParameter("color","0.8 0.8 0.8");
    region.setParameter("viewportRegion.borderColor","0 1 0");
    region.setParameter("viewportRegion.horizontalBorder","2");
    region.setParameter("viewportRegion.verticalBorder","2");
    region.plot(*h_add);
  }

  AIDA::IHistogram1D* h_div = histogramFactory->divide("h_div",*BW,*BW);
  h_div->setTitle("BW/BW");
  {
    AIDA::IPlotterRegion& region = plotter->next();
    region.plot(*h_div);
  }

  AIDA::IHistogram1D* h_mul = histogramFactory->multiply("h_mul",*BW,*BW);
  h_mul->setTitle("BW*BW");
  {
    AIDA::IPlotterRegion& region = plotter->next();
    region.plot(*h_mul);
  }

  {
    AIDA::IPlotterRegion& region = plotter->next();

    region.setParameter("plotter.superposeBins","TRUE");

    BW->annotation().addItem("plotter.modeling","boxes");
    BW->annotation().addItem("plotter.color","1 0 0");

    h_add->annotation().addItem("plotter.modeling","boxes");
    h_add->annotation().addItem("plotter.color","0 0 1");

    // Order is not important in "superpose" mode.
    region.plot(*BW);    //Red.
    region.plot(*h_add); //On top. Blue.
  }

  if(a_ui) {
    plotter->show();
    plotter->interact();
  }

  delete plotter;

  }

  delete histogramFactory;

}

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_hopera :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  hopera(*aida,std::cout,"",true);
  delete aida;
  return 0;
}

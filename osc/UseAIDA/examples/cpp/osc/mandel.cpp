
static double mandel_func(double aX,double aY){
  // From pawex10.kumac mandel.f.
  int NMAX = 30;
  double X = aX;
  double Y = aY;
  double XX = 0;
  double YY = 0;
  int N;
  for(N=1;N<=NMAX;N++) {
    double TT = XX*XX-YY*YY+X;
    YY = 2*XX*YY+Y;
    XX = TT;
    if(4<(XX*XX+YY*YY)) break;
  }
  return ((double)N)/((double)NMAX);
}


//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Mandelbrot function.

static void mandel(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Factories :
  AIDA::IHistogramFactory* histogramFactory = 
    a_aida.createHistogramFactory(*mem);
  if(!histogramFactory) return;

  AIDA::IHistogram2D* mandel = 
    histogramFactory->createHistogram2D("mandel","Mandel",
      100,-2.4,0.8,100,-1.2,1.2);

  delete histogramFactory;

  for(int i=0;i<100;i++) {
    double x = mandel->xAxis().binLowerEdge(i)+mandel->xAxis().binWidth(i)/2;
    for(int j=0;j<100;j++) {
      double y = mandel->yAxis().binLowerEdge(j)+mandel->yAxis().binWidth(j)/2;
      mandel->fill(x,y,mandel_func(x,y));
    }
  }

  // Get facilities :
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->createRegions(1,1,0);
    plotter->setTitle("Mandel");

    AIDA::IPlotterRegion& region = plotter->next();
    mandel->annotation().addItem("plotter.modeling","wire_boxes");
    region.plot(*mandel);

    if(a_ui) {
      plotter->show();
      plotter->interact();
    }  
    delete plotter;
  }
}

int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_mandel :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  mandel(*aida,std::cout,"",true);
  delete aida;
  return 0;
}

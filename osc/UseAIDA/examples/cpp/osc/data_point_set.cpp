
//NOTE : this file is included by OnXLab/examples/Callbacks/AIDA.cxx.

#include <AIDA/AIDA.h>

#include <amanip/all.h>

// Data point set.

static void data_point_set(AIDA::IAnalysisFactory& a_aida,std::ostream& a_out,const std::string& a_plotter,bool a_ui) {
  // Find memory tree :
  AIDA::ITree* mem = amanip::find_mem(a_aida);
  if(!mem) return;

  // Factories :
  AIDA::IDataPointSetFactory* dataPointSetFactory = 
    a_aida.createDataPointSetFactory(*mem);
  if(!dataPointSetFactory) return;

  AIDA::IDataPointSet* dps1D = 
    dataPointSetFactory->create("dps1D","one dimensional IDataPointSet",1);
  if(dps1D) {
    // Fill the one dimensional IDataPointSet
    double yVals1D[] = { 0.32, 0.45, 0.36, 0.29, 0.34 };
    double yErrP1D[] = { 0.06, 0.07, 0.03, 0.07, 0.04 };

    int number = 5;
    for ( int i = 0; i<number; i++ ) {
      dps1D->addPoint();
      dps1D->point(i)->coordinate(0)->setValue( yVals1D[i] );
      dps1D->point(i)->coordinate(0)->setErrorPlus( yErrP1D[i] );
    }
  }

  // Create a two dimensional IDataPointSet.
  AIDA::IDataPointSet* dps2D = 
    dataPointSetFactory->create("dps2D","two dimensional IDataPointSet",2);
  if(dps2D) {
    // Fill the two dimensional IDataPointSet
    double yVals2D[] = { 0.12, 0.22, 0.35, 0.42, 0.54 , 0.61 };
    double yErrP2D[] = { 0.01, 0.02, 0.03, 0.03, 0.04 , 0.04 };
    double yErrM2D[] = { 0.02, 0.02, 0.02, 0.04, 0.06 , 0.05 };
    double xVals2D[] = { 1.5, 2.6, 3.4, 4.6, 5.5 , 6.4 };
    double xErrP2D[] = { 0.5, 0.5, 0.4, 0.4, 0.5 , 0.5 };

    int number = 6;
    for ( int i = 0; i<number; i++ ) {
      dps2D->addPoint();
      dps2D->point(i)->coordinate(0)->setValue( xVals2D[i] );
      dps2D->point(i)->coordinate(0)->setErrorPlus( xErrP2D[i] );
      dps2D->point(i)->coordinate(1)->setValue( yVals2D[i] );
      dps2D->point(i)->coordinate(1)->setErrorPlus( yErrP2D[i] );
      dps2D->point(i)->coordinate(1)->setErrorMinus( yErrM2D[i] );
    }
  }
      
  AIDA::IPlotter* plotter = 
    amanip::create_plotter(a_aida,a_plotter);
  if(plotter) {
    plotter->createRegions(2,1,0);
    if(dps1D) plotter->region(0)->plot(*dps1D);
    if(dps2D) plotter->region(1)->plot(*dps2D);

    if(a_ui) {
      plotter->show();
      plotter->interact();
    }

    delete plotter;
  }

  delete dataPointSetFactory;
}


int main(int argc,char* argv[]){
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << "aida_exa_data_point_set :" 
              << " aida not found." 
              << std::endl;
    return 1;
  } 
  data_point_set(*aida,std::cout,"",true);
  delete aida;
  return 0;
}

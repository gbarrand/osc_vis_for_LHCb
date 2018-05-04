
// C++ conversion of the java/Restore.java example.

#include <AIDA/AIDA.h>

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create("myFile.aida","xml");
  delete trf;
  if(!tree) {
    std::cerr << "cant't create tree."
              << " You can create the myFile.aida file with"
              << " the create_myFile_aida example."
              << std::endl;
    return EXIT_FAILURE;
  }

  IManagedObject* mo = tree->find("test 1d");
  if(!mo) return EXIT_FAILURE;
  IHistogram1D* h1d = dynamic_cast<IHistogram1D*>(mo);
  if(!h1d) return EXIT_FAILURE;

  mo = tree->find("test 2d");
  if(!mo) return EXIT_FAILURE;
  IHistogram2D* h2d = dynamic_cast<IHistogram2D*>(mo);
  if(!h2d) return EXIT_FAILURE;
      
  IPlotterFactory* pf = af->createPlotterFactory(argc,argv);
  if(pf) {
  IPlotter* plotter = pf->create();
  delete pf;
  if(plotter) {
    plotter->createRegions(2,1,0);
    plotter->region(0)->plot(*h1d);
    plotter->region(1)->plot(*h2d);
    plotter->show();
    plotter->interact();
    delete plotter;
  }}

  delete af;

  return EXIT_SUCCESS;
}

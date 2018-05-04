
// C++ conversion of the java/ProfileCreate.java example.

#include <AIDA/AIDA.h>

using namespace AIDA;

#include <stdlib.h>

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return EXIT_FAILURE;

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return EXIT_FAILURE;
  ITree* tree = trf->create();
  delete trf;
  if(!tree) return EXIT_FAILURE;

  IHistogramFactory* hf = af->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;

  // Create 1D and 2D IProfile with fixed bin width
  hf->createProfile1D("prof1DFixedBinWidth","Fixed bin width 1D", 10, 0, 1);
  hf->createProfile2D("prof2DFixedBinWidth","Fixed bin width 2D", 10, 0, 1, 10, -5, 5);

  std::vector<double> xBinEdges;
  xBinEdges.push_back(0);
  xBinEdges.push_back(0.1);
  xBinEdges.push_back(0.21);
  xBinEdges.push_back(0.35);
  xBinEdges.push_back(0.48);
  xBinEdges.push_back(0.52);
  xBinEdges.push_back(0.65);
  xBinEdges.push_back(0.75);
  xBinEdges.push_back(0.83);
  xBinEdges.push_back(0.94);
  xBinEdges.push_back(1.0);

  std::vector<double> yBinEdges;
  yBinEdges.push_back(-5.0);
  yBinEdges.push_back(-4.1);
  yBinEdges.push_back(-3.2);
  yBinEdges.push_back(-2.0);
  yBinEdges.push_back(-1.1);
  yBinEdges.push_back(-0.4);
  yBinEdges.push_back(1.2);
  yBinEdges.push_back(2.3);
  yBinEdges.push_back(3.5);
  yBinEdges.push_back(4.2);
  yBinEdges.push_back(5.0);

  // Create 1D and 2D IProfile with variable bin width
  hf->createProfile1D("prof1DVariableBinWidth", "Variable bin width 1D", xBinEdges);
  hf->createProfile2D("prof2DVariableBinWidth", "Variable bin width 2D", xBinEdges, yBinEdges);

  delete hf;

  delete af;

  return EXIT_SUCCESS;
}

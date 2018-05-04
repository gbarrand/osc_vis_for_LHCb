//
// Usage :
//   OS> AIDA_example_Store
//   OS> AIDA_example_Store [aida,root,hbook,fs]
//

#include <AIDA/AIDA.h>

#include <stdlib.h>

double rgauss();

int main(int argc,char** argv) {
  std::string format("aida");
  if(argc>=2) format = std::string(argv[1]);
  std::string compress;
  if(argc>=3) compress = std::string(argv[2]);

  // The main and only concrete entry point !
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return EXIT_FAILURE; //Bad luck. Contact your AIDA provider...

  // Someone get everything through factories...
  AIDA::ITreeFactory* trf = aida->createTreeFactory();
  if(!trf) return EXIT_FAILURE; //Contact your provider...

  // Create a tree-like container associated to a file...
  std::string file("AIDA_example_Store");
  file += "."+format;
  std::string opts;
  if(compress=="compress") opts = "compress=yes";
  AIDA::ITree* tree = trf->create(file,format,false,true,opts);
  delete trf;
  if(!tree) return EXIT_FAILURE; //File format not supported ?

  AIDA::IHistogramFactory* hf = aida->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE; //Contact your provider...

  // Ah ! my histo at last...
  std::string name = "test 1d";
  if(format=="hbook") name = "10";
  AIDA::IHistogram1D* h = hf->createHistogram1D(name,50,-3,3);
  delete hf;
  if(!h) return EXIT_FAILURE; //Contact your provider...
      
  // Well, do something with the histo...
  for (int i=0; i<10000; i++) h->fill(rgauss());
      
  tree->commit();  // Write to file...

  delete aida;  // Bye, bye the AIDA implementation...

  return EXIT_SUCCESS; //Yeee...
}
#include <math.h>
#include <stdlib.h>
double rgauss() {
  double sigma = 1; double mean = 0;
  double v1,v2,r;
  do {
    v1 = 2.0 * ((double)::rand()/(double)RAND_MAX) - 1.0;
    v2 = 2.0 * ((double)::rand()/(double)RAND_MAX) - 1.0;
    r = v1*v1 + v2*v2;
  } while ( r > 1.0 );
  double fac = ::sqrt(-2.0*::log(r)/r);
  return (v2 * fac) * sigma + mean;
}

//
// Usage :
//   OS> AIDA_example_StoreTuple
//   OS> AIDA_example_StoreTuple [aida,root,hbook,fs]
//

#include <AIDA/AIDA.h>

#include <stdlib.h>

double rgauss();

int main(int argc,char** argv) {
  std::string format("aida");
  if(argc>=2) format = std::string(argv[1]);

  // The main and only concrete entry point !
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return EXIT_FAILURE; //Bad luck. Contact your AIDA provider...

  // Someone get everything through factories...
  AIDA::ITreeFactory* trf = aida->createTreeFactory();
  if(!trf) return EXIT_FAILURE; //Contact your provider...

  // Create a tree-like container associated to a file...
  std::string file("AIDA_example_StoreTuple");
  file += "."+format;
  AIDA::ITree* tree = trf->create(file,format,false,true);
  delete trf;
  if(!tree) return EXIT_FAILURE; //File format not supported ?

  AIDA::ITupleFactory* tf = aida->createTupleFactory(*tree);
  if(!tf) return EXIT_FAILURE; //Contact your provider...

  AIDA::ITuple* tuple = tf->create("tuple","My tuple",
                    "double rand_gauss, double step");
  delete tf;
  if(!tuple) return EXIT_FAILURE; //Contact your provider...
      
  for (int i=0; i<10000; i++) {
    tuple->fill(0,rgauss());
    tuple->fill(1,double(i));
    tuple->addRow();
  }    
  
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

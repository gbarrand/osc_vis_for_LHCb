//
// Example to produce a :
//   AIDA_example_A2R.[aida,root,hdf5,hbook]
// data file used by :
//   examples/ROOT/A2R.C
//

//
// Usage :
//   OS> AIDA_example_A2R
//   OS> AIDA_example_A2R [aida,root,hbook,fs]
//

#include <AIDA/AIDA.h>

#include <cstdlib>

double rgauss();
double rbw();

int main(int argc,char** argv) {
  //std::string format("aida");

  //std::string format("hbook");
  //std::string format("hdf5"); //with an OpenScientist AIDA implementation.
  std::string format("root");

  if(argc>=2) format = std::string(argv[1]);
  std::string compress;
  if(argc>=3) compress = std::string(argv[2]);

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) return EXIT_FAILURE;

  AIDA::ITreeFactory* trf = aida->createTreeFactory();
  if(!trf) return EXIT_FAILURE;

  std::string file("AIDA_example_A2R");
  file += "."+format;

  std::string opts;
  if(format=="root") {
    //if(opts.size()) opts += ";";
    //opts += "streaming=native";
  }
  if(compress=="compress") {
    if(opts.size()) opts += ";";
    opts += "compress=yes";
  }

  AIDA::ITree* tree = trf->create(file,format,false,true,opts);
  delete trf;
  if(!tree) return EXIT_FAILURE;

  AIDA::IHistogramFactory* hf = aida->createHistogramFactory(*tree);
  if(!hf) return EXIT_FAILURE;

 {std::string name = "histo_1D";
  if(format=="hbook") name = "10";
  AIDA::IHistogram1D* h = hf->createHistogram1D(name,100,-5,5);
  if(!h) return EXIT_FAILURE;      
  for(int i=0;i<10000;i++) h->fill(rgauss());}
      
 {std::string name = "histo_2D";
  if(format=="hbook") name = "20";
  AIDA::IHistogram2D* h = hf->createHistogram2D(name,100,-5,5,100,-5,5);
  if(!h) return EXIT_FAILURE;      
  for(int i=0;i<10000;i++) h->fill(rgauss(),rbw());}

  //NOTE : IProfile1D ROOT streaming available only from a 16.11.2.
  if(format!="root") {
   {std::string name = "profile_1D";
    if(format=="hbook") name = "30";
    AIDA::IProfile1D* p = hf->createProfile1D(name,100,-5,5);
    if(!p) return EXIT_FAILURE;
    for(int i=0;i<10000;i++) {
      p->fill(rgauss(),rbw());
    }}

   {std::string name = "profile_2D";
    if(format=="hbook") name = "40";
    AIDA::IProfile2D* p = hf->createProfile2D(name,100,-5,5,100,-5,5);
    if(!p) return EXIT_FAILURE;
    for(int i=0;i<10000;i++) {
      p->fill(rgauss(),rgauss(),rbw());
    }}
  }
  
  tree->commit();

  delete hf;
  delete aida;

  return EXIT_SUCCESS; //Yeee...
}

#include <cmath>
#include <cstdlib>
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

#ifndef M_PI
#define M_PI       3.1415926535897931160E0
#define M_PI_2     1.5707963267948965580E0  
#endif

double rbw() {
  double mean = 0;
  double gamma = 1;
  double rval = 2.0 * ((double)::rand()/(double)RAND_MAX) - 1.0;
  double displ = 0.5 * gamma * ::tan(rval * M_PI_2);
  return mean + displ;
}


// C++ conversion of the java/DataPointSetCreateFromData.java example.

#include <AIDA/AIDA.h>

using namespace AIDA;

#include <stdlib.h>

#include <iostream>

//G.Barrand : introduce the below error function.
static int error(const std::string& a_message) {
  std::cerr << "DataPointSetCreateFromData :" 
            << a_message
            << std::endl;
  return EXIT_FAILURE;
}

int main(int argc,char* argv[]) {

  IAnalysisFactory* af = AIDA_createAnalysisFactory();
  if(!af) return error("can't get AIDA.");

  ITreeFactory* trf = af->createTreeFactory();
  if(!trf) return error("can't create a tree factory.");
  // Create a tree loading the AIDA objects stored in an AIDA file.
  ITree* tree = trf->create("aidaStore.aida");
  delete trf;
  if(!tree) return error("cant't create tree. You can create the aidaStore.aida file with the create_aidaStore_aida example.");

  IManagedObject* mo = tree->find("h1");
  if(!mo) return error("can't get h1.");
  IHistogram1D* h1 = dynamic_cast<IHistogram1D*>(mo);
  if(!h1) return error("object not an IHistogram1D.");

  mo = tree->find("p2");
  if(!mo) return error("can't get h2.");
  IProfile2D* p2 = dynamic_cast<IProfile2D*>(mo);
  if(!p2) return error("object not an IProfile2D.");

  mo = tree->find("c3");
  if(!mo) return error("can't get c3.");
  ICloud3D* c3 = dynamic_cast<ICloud3D*>(mo);
  if(!c3) return error("object not an ICloud3D.");
        
  IDataPointSetFactory* dpsf = af->createDataPointSetFactory(*tree);
  if(!dpsf) return error("can't create a DPS factory.");

  // Create IDataPointSets from the the above AIDA objects.
  if(!dpsf->create("dps1DFromHist",*h1)) 
    return error("create DPS from histo failed.");
  if(!dpsf->create("dps2DFromProf",*p2))
    return error("create DPS from profile failed.");
  if(!dpsf->create("dps2DFromCloud",*c3))
    return error("create DPS from cloud failed.");

  delete dpsf;

  delete af;

  return EXIT_SUCCESS;
}

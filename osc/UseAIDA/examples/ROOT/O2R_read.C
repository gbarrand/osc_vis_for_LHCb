
//
// See README file for instructions.
//

//
// This .C coworks with the A2R_write.C, or 
// the examples/cpp/osc/A2R compiled example.
// that produces an AIDA_example_A2R.[aida,root,hdf5,hbook]
// file used here.
// 

//
// NOTE : CINT can't handle the dynamic_cast, then we use
//  the cast methods found on the AIDA interfaces basic classes.
//

int O2R_read() {
  std::string program = "O2R";
  std::string header = program + " : ";

  std::cout << header << "begin..." << std::endl;

  ////////////////////////////////////////////////////////
  /// Open an AIDA::ITree attached to ////////////////////
  /// an AIDA_example_A2R file.       ////////////////////
  ////////////////////////////////////////////////////////

  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
  if(!aida) {
    std::cout << header << "AIDA not found." << std::endl;
    return 1;
  }

  AIDA::ITreeFactory* treeFactory = aida->createTreeFactory();
  if(!treeFactory) {
    std::cout << header << "can't get a TreeFactory." << std::endl;
    return 1;
  }

  std::string format("aida"); //or "xml"
  //std::string format("hbook");
  //std::string format("hdf5"); //with an OpenScientist AIDA implementation.
  //std::string format("root");

  std::string file("AIDA_example_A2R");
  file += "."+format;

  AIDA::ITree* tree = treeFactory->create(file,format,true,false);
  if(!tree) {
    std::cout << header 
              << "can't open data file " << file << "." 
              << " Use the A2R_write.C script or the "
              << " examples/cpp/osc/A2R.cpp compiled example"
              << " to produce this file."
              << std::endl;
    return 1;
  }

  /////////////////////////////////////////////////
  /// Read an histogram 1D ////////////////////////
  /////////////////////////////////////////////////
  AIDA::IManagedObject* o1D = tree->find("histo_1D");
  if(!o1D) {
    std::cout << header 
              << "object \"histo_1D\" not found in tree." 
              << std::endl;
    return 1;
  }
  std::cout << header 
            << "object \"histo_1D\" found in tree." 
            << std::endl;
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::IHistogram1D* h1D = 
    (AIDA::IHistogram1D*)o1D->cast("AIDA::IHistogram1D");
#else
  AIDA::IHistogram1D* h1D = dynamic_cast<AIDA::IHistogram1D*>(o1D);
#endif
  if(!h1D) {
    std::cout << header << "object not an AIDA::IHistogram1D." << std::endl;
    return 1;
  }
  OTH1D* th1D = new OTH1D(*h1D);
  th1D->SetName("h1D");

  /////////////////////////////////////////////////
  /// Read an histogram 2D ////////////////////////
  /////////////////////////////////////////////////
  AIDA::IManagedObject* o2D = tree->find("histo_2D");
  if(!o2D) {
    std::cout << header 
              << "object \"histo_2D\" not found in tree." 
              << std::endl;
    return 1;
  }
  std::cout << header 
            << "object \"histo_2D\" found in tree." 
            << std::endl;

#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::IHistogram2D* h2D = 
    (AIDA::IHistogram2D*)o2D->cast("AIDA::IHistogram2D");
#else
  AIDA::IHistogram2D* h2D = dynamic_cast<AIDA::IHistogram2D*>(o2D);
#endif
  if(!h2D) {
    std::cout << header << "object not an AIDA::IHistogram2D." << std::endl;
    return 1;
  }
  OTH2D* th2D = new OTH2D(*h2D);
  th2D->SetName("h2D");

  /////////////////////////////////////////////////
  /// Read a profile 1D ///////////////////////////
  /////////////////////////////////////////////////
  AIDA::IManagedObject* op1D = tree->find("profile_1D");
  if(!op1D) {
    std::cout << header 
              << "object \"profile_1D\" not found in tree." 
              << std::endl;
    return 1;
  }
  std::cout << header 
            << "object \"profile_1D\" found in tree." 
            << std::endl;
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::IProfile1D* p1D = 
    (AIDA::IProfile1D*)op1D->cast("AIDA::IProfile1D");
#else
  AIDA::IProfile1D* p1D = dynamic_cast<AIDA::IProfile1D*>(op1D);
#endif
  if(!p1D) {
    std::cout << header << "object not an AIDA::IProfile1D." << std::endl;
    return 1;
  }
  OTProfile* tp1D = new OTProfile(*p1D);
  th1D->SetName("p1D");

  /////////////////////////////////////////////////
  /// Read a profile 2D ///////////////////////////
  /////////////////////////////////////////////////
  AIDA::IManagedObject* op2D = tree->find("profile_2D");
  if(!op2D) {
    std::cout << header 
              << "object \"profile_2D\" not found in tree." 
              << std::endl;
    return 1;
  }
  std::cout << header 
            << "object \"profile_2D\" found in tree." 
            << std::endl;
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::IProfile2D* p2D = 
    (AIDA::IProfile2D*)op2D->cast("AIDA::IProfile2D");
#else
  AIDA::IProfile2D* p2D = dynamic_cast<AIDA::IProfile2D*>(op2D);
#endif
  if(!p2D) {
    std::cout << header << "object not an AIDA::IProfile2D." << std::endl;
    return 1;
  }
  OTProfile2D* tp2D = new OTProfile2D(*p2D);
  th2D->SetName("p2D");

  /////////////////////////////////////////////////
  /// Plot histograms /////////////////////////////
  /////////////////////////////////////////////////
  std::cout << header << "plot..." << std::endl;

  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(2,2);  

  plotter->cd(1);
  th1D->Draw();

  plotter->cd(2);
  th2D->Draw();

  plotter->cd(3);
  tp1D->Draw();

  plotter->cd(4);
  tp2D->Draw();

  plotter->Update();
  /////////////////////////////////////////////////
  /////////////////////////////////////////////////
  /////////////////////////////////////////////////
  delete treeFactory;
  delete aida;

  return 0;
}


//
// See README_A2R file for instructions.
//

//
// NOTE : CINT can't handle the dynamic_cast, then we use
//  the cast methods found on the AIDA interfaces basic classes.
//

AIDA::ITuple* cast_Tuple(AIDA::ITupleEntry* aEntry) {
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  //AIDA-3.2.1 : ITupleEntry does not have the cast method.
  //return (AIDA::ITuple*)aEntry->cast("AIDA::ITuple");
  return cast_ITuple(aEntry);
#else
  return dynamic_cast<AIDA::ITuple*>(aEntry);
#endif
}

bool plot(TH1D& aHisto1D) {
  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(1,1);  

  plotter->cd(1);
  aHisto1D.Draw();

  plotter->Update();

  return true;
}


int jec() {
  int col_eventId = 0;
  int col_nPart = 1;
  int col_track = 4;

  int col_pId = 0;
  int col_parent = 1;
  int col_direction = 2;
  int col_ETot = 3;

  int col_dx = 0;
  int col_dy = 1;
  int col_dz = 2;

  bool dump = true;
  bool do_dir = true;

  std::string program = "jec";
  std::string header = program + " : ";

  std::cout << header << "begin..." << std::endl;

  ////////////////////////////////////////////////////////
  // Create histograms ///////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* h_ETot = new TH1D("h_ETot","pxHist",100,0,30);

  ////////////////////////////////////////////////////////
  /// Read data an fill histos ///////////////////////////
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

  AIDA::ITree* tree = treeFactory->create("jec.root","root",true,false);
  if(!tree) {
    std::cout << header << "can't open data file." << std::endl;
    return 1;
  }

  AIDA::IManagedObject* object = tree->find("jec");
  if(!object) {
    std::cout << header 
              << "object jec not found in tree." 
              << std::endl;
    return 1;
  }
#ifdef __CINT__
  //FIXME : CINT can't handle dynamic_cast :
  AIDA::ITuple* tuple = (AIDA::ITuple*)object->cast("AIDA::ITuple");
#else
  AIDA::ITuple* tuple = dynamic_cast<AIDA::ITuple*>(object);
#endif
  if(!tuple) {
    std::cout << header << "object not an AIDA::ITuple." << std::endl;
    return 1;
  }

  int coln = tuple->columns();
  for(int index=0;index<coln;index++) {
    std::cout << header 
              << " icol = " << index
              << ", label = " << tuple->columnName(index) 
              << ", type = " << tuple->columnType(index) 
              << std::endl;
  }

  std::cout << header << "rows = " << tuple->rows() << std::endl;

  int nentries = tuple->rows();
  //nentries = 100000;
  //nentries = 40;
  std::cout << header 
            << "traitements de " << nentries << " entrees..." 
            << std::endl;

  int irow = 0;
  tuple->start();
  while(tuple->next() && (irow<nentries)) {

    int nPart = tuple->getInt(col_nPart);

    if(dump) {
      std::cout << "jec : ievt " << irow << " nPart " << nPart << std::endl;
    }

    AIDA::ITupleEntry* tracksEntry = 
      (AIDA::ITupleEntry*)tuple->getObject(col_track);
    if(!tracksEntry) {
      std::cout << "read: FATAL: can't get tuple object." 
                << std::endl;
      return false;
    }
    
    AIDA::ITuple* tracks = cast_Tuple(tracksEntry);
    if(!tracks) {
      std::cout << "read: FATAL: tuple object not a AIDA::ITuple." 
                << std::endl;
      return false;
    }

    if(nPart!=tracks->rows()) {
      std::cout << "read: nPart / tracks mismatch " 
                << nPart << " " << tracks->rows() << std::endl;
      return false;
    }

    tracks->start();
    while ( tracks->next() ) {
      float dx = 0;
      float dy = 0;
      float dz = 0;
      if(do_dir) {
      AIDA::ITupleEntry* dirEntry = 
        (AIDA::ITupleEntry*)tracks->getObject(col_direction);
      if(!dirEntry) {
        std::cout << "read: FATAL: can't tuple object." 
                  << std::endl;
        return false;
      }
      AIDA::ITuple* dir = cast_Tuple(dirEntry);
      if(!dir) {
        std::cout << "read: FATAL: tuple object not a AIDA::ITuple." 
                  << std::endl;
        return false;
      }

      dir->start();
      dir->next();
      dx = dir->getFloat(col_dx);
      dy = dir->getFloat(col_dy);
      dz = dir->getFloat(col_dz);
      }

      float ETot = tracks->getFloat(col_ETot);

      if(dump) {
        std::cout << " ETot " << ETot 
                  << " dx " << dx
                  << " dy " << dy
                  << " dz " << dz
                  << std::endl;
      }

      h_ETot->Fill( ETot);
    }

    irow++;
  }

  delete treeFactory;
  delete aida;

  ////////////////////////////////////////////////////////
  // Plot histograms /////////////////////////////////////
  ////////////////////////////////////////////////////////
  std::cout << header << "plot..." << std::endl;
  plot(*h_ETot);

  return 0;
}


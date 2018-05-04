//
//  This script shows how to read, with ROOT, a jec.root 
// file containing a "TTree within TTree" produced
// by the AIDA_example_jec program.
//
//  See README_A2R file for instructions.
//

bool plot(TH1D& aHisto1D) {
  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(1,1);  

  plotter->cd(1);
  aHisto1D.Draw();

  plotter->Update();

  return true;
}

void jec_ROOT() {

  ////////////////////////////////////////////////////////
  // Create histograms ///////////////////////////////////
  ////////////////////////////////////////////////////////
  TH1D* h_ETot = new TH1D("h_ETot","pxHist",100,0,30);

  int hit_count = 0;
  bool dump = true;
  //bool dump = false;

  ////////////////////////////////////////////////////////
  /// Read data an file histos ///////////////////////////
  ////////////////////////////////////////////////////////
  TFile* f = new TFile("jec.root");
  TTree* tEvent = (TTree*)f->Get("jec");

 {TObjArray* brs = tEvent->GetListOfBranches();
  for(int i=0;i<brs->GetEntries();i++) {
    TBranch* b = (TBranch*)brs->At(i);
    std::cout << " Event branch : " << b->GetName()
    	      << std::endl;    
  }}

  //declare Non subTuple part
  Int_t eventId,nPart,positiveIndex,negativeIndex;
  tEvent->SetBranchAddress("eventId",&eventId);
  tEvent->SetBranchAddress("nPart",&nPart);
  tEvent->SetBranchAddress("positiveIndex",&positiveIndex);
  tEvent->SetBranchAddress("negativeIndex",&negativeIndex);

  Int_t nEvent = (Int_t)tEvent->GetEntries();  
  //nEvent = 1;
  //nEvent = 10000;
  std::cout << " nEvents = " << nEvent << std::endl;

  for (Int_t i=0; i<nEvent; ++i) {

    TTree* Event_track = new TTree();
    tEvent->SetBranchAddress("track",&Event_track);

    tEvent->GetEntry(i);

    //{TObjArray* brs = Event_track->GetListOfBranches();
    //for(int i=0;i<brs->GetEntries();i++) {
    //  TBranch* b = (TBranch*)brs->At(i);
    //  std::cout << " Event_track branch : " << b->GetName() << std::endl;    
    //  }}

    Int_t pId,parent;
    Float_t ETot;
    Event_track->SetBranchAddress("pId",&pId);
    Event_track->SetBranchAddress("parent",&parent);
    Event_track->SetBranchAddress("ETot",&ETot);

    if(dump)
    std::cout << ">>>>>>>>>>>>> Event{" << i << "}: "
    	      << " evt Id " << eventId 
    	      <<" #tracks: " << nPart
    	      << std::endl;

    Int_t nTracks = (Int_t)Event_track->GetEntries();

    if(dump)
    std::cout << "Verif :" 
              << " nTracks = " << nTracks 
    	      << std::endl;

    for (Int_t j=0; j<nTracks; ++j) {

      //Sub tuples of Event_tracks :
      TTree* Track_direction = new TTree();
      Event_track->SetBranchAddress("direction",&Track_direction);

      Event_track->GetEntry(j);

      Float_t dx,dy,dz;
      Track_direction->SetBranchAddress("dx",&dx);
      Track_direction->SetBranchAddress("dy",&dy);
      Track_direction->SetBranchAddress("dz",&dz);

      h_ETot->Fill(ETot);

      if(dump) {
      std::cout << "----> Tk{"<<j<<"}: " 
	   	<< " pId " << pId
		<< " parent " << parent
		<< " dx,dy,dz " << dx << " " << dy << " " << dz << "\n"
		<< " ETot " << ETot
                << std::endl;
      }

      delete Track_direction;

    }//loop on Tracks

    delete Event_track;

  }//loop on event

  if(dump)
  std::cout << " nEvents = " << nEvent << std::endl;

  ////////////////////////////////////////////////////////
  // Plot histograms /////////////////////////////////////
  ////////////////////////////////////////////////////////
  plot(*h_ETot);

}

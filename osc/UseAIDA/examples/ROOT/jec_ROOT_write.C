void jec_write(){

  Int_t compress = 0;
  TFile f("jec.root","recreate","",compress);
  
  TTree* tMain = new TTree("jec","Water Cerenkov Analysis");
  
  Int_t eventId,nPart,positiveIndex,negativeIndex;
  tMain->Branch("eventId",&eventId,"eventId/I");
  tMain->Branch("nPart",&nPart,"nPart/I");
  tMain->Branch("positiveIndex",&positiveIndex,"positiveIndex/I");
  tMain->Branch("negativeIndex",&negativeIndex,"negativeIndex/I");

  TTree* track = new TTree("track","Sub TTree for the Tracks");
  tMain->Branch("track","TTree",&track,32000,0); //DO NOT SPLIT
  Int_t pId,parent;
  Float_t ETot;
  track->Branch("pId",&pId,"pId/I");
  track->Branch("parent",&parent,"parent/I");
  track->Branch("ETot",&ETot,"ETot/F");

  TTree* direction = new TTree("direction","Sub TTree for the Direction");
  track->Branch("direction","TTree",&direction,32000,0); //DO NOT SPLIT
  Float_t dx,dy,dz;
  direction->Branch("dx",&dx,"dx/F");
  direction->Branch("dy",&dy,"dy/F");
  direction->Branch("dz",&dz,"dz/F");

  int tot_nPart = 0;

  for (Int_t i=0; i<100 ; ++i){
    eventId = i;
    nPart = 1 + Int_t(gRandom->Rndm(1) * 10);
    nPart += 2;

    tot_nPart += nPart;

    std::cout << ">>>>>>>>>>>>> Event:" << eventId 
	      <<" #tracks: " << nPart << std::endl;
    
    track->Reset();  //clear the Track TTree
    for (Int_t j=0; j<nPart; ++j) {
      pId = Int_t(-10.0 + 20.0*gRandom->Rndm(1));
      parent = pId;
      ETot =  30.0*gRandom->Rndm(1); //[0,30.0]

      direction->Reset();
      dx  = -5.0 + 10.0*gRandom->Rndm(1);
      dy  = -5.0 + 10.0*gRandom->Rndm(1);
      dz  = -5.0 + 10.0*gRandom->Rndm(1);
      direction->Fill();
  
      std::cout << "----> Tk{"<<j<<"}: " 
		<< " pId " << pId
		<< " ETot " << ETot
		<< " dx " << dx
		<< " dy " << dy
		<< " dz " << dz
                << std::endl;

      track->Fill(); //save a Track
    }//loop on tracks

    //    track->Print();
    tMain->Fill();   //save an Event
  }//loop on eventId

  std::cout << "number of particles " << tot_nPart << std::endl;

  delete direction;
  delete track;

  //Write the TTree
  f.Write();
  
  //Print the TTree
  //tMain->Print();
}//writeTree

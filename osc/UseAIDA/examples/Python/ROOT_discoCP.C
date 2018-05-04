{

  std::string osc_home_dir = getenv("OSC_HOME_DIR");
  if(osc_home_dir.empty()) {
    ::printf("OSC_HOME_DIR not defined.");
  }
  std::string data_dir = osc_home_dir+"/Resources/AIDA/examples/data/";

  Int_t nLogSin22q13 = 21;
  Double_t logSin2Min = -3.3;
  Double_t logSin2Max = -1;
  Double_t logSin2BinWidth = 
    (logSin2Max - logSin2Min)/double(nLogSin22q13-1.);
  Double_t logSin2EdgeMax = logSin2Max + logSin2BinWidth/2.;
  Double_t logSin2EdgeMin = logSin2Min - logSin2BinWidth/2.;

  Int_t nDeltaCP  = 21;
  Double_t deltaMin = 0.; //rad/pi
  Double_t deltaMax =  1.;
  Double_t deltaBinWidth = (deltaMax - deltaMin)/double(nDeltaCP-1.);
  Double_t deltaEdgeMax = deltaMax + deltaBinWidth/2.;
  Double_t deltaEdgeMin = deltaMin - deltaBinWidth/2.;

  TLegend* leg = new TLegend(0.06,0.40,0.97,0.60);
  leg->SetTextFont(gStyle->GetTextFont());
  leg->SetTextSize(0.06);
  leg->SetFillColor(10);
  
  TCanvas* c1 = new TCanvas("c1","Contour",100,100,600,600);
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLogx();

  TH2D* hdum = new TH2D("hdum","3#sigma CP discovery (#Delta #chi^{2}=9,1dof)",
			nLogSin22q13,TMath::Power(10.,logSin2Min),
			TMath::Power(10.,logSin2Max),
			nDeltaCP,deltaMin,deltaMax
			);
  hdum->SetYTitle("true #delta_{CP}/#pi");
  hdum->SetXTitle("true sin^{2}(2#theta_{13})");
  hdum->Draw();

  TPad *overlay = new TPad("overlay","",
			   gStyle->GetPadLeftMargin(),
			   gStyle->GetPadBottomMargin(),
			   1-gStyle->GetPadRightMargin(),
			   1-gStyle->GetPadTopMargin());
  overlay->SetFillStyle(4000);
  overlay->Draw();
  overlay->Range(logSin2Min,deltaMin,logSin2Max,deltaMax);

  std::string f_35 = data_dir+"SPLCPDisco-allsigvar-mateff-bin100-h0-o0.root";
  TFile f35(f_35.c_str());
  TTree* tree35 = (TTree*)f35.Get("SplGlb");
  TH2D* histo35 = new TH2D("histo35"," Theta vs Delta",
			  nLogSin22q13,logSin2EdgeMin,logSin2EdgeMax,
			  nDeltaCP,deltaEdgeMin,deltaEdgeMax
			  );
  tree35->Project(histo35->GetName(),"deltaCP/pi:TMath::Log10(pow(sin(2*theta13),2))","chi2");

  histo35->SetContour(1);
  histo35->SetContourLevel(0,9); //Chi2 for 1 dof and 3sigma
  histo35->SetLineColor(2); //blue
  histo35->SetFillStyle(3004);
  overlay->cd();
  histo35->Draw("CONT3 SAME");
  leg->AddEntry(histo35,"matrices de migrations","l");

  std::string f_36 = data_dir+"SPLCPDisco-matbb-h0-o0.root";
  TFile f36(f_36.c_str());
  TTree* tree36 = (TTree*)f36.Get("SplGlb");
  TH2D* histo36 = new TH2D("histo36"," Theta vs Delta",
			  nLogSin22q13,logSin2EdgeMin,logSin2EdgeMax,
			  nDeltaCP,deltaEdgeMin,deltaEdgeMax
			  );
  tree36->Project(histo36->GetName(),"deltaCP/pi:TMath::Log10(pow(sin(2*theta13),2))","chi2");

  histo36->SetContour(1);
  histo36->SetContourLevel(0,9); //Chi2 for 1 dof and 3sigma
  histo36->SetLineColor(4); //blue
  histo36->SetFillStyle(3004);
  overlay->cd();
  histo36->Draw("CONT3 SAME");
  leg->AddEntry(histo36,"matrices de migrations, #beta Beam","l");


  leg->Draw("SAME");
  overlay->Update();


  //  TCanvas c2;
  //histo1->Draw("SURF");



}

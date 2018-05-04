
// To compare with AIDA, four basic histo types in a file : 
//   TH1D, TH2D, TProfile, TProfile2D

// To compare with aida_histos.py, we use the inlib::random
// code put in inlib.C.

#include "../ROOT/inlib.C"

void ROOT_histos() {

  TH1D* h1D = new TH1D("h1D","Histogram 1D",100,-5,5);
  for (Int_t i=0; i<10000; ++i) {
    h1D->Fill(rgauss(0,1));
  }

  TH2D* h2D = new TH2D("h2D","Histogram 2D",100,-5,5,100,-4,4);
  for (Int_t i=0; i<10000; ++i) {
    h2D->Fill(rgauss(0,1),rbw(0,1));
  }

  TProfile* p1D = new TProfile("p1D","Profile 1D",100,-5,5);
  for (Int_t i=0; i<10000; ++i) {
    p1D->Fill(rgauss(0,1),rbw(0,1));
  }

  TProfile2D* p2D = new TProfile2D("p2D","Profile 2D",100,-5,5,100,-3,3);
  for (Int_t i=0; i<10000; ++i) {
    p2D->Fill(rgauss(0,1),rbw(0,1),rbw(0,1));
  }

  TCanvas* plotter = new TCanvas("canvas","",10,10,800,600);
  plotter->Divide(2,2);  
  plotter->cd(1);
  h1D->Draw();
  plotter->cd(2);
  h2D->Draw();
  plotter->cd(3);
  p1D->Draw();
  plotter->cd(4);
  p2D->Draw();
  plotter->Update();

}

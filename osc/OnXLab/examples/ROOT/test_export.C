//
//  To do some analysis over the examples/data/analysis.root file.
//

{
  TFile* file = new TFile("analysis.root");
  file->ls();

  TDirectory* directory = (TDirectory*)file->Get("histograms");
  directory->ls();

 {TH1D* gauss = (TH1D*)directory->Get("gauss");
  TCanvas* canvas = new TCanvas("gauss","",10,10,800,600);
  gauss->Draw();}                                 

 {TH2D* gauss_BW = (TH2D*)directory->Get("gauss_BW");
  TCanvas* canvas = new TCanvas("gauss_BW","",10,10,800,600);
  gauss_BW->Draw();}                                 

 /*FIXME
 {TProfile* profile = (TProfile*)directory->Get("profile");
  TCanvas* canvas = new TCanvas("profile","",10,10,800,600);
  profile->Draw();}                                 
 */

  canvas->Update();
}

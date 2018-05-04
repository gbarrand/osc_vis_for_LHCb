//
//  To do some analysis over the examples/data/analysis.root file.
//

{
  TFile* file = new TFile("analysis.root");
  file->ls();

  TTree* tree = (TTree*)file->Get("/tuples/tuple2");
  tree->Print();

  for(int index=0;index<10;index++) {
    tree->Show(index);
  }

  TCanvas* canvas = new TCanvas("canvas","EAbs",10,10,800,600);

  tree->Draw("rand_gauss","","");                                 

  canvas->Update();
}

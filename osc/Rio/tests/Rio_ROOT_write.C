//
// To produce a file with ROOT that Rio_test
// must be able to read.
//
//     OS> root
//     root[] .x Rio_ROOT_write.C
//     root[] .qqq
//
//     OS> Rio_test -read
//
//

{
  TFile* file = new TFile("Rio_ROOT_write.root","RECREATE");
  file->ls();

  TDirectory* dir = file->mkdir("trees");
  dir->cd();

  TTree* tree = new TTree("myTree","My tree");

  struct {
    Int_t v1;
    Float_t v3;
  } st;
  Double_t v4;
  Short_t v5;
  Char_t v6;
  char v7[32];

  TNamed* named = new TNamed;

  //tree->Branch("st",&st,"v1/I:v2/F:v3/D:v4/S:v5/B");
  tree->Branch("st",&st,"v1/I:v3/F");
  tree->Branch("double",&v4,"v4/D");
  tree->Branch("short",&v5,"v5/S");
  tree->Branch("char",&v6,"v6/B");
  tree->Branch("string",v7,"v7/C");
  TTree::SetBranchStyle(0);
  //tree->Branch("nameds","TNamed",&named);

  char s[32];
  for(int count=0;count<500;count++) {
    st.v1 = count;
    st.v3 = 3.0F * count;
    v4 = 3 * count;
    v5 = count;
    v6 = count;
    sprintf(v7,"hello_%d",count);
    tree->Fill();
  }

  file->Write();

  file->Close();
}

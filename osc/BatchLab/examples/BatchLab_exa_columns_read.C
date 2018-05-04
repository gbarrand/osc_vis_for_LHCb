//
//

{
  TFile* file = new TFile("BatchLab_exa_columns.root","READ");
  file->ls();

  TTree* tree = (TTree*)file->Get("tuples/tuple");
  if(tree) {
    int number = tree->GetEntries();
    printf("debug : found TTree : %d\n",number);

    TLeaf* leaf = tree->GetLeaf("av3");
    if(leaf) {
      printf("debug : found TLeaf array_double %s\n",leaf->GetTypeName());
    }

    std::vector<double>* array_double = new std::vector<double>;
    tree->SetBranchAddress("av3",&array_double);
    for(int index=0;index<10;index++) {
      tree->GetEntry(index);
      printf("%d %lu : <double> : %d %g\n",index,
        array_double,array_double->size(),(*array_double)[2]);
    }

    //delete array_double;

  }

  file->Close();
}

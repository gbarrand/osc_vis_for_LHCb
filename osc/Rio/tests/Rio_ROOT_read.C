//
//

{
  TFile* file = new TFile("Rio_ROOT_write.root","READ");
  //file->ShowStreamerInfo();
  file->ls();

  TDirectory* dir1 = (TDirectory*)file->Get("histos");
  if(dir1) {
    TObject* obj = dir1->Get("myObject");
  }

  TDirectory* dir2 = (TDirectory*)file->Get("trees");
  if(dir2) {
    TTree* tree = (TTree*)dir2->Get("myTree");
    if(tree) {
      int number = tree->GetEntries();
      int index;
      for(index=0;index<10;index++) tree->Show(index);
      for(index=number-10;index<number;index++) tree->Show(index);

      TLeaf* leaf3 = tree->GetLeaf("v3");
      TLeafC* leaf7 = (TLeafC*)tree->GetLeaf("v7");
      if(leaf3 && leaf7) {
        TBranch* branch3 = leaf3->GetBranch();
        TBranch* branch7 = leaf7->GetBranch();
        for(index=0;index<100;index++) {
          if(branch3->GetEvent(index) && branch7->GetEvent(index)) {
            Float_t val3 = leaf3->GetValue();
            char* val7 = leaf7->GetValueString();
            printf("v3 %d %g \"%s\"\n",index,val3,val7);
          }
        }
      }
    }
  }

  file->Close();
}

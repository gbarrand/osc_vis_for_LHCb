//
//

{
  TFile* file = new TFile("Rio_test_stl.root","READ");
  //TFile* file = new TFile("Rio_ROOT_array_write.root","READ");
  file->ls();
  file->ShowStreamerInfo();

  TTree* tree = (TTree*)file->Get("myTree");
  if(tree) {
    int number = tree->GetEntries();
    printf("debug : found TTree : %d\n",number);

    TLeaf* leaf = tree->GetLeaf("arrayD");
    if(leaf) {
      printf("debug : found TLeaf arrayD %s\n",leaf->GetTypeName());
      printf("debug : found TLeaf class name %s\n",leaf->ClassName());

      TArrayD* array = new TArrayD;
      tree->SetBranchAddress("arrayD",&array);
      for(int index=0;index<10;index++) {
        tree->GetEntry(index);
        //printf("entryD : %d %lu : %d\n",index,array,array->GetSize());
        printf("entryD : %d %lu : %d %g\n",
          index,array,array->GetSize(),(*array)[2]);
      }
    }

    TLeaf* leaf = tree->GetLeaf("array_double");
    if(leaf) {
      printf("debug : found TLeaf array_double %s\n",leaf->GetTypeName());
      printf("debug : found TLeaf class name %s\n",leaf->ClassName());

      std::vector<double>* array_double = new std::vector<double>;
      tree->SetBranchAddress("array_double",&array_double);
      //std::vector<char>* array_char = new std::vector<char>;
      //tree->SetBranchAddress("array_char",&array_char);
      for(int index=0;index<10;index++) {
        tree->GetEntry(index);
        //printf("entryS : %d %lu : <double> : %d\n",index,
        //  array_double,array_double->size());
        printf("entryS : %d %lu : <double> : %d %g\n",index,
          array_double,array_double->size(),(*array_double)[2]);
      }

    }

/*
    TLeaf* leaf = tree->GetLeaf("arrayD");
    if(leaf) {
      printf("debug : found TLeaf arrayD %s\n",leaf->GetTypeName());
      TBranch* branch = leaf->GetBranch();
      for(int index=0;index<10;index++) {
        if(branch->GetEvent(index)) {
          TArrayD* array = (TArrayD*)leaf->GetValuePointer();
          printf("%d %lu\n",index,array);
          printf("%d\n",array->GetSize());
        }
      }
    }

    TLeaf* leaf = tree->GetLeaf("arrayD");
    if(leaf) {
      printf("debug : found TLeaf arrayD %s\n",leaf->GetTypeName());
      TBranch* branch = leaf->GetBranch();
      for(int index=0;index<10;index++) {
        if(branch->GetEvent(index)) {
          std::vector<double>* array = leaf->GetValuePointer();
          printf("%d %lu\n",index,array);
          printf("%d\n",array->size());
        }
      }
    }
*/

  }

  file->Close();
}

{
  TFile* file = new TFile("Rio_test_stl.root","RECREATE");
  //TFile* file = new TFile("Rio_ROOT_array_write.root","RECREATE");
  file->ls();
  file->ShowStreamerInfo();

  TTree* tree = new TTree("myTree","My tree");

/*
  std::vector<char>* array_char = new std::vector<char>;
  tree->Branch("array_char","std::vector<char>",&array_char);

  std::vector<unsigned char>* array_uchar = new std::vector<unsigned char>;
  tree->Branch("array_uchar","std::vector<unsigned char>",&array_uchar);

  std::vector<short>* array_short = new std::vector<short>;
  tree->Branch("array_short","std::vector<short>",&array_short);

  std::vector<int>* array_int = new std::vector<int>;
  tree->Branch("array_int","std::vector<int>",&array_int);

  std::vector<float>* array_float = new std::vector<float>;
  tree->Branch("array_float","std::vector<float>",&array_float);

  std::vector<double>* array_double = new std::vector<double>;
  tree->Branch("array_double","std::vector<double>",&array_double);

  std::vector<std::string>* array_std_string = new std::vector<std::string>;
  tree->Branch("array_std_string","std::vector<std::string>",&array_std_string);
*/
/*
  //std::vector<unsigned short>* array_ushort = new std::vector<unsigned short>;
  //tree->Branch("array_ushort","std::vector<unsigned short>",&array_ushort);
  //std::vector<unsigned int>* array_uint = new std::vector<unsigned int>;
  //tree->Branch("array_uint","std::vector<unsigned int>",&array_uint);

  // Crashes :
  //std::vector<Long64_t>* array_Long64_t = new std::vector<Long64_t>;
  //tree->Branch("array_Long64_t","std::vector<Long64_t>",&array_Long64_t);

  //std::vector<bool>* array_bool = new std::vector<bool>;
  //tree->Branch("array_bool","std::vector<bool>",&array_bool);

  //std::vector<Bool_t>* array_Bool_t = new std::vector<Bool_t>;
  //tree->Branch("array_Bool_t","std::vector<Bool_t>",&array_Bool_t);
*/

  TArrayD* arrayD = new TArrayD;
  tree->Branch("arrayD","TArrayD",&arrayD);

  std::vector<double>* array_double = new std::vector<double>;
  tree->Branch("array_double","std::vector<double>",&array_double);

  //for(int count=0;count<500;count++) {
  for(int count=0;count<10;count++) {
    //printf("debug : %d\n",count);
    arrayD->Set(3);
    (*arrayD)[0] = count;
    (*arrayD)[1] = count+1;
    (*arrayD)[2] = count+2;

    array_double->resize(5);
    (*array_double)[0] = count;
    (*array_double)[1] = count+1;
    (*array_double)[2] = count+2;
    (*array_double)[3] = count+3;
    (*array_double)[4] = count+4;

    tree->Fill();
  }
  printf("debug : end fill with TArrayD\n");

  file->Write();

  file->Close();
}
